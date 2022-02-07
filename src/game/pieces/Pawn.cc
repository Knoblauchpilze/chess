
# include "Move.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace pieces {
    namespace pawn {

      std::vector<Coordinates>
      reachable(const Color& c,
                const Coordinates& p,
                const Board& b) noexcept
      {
        std::vector<Coordinates> out;

        // Pawns can move forward one or two cells, and also
        // diagonally in case there's a piece to capture or
        // a possibility to perform en passant.
        std::vector<Coordinates> cells;

        int dy = 1;
        // In case the pawn is a black one, the delta along
        // the y axis is negative.
        if (c == Black) {
          dy = -1;
        }

        // Check the cell right in front of the pawn.
        bool clearAhead = false;
        if (p.y() + dy >= 0 && p.y() + dy < b.h()) {
          Cell ce = b.at(p.x(), p.y() + dy);
          if (ce.type == None) {
            out.push_back(Coordinates(p.x(), p.y() + dy));
            clearAhead = true;
          }
        }

        // Check the cell two in front of the pawn in case it
        // is on its first line. We only allow it in case the
        // cell ahead is free.
        if (c == White && p.y() == 1 && clearAhead) {
          Cell ce = b.at(p.x(), p.y() + 2);
          if (ce.type == None) {
            out.push_back(Coordinates(p.x(), p.y() + 2));
          }
        }
        if (c == Black && p.y() == 6 && clearAhead) {
          Cell ce = b.at(p.x(), p.y() - 2);
          if (ce.type == None) {
            out.push_back(Coordinates(p.x(), p.y() - 2));
          }
        }

        // Handle capture on each side.
        Coordinates ctl(p.x() - 1, p.y() + dy);
        if (ctl.x() >= 0 && ctl.x() < b.w() && ctl.y() >= 0 && ctl.y() < b.h()) {
          Cell ce = b.at(ctl);
          if (ce.type != None && ce.color != c) {
            out.push_back(ctl);
          }
        }

        Coordinates ctr(p.x() + 1, p.y() + dy);
        if (ctr.x() >= 0 && ctr.x() < b.w() && ctr.y() >= 0 && ctr.y() < b.h()) {
          Cell ce = b.at(ctr);
          if (ce.type != None && ce.color != c) {
            out.push_back(ctr);
          }
        }

        // Handle en passant on each side. See here:
        // https://en.wikipedia.org/wiki/En_passant#Conditions
        // for more info about how it works.
        /// TODO: Handle last move.
        bool enPassantAllowed = (c == White && p.y() == 4) || (c == Black && p.y() == 3);
        if (enPassantAllowed) {
          Coordinates eptl(p.x() - 1, p.y());
          if (eptl.x() >= 0 && eptl.x() < b.w() && eptl.y() >= 0 && eptl.y() < b.h()) {
            Cell ce = b.at(eptl);
            if (ce.type == Pawn && ce.color != c) {
              // Make sure that the en passant cell is empty.
              Coordinates ep(p.x() - 1, p.y() + dy);
              Cell epce = b.at(ep);
              if (epce.type == None) {
                out.push_back(ep);
              }
            }
          }

          Coordinates eptr(p.x() + 1, p.y());
          if (eptr.x() >= 0 && eptr.x() < b.w() && eptr.y() >= 0 && eptr.y() < b.h()) {
            Cell ce = b.at(eptr);
            if (ce.type == Pawn && ce.color != c) {
              // Make sure that the en passant cell is empty.
              Coordinates ep(p.x() + 1, p.y() + dy);
              Cell epce = b.at(ep);
              if (epce.type == None) {
                out.push_back(ep);
              }
            }
          }
        }

        return out;
      }

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
        return false;
      }

    }
  }
}
