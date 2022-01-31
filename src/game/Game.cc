
# include "Game.hh"
# include <cxxabi.h>
# include "Menu.hh"

namespace {

  std::shared_ptr<chess::Coordinates>
  convertCoords(float x, float y, float w, float h) {
    // Account for center of tile.
    x += 0.5f;
    y += 0.5f;

    if (x < 0.0f || y < 0.0f || x > w || y > h) {
      // Outside of the board.
      return nullptr;
    }

    unsigned ux = static_cast<unsigned>(x);
    unsigned uy = static_cast<unsigned>(y);

    // Note that the board is actually displayed upside down.
    uy = h - 1u - uy;

    return std::make_shared<chess::Coordinates>(ux, uy);
  }

}

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

    CoordinatesShPtr coords = convertCoords(x, y, 1.0f * m_board->w(), 1.0f * m_board->h());
    if (coords == nullptr) {
      // Reset the current selected starting position for
      // a piece move.
      m_start.reset();
      return;
    }

    // Two main possbilities: either we already have a valid
    // starting location, or we don't.
    if (!m_start) {
      chess::pieces::Cell c = m_board->at(*coords);
      if (c.type == chess::pieces::None) {
        return;
      }

      m_start = coords;
      log("Click at " + m_start->toString());
      return;
    }

    // Attempt to move this piece: this can fail if the piece
    // is not allowed to move there (for any reason).
    if (!m_board->move(*m_start, *coords)) {
      // Keep the initial position, the user can try again.
      return;
    }

    // Reset starting location after the move.
    m_start.reset();
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
