
# include "ChessGame.hh"
# include <unordered_set>
# include <algorithm>
# include "Round.hh"

namespace chess {

  ChessGame::ChessGame(int  width,
                       int  height) noexcept:
    utils::CoreObject("board"),

    // White by default.
    m_board(width, height),

    m_index(0u),
    m_current(Color::White),
    m_state({
      false, // Dirty state
      { false, false, false }, // White state
      { false, false, false }  // Black state
    }),
    m_round(m_index),
    m_rounds()
  {
    setService("chess");

    initialize();
  }

  const Board&
  ChessGame::operator()() const noexcept {
    return m_board;
  }

  void
  ChessGame::initialize() noexcept {
    // Initialize the board and any custom initialization.
    m_board.initialize();

    // Reset rounds.
    m_index = 0u;
    m_current = Color::White;

    m_state.dirty = false;
    m_state.white = { false, false, false };
    m_state.black = { false, false, false };

    m_round = Round(m_index);
    m_rounds.clear();
  }

  Color
  ChessGame::getPlayer() const noexcept {
    return m_current;
  }

  Round
  ChessGame::getCurrentRound() const noexcept {
    return m_round;
  }

  Round
  ChessGame::getLastRound() const noexcept {
    if (m_rounds.empty()) {
      return m_round;
    }

    return m_rounds.back();
  }

  bool
  ChessGame::isInCheck(const Color& color) const noexcept {
    if (!m_state.dirty) {
      return (color == Color::White ? m_state.white.check : m_state.black.check);
    }

    updateState();

    // Call the same routine.
    return isInCheck(color);
  }

  bool
  ChessGame::isInCheckmate(const Color& color) const noexcept {
    if (!m_state.dirty) {
      return (color == Color::White ? m_state.white.checkmate : m_state.black.checkmate);
    }

    updateState();

    // Call the same routine.
    return isInCheckmate(color);
  }

  bool
  ChessGame::isInStalemate(const Color& color) const noexcept {
    if (!m_state.dirty) {
      return (color == Color::White ? m_state.white.stalemate : m_state.black.stalemate);
    }

    updateState();

    // Call the same routine.
    return isInStalemate(color);
  }

  bool
  ChessGame::move(const Coordinates& start, const Coordinates& end) {
    // Prevent wrong pieces to move.
    const Piece& sp = m_board.at(start);
    if (sp.color() != m_current) {
      warn("Trying to move " + colorToString(sp.color()) + " when " + colorToString(m_current) + " should play");
      return false;
    }

    if (!m_board.legal(start, end)) {
      return false;
    }

    // Perform the move.
    movePiece(start, end);

    return true;
  }

  void
  ChessGame::promote(const Coordinates& p, const Type& promote) {
    m_board.promote(p, promote);

    // Dirty the check state.
    m_state.dirty = true;
  }

  inline
  void
  ChessGame::updateState() const noexcept {
    // Check if the king of each color stands among the
    // list of threatened cells.
    m_state.white.check = m_board.computeCheck(Color::White);
    m_state.black.check = m_board.computeCheck(Color::Black);

    if (m_state.white.check) {
      warn("White is in check");
    }
    if (m_state.black.check) {
      warn("Black is in check");
    }

    bool wStalemate = m_board.computeStalemate(Color::White);
    bool bStalemate = m_board.computeStalemate(Color::Black);
    m_state.white.stalemate = !m_state.white.check && wStalemate;
    m_state.black.stalemate = !m_state.black.check && bStalemate;

    if (m_state.white.stalemate) {
      warn("White is in stalemate");
    }
    if (m_state.black.stalemate) {
      warn("Black is in stalemate");
    }

    m_state.white.checkmate = m_state.white.check && wStalemate;
    m_state.black.checkmate = m_state.black.check && bStalemate;

    if (m_state.white.checkmate) {
      warn("White is in checkmate");
    }
    if (m_state.black.checkmate) {
      warn("Black is in checkmate");
    }

    // The state has been updated.
    m_state.dirty = false;
  }

  inline
  void
  ChessGame::movePiece(const Coordinates& start, const Coordinates& end) noexcept {
    // Fetch starting and ending position.
    Piece sp = m_board.at(start);
    Piece e = m_board.at(end);

    // Move the piece.
    m_board.move(start, end);

    // Invalidate cached data and update internal states.
    m_state.dirty = true;
    updateState();

    // Register the move.
    bool check = sp.color() == Color::White ? m_state.black.check : m_state.white.check;
    bool checkmate = sp.color() == Color::White ? m_state.black.checkmate : m_state.white.checkmate;
    bool stalemate = sp.color() == Color::White ? m_state.black.stalemate : m_state.white.stalemate;
    /// TODO: Handle promoting.
    m_round.registerMove(sp, start, end, e.valid(), check, checkmate, stalemate);

    // Update the round's data.
    if (m_round.valid() || checkmate || stalemate) {
      m_rounds.push_back(m_round);
      ++m_index;
      m_round = Round(m_index);

      log("Adding round " + m_rounds.back().toString(), utils::Level::Info);
    }

    // Move to the next player if the other player
    // is not in checkmate.
    if (!checkmate && !stalemate) {
      m_current = (m_current == Color::White ? Color::Black : Color::White);
    }
  }

}
