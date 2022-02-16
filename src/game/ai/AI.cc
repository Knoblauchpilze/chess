
# include "AI.hh"
# include "MoveGeneration.hh"

namespace chess {

  AI::AI(const Color& color,
         const std::string& name):
    utils::CoreObject(name),

    m_color(color)
  {
    setService("ai");
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

    // Generate the best move using the interface method.
    std::vector<ai::Move> moves = generateMoves(b());
    if (moves.empty()) {
      log("No legal moves for " + colorToString(m_color));
      return false;
    }

    log("Generated " + std::to_string(moves.size()) + " move(s) for " + colorToString(m_color));

    // Sort moves based on how favourable they are.
    std::sort(
      moves.begin(),
      moves.end(),
      [](const ai::Move& lhs, const ai::Move& rhs) {
        return lhs.weight > rhs.weight;
      }
    );

    ai::Move best = moves[0];
    log("Picked move from " + best.start.toString() + " to " + best.end.toString() + " with weight " + std::to_string(best.weight), utils::Level::Info);

    if (!b.move(best.start, best.end)) {
      warn("Failed to apply move from " + best.start.toString() + " to " + best.end.toString() + " picked by AI");
      return false;
    }

    // Handle pawn promotion.
    /// TODO: Move that to the AI as well.
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
