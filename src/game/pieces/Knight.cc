
# include "Move.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace pieces {
    namespace knight {

      std::vector<Coordinates>
      reachable(const Color& c,
                const Coordinates& p,
                const Board& b) noexcept
      {
        std::vector<Coordinates> out;

        // Generate the hard coded values for the
        // positions and then filter them based on
        // whether there are within the board.
        std::vector<Coordinates> coords = {
          // Top right quadrant.
          Coordinates(p.x() + 1, p.y() + 2),
          Coordinates(p.x() + 2, p.y() + 1),

          // Bottom right quadrant.
          Coordinates(p.x() + 2, p.y() - 1),
          Coordinates(p.x() + 1, p.y() - 2),

          // Bottom left quadrant.
          Coordinates(p.x() - 1, p.y() - 2),
          Coordinates(p.x() - 2, p.y() - 1),

          // Top left quadrant.
          Coordinates(p.x() - 2, p.y() + 1),
          Coordinates(p.x() - 1, p.y() + 2),
        };

        for (unsigned id = 0u ; id < coords.size() ; ++id) {
          if (coords[id].x() < 0 || coords[id].x() >= b.w()) {
            continue;
          }
          if (coords[id].y() < 0 || coords[id].y() >= b.h()) {
            continue;
          }

          // Ignore coordinates where there's a piece of
          // the same color.
          Cell ce = b.at(coords[id]);
          if (ce.type != None && ce.color == c) {
            continue;
          }

          out.push_back(coords[id]);
        }

        return out;
      }

      bool
      valid(const Color& /*c*/,
            const Coordinates& start,
            const Coordinates& end,
            const Board& /*b*/) noexcept
      {
        // Pawn can move in a weird way and also jump over
        // pieces of the same color (so ignoring any form
        // of obstruction).
        int dx, dy;
        differentials(start, end, dx, dy);

        // Combination of `(dx, dy)` can be:
        // (1, 2) (1, -2)
        // (-1, 2) (-1, -2)
        // (2, 1) (-2, 1)
        // (2, -1), (-2, -1)
        if (std::abs(dx) == 1 && std::abs(dy) == 2) {
          return true;
        }
        if (std::abs(dx) == 2 && std::abs(dy) == 1) {
          return true;
        }

        return false;
      }

    }
  }
}
