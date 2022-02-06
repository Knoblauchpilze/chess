
# include "Move.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace pieces {
    namespace queen {

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

      std::vector<Coordinates>
      threaten(const Color& /*c*/,
               const Coordinates& p,
               const Board& b) noexcept
      {
        // A queen threatens all the cells horizontally but
        // also vertically and diagonally from its position,
        // as long but up to the blocked cell or the edge of
        // the board.
        std::vector<Coordinates> out;

        // Positive horizontal.
        int delta = p.x() + 1;
        bool blocked = false;

        while (delta < b.w() && !blocked) {
          Coordinates co(delta, p.y());
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          ++delta;
        }

        // Negative horizontal.
        delta = p.x() - 1;
        blocked = false;

        while (delta >= 0 && !blocked) {
          Coordinates co(delta, p.y());
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          --delta;
        }

        // Positive vertical.
        delta  = p.y() + 1;
        blocked = false;

        while (delta < b.h() && !blocked) {
          Coordinates co(p.x(), delta);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          ++delta;
        }

        // Negative vertical.
        delta = p.y() - 1;
        blocked = false;

        while (delta >= 0 && !blocked) {
          Coordinates co(p.x(), delta);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          --delta;
        }

        // Top right diagonal.
        int x = p.x() + 1;
        int y = p.y() + 1;
        blocked = false;

        while (x < b.w() && y < b.h() && !blocked) {
          Coordinates co(x, y);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          ++x;
          ++y;
        }

        // Top left diagonal.
        x = p.x() - 1;
        y = p.y() + 1;
        blocked = false;

        while (x >= 0 && y < b.h() && !blocked) {
          Coordinates co(x, y);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          --x;
          ++y;
        }

        // Bottom left diagonal.
        x = p.x() - 1;
        y = p.y() - 1;
        blocked = false;

        while (x >= 0 && y >= 0 && !blocked) {
          Coordinates co(x, y);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          --x;
          --y;
        }

        // Bottom right diagonal.
        x = p.x() + 1;
        y = p.y() - 1;
        blocked = false;

        while (x < b.w() && y >= 0 && !blocked) {
          Coordinates co(x, y);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          ++x;
          --y;
        }

        return out;
      }

      std::vector<Coordinates>
      move(const Color& c,
           const Coordinates& p,
           const Board& b) noexcept
      {
        // A queen threatens all the cells horizontally but
        // also vertically and diagonally from its position,
        // as long but up to the blocked cell or the edge of
        // the board.
        std::vector<Coordinates> out;

        // Positive horizontal.
        int delta = p.x() + 1;
        bool blocked = false;

        while (delta < b.w() && !blocked) {
          Coordinates co(delta, p.y());
          Cell ce = b.at(co);

          blocked = (ce.type != None);

          if (!blocked || ce.color != c) {
            out.push_back(co);
          }

          ++delta;
        }

        // Negative horizontal.
        delta = p.x() - 1;
        blocked = false;

        while (delta >= 0 && !blocked) {
          Coordinates co(delta, p.y());
          Cell ce = b.at(co);

          blocked = (ce.type != None);

          if (!blocked || ce.color != c) {
            out.push_back(co);
          }

          --delta;
        }

        // Positive vertical.
        delta  = p.y() + 1;
        blocked = false;

        while (delta < b.h() && !blocked) {
          Coordinates co(p.x(), delta);
          Cell ce = b.at(co);

          blocked = (ce.type != None);

          if (!blocked || ce.color != c) {
            out.push_back(co);
          }

          ++delta;
        }

        // Negative vertical.
        delta = p.y() - 1;
        blocked = false;

        while (delta >= 0 && !blocked) {
          Coordinates co(p.x(), delta);
          Cell ce = b.at(co);

          blocked = (ce.type != None);

          if (!blocked || ce.color != c) {
            out.push_back(co);
          }

          --delta;
        }

        // Top right diagonal.
        int x = p.x() + 1;
        int y = p.y() + 1;
        blocked = false;

        while (x < b.w() && y < b.h() && !blocked) {
          Coordinates co(x, y);
          Cell ce = b.at(co);

          blocked = (ce.type != None);

          if (!blocked || ce.color != c) {
            out.push_back(co);
          }

          ++x;
          ++y;
        }

        // Top left diagonal.
        x = p.x() - 1;
        y = p.y() + 1;
        blocked = false;

        while (x >= 0 && y < b.h() && !blocked) {
          Coordinates co(x, y);
          Cell ce = b.at(co);

          blocked = (ce.type != None);

          if (!blocked || ce.color != c) {
            out.push_back(co);
          }

          --x;
          ++y;
        }

        // Bottom left diagonal.
        x = p.x() - 1;
        y = p.y() - 1;
        blocked = false;

        while (x >= 0 && y >= 0 && !blocked) {
          Coordinates co(x, y);
          Cell ce = b.at(co);

          blocked = (ce.type != None);

          if (!blocked || ce.color != c) {
            out.push_back(co);
          }

          --x;
          --y;
        }

        // Bottom right diagonal.
        x = p.x() + 1;
        y = p.y() - 1;
        blocked = false;

        while (x < b.w() && y >= 0 && !blocked) {
          Coordinates co(x, y);
          Cell ce = b.at(co);

          blocked = (ce.type != None);

          if (!blocked || ce.color != c) {
            out.push_back(co);
          }

          ++x;
          --y;
        }

        return out;
      }

    }
  }
}
