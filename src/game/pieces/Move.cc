
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

    std::unordered_set<Coordinates>
    reachable(const Type& t,
              const Color& c,
              const Coordinates& p,
              const Board& b) noexcept
    {
      std::vector<Coordinates> v;
      switch (t) {
        case Pawn:
          v = pawn::reachable(c, p, b);
          break;
        case Knight:
          v = knight::reachable(c, p, b);
          break;
        case Bishop:
          v = bishop::reachable(c, p, b);
          break;
        case Rook:
          v = rook::reachable(c, p, b);
          break;
        case Queen:
          v = queen::reachable(c, p, b);
          break;
        case King:
          v = king::reachable(c, p, b);
          break;
        case None:
        default:
          v = std::vector<Coordinates>();
          break;
      }

      std::unordered_set<Coordinates> out;
      out.insert(v.begin(), v.end());

      return out;
    }

  }
}
