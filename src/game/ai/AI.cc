
# include "AI.hh"
# include "MoveGeneration.hh"
# include "BoardEvaluation.hh"

namespace chess {

  AI::AI(const Color& color):
    utils::CoreObject("ai"),

    m_color(color)
  {
    setService("chess");
  }

  Color
  AI::side() const noexcept {
    return m_color;
  }

  bool
  AI::play(ChessGame& b) noexcept {
    // Make sure that the current player is the one
    // assigned to the player.
    Round r = b.getCurrentRound();

    if (r.whitePlayed() && m_color == Color::White) {
      // Skip as white already played.
      return false;
    }
    if (!r.whitePlayed() && m_color == Color::Black) {
      // Skip as white still didn't play.
      return false;
    }
    if (r.whitePlayed() && r.blackPlayed() && m_color == Color::Black) {
      // Skip as black already played.
      return false;
    }

    // Generate the list of moves available for all the
    // pieces of the color managed by the AI.
    std::vector<ai::Move> moves = ai::generate(m_color, b());

    // In case there's no valid move, do nothing.
    if (moves.empty()) {
      log("No legal moves for " + colorToString(m_color));
      return false;
    }

    log("Generated " + std::to_string(moves.size()) + " move(s) for " + colorToString(m_color));

    // Prune the moves and evaluate all of them.
    ai::Strategy strat = ai::Strategy::Minimax;
    ai::evaluate(moves, m_color, b(), strat);

    ai::Move best = moves[0];
    log("Picked move from " + best.start.toString() + " to " + best.end.toString() + " with weight " + std::to_string(best.weight), utils::Level::Info);

    if (!b.move(best.start, best.end)) {
      warn("Failed to apply move from " + best.start.toString() + " to " + best.end.toString() + " picked by AI");
      return false;
    }

    // Handle pawn promotion.
    if (best.end.y() == 0 || best.end.y() == b().h() - 1) {
      const Piece& p = b().at(best.end);
      if (p.pawn()) {
        // For now, we always promote to a queen.
        b.promote(best.end, Type::Queen);
      }
    }

    return true;
  }

}
