#ifndef    COLOR_HH
# define   COLOR_HH

# include <iostream>

namespace chess {

  /// @brief - The possible colors.
  enum class Color {
    White,
    Black
  };

  /**
   * @brief - Returns a string describing the color.
   * @param c - the color whose name should be retrieved.
   * @return - the corresponding string.
   */
  std::string
  colorToString(const Color& c) noexcept;

  /**
   * @brief - Return the opposite color.
   * @param c - the color to reverse.
   * @return - the opposite color to the input one.
   */
  Color
  oppositeColor(const Color& c) noexcept;

}

#endif    /* COLOR_HH */
