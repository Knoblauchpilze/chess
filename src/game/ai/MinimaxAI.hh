#ifndef    MINIMAX_AI_HH
# define   MINIMAX_AI_HH

# include "AI.hh"

namespace chess {

  class MinimaxAI: public AI {
    public:

      /**
       * @brief - Create an AI playing moves based on a
       *          min max algorithm.
       * @param color - the color the AI should play.
       * @param depth - the depth considered by the AI.
       */
      MinimaxAI(const Color& color,
                unsigned depth);

    protected:

      /**
       * @brief - Implementation of the interface method to handle
       *          the generation of the available moves based on the
       *          AI's strategy.
       * @param b - the starting position of the board.
       * @return -  the sorted list of moves from the most favourable
       *            one to the least favourable one.
       */
      std::vector<ai::Move>
      generateMoves(const Board& b) noexcept override;

    private:

      /**
       * @brief - Evaluate the best move for the current depth by
       *          generating more moves if needed and aggregating
       *          the result.
       *          We use a minimax approach with a alpha-beta to
       *          prune suboptimal results.
       * @param c - the color for which the move should be found.
       * @param b - the current state of the board.
       * @param maximizing - defines whether we should try to find
       *                     the maximum or minimum score for this
       *                     iteration.
       * @param alpha - used for alpha-beta pruning, characterizes the
       *                minimum score that the maximizing player is
       *                assured of.
       * @param beta - used for alpha beta pruning. Defines the
       *               maximum score that the minimizing player is
       *               assured of.
       * @param depth - the current depth of the evaluation.
       * @param nodes - information about how many nodes where visited.
       * @return - the evaluation of the current state of the board
       *           after all possible moves up until the input depth.
       */
      float
      evaluate(const Color& c,
               const Board& b,
               bool maximizing,
               float alpha,
               float beta,
               unsigned depth,
               unsigned* nodes,
               unsigned logDepth) const noexcept;

    private:

      /***
       * @brief - The depth to consider when generating possible moves.
       */
      unsigned m_depth;
  };

}

#endif    /* MINIMAX_AI_HH */
