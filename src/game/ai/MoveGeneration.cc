
# include "MoveGeneration.hh"
# include "Board.hh"

namespace chess {
  namespace ai {

    std::vector<Move>
    generate(const Color& side, const Board& b) noexcept {
      std::vector<std::pair<Piece, Coordinates>> pieces;

      // Gather all the pieces of the input color.
      for (int y = 0 ; y < b.h() ; ++y) {
        for (int x = 0 ; x < b.w() ; ++x) {
          const Piece& p = b.at(x, y);

          // Ignore empty cells and pieces of our opponent.
          if (!p.valid() || p.color() != side) {
            continue;
          }

          pieces.push_back(std::make_pair(p, Coordinates(x, y)));
        }
      }

      // Generate all possible moves.
      std::vector<Move> out;

      for (unsigned id = 0u ; id < pieces.size() ; ++id) {
        CoordinatesSet av = pieces[id].first.reachable(pieces[id].second, b);

        for (CoordinatesSet::const_iterator it = av.cbegin() ; it != av.cend() ; ++it) {
          // Filter invalid moves.
          if (b.leadsToCheck(pieces[id].second, *it)) {
            continue;
          }

          Move m = {
            pieces[id].second, // Starting position.
            *it,               // End position.
            0.0f               // Weight.
          };

          out.push_back(m);
        }
      }

      return out;
    }

  }
}
