
# include "Move.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace pieces {
    namespace rook {

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

      std::vector<Coordinates>
      threaten(const Color& /*c*/,
               const Coordinates& p,
               const Board& b) noexcept
      {
        // A rook threatens all the horizontal and vertical
        // cells from its position, as long but up to the
        // blocked cell or the edge of the board.
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

        return out;
      }

      std::vector<Coordinates>
      move(const Color& c,
           const Coordinates& p,
           const Board& b) noexcept
      {
        // A rook can move horizontally and diagonally for
        // as long as the cell is not obstructed.
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

        return out;
      }

    }
  }
}
