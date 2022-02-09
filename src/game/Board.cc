
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
    m_state({false, false, false, false, false}),
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
      return (color == Color::White ? m_state.whiteInCheck : m_state.blackInCheck);
    }

    updateState();

    // Call the same routine.
    return isInCheck(color);
  }

  bool
  Board::isInCheckmate(const Color& color) const noexcept {
    if (!m_state.dirty) {
      return (color == Color::White ? m_state.whiteInCheckmate : m_state.blackInCheckmate);
    }

    updateState();

    // Call the same routine.
    return isInCheckmate(color);
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
  Board::leadsToCheck(const Coordinates& start, const Coordinates& end) {
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

  void
  Board::initialize() noexcept {
    // Whites.
    m_board[cells::A1] = Piece::generate(Type::Rook, Color::White);
    m_board[cells::B1] = Piece::generate(Type::Knight, Color::White);
    m_board[cells::C1] = Piece::generate(Type::Bishop, Color::White);
    m_board[cells::D1] = Piece::generate(Type::Queen, Color::White);
    m_board[cells::E1] = Piece::generate(Type::King, Color::White);
    m_board[cells::F1] = Piece::generate(Type::Bishop, Color::White);
    m_board[cells::G1] = Piece::generate(Type::Knight, Color::White);
    m_board[cells::H1] = Piece::generate(Type::Rook, Color::White);

    m_board[cells::A2] = Piece::generate(Type::Pawn, Color::White);
    m_board[cells::B2] = Piece::generate(Type::Pawn, Color::White);
    m_board[cells::C2] = Piece::generate(Type::Pawn, Color::White);
    m_board[cells::D2] = Piece::generate(Type::Pawn, Color::White);
    m_board[cells::E2] = Piece::generate(Type::Pawn, Color::White);
    m_board[cells::F2] = Piece::generate(Type::Pawn, Color::White);
    m_board[cells::G2] = Piece::generate(Type::Pawn, Color::White);
    m_board[cells::H2] = Piece::generate(Type::Pawn, Color::White);

    // Blacks.
    m_board[cells::A7] = Piece::generate(Type::Pawn, Color::Black);
    m_board[cells::B7] = Piece::generate(Type::Pawn, Color::Black);
    m_board[cells::C7] = Piece::generate(Type::Pawn, Color::Black);
    m_board[cells::D7] = Piece::generate(Type::Pawn, Color::Black);
    m_board[cells::E7] = Piece::generate(Type::Pawn, Color::Black);
    m_board[cells::F7] = Piece::generate(Type::Pawn, Color::Black);
    m_board[cells::G7] = Piece::generate(Type::Pawn, Color::Black);
    m_board[cells::H7] = Piece::generate(Type::Pawn, Color::Black);

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
    m_state.whiteInCheck = computeCheck(Color::White);
    m_state.blackInCheck = computeCheck(Color::Black);

    if (m_state.whiteInCheck) {
      warn("White is in check");
    }
    if (m_state.blackInCheck) {
      warn("Black is in check");
    }

    /// TODO: Handle checkmate by comparing if any of the
    /// accessible squares for the king is not in threat.
    /// We also need to take into consideration possible
    /// moves of other pieces.

    if (m_state.whiteInCheckmate) {
      warn("White is in checkmate");
    }
    if (m_state.blackInCheckmate) {
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
    std::unordered_set<Coordinates> threats;
    for (unsigned id = 0u ; id < pieces.size() ; ++id) {
      std::unordered_set<Coordinates> t = pieces[id].first.reachable(pieces[id].second, *this);
      threats.merge(t);
    }

    // The king is in check if its position appears in
    // the final list of threats.
    return threats.count(king) > 0u;
  }

}
