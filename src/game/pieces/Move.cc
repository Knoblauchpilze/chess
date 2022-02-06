
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
          return false;
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
          return knight::reachable(c, p, b);
        case Bishop:
          return bishop::reachable(c, p, b);
        case Rook:
          return rook::reachable(c, p, b);
        case Queen:
          return queen::reachable(c, p, b);
        case King:
          return king::reachable(c, p, b);
        case None:
        default:
          return std::vector<Coordinates>();
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
          return knight::reachable(c, p, b);
        case Bishop:
          return bishop::reachable(c, p, b);
        case Rook:
          return rook::reachable(c, p, b);
        case Queen:
          return queen::reachable(c, p, b);
        case King:
          return king::reachable(c, p, b);
        case None:
        default:
          return std::vector<Coordinates>();
      }
    }

  }
}
