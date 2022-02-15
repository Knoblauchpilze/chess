
# include "Knight.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace knight {

    CoordinatesSet
    reachable(const Color& c,
              const Coordinates& p,
              const Board& b) noexcept
    {
      CoordinatesSet out;

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
        if (!b.validCoordinates(coords[id])) {
          continue;
        }

        // Ignore coordinates where there's a piece of
        // the same color.
        const Piece& ce = b.at(coords[id]);
        if (ce.valid() && ce.color() == c) {
          continue;
        }

        out.insert(coords[id]);
      }

      return out;
    }

  }
}
