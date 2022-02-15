#ifndef    ROUND_HH
# define   ROUND_HH

# include <string>
# include <core_utils/CoreObject.hh>
# include "Coordinates.hh"
# include "Piece.hh"

namespace chess {

  /// @brief - Forward declaration of the Board to allow its
  /// use as parameter in the functions.
  class Board;

  class Round: public utils::CoreObject {
    public:

      /**
       * @brief - Create a move with the specified ID and
       *          no data.
       * @param id - the index of the move.
       */
      Round(unsigned id);

      /**
       * @brief - Used to register the part of the move related
       *          to the input color's actions.
       * @param p - the piece which moved.
       * @param t - the type of piece executing the move.
       * @param start - the starting position of the move.
       * @param end - the end position of the move.
       * @param captured - whether the move resulted in a capture.
       * @param check - whether the move creates a check.
       * @param checkmate - whether the move creates a checkmate.
       * @param stalemate - whether the move creates a stalemate.
       */
      void
      registerMove(Piece p,
                   const Coordinates& start,
                   const Coordinates& end,
                   bool captured,
                   bool check,
                   bool checkmate,
                   bool stalemate) noexcept;

      /**
       * @brief - Wehether or not the information for the
       *          white move is registered already.
       * @return - `true` if the white move is available.
       */
      bool
      whitePlayed() const noexcept;

      /**
       * @brief - Wehether or not the information for the
       *          black move is registered already.
       * @return - `true` if the black move is available.
       */
      bool
      blackPlayed() const noexcept;

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
       * @brief - Returns the starting position for the move
       *          related to the input color. In case the info
       *          for this move is not available an error is
       *          raised.
       * @param c - the color of the move to return.
       * @return - the starting coordinates for the move.
       */
      Coordinates
      getMoveStart(const Color& c) const;

      /**
       * @brief - Returns the ending position for the move
       *          related to the input color. In case the info
       *          for this move is not available an error is
       *          raised.
       * @param c - the color of the move to return.
       * @return - the ending coordinates for the move.
       */
      Coordinates
      getMoveEnd(const Color& c) const;

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
        Piece piece;

        /// @brief - The starting position of the piece that
        /// was moved.
        Coordinates start;

        /// @brief - The end position of the piece that was
        /// moved.
        Coordinates end;

        /// @brief - Whether or not the move resulted in a
        /// capture of a piece.
        bool captured;

        /// @brief - Whether this move put the enemy king in
        /// check.
        bool check;

        /// @brief - Whether this move put the enemy king in
        /// checkmate.
        bool checkmate;

        /// @brief - Whether this move put the enemy king in
        /// stalemate.
        bool stalemate;
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

}

#endif    /* ROUND_HH */
