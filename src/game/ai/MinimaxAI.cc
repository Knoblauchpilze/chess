
# include "MinimaxAI.hh"
# include "MoveGeneration.hh"

namespace {

  float
  pieceValue(const chess::Piece& p) noexcept {
    if (p.pawn()) {
      return 10.0f;
    }
    else if (p.knight()) {
      return 30.0f;
    }
    else if (p.bishop()) {
      return 30.0f;
    }
    else if (p.rook()) {
      return 50.0f;
    }
    else if (p.queen()) {
      return 90.0f;
    }
    else if (p.king()) {
      return 900.0f;
    }
    else {
      // No piece or invalid type, null value.
      return 0.0f;
    }
  }

  float
  evaluateBoard(const chess::Color& c,
                const chess::Board& b) noexcept
  {
    float weight = 0.0f;

    // Traverse the board and evaluate it.
    for (int y = 0 ; y < b.h() ; ++y) {
      for (int x = 0 ; x < b.w() ; ++x) {
        const chess::Piece& p = b.at(x, y);
        float ev = pieceValue(p);

        weight += (p.color() == c ? ev : -ev);
      }
    }

    return weight;
  }

}

# define ROOT_LOG
# define EVALUATE_LOG
# define EXPLORE_LOG
# define SUMMARY_LOG

namespace chess {

  MinimaxAI::MinimaxAI(const Color& color,
                       unsigned depth):
    AI(color, "minimax"),
    m_depth(depth)
  {}

  std::vector<ai::Move>
  MinimaxAI::generateMoves(const Board& b) noexcept {
    // The algorithm behind what is done here has been taken
    // from the following link:
    // https://www.freecodecamp.org/news/simple-chess-ai-step-by-step-1d55a9266977/

    // Generate moves.
    std::vector<ai::Move> moves = ai::generate(m_color, b);

    // For each available position, evaluate the
    // state of the board after making the move.
    for (unsigned id = 0u ; id < moves.size() ; ++id) {
      // Apply the move.
      Board cb(b);
      cb.move(moves[id].start, moves[id].end);

# ifdef ROOT_LOG
      log(
        "[" + std::to_string(id) + "][" + std::to_string(m_depth) + "]"
        " Evaluating " + colorToString(cb.at(moves[id].end).color()) + " " + cb.at(moves[id].end).name() +
        " from " + moves[id].start.toString() + " to " + moves[id].end.toString() +
        " for " + colorToString(m_color),
        utils::Level::Notice
      );
# endif
      moves[id].weight = evaluate(m_depth - 1u, m_color, cb);
# ifdef ROOT_LOG
      log(
        "[" + std::to_string(id) + "][" + std::to_string(m_depth) + "]"
        " Evaluated " + colorToString(cb.at(moves[id].end).color()) + " " + cb.at(moves[id].end).name() +
        " from " + moves[id].start.toString() + " to " + moves[id].end.toString() +
        " for " + colorToString(m_color) +
        " to " + std::to_string(moves[id].weight),
        utils::Level::Notice
      );
# endif
    }

    return moves;

  }

  float
  MinimaxAI::evaluate(std::string uuid,
                      unsigned logDepth,
                      unsigned depth,
                      const Color& c,
                      const Board& b) const noexcept
  {
    bool opponent = (c != m_color);
    // In case the depth is `0`, evaluate the board.
    if (depth == 0u) {
      float w = evaluateBoard(c, b);
# ifdef EVALUATE_LOG
      if (depth >= logDepth) {
        log(
          "[" + uuid + "][" + std::to_string(depth) + "] " +
          "Board has a value of " + std::to_string(opponent ? -w : w) +
          " for " + colorToString(c),
          utils::Level::Info
        );
      }
# endif

      return (opponent ? -w : w);
    }

    // Generate moves.
    std::vector<ai::Move> moves = ai::generate(oppositeColor(c), b);

    // For each available position, evaluate the
    // state of the board after making the move.
    for (unsigned id = 0u ; id < moves.size() ; ++id) {
      Board cb(b);
      cb.move(moves[id].start, moves[id].end);

# ifdef EXPLORE_LOG
      if (depth >= logDepth) {
        log(
          "[" + uuid + "][" + std::to_string(depth) + "] " +
          "Evaluating " + colorToString(cb.at(moves[id].end).color()) + " " + cb.at(moves[id].end).name() +
          " from " + moves[id].start.toString() + " to " + moves[id].end.toString() +
          " for " + colorToString(c),
          utils::Level::Notice
        );
      }
# endif
      moves[id].weight = evaluate(depth - 1u, oppositeColor(c), cb);
# ifdef EXPLORE_LOG
      if (depth >= logDepth) {
        log(
          "[" + uuid + "][" + std::to_string(depth) + "] " +
          "Evaluated " + colorToString(cb.at(moves[id].end).color()) + " " + cb.at(moves[id].end).name() +
          " from " + moves[id].start.toString() + " to " + moves[id].end.toString() +
          " for " + colorToString(c) +
          " to " + std::to_string(moves[id].weight),
          utils::Level::Info
        );
      }
# endif
    }

    // Sort moves based on how favourable they are.
    std::sort(
      moves.begin(),
      moves.end(),
      [opponent](const ai::Move& lhs, const ai::Move& rhs) {
        return (opponent ? lhs.weight > rhs.weight : lhs.weight < rhs.weight);
      }
    );

    std::string score = "N/A";
    if (!moves.empty()) {
      score = std::to_string(moves[0].weight);
    }

# ifdef SUMMARY_LOG
    if (depth >= logDepth) {
      log(
        "[" + uuid + "][" + std::to_string(depth) + "] " +
        "Board has a value of " + score +
        " for " + colorToString(c) +
        " out of " + std::to_string(moves.size()) + " move(s)",
        utils::Level::Info
      );
    }
# endif

    // In case we don't have any legal moves, it means
    // that we're either in stalemate or we can't get
    // out of check.
    if (moves.empty()) {
      warn(
        "[" + uuid + "][" + std::to_string(depth) + "] " +
        "Board has no legal move for " + colorToString(c) +
        " (check: " + std::to_string(b.computeCheck(c)) +
        ", stalemate: " + std::to_string(b.computeStalemate(c)) + ")"
      );

      /// TODO: Handle end of game weights.
      return 0.0f;
    }

    return moves[0].weight;
  }

}
