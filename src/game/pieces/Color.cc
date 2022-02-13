
# include "Color.hh"

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

  Color
  oppositeColor(const Color& c) noexcept {
    return (c == Color::White ? Color::Black : Color::White);
  }

}
