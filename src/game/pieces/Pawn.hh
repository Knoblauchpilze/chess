#ifndef    PAWN_HH
# define   PAWN_HH

# include <vector>
# include "Pieces.hh"

namespace chess {

  /// @brief - Forward declaration of the Board to allow its
  /// use as parameter in the functions.
  class Board;

  namespace pieces {
    namespace pawn {

      CoordinatesSet
      reachable(const Color& c,
                const Coordinates& p,
                const Board& b) noexcept;

    }
  }
}

#endif    /* PAWN_HH */
