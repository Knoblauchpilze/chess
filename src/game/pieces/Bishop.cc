
# include "Move.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace pieces {
    namespace bishop {

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

      std::vector<Coordinates>
      threaten(const Color& /*c*/,
               const Coordinates& p,
               const Board& b) noexcept
      {
        // A bishop threatens all the diagonal cells from
        // its position, as long but up to the blocked cell
        // or the edge of the board.
        std::vector<Coordinates> out;

        // Top right diagonal.
        int x = p.x() + 1;
        int y = p.y() + 1;
        bool blocked = false;

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
        // A bishop can move diagonally for as long as the
        // cells are not obstructed.
        std::vector<Coordinates> out;

        // Top right diagonal.
        int x = p.x() + 1;
        int y = p.y() + 1;
        bool blocked = false;

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
