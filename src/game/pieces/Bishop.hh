#ifndef    BISHOP_HH
# define   BISHOP_HH

# include "Coordinates.hh"
# include "Piece.hh"

namespace chess {

  /// @brief - Forward declaration of the Board to allow its
  /// use as parameter in the functions.
  class Board;

  namespace bishop {

    CoordinatesSet
    reachable(const Color& c,
              const Coordinates& p,
              const Board& b) noexcept;

  }
}

#endif    /* BISHOP_HH */
