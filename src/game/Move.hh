#ifndef    MOVE_HH
# define   MOVE_HH

# include "Pieces.hh"

namespace chess {
  namespace pieces {

    /**
     * @brief - Determines whether or not the move from the input
     *          piece is valid.
     * @param t - the type of the piece.
     * @param start - the starting position.
     * @param end - the end position.
     * @return - `true` if the motion is valid.
     */
    bool
    valid(const Type& t,
          const Coordinates& start,
          const Coordinates& end) noexcept;
  }
}

#endif    /* MOVE_HH */
