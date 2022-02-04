#ifndef    MOVE_HH
# define   MOVE_HH

# include "Pieces.hh"

namespace chess {

  /// @brief - Forward declaration of the Board to allow its
  /// use as parameter in the functions.
  class Board;

  namespace pieces {

    /**
     * @brief - Determines whether or not the move from the input
     *          piece is valid.
     *          Note that this only account for whether the piece
     *          is allowed to move but not considering if there's
     *          actually a piece at the initial location or one
     *          at the end position.
     *          We also assume that the positions are valid and
     *          are not checked against anything.
     * @param t - the type of the piece.
     * @param c - the color of the piece.
     * @param start - the starting position.
     * @param end - the end position.
     * @param b - the current state of the board used to get some
     *            additional position about the position of the
     *            pieces.
     * @return - `true` if the motion is valid.
     */
    bool
    valid(const Type& t,
          const Color& c,
          const Coordinates& start,
          const Coordinates& end,
          const Board& b) noexcept;
  }
}

#endif    /* MOVE_HH */
