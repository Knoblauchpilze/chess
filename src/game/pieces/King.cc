
# include "King.hh"
# include "Board.hh"
# include "Common.hh"

namespace chess {
  namespace king {

    CoordinatesSet
    reachable(const Color& c,
              const Coordinates& p,
              const Board& b) noexcept
    {
      CoordinatesSet out;

      // Generate a 3x3 square around the initial
      // position, ignoring invalid values and the
      // starting position.
      for (int y = p.y() - 1 ; y <= p.y() + 1 ; ++y) {
        for (int x = p.x() - 1 ; x <= p.x() + 1 ; ++x) {
          Coordinates co(x, y);
          if (!b.validCoordinates(co)) {
            continue;
          }

          // Ignore own cell.
          if (x == p.x() && y == p.y()) {
            continue;
          }

          // Ignore cell with a piece of the same color.
          const Piece& ce = b.at(x, y);
          if (ce.valid() && ce.color() == c) {
            continue;
          }

          out.insert(co);
        }
      }

      // We also need to check for castling. This
      // can only be an option if the king hasn't
      // moved yet.
      if (b.hasMoved(p)) {
        return out;
      }

      // Handle king side castling.
      Coordinates co(b.w() - 1, c == Color::White ? 0 : b.h() - 1);
      Piece r = b.at(co);
      if (r.rook() && r.color() == c && !b.hasMoved(co)) {
        // Check that the position of the on the right
        // of the king is reachable by the rook. This
        // will be enough to verify that pieces can
        // move.
        CoordinatesSet av = r.reachable(co, b);
        Coordinates dest(p.x() + 1, co.y());
        if (av.count(dest) > 0) {
          // We need to check that the king is not in
          // check at the moment and that none of the
          // position traversed is a check.
          dest = Coordinates(b.w() - 2, co.y());

          bool valid = true;
          int x = p.x() + 1;
          while (x <= dest.x() && valid) {
            valid = !b.leadsToCheck(p, Coordinates(x, dest.y()));
            ++x;
          }

          if (valid) {
            out.insert(dest);
          }
        }
      }

      // And also queen side.
      co = Coordinates(0, c == Color::White ? 0 : b.h() - 1);
      r = b.at(co);
      if (r.rook() && r.color() == c && !b.hasMoved(co)) {
        // Check that the position of the on the left
        // of the king is reachable by the rook. This
        // will be enough to verify that pieces can
        // move.
        CoordinatesSet av = r.reachable(co, b);
        Coordinates dest(p.x() - 1, co.y());
        if (av.count(dest) > 0) {
          // We need to check that the king is not in
          // check at the moment and that none of the
          // position traversed is a check.
          dest = Coordinates(2, co.y());

          bool valid = true;
          int x = p.x() - 1;
          while (x >= dest.x() && valid) {
            valid = !b.leadsToCheck(p, Coordinates(x, dest.y()));
            --x;
          }

          if (valid) {
            out.insert(dest);
          }
        }
      }

      return out;
    }

  }
}
