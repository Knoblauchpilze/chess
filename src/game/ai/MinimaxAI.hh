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
       * @param depth - the depth available to analyze moves.
       * @param c - the color for which the move should be found.
       * @param b - the current state of the board.
       * @return - the evaluation of the current state of the board
       *           after all possible moves up until the input depth.
       */
      float
      evaluate(unsigned depth,
               const Color& c,
               const Board& b) const noexcept;

    private:

      /***
       * @brief - The depth to consider when generating possible moves.
       */
      unsigned m_depth;
  };

}

#endif    /* MINIMAX_AI_HH */
