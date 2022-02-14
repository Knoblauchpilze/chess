#ifndef    PIECES_HH
# define   PIECES_HH

# include <vector>
# include <memory>
# include <core_utils/CoreObject.hh>
# include "Piece.hh"

namespace chess {

  class Pieces: public utils::CoreObject {
    public:

      Pieces(int  width = 8u,
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
       * @brief - Initialize the board with a new game.
       */
      void
      initialize() noexcept;

      /**
       * @brief - Returns the piece at the specified position or none in
       *          case the cell is empty.
       * @param x - the x coordinate.
       * @param y - the y coordinates.
       * @return - the piece at this place.
       */
      const Piece&
      at(int x, int y) const;

      /**
       * @brief - Returns the piece at the specified position or none in
       *          case the cell is empty.
       * @param c - the coordinates to check.
       * @return - the piece at this place.
       */
      const Piece&
      at(const Coordinates& c) const;

    private:

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
      mutable std::vector<Piece> m_board;
  };

  using PiecesShPtr = std::shared_ptr<Pieces>;
}

#endif    /* PIECES_HH */
