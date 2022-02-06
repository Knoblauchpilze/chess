
# include "Move.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace pieces {
    namespace bishop {

      std::vector<Coordinates>
      reachable(const Color& c,
                const Coordinates& p,
                const Board& b) noexcept
      {
        std::vector<Coordinates> out;

        std::vector<Coordinates> d;
        // Top right diagonal.
        d = generate(c, p, 1, 1, -1, b, false, true);
        out.insert(out.end(), d.begin(), d.end());
        // Top left diagonal.
        d = generate(c, p, -1, 1, -1, b, false, true);
        out.insert(out.end(), d.begin(), d.end());
        // Bottom left diagonal.
        d = generate(c, p, -1, -1, -1, b, false, true);
        out.insert(out.end(), d.begin(), d.end());
        // Bottom right diagonal.
        d = generate(c, p, 1, -1, -1, b, false, true);
        out.insert(out.end(), d.begin(), d.end());

        return out;
      }

      bool
      valid(const Color& /*c*/,
            const Coordinates& start,
            const Coordinates& end,
            const Board& b) noexcept
      {
        // Rooks can only move diagonally.
        int dx, dy;
        differentials(start, end, dx, dy);

        if (dx == 0 || dy == 0) {
          return false;
        }

        if (std::abs(dx) != std::abs(dy)) {
          return false;
        }

        return obstructed(b, start, (dx > 0 ? 1 : -1), (dy > 0 ? 1 : -1), std::abs(dx));
      }

    }
  }
}
