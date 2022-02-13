#ifndef    BOARD_HH
# define   BOARD_HH

# include <vector>
# include <unordered_set>
# include <memory>
# include <core_utils/CoreObject.hh>
# include "Round.hh"
# include "Piece.hh"

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
      Color
      getPlayer() const noexcept;

      /**
       * @brief - Returns the current round.
       * @return - the current round.
       */
      Round
      getCurrentRound() const noexcept;

      /**
       * @brief - Returns the last round played.
       * @return - the last round played.
       */
      Round
      getLastRound() const noexcept;

      /**
       * @brief - Determines whether the king of the input color is
       *          currently in check.
       * @param color - the color to check.
       * @return - `true` if the king of the input color is in check.
       */
      bool
      isInCheck(const Color& color) const noexcept;

      /**
       * @brief - Determines whether the king of the input color is
       *          currently in checkmate.
       * @param color - the color to check.
       * @return - `true` if the king of the input color is in a
       *           checkmate position.
       */
      bool
      isInCheckmate(const Color& color) const noexcept;

      /**
       * @brief - Determines whether the player of the input color is
       *          currently in stalemate.
       * @param color - the color to check.
       * @return - `true` if the player of the input color is in a
       *           stalemate position.
       */
      bool
      isInStalemate(const Color& color) const noexcept;

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
       * @brief - Return the list of piece for a certain color.
       * @param color - the color of the piece.
       * @return - the list of pieces for this color.
       */
      std::vector<Piece>
      pieces(const Color& color) const noexcept;

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

      /**
       * @brief - Attempts to promote the piece at the position in
       *          input to the desired value.
       *          Raises an error if the piece is not valid.
       * @param p - the coordinates of the piece to promote.
       * @param promote - the promotion to apply.
       */
      void
      promote(const Coordinates& p, const Type& promote);

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
       * @brief - Used to determine whether the piece currently at
       *          the input coordinates already moved in the past.
       * @param p - the coordinates of the piece to check.
       * @return - `true` if the piece already moved.
       */
      bool
      hasMoved(const Coordinates& p) const noexcept;

    private:

      void
      initialize() noexcept;

      void
      initializeCustom() noexcept;

      unsigned
      linear(int x, int y) const noexcept;

      unsigned
      linear(const Coordinates& c) const noexcept;

      /**
       * @brief - Used to update the internal state and perform the
       *          computation about checks, checkmates, etc.
       */
      void
      updateState() const noexcept;

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
       * @brief - ALlows to handle the actual action to move a piece on
       *          the board, also handling the post-processing needed
       *          for some complex moves (like castling or en passant)
       *          without cluttering the generic move function.
       *          NOTE: no controls are performed to verify that the
       *          move is valid.
       * @param start - the starting position.
       * @param end - the ending position.
       */
      void
      movePiece(const Coordinates& start, const Coordinates& end) noexcept;

    private:

      /// @brief - The data for a single color.
      struct Player {
        // Whether the player is in check.
        bool check;

        // Whether the player is in checkmate.
        bool checkmate;

        // Whether the player is in stalemate.
        bool stalemate;
      };

      /// @brief - Convenience structure allowing to keep track
      /// of the state of the board to speed-up some computations.
      /// This information is used as a sort of cache and needs
      /// to be recomputed when needed.
      struct State {
        // Whether or not the information here is up to date.
        bool dirty;

        // information about th state of white.
        Player white;

        // Information about the state of black.
        Player black;
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
      mutable std::vector<Piece> m_board;

      /**
       * @brief - The current move index.
       */
      unsigned m_index;

      /**
       * @brief - The color of pieces currently allowed to
       *          move. Gets updated after each round.
       */
      Color m_current;

      /**
       * @brief - The current state of the board.
       */
      mutable State m_state;

      /**
       * @brief - The current round.
       */
      Round m_round;

      /**
       * @brief - The list of rounds.
       */
      std::vector<Round> m_rounds;
  };

  using BoardShPtr = std::shared_ptr<Board>;
}

#endif    /* BOARD_HH */
