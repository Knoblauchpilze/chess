
# include "Move.hh"
# include "Board.hh"
# include "Pawn.hh"
# include "Knight.hh"
# include "Bishop.hh"
# include "Rook.hh"
# include "Queen.hh"
# include "King.hh"

namespace chess {
  namespace pieces {

    /// @brief - None related functions.
    namespace none {

      inline
      bool
      valid(const Color& /*c*/,
            const Coordinates& /*start*/,
            const Coordinates& /*end*/,
            const Board& /*b*/) noexcept
      {
        return false;
      }

      inline
      std::vector<Coordinates>
      threaten(const Color& /*c*/,
               const Coordinates& /*p*/,
               const Board& /*b*/) noexcept
      {
        return std::vector<Coordinates>();
      }

      inline
      std::vector<Coordinates>
      move(const Color& /*c*/,
           const Coordinates& /*p*/,
           const Board& /*b*/) noexcept
      {
        return std::vector<Coordinates>();
      }
    }

    bool
    valid(const Type& t,
          const Color& c,
          const Coordinates& start,
          const Coordinates& end,
          const Board& b) noexcept
    {
      switch (t) {
        case Pawn:
          return pawn::valid(c, start, end, b);
        case Knight:
          return knight::valid(c, start, end, b);
        case Bishop:
          return bishop::valid(c, start, end, b);
        case Rook:
          return rook::valid(c, start, end, b);
        case Queen:
          return queen::valid(c, start, end, b);
        case King:
          return king::valid(c, start, end, b);
        case None:
        default:
          return none::valid(c, start, end, b);
      }
    }

    std::vector<Coordinates>
    threaten(const Type& t,
             const Color& c,
             const Coordinates& p,
             const Board& b) noexcept
    {
      switch (t) {
        case Pawn:
          return pawn::threaten(c, p, b);
        case Knight:
          return knight::threaten(c, p, b);
        case Bishop:
          return bishop::threaten(c, p, b);
        case Rook:
          return rook::threaten(c, p, b);
        case Queen:
          return queen::threaten(c, p, b);
        case King:
          return king::threaten(c, p, b);
        case None:
        default:
          return none::threaten(c, p, b);
      }
    }

    std::vector<Coordinates>
    move(const Type& t,
         const Color& c,
         const Coordinates& p,
         const Board& b) noexcept
    {
      switch (t) {
        case Pawn:
          return pawn::move(c, p, b);
        case Knight:
          return knight::move(c, p, b);
        case Bishop:
          return bishop::move(c, p, b);
        case Rook:
          return rook::move(c, p, b);
        case Queen:
          return queen::move(c, p, b);
        case King:
          return king::move(c, p, b);
        case None:
        default:
          return none::move(c, p, b);
      }
    }

  }
}
