
# include "Move.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace pieces {
    namespace pawn {

      bool
      valid(const Color& c,
            const Coordinates& start,
            const Coordinates& end,
            const Board& b) noexcept
      {
        // Pawn can move only vertically and diagonally
        // when capturing or doing _en passant_.
        int dx, dy;
        differentials(start, end, dx, dy);

        // Backward moves are prevented.
        if (c == White && dy <= 0) {
          return false;
        }
        if (c == Black && dy >= 0) {
          return false;
        }

        if (dx == 0) {
          // Depending on the color we want to allow the
          // move to the top or to the bottom of the board.
          if (c == White && dy < 0) {
            return false;
          }

          if (c == Black && dy > 0) {
            return false;
          }

          // We only allow two cells move in case the pawn
          // is on its initial line.
          if (c == White && start.y() != 1 && dy > 1) {
            return false;
          }
          if (c == White && dy > 2) {
            return false;
          }

          if (c == Black && start.y() != 6 && dy < -1) {
            return false;
          }
          if (c == Black && dy < -2) {
            return false;
          }

          // Prevent move to an obstructed cell.
          Cell ce = b.at(end.x(), end.y());
          if (ce.type != None) {
            return false;
          }

          return true;
        }

        if (std::abs(dy) > 1) {
          return false;
        }

        Cell ce = b.at(end.x(), end.y());
        if (ce.type != None) {
          // In case there's something, we have to make
          // sure that the piece is of opposite color.
          return ce.color != c;
        }

        // Otherwise, handle the en passant case. According
        // to this page: https://en.wikipedia.org/wiki/En_passant
        // The pawns should be in their fifth row.
        if (c == White && start.y() != 4) {
          return false;
        }
        if (c == Black && start.y() != 3) {
          return false;
        }

        if (dx > 0 && start.x() >= b.w() - 2) {
          return false;
        }
        if (dx < 0 && start.x() < 1) {
          return false;
        }

        // There must be an adjacent pawn in the direction
        // of the move.
        Cell toCapture = b.at(end.x(), start.y());
        if (toCapture.color == c || toCapture.type != Pawn) {
          return false;
        }

        // The last move must have been the double-step move
        // from the opposite pawn.
        /// TODO: Handle last move.
        return false;
      }

      std::vector<Coordinates>
      threaten(const Color& c,
               const Coordinates& p,
               const Board& b) noexcept
      {
        // A pawn threatens the two cells in diagonally in front
        // of it as long as they are part of the board.
        std::vector<Coordinates> out;

        int row = p.y() + 1;
        if (c == Black) {
          row = p.y() - 1;
        }

        if (p.x() > 0 && row >= 0 && row < b.h()) {
          out.push_back(Coordinates(p.x() - 1, row));
        }
        if (p.x() < b.w() - 1 && row >= 0 && row < b.h()) {
          out.push_back(Coordinates(p.x() + 1, row));
        }

        return out;
      }

      std::vector<Coordinates>
      move(const Color& c,
           const Coordinates& p,
           const Board& b) noexcept
      {
        // A pawn can move ony forward, one or two cells in case
        // it's its first move. It can also handle the en passant
        // cell if needed.
        std::vector<Coordinates> out;

        std::vector<Coordinates> cells;
        if (c == White) {
          cells = {
            Coordinates(p.x(), p.y() + 1),
            Coordinates(p.x(), p.y() + 2),
            Coordinates(p.x() + 1, p.y() + 1),
            Coordinates(p.x() - 1, p.y() + 1),
          };
        }
        else {
          cells = {
            Coordinates(p.x(), p.y() - 1),
            Coordinates(p.x(), p.y() - 2),
            Coordinates(p.x() + 1, p.y() - 1),
            Coordinates(p.x() - 1, p.y() - 1),
          };
        }

        for (unsigned id = 0u ; id < cells.size() ; ++id) {
          const Coordinates& co = cells[id];

          if (co.x() < 0 || co.x() >= b.w() || co.y() < 0 || co.y() >= b.h()) {
            continue;
          }

          // Unable to move to cells that are occupied by
          // pieces of the same color.
          Cell ce = b.at(co);
          if (ce.type != None && ce.color == c) {
            continue;
          }

          int dx, dy;
          differentials(p, co, dx, dy);

          // Prevent cells that are two rows in advance if
          // the pawn is not on its started row.
          bool firstRow = (c == White && p.y() == 1) || (c == Black && p.y() == 6);
          if (std::abs(dy) > 1 && !firstRow) {
            continue;
          }

          // Handle cases where there's nothing to capture.
          if (dx != 0 && ce.type == None) {
            // En passant might still make that valid.
            Cell tc;
            if (dx > 0) {
              tc = b.at(p.x() + 1, p.y());
            }
            else { // dx < 0
              tc = b.at(p.x() - 1, p.y());
            }

            if (c == White && p.y() == 4 && tc.type == Pawn && tc.color != c) {
              out.push_back(co);
            }
            if (c == Black && p.y() == 3 && tc.type == Pawn && tc.color != c) {
              out.push_back(co);
            }

            continue;
          }

          /// TODO: Handle en passant.
          out.push_back(co);
        }

        return out;
      }

    }
  }
}
