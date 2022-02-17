#ifndef    TYPES_HH
# define   TYPES_HH

# include "Coordinates.hh"

namespace chess {

  /// @brief - Forward declaration of the board class to be
  /// able to use it as an argument.
  class Board;

  namespace ai {

    /// @brief - Defines a pair of coordinates which represent
    /// a move on the board.
    struct Move {
      // Defines the starting coordinates for the move.
      Coordinates start;

      // Defines the ending coordinates for the move.
      Coordinates end;

      // An information of the weight of this move, which
      // describe how favorable it is for the side playing
      // it.
      int weight;
    };

  }
}

#endif    /* TYPES_HH */
