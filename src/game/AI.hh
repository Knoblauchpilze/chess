#ifndef    AI_HH
# define   AI_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include "Board.hh"

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
       * @brief - Plays a round with the specified color.
       * @param b - the board on which to play.
       */
      void
      play(Board& b) noexcept;

    private:

      /**
       * @brief - The color the AI is playing.
       */
      Color m_color;
  };

  using AIShPtr = std::shared_ptr<AI>;
}

#endif    /* AI_HH */
