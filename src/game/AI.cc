
# include "AI.hh"

namespace chess {

  AI::AI(const pieces::Color& color):
    utils::CoreObject("ai"),

    m_color(color)
  {
    setService("chess");
  }

  void
  AI::play(Board& /*b*/) noexcept {
    /// TODO: Handle the AI play.
    warn("Should play for \"" + pieces::toString(m_color) + "\"");
  }

}
