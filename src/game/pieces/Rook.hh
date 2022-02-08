#ifndef    ROOK_HH
# define   ROOK_HH

# include "Coordinates.hh"
# include "Piece.hh"

namespace chess {

  /// @brief - Forward declaration of the Board to allow its
  /// use as parameter in the functions.
  class Board;

  namespace rook {

    CoordinatesSet
    reachable(const Color& c,
              const Coordinates& p,
              const Board& b) noexcept;

  }
}

#endif    /* ROOK_HH */
