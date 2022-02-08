
# include "Rook.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace rook {

    CoordinatesSet
    reachable(const Color& c,
              const Coordinates& p,
              const Board& b) noexcept
    {
      CoordinatesSet out;

      std::vector<Coordinates> d;
      // Positive horizontal.
      d = generate(c, p, 1, 0, -1, b, false, true);
      out.insert(d.begin(), d.end());
      // Negative horizontal.
      d = generate(c, p, -1, 0, -1, b, false, true);
      out.insert(d.begin(), d.end());
      // Positive vertical.
      d = generate(c, p, 0, 1, -1, b, false, true);
      out.insert(d.begin(), d.end());
      // Negative vertical.
      d = generate(c, p, 0, -1, -1, b, false, true);
      out.insert(d.begin(), d.end());

      return out;
    }

  }
}
