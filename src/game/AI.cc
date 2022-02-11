
# include "AI.hh"

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

  void
  AI::play(Board& b) noexcept {
    // Make sure that the current player is the one
    // assigned to the player.
    Round r = b.getCurrentRound();

    if (r.whitePlayed() && m_color == Color::White) {
      // Skip as white already played.
      return;
    }
    if (!r.whitePlayed() && m_color == Color::Black) {
      // Skip as white still didn't play.
      return;
    }
    if (r.whitePlayed() && r.blackPlayed() && m_color == Color::Black) {
      // Skip as black already played.
      return;
    }

    /// TODO: Handle the AI play.
    warn("Should play for \"" + colorToString(m_color) + "\"");
  }

}
