
# include "Move.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace pieces {
    namespace queen {

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
        // Queens can move any direction as long as it
        // is a horizontal, vertical or diagonal move.
        int dx, dy;
        differentials(start, end, dx, dy);

        // Vertical move.
        if (dx == 0) {
          return obstructed(b, start, 0, (dy > 0 ? 1 : -1), std::abs(dy));
        }

        if (dy == 0) {
          return obstructed(b, start, (dx > 0 ? 1 : -1), 0, std::abs(dx));
        }

        // Control that the motion is a diagonal move and
        // not some strange combination of horizontal and
        // vertical move.
        if (std::abs(dx) != std::abs(dy)) {
          return false;
        }

        return obstructed(b, start, (dx > 0 ? 1 : -1), (dy > 0 ? 1 : -1), std::abs(dx));
      }

    }
  }
}
