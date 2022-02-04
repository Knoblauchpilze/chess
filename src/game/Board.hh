#ifndef    BOARD_HH
# define   BOARD_HH

# include <vector>
# include <memory>
# include <core_utils/CoreObject.hh>
# include "Pieces.hh"
# include "Move.hh"

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
       * @brief - Returns the current color playing the next
       *          round.
       * @return - the next color to play.
       */
      chess::pieces::Color
      getPlayer() const noexcept;

      /**
       * @brief - Returns the piece at the specified position or none in
       *          case the cell is empty.
       * @param x - the x coordinate.
       * @param y - the y coordinates.
       * @return - the piece at this place.
       */
      pieces::Cell
      at(int x, int y) const;

      /**
       * @brief - Returns the piece at the specified position or none in
       *          case the cell is empty.
       * @param c - the coordinates to check.
       * @return - the piece at this place.
       */
      pieces::Cell
      at(const Coordinates& c) const;

      /**
       * @brief - Attempts to move whatever piece might be located at the
       *          starting position to the end location.
       *          Also updates the board state based on this move.
       * @param start - the start location to move from.
       * @param end - the end location to move to.
       * @return - `true` if the move could be performed.
       */
      bool
      move(const Coordinates& start, const Coordinates& end);

    private:

      void
      initialize() noexcept;

      unsigned
      linear(int x, int y) const noexcept;

      unsigned
      linear(const Coordinates& c) const noexcept;

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

      /**
       * @brief - The current move index.
       */
      unsigned m_index;

      /**
       * @brief - The color of pieces currently allowed to
       *          move. Gets updated after each round.
       */
      pieces::Color m_current;

      /**
       * @brief - The current move.
       */
      Move m_move;

      /**
       * @brief - The stack of moves.
       */
      std::vector<Move> m_moves;
  };

  using BoardShPtr = std::shared_ptr<Board>;
}

#endif    /* BOARD_HH */
