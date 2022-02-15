
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

      log("Evaluating move of " + colorToString(cb.at(moves[id].end).color()) + " " + cb.at(moves[id].end).name() + " from " + moves[id].start.toString() + " to " + moves[id].end.toString() + " for " + colorToString(m_color) + " at depth " + std::to_string(m_depth), utils::Level::Notice);
      moves[id].weight = evaluate(m_depth - 1u, m_color, cb);
    }

    return moves;

  }

  float
  MinimaxAI::evaluate(unsigned depth,
                      const Color& c,
                      const Board& b) const noexcept
  {
    // In case the depth is `0`, evaluate the board.
    if (depth == 0u) {
      float w = evaluateBoard(c, b);
      log("Evaluated board to " + std::to_string(w) + " for " + colorToString(c) + " at depth " + std::to_string(depth), utils::Level::Info);
      return w;
    }

    // Generate moves.
    std::vector<ai::Move> moves = ai::generate(oppositeColor(c), b);

    // For each available position, evaluate the
    // state of the board after making the move.
    for (unsigned id = 0u ; id < moves.size() ; ++id) {
      Board cb(b);
      cb.move(moves[id].start, moves[id].end);

      // log("Evaluating move of " + colorToString(cb.at(moves[id].end).color()) + " " + cb.at(moves[id].end).name() + " from " + moves[id].start.toString() + " to " + moves[id].end.toString() + " for " + colorToString(oppositeColor(c)) + " at depth " + std::to_string(depth));
      moves[id].weight = evaluate(depth - 1u, oppositeColor(c), cb);
    }

    // Sort moves based on how favourable they are.
    std::sort(
      moves.begin(),
      moves.end(),
      [](const ai::Move& lhs, const ai::Move& rhs) {
        return lhs.weight > rhs.weight;
      }
    );

    std::string score = "N/A";
    if (!moves.empty()) {
      score = std::to_string(moves[0].weight);
    }
    log("Found " + std::to_string(moves.size()) + " in depth " + std::to_string(depth) + " for " + colorToString(c) + ", best score is " + score, utils::Level::Debug);

    return moves[0].weight;
  }

}
