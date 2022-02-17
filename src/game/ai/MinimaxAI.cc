
# include "MinimaxAI.hh"
# include <core_utils/Chrono.hh>
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
    unsigned nodes = 0u;

    // For each available position, evaluate the
    // state of the board after making the move.
    {
      utils::Chrono<> clock("Evaluation of " + std::to_string(moves.size()) + " move(s)", "moves", utils::Level::Info);

      float alpha = std::numeric_limits<float>::lowest();
      float beta = std::numeric_limits<float>::max();

      for (unsigned id = 0u ; id < moves.size() ; ++id) {
        // Apply the move and auto-promote to queen.
        Board cb(b);
        cb.move(moves[id].start, moves[id].end, true, Type::Queen);

# ifdef ROOT_LOG
        std::string msg = "Evaluating ";
        msg += cb.at(moves[id].end).fullName();
        msg += " from ";
        msg += moves[id].start.toString();
        msg += " to ";
        msg += moves[id].end.toString();

        // log("[0] " + colorToString(m_color) + " " + msg, utils::Level::Notice);
# endif

        // The idea of the alpha-beta pruning is described
        // in the following link:
        // https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning#Pseudocode
        unsigned logDepth = 0u;

        unsigned visited = 0u;
        moves[id].weight = -evaluate(oppositeColor(m_color), cb, false, -beta, -alpha, 1u, &visited, logDepth);
        nodes += visited;

# ifdef ROOT_LOG
        msg = "Evaluated ";
        msg += cb.at(moves[id].end).fullName();
        msg += " from ";
        msg += moves[id].start.toString();
        msg += " to ";
        msg += moves[id].end.toString();
        msg += " to ";
        msg += std::to_string(moves[id].weight);
        msg += " (nodes: ";
        msg += std::to_string(visited);
        msg += ")";

        log("[0] " + colorToString(m_color) + " " + msg, utils::Level::Notice);
# endif
        // Handle alpha-beta pruning.
        alpha = std::max(alpha, moves[id].weight);
      }

      log("Visited " + std::to_string(nodes) + " node(s) to analyze " + std::to_string(moves.size()) + " move(s)", utils::Level::Verbose);
    }

    return moves;

  }

  float
  MinimaxAI::evaluate(const Color& c,
                      const Board& b,
                      bool maximizing,
                      float alpha,
                      float beta,
                      unsigned depth,
                      unsigned* nodes,
                      unsigned logDepth) const noexcept
  {
    auto indent = [](unsigned depth) {
      return std::string(2u * depth, ' ');
    };

    auto print = [this, &depth, &logDepth, &indent, &c](const std::string msg, const utils::Level& level) {
      if (depth <= logDepth) {
        log(
          "[" + std::to_string(depth) + "] " + indent(depth) +
          colorToString(c) + " " + msg,
          level
        );
      }
    };

    // Color represents the player that just moved and
    // led to this state of the board.
    // We want this person to be maximizing the score.
    // Hence the opposite color is the minimizing player.
    if (depth >= m_depth) {
      // We reached the terminal evaluation, evaluate
      // the board for the player that requested the
      // call.
      float w = evaluateBoard(c, b);
# ifdef EVALUATE_LOG
      print("board: " + std::to_string(w), utils::Level::Notice);
# endif

      *nodes = 1u;

      return w;
    }

    // Generate moves for the current color.
    std::vector<ai::Move> moves = ai::generate(c, b);

    // For each available position, evaluate the
    // state of the board after making the move.
    for (unsigned id = 0u ; id < moves.size() ; ++id) {
      Board cb(b);
      // Allow auto-promotion to queen.
      cb.move(moves[id].start, moves[id].end, true, Type::Queen);

# ifdef EXPLORE_LOG
      std::string msg = "Evaluating ";
      msg += cb.at(moves[id].end).fullName();
      msg += " from ";
      msg += moves[id].start.toString();
      msg += " to ";
      msg += moves[id].end.toString();
      print(msg, utils::Level::Notice);
# endif

      unsigned visited = 0u;

      // The returned value represents the evaluation of the
      // board and the best moves for the opponent. To obtain
      // the valuation for us, we need to negate it. This is
      // controlled by the maximizing value.
      moves[id].weight = -evaluate(oppositeColor(c), cb, !maximizing, -beta, -alpha, depth + 1u, &visited, logDepth);
      *nodes += visited;

# ifdef EXPLORE_LOG
      msg = "Evaluated ";
      msg += cb.at(moves[id].end).fullName();
      msg += " from ";
      msg += moves[id].start.toString();
      msg += " to ";
      msg += moves[id].end.toString();
      msg += " to ";
      msg += std::to_string(moves[id].weight);
      msg += " (nodes: ";
      msg += std::to_string(visited);
      msg += ")";
      print(msg, utils::Level::Notice);
# endif

      // Handle alpha-beta pruning.
      alpha = std::max(alpha, moves[id].weight);
      if (alpha >= beta) {
        break;
      }
    }

    // Sort the move in ascending or descending order
    // based on whether we should minimize or maximize
    // the score.
    std::sort(
      moves.begin(),
      moves.end(),
      [](const ai::Move& lhs, const ai::Move& rhs) {
        return lhs.weight > rhs.weight;
      }
    );

    // In case we don't have any legal moves, it means
    // that we're either in stalemate or we can't get
    // out of check.
    if (moves.empty()) {
      warn(
        colorToString(c) + " has no legal move (check: " +
        std::to_string(b.computeCheck(c)) +
        ", stalemate: " + std::to_string(b.computeStalemate(c)) + ")"
      );

      /// TODO: Handle end of game weights.
      return maximizing ? std::numeric_limits<float>::lowest() : std::numeric_limits<float>::max();
    }

# ifdef SUMMARY_LOG
    std::string msg = "Analyzed ";
    msg += std::to_string(moves.size());
    msg += " move(s)";
    msg += ", after ";
    msg += (maximizing ? "maximizing" : "minimizing");
    msg += " best: ";
    msg += std::to_string(moves[0].weight);
    msg += " (nodes: ";
    msg += std::to_string(*nodes);
    msg += ")";
    print(msg, utils::Level::Notice);
# endif

    return moves[0].weight;
  }

}
