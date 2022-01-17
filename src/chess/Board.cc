
# include "Board.hh"

namespace chess {

  Board::Board(unsigned  width,
               unsigned  height) noexcept:
    m_width(width),
    m_height(height),

    m_board(width * height)
  {
    initialize();
  }

  void
  Board::initialize() noexcept {
    // Whites.
    m_board[cells::A1] = pieces::Type::Rook;
    m_board[cells::B1] = pieces::Type::Knight;
    m_board[cells::C1] = pieces::Type::Bishop;
    m_board[cells::D1] = pieces::Type::Queen;
    m_board[cells::E1] = pieces::Type::King;
    m_board[cells::F1] = pieces::Type::Bishop;
    m_board[cells::G1] = pieces::Type::Knight;
    m_board[cells::H1] = pieces::Type::Rook;

    m_board[cells::A2] = pieces::Type::Pawn;
    m_board[cells::B2] = pieces::Type::Pawn;
    m_board[cells::C2] = pieces::Type::Pawn;
    m_board[cells::D2] = pieces::Type::Pawn;
    m_board[cells::E2] = pieces::Type::Pawn;
    m_board[cells::F2] = pieces::Type::Pawn;
    m_board[cells::G2] = pieces::Type::Pawn;
    m_board[cells::H2] = pieces::Type::Pawn;

    // Blacks.
    m_board[cells::A7] = pieces::Type::Pawn;
    m_board[cells::B7] = pieces::Type::Pawn;
    m_board[cells::C7] = pieces::Type::Pawn;
    m_board[cells::D7] = pieces::Type::Pawn;
    m_board[cells::E7] = pieces::Type::Pawn;
    m_board[cells::F7] = pieces::Type::Pawn;
    m_board[cells::G7] = pieces::Type::Pawn;
    m_board[cells::H7] = pieces::Type::Pawn;

    m_board[cells::A8] = pieces::Type::Rook;
    m_board[cells::B8] = pieces::Type::Knight;
    m_board[cells::C8] = pieces::Type::Bishop;
    m_board[cells::D8] = pieces::Type::Queen;
    m_board[cells::E8] = pieces::Type::King;
    m_board[cells::F8] = pieces::Type::Bishop;
    m_board[cells::G8] = pieces::Type::Knight;
    m_board[cells::H8] = pieces::Type::Rook;
  }

}
