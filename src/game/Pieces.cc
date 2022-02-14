
# include "Pieces.hh"

namespace chess {

  Pieces::Pieces(int  width,
                 int  height) noexcept:
    utils::CoreObject("pieces"),

    m_width(width),
    m_height(height),

    // White by default.
    m_board(width * height, Piece::generate())
  {
    setService("chess");

    initialize();
  }

  int
  Pieces::w() const noexcept {
    return m_width;
  }

  int
  Pieces::h() const noexcept {
    return m_height;
  }

  void
  Pieces::initialize() noexcept {
    // Initialize the board and any custom initialization.
    m_board = std::vector<Piece>(w() * h(), Piece::generate());

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
    m_board[cells::E8] = Piece::generate(Type::King, Color::Black);
    m_board[cells::F8] = Piece::generate(Type::Bishop, Color::Black);
    m_board[cells::G8] = Piece::generate(Type::Knight, Color::Black);
    m_board[cells::H8] = Piece::generate(Type::Rook, Color::Black);
  }

  const Piece&
  Pieces::at(int x, int y) const {
    if (x >= m_width || y >= m_height) {
      error(
        "Failed to fetch board piece",
        "Invalid coordinate " + std::to_string(x) + "x" + std::to_string(y)
      );
    }

    return m_board[linear(x, y)];
  }

  const Piece&
  Pieces::at(const Coordinates& c) const {
    return at(c.x(), c.y());
  }

  inline
  unsigned
  Pieces::linear(int x, int y) const noexcept {
    return y * m_width + x;
  }

  inline
  unsigned
  Pieces::linear(const Coordinates& c) const noexcept {
    return linear(c.x(), c.y());
  }

}
