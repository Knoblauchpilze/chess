
# include "Pawn.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace pawn {

    CoordinatesSet
    reachable(const Color& c,
              const Coordinates& p,
              const Board& b) noexcept
    {
      CoordinatesSet out;

      // Pawns can move forward one or two cells, and also
      // diagonally in case there's a piece to capture or
      // a possibility to perform en passant.
      std::vector<Coordinates> cells;

      int dy = 1;
      // In case the pawn is a black one, the delta along
      // the y axis is negative.
      if (c == Color::Black) {
        dy = -1;
      }

      // Check the cell right in front of the pawn.
      bool clearAhead = false;
      if (p.y() + dy >= 0 && p.y() + dy < b.h()) {
        const Piece& ce = b.at(p.x(), p.y() + dy);
        if (ce.invalid()) {
          out.insert(Coordinates(p.x(), p.y() + dy));
          clearAhead = true;
        }
      }

      // Check the cell two in front of the pawn in case it
      // is on its first line. We only allow it in case the
      // cell ahead is free.
      if (c == Color::White && p.y() == 1 && clearAhead) {
        const Piece& ce = b.at(p.x(), p.y() + 2);
        if (ce.invalid()) {
          out.insert(Coordinates(p.x(), p.y() + 2));
        }
      }
      if (c == Color::Black && p.y() == 6 && clearAhead) {
        const Piece& ce = b.at(p.x(), p.y() - 2);
        if (ce.invalid()) {
          out.insert(Coordinates(p.x(), p.y() - 2));
        }
      }

      // Handle capture on each side.
      Coordinates ctl(p.x() - 1, p.y() + dy);
      if (ctl.x() >= 0 && ctl.x() < b.w() && ctl.y() >= 0 && ctl.y() < b.h()) {
        const Piece& ce = b.at(ctl);
        if (ce.valid() && ce.color() != c) {
          out.insert(ctl);
        }
      }

      Coordinates ctr(p.x() + 1, p.y() + dy);
      if (ctr.x() >= 0 && ctr.x() < b.w() && ctr.y() >= 0 && ctr.y() < b.h()) {
        const Piece& ce = b.at(ctr);
        if (ce.valid() && ce.color() != c) {
          out.insert(ctr);
        }
      }

      // Handle en passant on each side. See here:
      // https://en.wikipedia.org/wiki/En_passant#Conditions
      // for more info about how it works.
      /// TODO: Handle last move for en passant.
# ifdef EN_PASSANT
      bool enPassantAllowed = (c == Color::White && p.y() == 4) || (c == Color::Black && p.y() == 3);
      if (enPassantAllowed) {
        Coordinates eptl(p.x() - 1, p.y());
        if (eptl.x() >= 0 && eptl.x() < b.w() && eptl.y() >= 0 && eptl.y() < b.h()) {
          const Piece& ce = b.at(eptl);
          if (ce.pawn() && ce.color() != c) {
            // Make sure that the en passant cell is empty.
            Coordinates ep(p.x() - 1, p.y() + dy);
            const Piece& epce = b.at(ep);
            if (epce.invalid()) {
              out.insert(ep);
            }
          }
        }

        Coordinates eptr(p.x() + 1, p.y());
        if (eptr.x() >= 0 && eptr.x() < b.w() && eptr.y() >= 0 && eptr.y() < b.h()) {
          const Piece& ce = b.at(eptr);
          if (ce.pawn() && ce.color() != c) {
            // Make sure that the en passant cell is empty.
            Coordinates ep(p.x() + 1, p.y() + dy);
            const Piece& epce = b.at(ep);
            if (epce.invalid()) {
              out.insert(ep);
            }
          }
        }
      }
# endif

      return out;
    }

  }
}
