
# include "King.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace king {

    CoordinatesSet
    reachable(const Color& c,
              const Coordinates& p,
              const Board& b) noexcept
    {
      CoordinatesSet out;

      // Generate a 3x3 square around the initial
      // position, ignoring invalid values and the
      // starting position.
      for (int y = p.y() - 1 ; y <= p.y() + 1 ; ++y) {
        if (y < 0 || y >= b.h()) {
          continue;
        }

        for (int x = p.x() - 1 ; x <= p.x() + 1 ; ++x) {
          if (x < 0 || x >= b.w()) {
            continue;
          }

          // Ignore own cell.
          if (x == p.x() && y == p.y()) {
            continue;
          }

          // Ignore cell with a piece of the same color.
          const Piece& ce = b.at(x, y);
          if (ce.valid() && ce.color() == c) {
            continue;
          }

          out.insert(Coordinates(x, y));
        }
      }

      return out;
    }

  }
}
