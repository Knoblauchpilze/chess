
# include "Board.hh"
# include "Move.hh"

namespace chess {

  Board::Board(int  width,
               int  height) noexcept:
    utils::CoreObject("board"),

    m_width(width),
    m_height(height),

    // White by default.
    m_board(width * height, {pieces::White, pieces::None}),

    m_index(1u),
    m_current(pieces::White),
    m_move(m_index),
    m_moves()
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

  chess::pieces::Color
  Board::getPlayer() const noexcept {
    return m_current;
  }

  pieces::Cell
  Board::at(int x, int y) const {
    if (x >= m_width || y >= m_height) {
      error(
        "Failed to fetch board piece",
        "Invalid coordinate " + std::to_string(x) + "x" + std::to_string(y)
      );
    }

    return m_board[linear(x, y)];
  }

  pieces::Cell
  Board::at(const Coordinates& c) const {
    return at(c.x(), c.y());
  }

  bool
  Board::move(const Coordinates& start, const Coordinates& end) {
    // Check piece at the start and end location.
    pieces::Cell s = at(start);
    pieces::Cell e = at(end);

    if (s.type == pieces::None) {
      warn("Failed to move from " + start.toString(), "Empty location");
      return false;
    }
    if (e.type != pieces::None && s.color == e.color) {
      warn("Move from " + start.toString() + " would conflict with " + end.toString());
      return false;
    }

    // Prevent wrong pieces to move.
    if (s.color != m_current) {
      warn("Trying to move " + pieces::toString(s.color) + " when " + pieces::toString(m_current) + " should play");
      return false;
    }

    // Make sure the move is valid.
    pieces::Cell sp = m_board[linear(start)];
    if (!pieces::valid(sp.type, s.color, start, end, *this)) {
      warn("Move from " + start.toString() + " to " + end.toString() + " for " + pieces::toString(sp.type) + " is invalid");
      return false;
    }


    // Register the move.
    m_move.registerMove(s.color, s.type, start, end, e.type, false, false);

    if (m_move.valid()) {
      m_moves.push_back(m_move);
      m_move = Move(m_index);

      ++m_index;

      log("Adding move " + m_moves.back().toString());
    }

    m_current = (m_current == pieces::White ? pieces::Black : pieces::White);

    // Swap the piece at the starting position with the
    // one at the end position. We also need to erase
    // the data at the starting position.
    m_board[linear(end)] = sp;
    m_board[linear(start)].type = pieces::None;

    return true;
  }

  void
  Board::initialize() noexcept {
    // Whites.
    m_board[cells::A1] = { pieces::White, pieces::Rook };
    m_board[cells::B1] = { pieces::White, pieces::Knight };
    m_board[cells::C1] = { pieces::White, pieces::Bishop };
    m_board[cells::D1] = { pieces::White, pieces::Queen };
    m_board[cells::E1] = { pieces::White, pieces::King };
    m_board[cells::F1] = { pieces::White, pieces::Bishop };
    m_board[cells::G1] = { pieces::White, pieces::Knight };
    m_board[cells::H1] = { pieces::White, pieces::Rook };

    m_board[cells::A2] = { pieces::White, pieces::Pawn };
    m_board[cells::B2] = { pieces::White, pieces::Pawn };
    m_board[cells::C2] = { pieces::White, pieces::Pawn };
    m_board[cells::D2] = { pieces::White, pieces::Pawn };
    m_board[cells::E2] = { pieces::White, pieces::Pawn };
    m_board[cells::F2] = { pieces::White, pieces::Pawn };
    m_board[cells::G2] = { pieces::White, pieces::Pawn };
    m_board[cells::H2] = { pieces::White, pieces::Pawn };

    // Blacks.
    m_board[cells::A7] = { pieces::Black, pieces::Pawn };
    m_board[cells::B7] = { pieces::Black, pieces::Pawn };
    m_board[cells::C7] = { pieces::Black, pieces::Pawn };
    m_board[cells::D7] = { pieces::Black, pieces::Pawn };
    m_board[cells::E7] = { pieces::Black, pieces::Pawn };
    m_board[cells::F7] = { pieces::Black, pieces::Pawn };
    m_board[cells::G7] = { pieces::Black, pieces::Pawn };
    m_board[cells::H7] = { pieces::Black, pieces::Pawn };

    m_board[cells::A8] = { pieces::Black, pieces::Rook };
    m_board[cells::B8] = { pieces::Black, pieces::Knight };
    m_board[cells::C8] = { pieces::Black, pieces::Bishop };
    m_board[cells::D8] = { pieces::Black, pieces::Queen };
    m_board[cells::E8] = { pieces::Black, pieces::King };
    m_board[cells::F8] = { pieces::Black, pieces::Bishop };
    m_board[cells::G8] = { pieces::Black, pieces::Knight };
    m_board[cells::H8] = { pieces::Black, pieces::Rook };
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
