#ifndef    MOVE_HH
# define   MOVE_HH

# include <string>
# include <vector>
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
     *            additional information about the position of the
     *            pieces.
     * @return - `true` if the motion is valid.
     */
    bool
    valid(const Type& t,
          const Color& c,
          const Coordinates& start,
          const Coordinates& end,
          const Board& b) noexcept;

    /**
     * @brief - Provide a list of the cell that are threatened
     *          by the piece in input. Note that only positions
     *          that are valid based on the board are considered.
     * @param t - the type of the piece.
     * @param c - the color of the piece.
     * @param p - the location of the piece.
     * @param b - the current state of the board used to get some
     *            additional information about the positions to
     *            consider.
     * @return - the list of threatened cells by the piece.
     */
    std::vector<Coordinates>
    threaten(const Type& t,
             const Color& c,
             const Coordinates& p,
             const Board& b) noexcept;

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
    std::vector<Coordinates>
    move(const Type& t,
         const Color& c,
         const Coordinates& p,
         const Board& b) noexcept;
  }
}

#endif    /* MOVE_HH */
