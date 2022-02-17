
# include "MinimaxAI.hh"
# include <core_utils/Chrono.hh>
# include "MoveGeneration.hh"

/// @brief - Defines the evaluation of the checkmate position.
/// This value should be high enough to not be mistaken for
/// another position but small enough so that we can still be
/// able to distinguish between faster mates.
# define CHECKMATE_EVALUATION 32000

/// @brief - Activate these logs to debug the AI.
// # define PRE_ROOT_LOG
// # define ROOT_LOG
// # define EVALUATE_LOG
// # define EXPLORE_LOG
// # define SUMMARY_LOG

namespace {

  int
  pieceValue(const chess::Piece& p) noexcept {
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
  }

  int
  evaluateBoard(const chess::Color& c,
                const chess::Board& b) noexcept
  {
    int weight = 0;

    // Traverse the board and evaluate it.
    for (int y = 0 ; y < b.h() ; ++y) {
      for (int x = 0 ; x < b.w() ; ++x) {
        const chess::Piece& p = b.at(x, y);
        int ev = pieceValue(p);

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
    unsigned nodes = 0u;
    unsigned pruned = 0u;

    // For each available position, evaluate the
    // state of the board after making the move.
    {
      utils::Chrono<> clock("Evaluation of " + std::to_string(moves.size()) + " move(s)", "moves", utils::Level::Info);

      int alpha = -CHECKMATE_EVALUATION;
      int beta = CHECKMATE_EVALUATION;

      for (unsigned id = 0u ; id < moves.size() ; ++id) {
        // Apply the move and auto-promote to queen.
        Board cb(b);
        cb.move(moves[id].start, moves[id].end, true, Type::Queen);

# ifdef PRE_ROOT_LOG
        std::string msg = "Evaluating ";
        msg += cb.at(moves[id].end).fullName();
        msg += " from ";
        msg += moves[id].start.toString();
        msg += " to ";
        msg += moves[id].end.toString();

        log("[0] " + colorToString(m_color) + " " + msg, utils::Level::Notice);
# endif

        // The idea of the alpha-beta pruning is described
        // in the following link:
        // https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning#Pseudocode
        unsigned visited = 0u;
        moves[id].weight = -evaluate(oppositeColor(m_color), cb, false, -beta, -alpha, 1u, &visited, &pruned);
        nodes += visited;

# ifdef ROOT_LOG
#  ifdef PRE_ROOT_LOG
        msg = "Evaluated ";
#  else
        std::string msg = "Evaluated ";
#  endif
        msg += cb.at(moves[id].end).fullName();
        msg += " from ";
        msg += moves[id].start.toString();
        msg += " to ";
        msg += moves[id].end.toString();
        msg += " to ";
        msg += std::to_string(moves[id].weight);
        msg += " (nodes: ";
        msg += std::to_string(visited);
        msg += ", pruned: ";
        msg += std::to_string(pruned);
        msg += ")";

        log("[0] " + colorToString(m_color) + " " + msg, utils::Level::Notice);
# endif
        // Handle alpha-beta pruning.
        alpha = std::max(alpha, moves[id].weight);
      }

      log("Visited " + std::to_string(nodes) + " node(s) (" + std::to_string(pruned) + " pruned) to analyze " + std::to_string(moves.size()) + " move(s)", utils::Level::Info);
    }

    return moves;

  }

  int
  MinimaxAI::evaluate(const Color& c,
                      const Board& b,
                      bool maximizing,
                      int alpha,
                      int beta,
                      unsigned depth,
                      unsigned* nodes,
                      unsigned* pruned) const noexcept
  {
# if defined(EVALUATE_LOG) || defined(EXPLORE_LOG) || defined(SUMMARY_LOG)
    auto indent = [](unsigned depth) {
      return std::string(2u * depth, ' ');
    };

    auto print = [this, &depth, &indent, &c](const std::string msg, const utils::Level& level) {
      log(
        "[" + std::to_string(depth) + "] " + indent(depth) +
        colorToString(c) + " " + msg,
        level
      );
    };
# endif

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
      moves[id].weight = -evaluate(oppositeColor(c), cb, !maximizing, -beta, -alpha, depth + 1u, &visited, pruned);
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
        *pruned += moves.size() - id;
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
      bool check = b.computeCheck(c);
      bool stalemate = b.computeStalemate(c);

      // We want to value checkmate with a very high
      // value, but stalemate not that much. It also
      // depends on whether we want to maximize or
      // to minimize the baord evaluation.
      // Note that to favour the moves that lead to
      // a checkmate faster, we include the depth of
      // the evaluation in the weight.
      float w = CHECKMATE_EVALUATION - depth;
      if (maximizing) {
        w = -CHECKMATE_EVALUATION;
      }

      // In case of checkmate, reverse the values.
      if (check && stalemate) {
        w = -w;
      }

      return w;
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
