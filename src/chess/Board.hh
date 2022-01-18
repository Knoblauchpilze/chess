#ifndef    BOARD_HH
# define   BOARD_HH

# include <vector>
# include <core_utils/CoreObject.hh>
# include "Pieces.hh"

namespace chess {

  class Board: public utils::CoreObject {
    public:

      Board(unsigned  width = 8u,
            unsigned  height = 8u) noexcept;

      /**
       * @brief - Returns the piece at the specified position or none in
       *          case the cell is empty.
       * @param x - the x coordinate.
       * @param y - the y coordinates.
       * @return - the piece at this place.
       */
      pieces::Cell
      at(unsigned x, unsigned y) const;

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
      std::vector<pieces::Cell> m_board;
  };

}

#endif    /* BOARD_HH */
