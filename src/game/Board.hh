#ifndef    BOARD_HH
# define   BOARD_HH

# include <vector>
# include <memory>
# include <core_utils/CoreObject.hh>
# include "Piece.hh"

namespace chess {

  /// @brief - Convenience declaration to describe a
  /// collection of piece with their coordinates.
  using Pieces = std::vector<std::pair<const Piece, Coordinates>>;

  class Board: public utils::CoreObject {
    public:

      explicit
      Board(int  width = 8u,
            int  height = 8u) noexcept;

      explicit
      Board(const Board& b) noexcept;

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
       * @brief - Whether or not the input coordinates are valid.
       * @param c - the coordinates to check.
       * @return - whether the coordinates are valid.
       */
      bool
      validCoordinates(const Coordinates& c) const noexcept;

      /**
       * @brief - Initialize the board with a new game.
       */
      void
      initialize() noexcept;

      /**
       * @brief - Returns the piece at the specified position or none in
       *          case the cell is empty.
       * @param x - the x coordinates.
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

      /**
       * @brief - Return the list of piece for a certain color.
       * @param color - the color of the piece.
       * @return - the list of pieces for this color.
       */
      Pieces
      pieces(const Color& color) const noexcept;

      /**
       * @brief - Used to generate the possible positions that can
       *          be reached by the piece at the input coordinates.
       *          In case the cell is empty, the returned list will
       *          be empty.
       * @param coords - the coordinates of the piece to generate.
       * @return - the list of coordinates reachable by the piece.
       */
      CoordinatesSet
      availablePositions(const Coordinates& coords) const noexcept;

      /**
       * @brief - Used to determine whether the piece currently at
       *          the input coordinates already moved in the past.
       * @param p - the coordinates of the piece to check.
       * @return - `true` if the piece already moved.
       */
      bool
      hasMoved(const Coordinates& p) const noexcept;

      /**
       * @brief - Whether or not the piece at the input coordinates
       *          just moved.
       * @param p - the coordinates to check.
       * @return - `true` if the last registered move is the input
       *           coordinates.
       */
      bool
      justMoved(const Coordinates& p) const noexcept;

      /**
       * @brief - Used to compute the check status for the input color
       *          without using the cache.
       * @param c - the color for which the check status needs to be
       *            computed.
       * @return - `true` if the king of the input color is in check.
       */
      bool
      computeCheck(const Color& c) const noexcept;

      /**
       * @brief - Used to compute the stalemate status for the input
       *          color without using the cache.
       * @param c - the color for which the stalemate status needs to
       *            be computed.
       * @return - `true` if the king of the input color is in stalemate.
       */
      bool
      computeStalemate(const Color& c) const noexcept;

      /**
       * @brief - Determine whether the move defined by the input
       *          starting and end position would leave the king
       *          with the color of the piece at the starting
       *          position in check or not.
       *          We make no assumptions as to whether the king
       *          is already in check or not.
       *          In case one of the position is not valid then an
       *          error is raised.
       * @param start - the starting position.
       * @param end - the ending position.
       * @return - `true` if the move leaves the king in check.
       */
      bool
      leadsToCheck(const Coordinates& start, const Coordinates& end) const;

      /**
       * @brief - Attempts to move whatever piece might be located at the
       *          starting position to the end location and see if it is
       *          a legal move.
       * @param start - the start location to move from.
       * @param end - the end location to move to.
       * @return - `true` if the move can be performed.
       */
      bool
      legal(const Coordinates& start, const Coordinates& end);

      /**
       * @brief - Attempts to move whatever piece might be located at the
       *          starting position to the end location.
       *          Note that there's no control about whether the move makes
       *          sense or not, we just move the piece at the start position
       *          to the end position.
       * @param start - the start location to move from.
       * @param end - the end location to move to.
       * @param autoPromote - whether or not any pawn moved to the last rank
       *                      should automatically be promoted or not.
       * @param promotion - in case of auto promotion, the type to promote to.
       */
      void
      move(const Coordinates& start,
           const Coordinates& end,
           bool autoPromote = false,
           const Type& promotion = Type::Queen);

      /**
       * @brief - Attempts to promote the piece at the position in
       *          input to the desired value.
       *          Raises an error if the piece is not valid.
       * @param p - the coordinates of the piece to promote.
       * @param promote - the promotion to apply.
       */
      void
      promote(const Coordinates& p, const Type& promote);

    private:

      unsigned
      linear(int x, int y) const noexcept;

      unsigned
      linear(const Coordinates& c) const noexcept;

    private:

      /// @brief - Convenience structure allowing to keep
      /// track of the initial square of the piece.
      struct PieceData {
        // The piece attached to the data.
        Piece item;

        // Whether or not this piece moved.
        bool moved;
      };

      /// @brief - Convenience structure keeping track of the
      /// last move's data.
      struct LastMove {
        // The coordinates of the starting piece.
        Coordinates origin;

        // The coordinates of the ending piece.
        Coordinates end;

        // The captured piece if any.
        PieceData captured;

        // The initial type of the piece sitting at the starting
        // position. This value is used in case a promotion is
        // added to the piece that just moved.
        PieceData raw;
      };

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
      mutable std::vector<PieceData> m_board;

      /**
       * @brief - The information about the last move.
       */
      LastMove m_last;
  };

  using BoardShPtr = std::shared_ptr<Board>;
}

#endif    /* BOARD_HH */
