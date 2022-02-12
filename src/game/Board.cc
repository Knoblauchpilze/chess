
# include "Board.hh"
# include <unordered_set>
# include <algorithm>
# include "Round.hh"

namespace chess {

  Board::Board(int  width,
               int  height) noexcept:
    utils::CoreObject("board"),

    m_width(width),
    m_height(height),

    // White by default.
    m_board(width * height, Piece::generate()),

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

  int
  Board::w() const noexcept {
    return m_width;
  }

  int
  Board::h() const noexcept {
    return m_height;
  }

  Color
  Board::getPlayer() const noexcept {
    return m_current;
  }

  Round
  Board::getCurrentRound() const noexcept {
    return m_round;
  }

  Round
  Board::getLastRound() const noexcept {
    if (m_rounds.empty()) {
      return m_round;
    }

    return m_rounds.back();
  }

  bool
  Board::isInCheck(const Color& color) const noexcept {
    if (!m_state.dirty) {
      return (color == Color::White ? m_state.white.check : m_state.black.check);
    }

    updateState();

    // Call the same routine.
    return isInCheck(color);
  }

  bool
  Board::isInCheckmate(const Color& color) const noexcept {
    if (!m_state.dirty) {
      return (color == Color::White ? m_state.white.checkmate : m_state.black.checkmate);
    }

    updateState();

    // Call the same routine.
    return isInCheckmate(color);
  }

  bool
  Board::isInStalemate(const Color& color) const noexcept {
    if (!m_state.dirty) {
      return (color == Color::White ? m_state.white.stalemate : m_state.black.stalemate);
    }

    updateState();

    // Call the same routine.
    return isInStalemate(color);
  }

  CoordinatesSet
  Board::availablePositions(const Coordinates& coords) noexcept {
    const Piece& c = at(coords);
    // Case of an empty piece: no available position.
    if (c.invalid()) {
      return CoordinatesSet();
    }

    return c.reachable(coords, *this);
  }

  std::vector<Piece>
  Board::pieces(const Color& color) const noexcept {
    std::vector<Piece> out;

    for (unsigned id = 0u ; id < m_board.size() ; ++id) {
      if (m_board[id].color() == color) {
        out.push_back(m_board[id]);
      }
    }

    return out;
  }

  const Piece&
  Board::at(int x, int y) const {
    if (x >= m_width || y >= m_height) {
      error(
        "Failed to fetch board piece",
        "Invalid coordinate " + std::to_string(x) + "x" + std::to_string(y)
      );
    }

    return m_board[linear(x, y)];
  }

  const Piece&
  Board::at(const Coordinates& c) const {
    return at(c.x(), c.y());
  }

  bool
  Board::move(const Coordinates& start, const Coordinates& end) {
    // Check piece at the start and end location.
    const Piece& sp = m_board[linear(start)];
    const Piece& e = at(end);

    if (sp.invalid()) {
      warn("Failed to move from " + start.toString(), "Empty location");
      return false;
    }
    if (e.valid() && sp.color() == e.color()) {
      warn("Move from " + start.toString() + " would conflict with " + end.toString());
      return false;
    }

    // Prevent wrong pieces to move.
    if (sp.color() != m_current) {
      warn("Trying to move " + colorToString(sp.color()) + " when " + colorToString(m_current) + " should play");
      return false;
    }

    // Make sure the move is valid: check the list of
    // reachable positions and verify that the ending
    // position belong to them.
    CoordinatesSet avail = sp.reachable(start, *this);
    if (avail.count(end) == 0) {
      warn("Move from " + start.toString() + " to " + end.toString() + " for " + sp.name() + " is invalid");
      return false;
    }

    // Discard moves that leaves our king in check.
    if (leadsToCheck(start, end)) {
      warn("Move from " + start.toString() + " to " + end.toString() + " for " + sp.name() + " would lead to a check");
      return false;
    }

    // Register the move.
    m_round.registerMove(sp, start, end, e.valid(), false, false);

    if (m_round.valid()) {
      m_rounds.push_back(m_round);
      ++m_index;
      m_round = Round(m_index);

      log("Adding round " + m_rounds.back().toString());
    }

    m_current = (m_current == Color::White ? Color::Black : Color::White);

    // Swap the piece at the starting position with the
    // one at the end position. We also need to erase
    // the data at the starting position.
    m_board[linear(end)] = sp;
    m_board[linear(start)].reset();

    // Invalidate cached data.
    m_state.dirty = true;

    return true;
  }

  bool
  Board::leadsToCheck(const Coordinates& start, const Coordinates& end) const {
    // Control the inputs.
    if (start.x() < 0 || start.x() >= w() || start.y() < 0 || start.y() >= h()) {
      error(
        "Failed to determine if move leads to check",
        "Invalid starting position " + start.toString()
      );
    }
    if (end.x() < 0 || end.x() >= w() || end.y() < 0 || end.y() >= h()) {
      error(
        "Failed to determine if move leads to check",
        "Invalid ending position " + end.toString()
      );
    }

    // Perform the move while preserving the old position.
    std::vector<Piece> board = m_board;
    Piece st = m_board[linear(start)];
    Piece en = m_board[linear(end)];

    // In case the cell is empty, that's an issue.
    if (!st.valid()) {
      error(
        "Failed to determine if move leads to check",
        "Position " + start.toString() + " is empty"
      );
    }

    m_board[linear(start)].reset();
    m_board[linear(end)] = st;

    // Determine whether the king with the color of the
    // starting position is in check.
    bool status = computeCheck(st.color());

    // Restore the state of the board.
    m_board[linear(start)] = st;
    m_board[linear(end)] = en;

    return status;
  }

  bool
  Board::hasMoved(const Coordinates& /*p*/) const noexcept {
    /// TODO: Handle this.
    return false;
  }

  void
  Board::initialize() noexcept {
    // Whites.
# define ONLY_KING
    m_board[cells::A1] = Piece::generate(Type::Rook, Color::White);
# ifndef ONLY_KING
    m_board[cells::B1] = Piece::generate(Type::Knight, Color::White);
    m_board[cells::C1] = Piece::generate(Type::Bishop, Color::White);
    m_board[cells::D1] = Piece::generate(Type::Queen, Color::White);
# endif
    m_board[cells::E1] = Piece::generate(Type::King, Color::White);
# ifndef ONLY_KING
    m_board[cells::F1] = Piece::generate(Type::Bishop, Color::White);
    m_board[cells::G1] = Piece::generate(Type::Knight, Color::White);
# endif
    m_board[cells::H1] = Piece::generate(Type::Rook, Color::White);

// # define PAWNS
# ifdef PAWNS
    m_board[cells::A2] = Piece::generate(Type::Pawn, Color::White);
    m_board[cells::B2] = Piece::generate(Type::Pawn, Color::White);
    m_board[cells::C2] = Piece::generate(Type::Pawn, Color::White);
    m_board[cells::D2] = Piece::generate(Type::Pawn, Color::White);
    m_board[cells::E2] = Piece::generate(Type::Pawn, Color::White);
    m_board[cells::F2] = Piece::generate(Type::Pawn, Color::White);
    m_board[cells::G2] = Piece::generate(Type::Pawn, Color::White);
    m_board[cells::H2] = Piece::generate(Type::Pawn, Color::White);
# endif

    // Blacks.
# ifdef PAWNS
    m_board[cells::A7] = Piece::generate(Type::Pawn, Color::Black);
    m_board[cells::B7] = Piece::generate(Type::Pawn, Color::Black);
    m_board[cells::C7] = Piece::generate(Type::Pawn, Color::Black);
    m_board[cells::D7] = Piece::generate(Type::Pawn, Color::Black);
    m_board[cells::E7] = Piece::generate(Type::Pawn, Color::Black);
    m_board[cells::F7] = Piece::generate(Type::Pawn, Color::Black);
    m_board[cells::G7] = Piece::generate(Type::Pawn, Color::Black);
    m_board[cells::H7] = Piece::generate(Type::Pawn, Color::Black);
# endif

    m_board[cells::A8] = Piece::generate(Type::Rook, Color::Black);
    m_board[cells::B8] = Piece::generate(Type::Knight, Color::Black);
    m_board[cells::C8] = Piece::generate(Type::Bishop, Color::Black);
    m_board[cells::D8] = Piece::generate(Type::Queen, Color::Black);
    m_board[cells::E8] = Piece::generate(Type::King, Color::Black);
    m_board[cells::F8] = Piece::generate(Type::Bishop, Color::Black);
    m_board[cells::G8] = Piece::generate(Type::Knight, Color::Black);
    m_board[cells::H8] = Piece::generate(Type::Rook, Color::Black);

    // Handle custom initialization.
    initializeCustom();
  }

  void
  Board::initializeCustom() noexcept {
    // Insert custom logic to initialize the board.
  }

  inline
  unsigned
  Board::linear(int x, int y) const noexcept {
    return y * m_width + x;
  }

  inline
  unsigned
  Board::linear(const Coordinates& c) const noexcept {
    return linear(c.x(), c.y());
  }

  inline
  void
  Board::updateState() const noexcept {
    // Check if the king of each color stands among the
    // list of threatened cells.
    m_state.white.check = computeCheck(Color::White);
    m_state.black.check = computeCheck(Color::Black);

    if (m_state.white.check) {
      warn("White is in check");
    }
    if (m_state.black.check) {
      warn("Black is in check");
    }

    bool bStalemate = computeStalemate(Color::White);
    bool wStalemate = computeStalemate(Color::Black);
    m_state.white.stalemate = !m_state.white.check && bStalemate;
    m_state.black.stalemate = !m_state.black.check && wStalemate;

    if (m_state.white.stalemate) {
      warn("White is in stalemate");
    }
    if (m_state.black.stalemate) {
      warn("Black is in stalemate");
    }

    m_state.white.checkmate = m_state.white.check && bStalemate;
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

  bool
  Board::computeCheck(const Color& c) const noexcept {
    // Gather the list of pieces remaining for the color
    // opposite to the input. Also, keep track of
    // position of the king as it will be useful later.
    // Note that we don't check whether the king can be
    // found as it is guaranteed by other means.
    std::vector<std::pair<const Piece, Coordinates>> pieces;
    Coordinates king(0, 0);

    for (int y = 0 ; y < m_height ; ++y) {
      for (int x = 0 ; x < m_width ; ++x) {
        const Piece& ce = m_board[linear(x, y)];

        if (ce.invalid()) {
          continue;
        }

        if (ce.color() != c) {
          pieces.push_back(std::make_pair(ce, Coordinates(x, y)));
        }

        if (ce.king() && ce.color() == c) {
          king = Coordinates(x, y);
        }
      }
    }

    // Compute the list of threats generated by each
    // piece for the other colors.
    CoordinatesSet threats;
    for (unsigned id = 0u ; id < pieces.size() ; ++id) {
      CoordinatesSet t = pieces[id].first.reachable(pieces[id].second, *this);
      threats.merge(t);
    }

    // The king is in check if its position appears in
    // the final list of threats.
    return threats.count(king) > 0u;
  }

  bool
  Board::computeStalemate(const Color& c) const noexcept {
    // Gather the pieces of the corresponding color.
    std::vector<std::pair<const Piece, Coordinates>> pieces;

    for (int y = 0 ; y < m_height ; ++y) {
      for (int x = 0 ; x < m_width ; ++x) {
        const Piece& ce = m_board[linear(x, y)];

        if (ce.invalid() || ce.color() != c) {
          continue;
        }

        pieces.push_back(std::make_pair(ce, Coordinates(x, y)));
      }
    }

    // For each piece, traverse the list of possible
    // moves and see if it leaves the king out of the
    // check.
    bool canLeaveCheck = false;
    unsigned id = 0u;

    while (id < pieces.size() && !canLeaveCheck) {
      CoordinatesSet s = pieces[id].first.reachable(pieces[id].second, *this);

      CoordinatesSet::const_iterator it = s.cbegin();
      while (it != s.cend() && !canLeaveCheck) {
        if (!leadsToCheck(pieces[id].second, *it)) {
          log("Move " + pieces[id].first.name() + " (" + colorToString(pieces[id].first.color()) + ", " + colorToString(c) + ") from " + pieces[id].second.toString() + " to " + it->toString() + " prevents check");
          canLeaveCheck = true;
        }

        ++it;
      }

      ++id;
    }

    return !canLeaveCheck;
  }

}
