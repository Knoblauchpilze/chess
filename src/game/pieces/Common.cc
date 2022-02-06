
# include "Common.hh"
# include "Board.hh"

namespace chess {
  namespace pieces {

    std::vector<Coordinates>
    generate(const Color& c,
             const Coordinates& s,
             int dx,
             int dy,
             int length,
             const Board& b,
             bool includeSelf,
             bool stopWhenBlocked) noexcept
    {
      std::vector<Coordinates> out;

      int x = s.x();
      int y = s.y();
      int count = 0;

      bool xOk = (x >= 0 && x < b.w());
      bool yOk = (y >= 0 && y < b.h());
      bool lOk = (length < 0 || count <= length);
      bool blocked = false;

      while (xOk && yOk && lOk && !blocked) {
        Coordinates co(x, y);

        // Handle self inclusion.
        if (x == s.x() && y == s.y()) {
          if (includeSelf) {
            out.push_back(co);
          }
        }

        if (x != s.x() || y != s.y()) {
          Cell ce = b.at(co);
          blocked = (ce.type != None);

          // We still consider the cell if the color
          // of the piece is different from our own:
          // in this case a capture is possible.
          if (!blocked || ce.color != c) {
            out.push_back(co);
          }
        }

        x += dx;
        y += dy;
        ++count;

        xOk = (x >= 0 && x < b.w());
        yOk = (y >= 0 && y < b.h());
        lOk = (length < 0 || count <= length);
        blocked = blocked && stopWhenBlocked;
      }

      return out;
    }

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
