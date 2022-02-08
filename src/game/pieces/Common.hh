#ifndef    COMMON_HH
# define   COMMON_HH

# include "Coordinates.hh"
# include "Piece.hh"

namespace chess {

  /// @brief - Forward declaration of the Board to allow its
  /// use as parameter in the functions.
  class Board;

  /**
   * @brief - Generates the list of coordinates starting from
   *          the initial location with the provided step for
   *          a length at most equal to the input value and
   *          maybe shorter in case it reaches the edge of the
   *          board.
   * @param c - the color of the piece (used in case the block
   *            behavior is active).
   * @param s - the starting coordinates.
   * @param dx - the step along the x axis.
   * @param dy - the step along the y axis.
   * @param length - maximum extent of the generation. If this
   *                 value is negative the edge of the board is
   *                 used as a delimiter.
   * @param b - information about the board.
   * @param includeSelf - `true` in case the starting position
   *                      should be included.
   * @param stopWhenBlocked - `true` if the generation should stop
   *                          after reaching another piece.
   * @return - the list of generated coordinates.
   */
  std::vector<Coordinates>
  generate(const Color& c,
           const Coordinates& s,
           int dx,
           int dy,
           int length,
           const Board& b,
           bool includeSelf,
           bool stopWhenBlocked) noexcept;

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

#endif    /* COMMON_HH */
