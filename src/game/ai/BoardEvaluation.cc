
# include "BoardEvaluation.hh"
# include <random>
# include <algorithm>
# include "Board.hh"

# include <iostream>

namespace {

  float
  evaluateBoard(chess::Coordinates start,
                chess::Coordinates end,
                const chess::Color& c,
                const chess::Board& b)
  {
    // Evaluation of a piece function.
    auto evaluate = [](const chess::Piece& p) {
      if (p.pawn()) {
        return 10;
      }
      else if (p.knight()) {
        return 30;
      }
      else if (p.bishop()) {
        return 30;
      }
      else if (p.rook()) {
        return 50;
      }
      else if (p.queen()) {
        return 90;
      }
      else if (p.king()) {
        return 900;
      }
      else {
        // No piece or invalid type, null value.
        return 0;
      }
    };

    float weight = 0.0f;

    // Traverse the board and evaluate it.
    const chess::Piece& p = b.at(start);

    for (int y = 0 ; y < b.h() ; ++y) {
      for (int x = 0 ; x < b.w() ; ++x) {
        // Ignore the starting coordinate and replace the
        // ending coordinate with the starting position.
        if (start.x() == x && start.y() == y) {
          continue;
        }

        float ev = 0.0f;
        if (end.x() == x && end.y() == y) {
          weight += evaluate(p);
          continue;
        }

        const chess::Piece& p = b.at(x, y);
        ev = evaluate(p);

        weight += (p.color() == c ? ev : -ev);
      }
    }

    return weight;
  }

}

namespace chess {
  namespace ai {

    void
    randomEvaluation(std::vector<Move>& out) noexcept {
      // Generate random devices.
      std::random_device rd;
      std::mt19937 rng(rd());
      std::uniform_real_distribution<float> distr(0.0f, 1.0f);

      // Assign a random evaluation to each move.
      for (unsigned id = 0u ; id < out.size() ; ++id) {
        out[id].weight = distr(rng);
      }
    }

    void
    minimaxEvaluation(std::vector<Move>& out,
                      const Color& side,
                      const Board& b) noexcept
    {
      // For each available position, evaluate the
      // state of the board after making the move.
      for (unsigned id = 0u ; id < out.size() ; ++id) {
        out[id].weight = evaluateBoard(out[id].start, out[id].end, side, b);
      }
    }

    void
    evaluate(std::vector<Move>& out,
             const Color& side,
             const Board& b,
             const Strategy& strat) noexcept
    {
      switch (strat) {
        case Strategy::Minimax:
          minimaxEvaluation(out, side, b);
          break;
        case Strategy::Random:
        default:
          // Assume default is random move.
          randomEvaluation(out);
          break;
      }

      // Once the moves have been evaluated, sort
      // them from maximum to minimum.
      std::sort(
        out.begin(),
        out.end(),
        [](const Move& lhs, const Move& rhs) {
          return lhs.weight > rhs.weight;
        }
      );
    }

  }
}
