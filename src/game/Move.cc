
# include "Move.hh"

namespace chess {
  namespace pieces {

    /// @brief - Pawn related functions.
    namespace pawn {

      inline
      bool
      valid(const Coordinates& /*start*/,
            const Coordinates& /*end*/) noexcept
      {
        return true;
      }
    }

    /// @brief - Knight related functions.
    namespace knight {

      inline
      bool
      valid(const Coordinates& /*start*/,
            const Coordinates& /*end*/) noexcept
      {
        return false;
      }
    }

    /// @brief - Bishop related functions.
    namespace bishop {

      inline
      bool
      valid(const Coordinates& /*start*/,
            const Coordinates& /*end*/) noexcept
      {
        return false;
      }
    }

    /// @brief - Rook related functions.
    namespace rook {

      inline
      bool
      valid(const Coordinates& /*start*/,
            const Coordinates& /*end*/) noexcept
      {
        return false;
      }
    }

    /// @brief - Queen related functions.
    namespace queen {

      inline
      bool
      valid(const Coordinates& /*start*/,
            const Coordinates& /*end*/) noexcept
      {
        return false;
      }
    }

    /// @brief - King related functions.
    namespace king {

      inline
      bool
      valid(const Coordinates& /*start*/,
            const Coordinates& /*end*/) noexcept
      {
        return false;
      }
    }

    /// @brief - None related functions.
    namespace none {

      inline
      bool
      valid(const Coordinates& /*start*/,
            const Coordinates& /*end*/) noexcept
      {
        return false;
      }
    }

    bool
    valid(const Type& t,
          const Coordinates& start,
          const Coordinates& end) noexcept
    {
      switch (t) {
        case Pawn:
          return pawn::valid(start, end);
        case Knight:
          return knight::valid(start, end);
        case Bishop:
          return bishop::valid(start, end);
        case Rook:
          return rook::valid(start, end);
        case Queen:
          return queen::valid(start, end);
        case King:
          return king::valid(start, end);
        case None:
        default:
          return none::valid(start, end);
      }
    }

  }
}
