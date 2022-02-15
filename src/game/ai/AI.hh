#ifndef    AI_HH
# define   AI_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include "ChessGame.hh"

namespace chess {

  class AI: public utils::CoreObject {
    public:

      /**
       * @brief - Create an AI that will play for the input
       *          color.
       * @param color - the color the AI should play.
       */
      AI(const Color& color);

      /**
       * @brief - Return the side for which this AI is playing.
       * @return - the side played by the AI.
       */
      Color
      side() const noexcept;

      /**
       * @brief - Plays a round with the specified color.
       * @param b - the board on which to play.
       * @return - whether a valid move could be find.
       */
      bool
      play(ChessGame& b) noexcept;

    private:

      /**
       * @brief - The color the AI is playing.
       */
      Color m_color;
  };

  using AIShPtr = std::shared_ptr<AI>;
}

#endif    /* AI_HH */
