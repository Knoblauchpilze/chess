#ifndef    THREAT_HH
# define   THREAT_HH

# include <vector>
# include "Pieces.hh"

namespace chess {

  /// @brief - Forward declaration of the Board to allow its
  /// use as parameter in the functions.
  class Board;

  namespace pieces {

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
  }
}

#endif    /* THREAT_HH */
