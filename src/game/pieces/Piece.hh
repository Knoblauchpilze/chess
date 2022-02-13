#ifndef    PIECE_HH
# define   PIECE_HH

# include <core_utils/CoreObject.hh>
# include "Color.hh"
# include "Coordinates.hh"

namespace chess {

  /// @brief - The possible types for a piece.
  enum class Type {
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    None
  };

  /**
   * @brief - Returns a string describing the piece.
   * @param p - the piece whose name should be retrieved.
   * @return - the corresponding string.
   */
  std::string
  pieceToString(const Type& p) noexcept;

  /// @brief - Forward declaration of a board to be used
  /// in the definition of methods.
  class Board;

  class Piece: public utils::CoreObject {
    public:

      /**
       * @brief - Build an invalid piece.
       */
      Piece() noexcept;

      /**
       * @brief - Generate a piece with the specified type. The
       *          default value generate an invalid piece.
       * @param type - the type of the piece to generate.
       * @param color - the color of the piece.
       * @return - the generated piece.
       */
      static
      Piece
      generate(const Type& type = Type::None,
               const Color& color = Color::White) noexcept;

      /**
       * @brief - Reset the piece to an invalid piece.
       */
      void
      reset() noexcept;

      /**
       * @brief - Whether or not this object defines a
       *          valid piece.
       * @return - `true` if the piece is valid.
       */
      bool
      valid() const noexcept;

      /**
       * @brief - Converse method to check if a piece is
       *          not valid.
       * @return - `true` if the piece is invalid.
       */
      bool
      invalid() const noexcept;

      /**
       * @brief - Returns the symbol to represent a piece.
       * @return - the string representing the piece.
       */
      std::string
      algebraic() const noexcept;

      /**
       * @brief - Return the color of this piece.
       * @return - the color of this piece.
       */
      Color
      color() const noexcept;

      /**
       * @brief - Returns a string representing this piece.
       * @return - a string for this piece.
       */
      std::string
      name() const noexcept;

      /**
       * @brief - Whether or not this piece is a pawn.
       */
      bool
      pawn() const noexcept;

      /**
       * @brief - Whether or not this piece is a knight.
       */
      bool
      knight() const noexcept;

      /**
       * @brief - Whether or not this piece is a bishop.
       */
      bool
      bishop() const noexcept;

      /**
       * @brief - Whether or not this piece is a rook.
       */
      bool
      rook() const noexcept;

      /**
       * @brief - Whether or not this piece is a queen.
       */
      bool
      queen() const noexcept;

      /**
       * @brief - Whether or not this piece is a king.
       */
      bool
      king() const noexcept;

      /**
       * @brief - Returns the reachable positions for this
       *          piece based on the current state of the
       *          board.
       * @param p - the current position of this piece.
       * @param b - the board.
       * @return - the list of coordinates reachable by this
       *           piece.
       */
      CoordinatesSet
      reachable(const Coordinates& p,
                const Board& b) const noexcept;

    protected:

      /**
       * @brief - Create a new piece with the specified type
       *          and color.
       * @param type - the type of the piece.
       * @param color - the color of the piece.
       */
      Piece(const Type& type,
            const Color& color) noexcept;

    private:

      /**
       * @brief - The type of the piece.
       */
      Type m_type;

      /**
       * @brief - The color of this piece.
       */
      Color m_color;
  };

}

#endif    /* PIECE_HH */
