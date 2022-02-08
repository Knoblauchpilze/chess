
# include "Bishop.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace bishop {

    CoordinatesSet
    reachable(const Color& c,
              const Coordinates& p,
              const Board& b) noexcept
    {
      CoordinatesSet out;

      std::vector<Coordinates> d;
      // Top right diagonal.
      d = generate(c, p, 1, 1, -1, b, false, true);
      out.insert(d.begin(), d.end());
      // Top left diagonal.
      d = generate(c, p, -1, 1, -1, b, false, true);
      out.insert(d.begin(), d.end());
      // Bottom left diagonal.
      d = generate(c, p, -1, -1, -1, b, false, true);
      out.insert(d.begin(), d.end());
      // Bottom right diagonal.
      d = generate(c, p, 1, -1, -1, b, false, true);
      out.insert(d.begin(), d.end());

      return out;
    }

  }
}
