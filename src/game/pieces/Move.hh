#ifndef    MOVE_HH
# define   MOVE_HH

# include <string>
# include <unordered_set>
# include "Pieces.hh"

namespace chess {

  /// @brief - Forward declaration of the Board to allow its
  /// use as parameter in the functions.
  class Board;

  namespace pieces {

    /**
     * @brief - Used to generate the list of possible positions which
     *          can be reached by the input piece.
     * @param t - the type of the piece.
     * @param c - the color of the piece.
     * @param p - the position of the piece.
     * @param b - the board to get additional information about the
     *            cells that are obstructed.
     * @return - a list of cells reachable by the piece.
     */
    CoordinatesSet
    reachable(const Type& t,
              const Color& c,
              const Coordinates& p,
              const Board& b) noexcept;
  }
}

#endif    /* MOVE_HH */
