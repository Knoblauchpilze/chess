
# include "RandomAI.hh"
# include <random>
# include "MoveGeneration.hh"

namespace chess {

  RandomAI::RandomAI(const Color& color):
    AI(color, "random")
  {}

  std::vector<ai::Move>
  RandomAI::generateMoves(const Board& b) noexcept {
    // Generate moves.
    std::vector<ai::Move> moves = ai::generate(m_color, b);

    // Randomly classify moves.
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> distr(0, moves.size());

    // Assign a random evaluation to each move.
    for (unsigned id = 0u ; id < moves.size() ; ++id) {
      moves[id].weight = distr(rng);
    }

    return moves;
  }

}

