#ifndef    ROOK_HH
# define   ROOK_HH

# include <vector>
# include "Pieces.hh"

namespace chess {

  /// @brief - Forward declaration of the Board to allow its
  /// use as parameter in the functions.
  class Board;

  namespace pieces {
    namespace rook {

      std::vector<Coordinates>
      reachable(const Color& c,
                const Coordinates& p,
                const Board& b) noexcept;

      bool
      valid(const Color& c,
            const Coordinates& start,
            const Coordinates& end,
            const Board& b) noexcept;

    }
  }
}

#endif    /* ROOK_HH */
