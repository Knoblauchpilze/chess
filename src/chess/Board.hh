#ifndef    BOARD_HH
# define   BOARD_HH

# include <vector>
# include <core_utils/CoreObject.hh>
# include "Pieces.hh"

namespace chess {

  class Board: public utils::CoreObject {
    public:

      Board(int  width = 8u,
            int  height = 8u) noexcept;

      /**
       * @brief - The width of the board.
       * @return - the width of the board.
       */
      int
      w() const noexcept;

      /**
       * @brief - The height of the board.
       * @return - the height of the board.
       */
      int
      h() const noexcept;

      /**
       * @brief - Returns the piece at the specified position or none in
       *          case the cell is empty.
       * @param x - the x coordinate.
       * @param y - the y coordinates.
       * @return - the piece at this place.
       */
      pieces::Cell
      at(int x, int y) const;

    private:

      void
      initialize() noexcept;

    private:

      /**
       * @brief - The width of the board.
       */
      int m_width;

      /**
       * @brief - The height of the board.
       */
      int m_height;

      /**
       * @brief - The current state of the board.
       */
      std::vector<pieces::Cell> m_board;
  };

}

#endif    /* BOARD_HH */
