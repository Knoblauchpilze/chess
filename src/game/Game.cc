
# include "Game.hh"
# include <cxxabi.h>
# include "Menu.hh"

namespace pge {

  Game::Game(chess::BoardShPtr board):
    utils::CoreObject("game"),

    m_state(
      State{
        true,  // paused
        true,  // disabled
        false, // terminated
      }
    ),

    m_board(board)
  {
    setService("game");
  }

  Game::~Game() {}

  std::vector<MenuShPtr>
  Game::generateMenus(float /*width*/,
                      float /*height*/)
  {
    log("Generate UI menus here", utils::Level::Info);
    return std::vector<MenuShPtr>();
  }

  void
  Game::performAction(float x, float y) {
    // Only handle actions when the game is not disabled.
    if (m_state.disabled) {
      log("Ignoring action while menu is disabled");
      return;
    }

    log("Click " + std::to_string(x) + "x" + std::to_string(y));
  }

  bool
  Game::step(float /*tDelta*/) {
    // When the game is paused it is not over yet.
    if (m_state.paused) {
      return true;
    }

    /// TODO: Handle this.
    log("Perform step method of the game", utils::Level::Verbose);

    updateUI();

    return true;
  }

  void
  Game::togglePause() {
    if (m_state.paused) {
      resume();
    }
    else {
      pause();
    }

    enable(!m_state.paused);
  }

  void
  Game::enable(bool enable) {
    m_state.disabled = !enable;

    if (m_state.disabled) {
      log("Disabled game UI", utils::Level::Verbose);
    }
    else {
      log("Enabled game UI", utils::Level::Verbose);
    }
  }

  void
  Game::updateUI() {
    /// TODO: Handle this.
    log("Perform update of UI menus", utils::Level::Verbose);
  }

}
