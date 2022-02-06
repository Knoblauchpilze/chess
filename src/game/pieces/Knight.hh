#ifndef    KNIGHT_HH
# define   KNIGHT_HH

# include <vector>
# include "Pieces.hh"

namespace chess {

  /// @brief - Forward declaration of the Board to allow its
  /// use as parameter in the functions.
  class Board;

  namespace pieces {
    namespace knight {

      bool
      valid(const Color& c,
            const Coordinates& start,
            const Coordinates& end,
            const Board& b) noexcept;

      std::vector<Coordinates>
      threaten(const Color& c,
               const Coordinates& p,
               const Board& b) noexcept;

      std::vector<Coordinates>
      move(const Color& c,
           const Coordinates& p,
           const Board& b) noexcept;

    }
  }
}

#endif    /* KNIGHT_HH */
