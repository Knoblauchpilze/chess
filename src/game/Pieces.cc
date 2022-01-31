
# include "Pieces.hh"
# include <algorithm>
# include <core_utils/CoreException.hh>

namespace chess {
  namespace cells {

    std::string
    toString(const Value& v) noexcept {
      switch (v) {
        case Value::A1:
          return "a1";
        case Value::B1:
          return "b1";
        case Value::C1:
          return "c1";
        case Value::D1:
          return "d1";
        case Value::E1:
          return "e1";
        case Value::F1:
          return "f1";
        case Value::G1:
          return "g1";
        case Value::H1:
          return "h1";
        case Value::A2:
          return "a2";
        case Value::B2:
          return "b2";
        case Value::C2:
          return "c2";
        case Value::D2:
          return "d2";
        case Value::E2:
          return "e2";
        case Value::F2:
          return "f2";
        case Value::G2:
          return "g2";
        case Value::H2:
          return "h2";
        case Value::A3:
          return "a3";
        case Value::B3:
          return "b3";
        case Value::C3:
          return "c3";
        case Value::D3:
          return "d3";
        case Value::E3:
          return "e3";
        case Value::F3:
          return "f3";
        case Value::G3:
          return "g3";
        case Value::H3:
          return "h3";
        case Value::A4:
          return "a4";
        case Value::B4:
          return "b4";
        case Value::C4:
          return "c4";
        case Value::D4:
          return "d4";
        case Value::E4:
          return "e4";
        case Value::F4:
          return "f4";
        case Value::G4:
          return "g4";
        case Value::H4:
          return "h4";
        case Value::A5:
          return "a5";
        case Value::B5:
          return "b5";
        case Value::C5:
          return "c5";
        case Value::D5:
          return "d5";
        case Value::E5:
          return "e5";
        case Value::F5:
          return "f5";
        case Value::G5:
          return "g5";
        case Value::H5:
          return "h5";
        case Value::A6:
          return "a6";
        case Value::B6:
          return "b6";
        case Value::C6:
          return "c6";
        case Value::D6:
          return "d6";
        case Value::E6:
          return "e6";
        case Value::F6:
          return "f6";
        case Value::G6:
          return "g6";
        case Value::H6:
          return "h6";
        case Value::A7:
          return "a7";
        case Value::B7:
          return "b7";
        case Value::C7:
          return "c7";
        case Value::D7:
          return "d7";
        case Value::E7:
          return "e7";
        case Value::F7:
          return "f7";
        case Value::G7:
          return "g7";
        case Value::H7:
          return "h7";
        case Value::A8:
          return "a8";
        case Value::B8:
          return "b8";
        case Value::C8:
          return "c8";
        case Value::D8:
          return "d8";
        case Value::E8:
          return "e8";
        case Value::F8:
          return "f8";
        case Value::G8:
          return "g8";
        case Value::H8:
          return "h8";
      }

      // Shouldn't happen.
      return "unknown";
    }

    Value
    fromString(const std::string& s) {
      std::string low = s;
      std::for_each(
        low.begin(),
        low.end(),
        [](char c) {
          return std::tolower(c);
        }
      );

      if (low.size() != 2) {
        throw utils::CoreException(
          "Failed to convert \"" + s + "\" to cell",
          "coordinates",
          "chess",
          "Invalid input format"
        );
      }
      if (low[0] < 'a' || low[0] > 'h') {
        throw utils::CoreException(
          "Failed to convert \"" + s + "\" to cell",
          "coordinates",
          "chess",
          "Invalid input format"
        );
      }
      if (low[1] < '1' || low[1] > '8') {
        throw utils::CoreException(
          "Failed to convert \"" + s + "\" to cell",
          "coordinates",
          "chess",
          "Invalid input format"
        );
      }

      unsigned x = 0u;
      unsigned y = 0u;

      if (low[0] == 'a') {
        x = 0u;
      }
      else if (low[0] == 'b') {
        x = 1u;
      }
      else if (low[0] == 'c') {
        x = 2u;
      }
      else if (low[0] == 'd') {
        x = 3u;
      }
      else if (low[0] == 'e') {
        x = 4u;
      }
      else if (low[0] == 'f') {
        x = 5u;
      }
      else if (low[0] == 'g') {
        x = 6u;
      }
      else { // low[0] == 'h'
        x = 7u;
      }

      if (low[1] == '1') {
        y = 0u;
      }
      else if (low[1] == '2') {
        y = 1u;
      }
      else if (low[1] == '3') {
        y = 2u;
      }
      else if (low[1] == '4') {
        y = 3u;
      }
      else if (low[1] == '5') {
        y = 4u;
      }
      else if (low[1] == '6') {
        y = 5u;
      }
      else if (low[1] == '7') {
        y = 6u;
      }
      else { // low[1] == '8'
        y = 7u;
      }

      return static_cast<cells::Value>(y * 8u + x);
    }
  }

  Coordinates::Coordinates(unsigned x, unsigned y) noexcept:
    m_x(x),
    m_y(y)
  {}

  Coordinates::Coordinates(const cells::Value& v) noexcept:
    m_x(0u),
    m_y(0u)
  {
    m_x = static_cast<unsigned>(v) % 8u;
    m_y = static_cast<unsigned>(v) / 8u;
  }

  cells::Value
  Coordinates::asValue() const {
    if (m_x > 7u || m_y > 7u) {
      throw utils::CoreException(
        "Unable to convert coordinates to cell",
        "coordinates",
        "chess",
        "Invalid coordinates " + std::to_string(m_x) + "x" + std::to_string(m_y)
      );
    }

    return static_cast<cells::Value>(m_y * 8u + m_x);
  }

  unsigned
  Coordinates::x() const noexcept {
    return m_x;
  }

  unsigned
  Coordinates::y() const noexcept {
    return m_y;
  }

  std::string
  Coordinates::toString() const noexcept {
    std::string out;

    out += "[";
    out += std::to_string(m_x);
    out += "x";
    out += std::to_string(m_y);
    out += ", ";
    if (m_x > 8u || m_y > 8u) {
      out += "N/A";
    }
    else {
      out += ('a' + m_x);
      out += ('0' + m_y);
    }
    out += "]";

    return out;
  }

}
