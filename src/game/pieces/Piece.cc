
# include "Piece.hh"
# include "Board.hh"

# include "Pawn.hh"
# include "Knight.hh"
# include "Bishop.hh"
# include "Rook.hh"
# include "Queen.hh"
# include "King.hh"

namespace chess {

  std::string
  colorToString(const Color& c) noexcept {
    switch (c) {
      case Color::Black:
        return "black";
      case Color::White:
      default:
        return "white";
    }
  }

  Piece::Piece() noexcept:
    utils::CoreObject("chess"),

    m_type(Type::None),
    m_color(Color::White)
  {
    setService("piece");
  }

  Piece::Piece(const Type& type,
               const Color& color) noexcept:
    utils::CoreObject("chess"),

    m_type(type),
    m_color(color)
  {
    setService("piece");
  }

  Piece
  Piece::generate(const Type& type,
                  const Color& color) noexcept
  {
    return Piece(type, color);
  }

  void
  Piece::reset() noexcept {
    m_type = Type::None;
    m_color = Color::White;
  }

  bool
  Piece::valid() const noexcept {
    return m_type != Type::None;
  }

  bool
  Piece::invalid() const noexcept {
    return !valid();
  }

  std::string
  Piece::algebraic() const noexcept {
    switch (m_type) {
      case Type::Pawn:
        // Empty string for a pawn.
        return "";
      case Type::Knight:
        return "N";
      case Type::Bishop:
        return "B";
      case Type::Rook:
        return "R";
      case Type::Queen:
        return "Q";
      case Type::King:
        return "K";
      case Type::None:
      default:
        return "?";
    }
  }

  Color
  Piece::color() const noexcept {
    return m_color;
  }

  std::string
  Piece::name() const noexcept {
    switch (m_type) {
      case Type::Pawn:
        return "pawn";
      case Type::Knight:
        return "knight";
      case Type::Bishop:
        return "bishop";
      case Type::Rook:
        return "rook";
      case Type::Queen:
        return "queen";
      case Type::King:
        return "king";
      case Type::None:
      default:
        return "none";
    }
  }

  bool
  Piece::pawn() const noexcept {
    return m_type == Type::Pawn;
  }

  bool
  Piece::knight() const noexcept {
    return m_type == Type::Knight;
  }

  bool
  Piece::bishop() const noexcept {
    return m_type == Type::Bishop;
  }

  bool
  Piece::rook() const noexcept {
    return m_type == Type::Rook;
  }

  bool
  Piece::queen() const noexcept {
    return m_type == Type::Queen;
  }

  bool
  Piece::king() const noexcept {
    return m_type == Type::King;
  }

  CoordinatesSet
  Piece::reachable(const Coordinates& p,
                   const Board& b) const noexcept
  {
    switch (m_type) {
      case Type::Pawn:
        return pawn::reachable(m_color, p, b);
      case Type::Knight:
        return knight::reachable(m_color, p, b);
      case Type::Bishop:
        return bishop::reachable(m_color, p, b);
      case Type::Rook:
        return rook::reachable(m_color, p, b);
      case Type::Queen:
        return queen::reachable(m_color, p, b);
      case Type::King:
        return king::reachable(m_color, p, b);
      case Type::None:
      default:
        return CoordinatesSet();
    }
  }

}
