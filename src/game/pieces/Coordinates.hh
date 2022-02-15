#ifndef    COORDINATES_HH
# define   COORDINATES_HH

# include <string>
# include <memory>
# include <unordered_set>
# include "Color.hh"

namespace chess {
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

    std::string
    row(const Value& v) noexcept;

    Value
    fromString(const std::string& s);
  }

  class Coordinates {
    public:

      explicit
      Coordinates() noexcept;

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

  /// @brief - Convenience define for a pointer on coordinates.
  using CoordinatesShPtr = std::shared_ptr<Coordinates>;

  /// @brief - Convenience define for a list of unique coordinates.
  using CoordinatesSet = std::unordered_set<Coordinates>;

  /**
   * @brief - Used to convert the coordinates expressed in raw
   *          cells to something that is usable in the board.
   * @param x - the raw coordinate along the x axis.
   * @param y - the raw coordinate along the y axis.
   * @param w - the width of the board.
   * @param h - the height of the board.
   * @param c - the color, used to adapt for the direction the
   *            board is displayed.
   * @param fx - optional output argument which will return the
   *             floating coordinates in the cell along the x
   *             axis.
   * @param fy - optional output argument which will return the
   *             floating coordinates in the cell along the y
   *             axis.
   * @return - a pointer to the generated coordinates.
   */
  CoordinatesShPtr
  convertCoords(float x,
                float y,
                float w,
                float h,
                const Color& c,
                float* fx = nullptr,
                float* fy = nullptr) noexcept;

}

# include "Coordinates_std_specialization.hh"

#endif    /* COORDINATES_HH */
