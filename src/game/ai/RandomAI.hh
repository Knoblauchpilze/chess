#ifndef    RANDOM_AI_HH
# define   RANDOM_AI_HH

# include "AI.hh"

namespace chess {

  class RandomAI: public AI {
    public:

      /**
       * @brief - Create an AI playing random move for the input
       *          color.
       * @param color - the color the AI should play.
       */
      RandomAI(const Color& color);

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
  };

}

#endif    /* RANDOM_AI_HH */
