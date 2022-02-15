
# include "MoveGeneration.hh"
# include "Board.hh"

namespace chess {
  namespace ai {

    std::vector<Move>
    generate(const Color& side, const Board& b) noexcept {
      // Gather the list of pieces and generate all possible
      // moves with a default weight.
      Pieces pieces = b.pieces(side);
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
