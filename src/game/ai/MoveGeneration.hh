#ifndef    MOVE_GENERATION_HH
# define   MOVE_GENERATION_HH

# include <vector>
# include "Types.hh"

namespace chess {
  namespace ai {

    /**
     * @brief - Generate the list of moves available for the
     *          input side based on the current state of the
     *          board.
     * @param side - the side for which the moves should be
     *               generated.
     * @param b - the current state of the board.
     * @return - the list of moves available to the side.
     */
    std::vector<Move>
    generate(const Color& side, const Board& b) noexcept;

  }
}

#endif    /* MOVE_GENERATION_HH */
