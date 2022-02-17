#ifndef    CHESS_GAME_HH
# define   CHESS_GAME_HH

# include <vector>
# include <unordered_set>
# include <memory>
# include <core_utils/CoreObject.hh>
# include "Round.hh"
# include "Piece.hh"
# include "Board.hh"

namespace chess {

  /// @brief - Forward declaration of a list of rounds.
  using Rounds = std::vector<Round>;

  class ChessGame: public utils::CoreObject {
    public:

      ChessGame(int  width = 8u,
                int  height = 8u) noexcept;

      /**
       * @brief - Decay the game into its board component.
       * @return - the board used by this chess game.
       */
      const Board&
      operator()() const noexcept;

      /**
       * @brief - Initialize the board with a new game.
       */
      void
      initialize() noexcept;

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
       * @brief - Returns the list of completed rounds.
       * @return - the list of rounds played.
       */
      Rounds
      getRounds() const noexcept;

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

    private:

      /**
       * @brief - Used to update the internal state and perform the
       *          computation about checks, checkmates, etc.
       */
      void
      updateState() const noexcept;

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
       * @brief - The current state of the board.
       */
      mutable Board m_board;

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
      Rounds m_rounds;
  };

  using ChessGameShPtr = std::shared_ptr<ChessGame>;
}

#endif    /* CHESS_GAME_HH */
