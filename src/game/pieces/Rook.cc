
# include "Move.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace pieces {
    namespace rook {

      std::vector<Coordinates>
      reachable(const Color& c,
                const Coordinates& p,
                const Board& b) noexcept
      {
        std::vector<Coordinates> out;

        std::vector<Coordinates> d;
        // Positive horizontal.
        d = generate(c, p, 1, 0, -1, b, false, true);
        out.insert(out.end(), d.begin(), d.end());
        // Negative horizontal.
        d = generate(c, p, -1, 0, -1, b, false, true);
        out.insert(out.end(), d.begin(), d.end());
        // Positive vertical.
        d = generate(c, p, 0, 1, -1, b, false, true);
        out.insert(out.end(), d.begin(), d.end());
        // Negative vertical.
        d = generate(c, p, 0, -1, -1, b, false, true);
        out.insert(out.end(), d.begin(), d.end());

        return out;
      }

      bool
      valid(const Color& /*c*/,
            const Coordinates& start,
            const Coordinates& end,
            const Board& b) noexcept
      {
        // Rooks can only move horizontally or vertically.
        int dx, dy;
        differentials(start, end, dx, dy);

        if (dx != 0 && dy != 0) {
          return false;
        }

        if (dx != 0) {
          return obstructed(b, start, (dx > 0 ? 1 : -1), 0, std::abs(dx));
        }

        // Assume vertical move.
        return obstructed(b, start, 0, (dy > 0 ? 1 : -1), std::abs(dy));
      }

    }
  }
}
