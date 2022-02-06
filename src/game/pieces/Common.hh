#ifndef    COMMON_HH
# define   COMMON_HH

# include <vector>
# include "Pieces.hh"

namespace chess {

  /// @brief - Forward declaration of the Board to allow its
  /// use as parameter in the functions.
  class Board;

  namespace pieces {

    /**
     * @brief - Compute the differentials along the x and y
     *          axis.
     * @param start - the start position.
     * @param end - the end position.
     * @param dx - output argument holding the difference in
     *             x axis.
     * @param dy - output argument holding the difference in
     *             y axis.
     */
    void
    differentials(const Coordinates& start,
                  const Coordinates& end,
                  int& dx,
                  int& dy) noexcept;

    /**
     * @brief - Computes whether the line defined from the
     *          starting position with the specified increment
     *          and with a certain length is obstructed or not.
     * @param b - the board to check for obstruction.
     * @param start - the initial position to start with.
     * @param dx - the delta in x at each step.
     * @param dy - the delta in y at each step.
     * @param length - the distance to check for obstruction.
     * @return - `true` if there's a piece obstructing the line.
     */
    bool
    obstructed(const Board& b,
               const Coordinates& start,
               int dx,
               int dy,
               unsigned length) noexcept;

  }
}

#endif    /* COMMON_HH */
