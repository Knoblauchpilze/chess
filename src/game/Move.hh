#ifndef    MOVE_HH
# define   MOVE_HH

# include <string>
# include "Pieces.hh"

namespace chess {

  /// @brief - Forward declaration of the Board to allow its
  /// use as parameter in the functions.
  class Board;

  class Move {
    public:

      /**
       * @brief - Create a move with the specified ID and
       *          no data.
       * @param id - the index of the move.
       */
      Move(unsigned id);

      /**
       * @brief - Used to register the part of the move related
       *          to the input color's actions.
       * @param c - the color for which the move should be set.
       * @param t - the type of piece executing the move.
       * @param start - the starting position of the move.
       * @param end - the end position of the move.
       * @param captured - the piece that was captured (if any).
       * @param check - whether the move creates a check.
       * @param checkmate - whether the move creates a checkmate.
       */
      void
      registerMove(const pieces::Color& c,
                   const pieces::Type& t,
                   const Coordinates& start,
                   const Coordinates& end,
                   const pieces::Type& captured,
                   bool check,
                   bool checkmate) noexcept;

      /**
       * @brief - Whether or not this move is valid.
       * @return - `true` in case the move is valid.
       */
      bool
      valid() const noexcept;

      /**
       * @brief - Returns the index of this move.
       * @return - the index of the move.
       */
      unsigned
      id() const noexcept;

      /**
       * @brief - Prints the move as a standard string to
       *          make it more readable. Note that in case
       *          the move is not complete an error string
       *          will be generated.
       * @return - the generated string.
       */
      std::string
      toString() const noexcept;

    private:

      void
      generateName() noexcept;

    private:

      /// @brief - Convenience structure recording half of
      /// a move with the information about a single move
      /// of either white or black.
      struct Part {
        /// @brief - The piece that was moved.
        pieces::Type piece;

        /// @brief - The starting position of the piece that
        /// was moved.
        Coordinates start;

        /// @brief - The end position of the piece that was
        /// moved.
        Coordinates end;

        /// @brief - The type of the piece that was captured.
        /// Set to `None` in case no piece was captured.
        pieces::Type captured;

        /// @brief - Whether this move put the enemy king in
        /// check.
        bool check;

        /// @brief - Whether this move put the enemy kind in
        /// checkmate.
        bool checkmate;
      };

      /**
       * @brief - The index of the move.
       */
      unsigned m_id;

      /**
       * @brief - Whether or not this move is valid.
       */
      bool m_valid;

      /**
       * @brief - The move performed by white.
       */
      Part m_white;

      /**
       * @brief - The move performed by black.
       */
      Part m_black;

      /**
       * @brief - The string representing this move. We do
       *          compute it once and then save it for cache
       *          purposes.
       */
      std::string m_name;
  };

  namespace pieces {

    /**
     * @brief - Determines whether or not the move from the input
     *          piece is valid.
     *          Note that this only account for whether the piece
     *          is allowed to move but not considering if there's
     *          actually a piece at the initial location or one
     *          at the end position.
     *          We also assume that the positions are valid and
     *          are not checked against anything.
     * @param t - the type of the piece.
     * @param c - the color of the piece.
     * @param start - the starting position.
     * @param end - the end position.
     * @param b - the current state of the board used to get some
     *            additional information about the position of the
     *            pieces.
     * @return - `true` if the motion is valid.
     */
    bool
    valid(const Type& t,
          const Color& c,
          const Coordinates& start,
          const Coordinates& end,
          const Board& b) noexcept;
  }
}

#endif    /* MOVE_HH */
