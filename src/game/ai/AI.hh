#ifndef    AI_HH
# define   AI_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include "ChessGame.hh"
# include "Types.hh"

namespace chess {

  class AI: public utils::CoreObject {
    public:

      /**
       * @brief - Create an AI that will play for the input
       *          color.
       * @param color - the color the AI should play.
       * @param name - the name of the AI.
       */
      AI(const Color& color,
         const std::string& name);

      /**
       * @brief - Return the side for which this AI is playing.
       * @return - the side played by the AI.
       */
      Color
      side() const noexcept;

      /**
       * @brief - Plays a round with the specified color. This
       *          method verifies that it is the AI's turn to
       *          play before calling the internal routine to
       *          generate the best move.
       * @param b - the board on which to play.
       * @return - whether a valid move could be find.
       */
      bool
      play(ChessGame& b) noexcept;

    protected:

      /**
       * @brief - Interface method allowing to generate the
       *          best move based on the state of the board.
       *          The available moves are returned as a list,
       *          and in case no available move is it means
       *          that the game is probably over.
       *          The weight of each move should have a very
       *          high positive value in case it is favorable
       *          for the side the AI's playing, and a very
       *          negative value if not.
       * @param b - the starting position of the board.
       * @return - the list of available moves, sorted by their
       *           weight. The list is not expected to be sorted
       *           we just expect all weights to be available.
       */
      virtual
      std::vector<ai::Move>
      generateMoves(const Board& b) noexcept = 0;

    protected:

      /**
       * @brief - The color the AI is playing.
       */
      Color m_color;
  };

  using AIShPtr = std::shared_ptr<AI>;
}

#endif    /* AI_HH */
