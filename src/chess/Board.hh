#ifndef    BOARD_HH
# define   BOARD_HH

# include <vector>
# include "Pieces.hh"

namespace chess {

  class Board {
    public:

      Board(unsigned  width = 8u,
            unsigned  height = 8u) noexcept;

    private:

      void
      initialize() noexcept;

    private:

      /**
       * @brief - The width of the board.
       */
      unsigned m_width;

      /**
       * @brief - The height of the board.
       */
      unsigned m_height;

      /**
       * @brief - The current state of the board.
       */
      std::vector<pieces::Type> m_board;
  };

}

#endif    /* BOARD_HH */
