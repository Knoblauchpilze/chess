
# include "Move.hh"
# include "Board.hh"

namespace chess {

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
      pieces::Cell c = b.at(x, y);
      if (c.type != pieces::None) {
        return false;
      }

      x += dx;
      y += dy;
      ++d;
    }

    return true;
  }

  namespace pieces {

    /// @brief - Pawn related functions.
    namespace pawn {

      inline
      bool
      valid(const Color& /*c*/,
            const Coordinates& /*start*/,
            const Coordinates& /*end*/,
            const Board& /*b*/) noexcept
      {
        /// TODO: Handle pawn moves
        return true;
      }
    }

    /// @brief - Knight related functions.
    namespace knight {

      inline
      bool
      valid(const Color& /*c*/,
            const Coordinates& /*start*/,
            const Coordinates& /*end*/,
            const Board& /*b*/) noexcept
      {
        /// TODO: Handle knight moves.
        return true;
      }
    }

    /// @brief - Bishop related functions.
    namespace bishop {

      inline
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
    }

    /// @brief - Rook related functions.
    namespace rook {

      inline
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
    }

    /// @brief - Queen related functions.
    namespace queen {

      inline
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
    }

    /// @brief - King related functions.
    namespace king {

      inline
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

        /// TODO: Prevent king from moving in chess.
        /// TODO: Allow king to castle.
        return true;
      }
    }

    /// @brief - None related functions.
    namespace none {

      inline
      bool
      valid(const Color& /*c*/,
            const Coordinates& /*start*/,
            const Coordinates& /*end*/,
            const Board& /*b*/) noexcept
      {
        return false;
      }
    }

    bool
    valid(const Type& t,
          const Color& c,
          const Coordinates& start,
          const Coordinates& end,
          const Board& b) noexcept
    {
      switch (t) {
        case Pawn:
          return pawn::valid(c, start, end, b);
        case Knight:
          return knight::valid(c, start, end, b);
        case Bishop:
          return bishop::valid(c, start, end, b);
        case Rook:
          return rook::valid(c, start, end, b);
        case Queen:
          return queen::valid(c, start, end, b);
        case King:
          return king::valid(c, start, end, b);
        case None:
        default:
          return none::valid(c, start, end, b);
      }
    }

  }
}
