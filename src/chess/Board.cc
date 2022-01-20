
# include "Board.hh"

namespace chess {

  Board::Board(int  width,
               int  height) noexcept:
    utils::CoreObject("board"),

    m_width(width),
    m_height(height),

    // White by default.
    m_board(width * height, {pieces::White, pieces::None})
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

  pieces::Cell
  Board::at(int x, int y) const {
    if (x >= m_width || y >= m_height) {
      error(
        "Failed to fetch board piece",
        "Invalid coordinate " + std::to_string(x) + "x" + std::to_string(y)
      );
    }

    return m_board[y * m_width + x];
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

}
