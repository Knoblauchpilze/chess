
# include "AI.hh"

namespace chess {

  AI::AI(const Color& color):
    utils::CoreObject("ai"),

    m_color(color)
  {
    setService("chess");
  }

  void
  AI::play(Board& /*b*/) noexcept {
    /// TODO: Handle the AI play.
    warn("Should play for \"" + colorToString(m_color) + "\"");
  }

}
