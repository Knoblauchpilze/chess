#ifndef    BOARD_EVALUATION_HH
# define   BOARD_EVALUATION_HH

# include <vector>
# include "Types.hh"

namespace chess {
  namespace ai {

    /// @brief - The approach to use when selecting moves for
    /// the AI to execute.
    enum class Strategy {
      Random,
      Minimax
    };

    /**
     * @brief - Evaluate the input list of moves based on the
     *          current state of the board and return a list
     *          of moves sorted by interest.
     * @param out - the list of moves. Will be changed in place
     *              and modified by the function.
     * @param side - the side for which the moves should be
     *               evaluated.
     * @param b - the current state of the board.
     * @param strat - the strategy to use to evaluate the moves.
     * @return - the list of moves sorted in a way that maximize
     *           the utility for the input side.
     */
    void
    evaluate(std::vector<Move>& out,
             const Color& side,
             const Board& b,
             const Strategy& strat) noexcept;

  }
}

#endif    /* BOARD_EVALUATION_HH */
