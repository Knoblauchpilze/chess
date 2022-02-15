
# include "Board.hh"

namespace chess {

  Board::Board(int  width,
               int  height) noexcept:
    utils::CoreObject("board"),

    m_width(width),
    m_height(height),

    m_board(),
    m_last(-1, -1)
  {
    setService("chess");

    initialize();
  }

  Board::Board(const Board& b) noexcept:
    utils::CoreObject("board"),

    m_width(b.m_width),
    m_height(b.m_height),

    m_board(b.m_board),
    m_last(b.m_last)
  {
    setService("chess");
  }

  int
  Board::w() const noexcept {
    return m_width;
  }

  int
  Board::h() const noexcept {
    return m_height;
  }

  bool
  Board::validCoordinates(const Coordinates& c) const noexcept {
    return c.x() >= 0 && c.x() < w() && c.y() >= 0 && c.y() < h();
  }

  void
  Board::initialize() noexcept {
    // Initialize the board and any custom initialization.
    m_board = std::vector<PieceData>(w() * h(), {Piece::generate(), false});

    // Whites.
    m_board[cells::A1] = {Piece::generate(Type::Rook, Color::White), false};
    m_board[cells::B1] = {Piece::generate(Type::Knight, Color::White), false};
    m_board[cells::C1] = {Piece::generate(Type::Bishop, Color::White), false};
    m_board[cells::D1] = {Piece::generate(Type::Queen, Color::White), false};
    m_board[cells::E1] = {Piece::generate(Type::King, Color::White), false};
    m_board[cells::F1] = {Piece::generate(Type::Bishop, Color::White), false};
    m_board[cells::G1] = {Piece::generate(Type::Knight, Color::White), false};
    m_board[cells::H1] = {Piece::generate(Type::Rook, Color::White), false};

    m_board[cells::A2] = {Piece::generate(Type::Pawn, Color::White), false};
    m_board[cells::B2] = {Piece::generate(Type::Pawn, Color::White), false};
    m_board[cells::C2] = {Piece::generate(Type::Pawn, Color::White), false};
    m_board[cells::D2] = {Piece::generate(Type::Pawn, Color::White), false};
    m_board[cells::E2] = {Piece::generate(Type::Pawn, Color::White), false};
    m_board[cells::F2] = {Piece::generate(Type::Pawn, Color::White), false};
    m_board[cells::G2] = {Piece::generate(Type::Pawn, Color::White), false};
    m_board[cells::H2] = {Piece::generate(Type::Pawn, Color::White), false};

    // Blacks.
    m_board[cells::A7] = {Piece::generate(Type::Pawn, Color::Black), false};
    m_board[cells::B7] = {Piece::generate(Type::Pawn, Color::Black), false};
    m_board[cells::C7] = {Piece::generate(Type::Pawn, Color::Black), false};
    m_board[cells::D7] = {Piece::generate(Type::Pawn, Color::Black), false};
    m_board[cells::E7] = {Piece::generate(Type::Pawn, Color::Black), false};
    m_board[cells::F7] = {Piece::generate(Type::Pawn, Color::Black), false};
    m_board[cells::G7] = {Piece::generate(Type::Pawn, Color::Black), false};
    m_board[cells::H7] = {Piece::generate(Type::Pawn, Color::Black), false};

    m_board[cells::A8] = {Piece::generate(Type::Rook, Color::Black), false};
    m_board[cells::B8] = {Piece::generate(Type::Knight, Color::Black), false};
    m_board[cells::C8] = {Piece::generate(Type::Bishop, Color::Black), false};
    m_board[cells::D8] = {Piece::generate(Type::Queen, Color::Black), false};
    m_board[cells::E8] = {Piece::generate(Type::King, Color::Black), false};
    m_board[cells::F8] = {Piece::generate(Type::Bishop, Color::Black), false};
    m_board[cells::G8] = {Piece::generate(Type::Knight, Color::Black), false};
    m_board[cells::H8] = {Piece::generate(Type::Rook, Color::Black), false};
  }

  const Piece&
  Board::at(int x, int y) const {
    if (x >= m_width || y >= m_height) {
      error(
        "Failed to fetch board piece",
        "Invalid coordinate " + std::to_string(x) + "x" + std::to_string(y)
      );
    }

    return m_board[linear(x, y)].item;
  }

  const Piece&
  Board::at(const Coordinates& c) const {
    return at(c.x(), c.y());
  }

  Pieces
  Board::pieces(const Color& color) const noexcept {
    Pieces out;

    for (unsigned id = 0u ; id < m_board.size() ; ++id) {
      if (m_board[id].item.valid() && m_board[id].item.color() == color) {
        out.push_back(std::make_pair(
          m_board[id].item,
          Coordinates(id % w(), id / w())
        ));
      }
    }

    return out;
  }

  CoordinatesSet
  Board::availablePositions(const Coordinates& coords) const noexcept {
    const Piece& c = at(coords);
    // Case of an empty piece: no available position.
    if (c.invalid()) {
      return CoordinatesSet();
    }

    CoordinatesSet tmp = c.reachable(coords, *this);

    // Filter coordinates that lead to the king being
    // in check.
    CoordinatesSet out;
    for (CoordinatesSet::const_iterator it = tmp.cbegin() ; it != tmp.cend() ; ++it) {
      if (!leadsToCheck(coords, *it)) {
        out.insert(*it);
      }
    }

    return out;
  }

  bool
  Board::hasMoved(const Coordinates& p) const noexcept {
    if (!validCoordinates(p)) {
      error(
        "Failed to check if piece at " + p.toString() + " has moved",
        "Invalid coordinates"
      );
    }

    const PieceData& c = m_board[linear(p)];
    return c.moved;
  }

  bool
  Board::justMoved(const Coordinates& p) const noexcept {
    return p == m_last;
  }

  bool
  Board::computeCheck(const Color& c) const noexcept {
    // Gather the list of pieces remaining for the color
    // opposite to the input. Also, keep track of
    // position of the king as it will be useful later.
    // Note that we don't check whether the king can be
    // found as it is guaranteed by other means.
    Pieces attackers = pieces(oppositeColor(c));
    Coordinates king;

    unsigned id = 0u;
    bool found = false;
    while (id < m_board.size() && !found) {
      const Piece& p = m_board[id].item;
      if (p.king() && p.color() == c) {
        king = Coordinates(id % w(), id / w());
        found = true;
      }

      ++id;
    }

    // Compute the list of threats generated by each
    // piece for the other colors.
    CoordinatesSet threats;
    for (unsigned id = 0u ; id < attackers.size() ; ++id) {
      CoordinatesSet t = attackers[id].first.reachable(attackers[id].second, *this);
      threats.merge(t);
    }

    // The king is in check if its position appears in
    // the final list of threats.
    return threats.count(king) > 0u;
  }

  bool
  Board::computeStalemate(const Color& c) const noexcept {
    // Gather the pieces of the corresponding color.
    Pieces av = pieces(c);

    // For each piece, traverse the list of possible
    // moves and see if it leaves the king out of the
    // check.
    bool canLeaveCheck = false;
    unsigned id = 0u;

    while (id < av.size() && !canLeaveCheck) {
      CoordinatesSet s = av[id].first.reachable(av[id].second, *this);

      CoordinatesSet::const_iterator it = s.cbegin();
      while (it != s.cend() && !canLeaveCheck) {
        if (!leadsToCheck(av[id].second, *it)) {
          log(
            "Move " + av[id].first.name() +
            " (" + colorToString(av[id].first.color()) + ", " + colorToString(c) +
            ") from " + av[id].second.toString() +
            " to " + it->toString() +
            " prevents check",
            utils::Level::Verbose
          );

          canLeaveCheck = true;
        }

        ++it;
      }

      ++id;
    }

    return !canLeaveCheck;
  }

  bool
  Board::leadsToCheck(const Coordinates& start, const Coordinates& end) const {
    // Control the inputs.
    if (!validCoordinates(start)) {
      error(
        "Failed to determine if move leads to check",
        "Invalid starting position " + start.toString()
      );
    }
    if (!validCoordinates(end)) {
      error(
        "Failed to determine if move leads to check",
        "Invalid ending position " + end.toString()
      );
    }

    // Perform the move while preserving the old position.
    PieceData st = m_board[linear(start)];
    PieceData en = m_board[linear(end)];

    // In case the cell is empty, that's an issue.
    if (!st.item.valid()) {
      error(
        "Failed to determine if move leads to check",
        "Position " + start.toString() + " is empty"
      );
    }

    bool moved = st.moved;
    st.moved = true;

    m_board[linear(start)].item.reset();
    m_board[linear(end)] = st;

    // Determine whether the king with the color of the
    // starting position is in check.
    bool status = computeCheck(st.item.color());

    // Restore the state of the board.
    st.moved = moved;
    m_board[linear(start)] = st;
    m_board[linear(end)] = en;

    return status;
  }

  bool
  Board::legal(const Coordinates& start, const Coordinates& end) {
    // Check piece at the start and end location.
    const Piece& sp = at(start);
    const Piece& e = at(end);

    if (sp.invalid()) {
      warn("Failed to move from " + start.toString(), "Empty location");
      return false;
    }
    if (e.valid() && sp.color() == e.color()) {
      warn("Move from " + start.toString() + " would conflict with " + end.toString());
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

    return true;
  }

  void
  Board::move(const Coordinates& start, const Coordinates& end) {
    // Fetch starting and ending position.
    PieceData sp = m_board[linear(start)];
    PieceData e = m_board[linear(end)];

    // Swap the piece at the starting position with the
    // one at the end position. We also need to erase
    // the data at the starting position.
    sp.moved = true;
    m_board[linear(end)] = sp;
    m_board[linear(start)].item.reset();

    // Handle case of castling.
    if (sp.item.king() && std::abs(start.x() - end.x()) > 1) {
      // We consider that the move is allowed so we just
      // need to move the position of the rook.
      Coordinates rs(start.x() < end.x() ? w() - 1 : 0, start.y());
      Coordinates re(start.x() < end.x() ? end.x() - 1 : end.x() + 1, start.y());

      PieceData r = m_board[linear(rs)];
      m_board[linear(re)] = r;
      m_board[linear(rs)].item.reset();
    }

    // Handle case of en passant.
    if (sp.item.pawn() && start.x() != end.x() && !e.item.valid()) {
      Coordinates cp(end.x() , start.y());
      m_board[linear(cp)].item.reset();
    }

    // Register the last move.
    m_last = end;
  }

  void
  Board::promote(const Coordinates& p, const Type& promote) {
    if (!validCoordinates(p)) {
      error(
        "Failed to promote " + p.toString() + " to " + pieceToString(promote),
        "Invalid coordinates"
      );
    }

    Piece& pi = m_board[linear(p)].item;

    if (!pi.valid() || !pi.pawn()) {
      error(
        "Failed to promote " + p.toString() + " to " + pieceToString(promote),
        "Piece has type " + pi.name()
      );
    }

    log("Promoting " + colorToString(pi.color()) + " " + pi.name() + " to " + pieceToString(promote), utils::Level::Info);

    pi = Piece::generate(promote, pi.color());
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

}
