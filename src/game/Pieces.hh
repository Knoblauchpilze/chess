#ifndef    PIECES_HH
# define   PIECES_HH

# include <string>
# include <memory>

namespace chess {
  /// @brief - Forward declaration of the coordinates class.
  class Coordinates;

  /// @brief - Convenience define for a pointer on coordinates.
  using CoordinatesShPtr = std::shared_ptr<chess::Coordinates>;

  namespace pieces {
    /// @brief - The possible pieces.
    enum Type {
      Pawn,
      Knight,
      Bishop,
      Rook,
      Queen,
      King,
      None
    };

    std::string
    toString(const Type& t) noexcept;

    std::string
    algebraic(const Type& t) noexcept;

    /// @brief - The possible colors.
    enum Color {
      White,
      Black
    };

    std::string
    toString(const Color& c) noexcept;

    /// @brief - A convenience structure to represent a piece.
    struct Cell {
      Color color;
      Type type;
    };
  }

  namespace cells {
    /// @brief - The cells of the board
    enum Value {
      A1,
      B1,
      C1,
      D1,
      E1,
      F1,
      G1,
      H1,
      A2,
      B2,
      C2,
      D2,
      E2,
      F2,
      G2,
      H2,
      A3,
      B3,
      C3,
      D3,
      E3,
      F3,
      G3,
      H3,
      A4,
      B4,
      C4,
      D4,
      E4,
      F4,
      G4,
      H4,
      A5,
      B5,
      C5,
      D5,
      E5,
      F5,
      G5,
      H5,
      A6,
      B6,
      C6,
      D6,
      E6,
      F6,
      G6,
      H6,
      A7,
      B7,
      C7,
      D7,
      E7,
      F7,
      G7,
      H7,
      A8,
      B8,
      C8,
      D8,
      E8,
      F8,
      G8,
      H8
    };

    std::string
    toString(const Value& v) noexcept;

    std::string
    file(const Value& v) noexcept;

    Value
    fromString(const std::string& s);
  }

  class Coordinates {
    public:

      Coordinates(int x, int y) noexcept;

      Coordinates(const cells::Value& v) noexcept;

      bool
      operator==(const Coordinates& rhs) const noexcept;

      bool
      operator!=(const Coordinates& rhs) const noexcept;

      cells::Value
      asValue() const;

      int
      x() const noexcept;

      int
      y() const noexcept;

      std::string
      toString() const noexcept;

    private:

      int m_x;
      int m_y;
  };

}

#endif    /* PIECES_HH */
