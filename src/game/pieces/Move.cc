
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

    CoordinatesSet
    reachable(const Type& t,
              const Color& c,
              const Coordinates& p,
              const Board& b) noexcept
    {
      switch (t) {
        case Pawn:
          return pawn::reachable(c, p, b);
          break;
        case Knight:
          return knight::reachable(c, p, b);
          break;
        case Bishop:
          return bishop::reachable(c, p, b);
          break;
        case Rook:
          return rook::reachable(c, p, b);
          break;
        case Queen:
          return queen::reachable(c, p, b);
          break;
        case King:
          return king::reachable(c, p, b);
          break;
        case None:
        default:
          return CoordinatesSet();
      }
    }

  }
}
