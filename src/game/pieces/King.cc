
# include "Move.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace pieces {
    namespace king {

      bool
      valid(const Color& /*c*/,
            const Coordinates& start,
            const Coordinates& end,
            const Board& /*b*/) noexcept
      {
        // Kings can only move one step at a time.
        int dx, dy;
        differentials(start, end, dx, dy);

        if (dx > 1 || dy > 1) {
          return false;
        }

        /// TODO: Prevent king from moving in check: this can be
        /// done by refining the list of threats and persisiting
        /// it in the state and then making that accessible with
        /// a dedicated method.
        /// TODO: Allow king to castle.
        return true;
      }

      std::vector<Coordinates>
      threaten(const Color& /*c*/,
               const Coordinates& p,
               const Board& b) noexcept
      {
        // A king threatens all adjacent cells as long as
        // they are part of the board.
        std::vector<Coordinates> out;

        for (int y = p.y() - 1 ; y <= p.y() + 1 ; ++y) {
          if (y < 0 || y >= b.h()) {
            continue;
          }

          for (int x = p.x() - 1 ; x <= p.x() + 1 ; ++x) {
            if (x < 0 || x >= b.w()) {
              continue;
            }

            // Ignore own cell.
            if (x == p.x() && y == p.y()) {
              continue;
            }

            out.push_back(Coordinates(x, y));
          }
        }

        return out;
      }

      std::vector<Coordinates>
      move(const Color& c,
           const Coordinates& p,
           const Board& b) noexcept
      {
        // A king can reach all cells around it as long as
        // they are occupied by an enemy piece or nothing.
        std::vector<Coordinates> out;

        for (int y = p.y() - 1 ; y <= p.y() + 1 ; ++y) {
          if (y < 0 || y >= b.h()) {
            continue;
          }

          for (int x = p.x() - 1 ; x <= p.x() + 1 ; ++x) {
            if (x < 0 || x >= b.w()) {
              continue;
            }

            // Ignore own cell.
            if (x == p.x() && y == p.y()) {
              continue;
            }

            // Prevent cells occupied by pieces of the same color
            // to be considered.
            Cell ce = b.at(x, y);
            if (ce.type != None && ce.color == c) {
              continue;
            }

            out.push_back(Coordinates(x, y));
          }
        }

        return out;
      }

    }
  }
}
