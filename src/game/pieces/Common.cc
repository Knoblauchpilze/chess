
# include "Common.hh"
# include "Board.hh"

namespace chess {
  namespace pieces {

    void
    differentials(const Coordinates& start,
                  const Coordinates& end,
                  int& dx,
                  int& dy) noexcept
    {
      dx = end.x() - start.x();
      dy = end.y() - start.y();
    }

    bool
    obstructed(const Board& b,
               const Coordinates& start,
               int dx,
               int dy,
               unsigned length) noexcept
    {
      unsigned d = 1u;
      int x = start.x() + dx;
      int y = start.y() + dy;

      while (d < length) {
        Cell c = b.at(x, y);
        if (c.type != None) {
          return false;
        }

        x += dx;
        y += dy;
        ++d;
      }

      return true;
    }

  }
}
