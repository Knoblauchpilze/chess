
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
      bool enPassantAllowed = (c == Color::White && p.y() == 4) || (c == Color::Black && p.y() == 3);
      if (enPassantAllowed) {
        // Check that the en passant cell has valid coordinates.
        Coordinates eptl(p.x() - 1, p.y());
        if (eptl.x() >= 0 && eptl.x() < b.w() && eptl.y() >= 0 && eptl.y() < b.h()) {
          // Check that the piece is a pawn of opposite color.
          const Piece& ce = b.at(eptl);
          if (ce.pawn() && ce.color() != c) {
            // Check that the pawn has just moved. Note that it
            // can happen that the opposite color didn't play
            // yet. Indeed, it could be that we are trying to
            // determine if the current move of the opposite
            // color is valid, in which case we didn't register
            // it yet and so it's not possible to have the info.
            // In this case, use the previous move.
            // Note that this can only happen for black, as we
            // have the guarantee that the last round is valid.
            Round r = (c == Color::White ? b.getLastRound() : b.getCurrentRound());
            if (c == Color::Black && !r.whitePlayed()) {
              r = b.getLastRound();
            }

            Coordinates slm = r.getMoveStart(oppositeColor(c));
            Coordinates elm = r.getMoveEnd(oppositeColor(c));

            if (std::abs(slm.y() - elm.y()) == 2 && elm == eptl) {
              // Make sure that the en passant cell is empty.
              Coordinates ep(p.x() - 1, p.y() + dy);
              const Piece& epce = b.at(ep);
              if (epce.invalid()) {
                out.insert(ep);
              }
            }
          }
        }

        // Check that the en passant cell has valid coordinates.
        Coordinates eptr(p.x() + 1, p.y());
        if (eptr.x() >= 0 && eptr.x() < b.w() && eptr.y() >= 0 && eptr.y() < b.h()) {
          // Check that the piece is a pawn of opposite color.
          const Piece& ce = b.at(eptr);
          if (ce.pawn() && ce.color() != c) {
            // Check that the pawn has just moved.
            Round r = (c == Color::White ? b.getLastRound() : b.getCurrentRound());
            if (c == Color::Black && !r.whitePlayed()) {
              r = b.getLastRound();
            }

            Coordinates slm = r.getMoveStart(oppositeColor(c));
            Coordinates elm = r.getMoveEnd(oppositeColor(c));

            if (std::abs(slm.y() - elm.y()) == 2 && elm == eptr) {
              // Make sure that the en passant cell is empty.
              Coordinates ep(p.x() + 1, p.y() + dy);
              const Piece& epce = b.at(ep);
              if (epce.invalid()) {
                out.insert(ep);
              }
            }
          }
        }
      }

      return out;
    }

  }
}
