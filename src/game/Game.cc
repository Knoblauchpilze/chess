
# include "Game.hh"
# include <cxxabi.h>
# include "Menu.hh"

# define ALERT_DURATION_MS 3000

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

  pge::MenuShPtr
  generateMenu(const olc::vi2d& size,
               const std::string& text,
               const std::string& name)
  {
    pge::menu::MenuContentDesc fd = pge::menu::newMenuContent(text, "", size);
    fd.color = olc::WHITE;

    return std::make_shared<pge::Menu>(
      olc::vi2d(),
      size,
      name,
      pge::menu::newColoredBackground(olc::VERY_DARK_GREEN),
      fd,
      pge::menu::Layout::Horizontal,
      false,
      false
    );
  }

  pge::MenuShPtr
  generateAlertMenu(const olc::vi2d& pos,
                    const olc::vi2d& size,
                    const std::string& text,
                    const std::string& name)
  {
    pge::menu::MenuContentDesc fd = pge::menu::newMenuContent(text, "", size);
    fd.color = olc::RED;
    fd.align = pge::menu::Alignment::Center;

    return std::make_shared<pge::Menu>(
      pos,
      size,
      name,
      pge::menu::newColoredBackground(olc::VERY_DARK_RED),
      fd,
      pge::menu::Layout::Horizontal,
      false,
      false
    );
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

    m_board(board),
    m_start(nullptr),
    m_ai(std::make_shared<chess::AI>(chess::Color::Black)),
    m_menus()
  {
    setService("game");
  }

  Game::~Game() {}

  std::vector<MenuShPtr>
  Game::generateMenus(float width,
                      float height)
  {
    // Generate the main menu.
    pge::MenuShPtr status = generateMenu(olc::vi2d(width, 50), "", "root");

    static const unsigned sk_lastMoves = 5u;
    // And additional menus: we will display 3 menus
    // and the number of moves. The percentage are
    // arbitrary.
    int wRound = static_cast<int>(width * 0.1f);
    int wPlayer = static_cast<int>(width * 0.2f);
    int wStatus = static_cast<int>(width * 0.2f);
    int wMove = static_cast<int>(width * 0.5f / sk_lastMoves);

    int mHeight = 30;

    m_menus.round = generateMenu(olc::vi2d(wRound, mHeight), "Round: N/A", "round");
    m_menus.player = generateMenu(olc::vi2d(wPlayer, mHeight), "White", "player");
    m_menus.status = generateMenu(olc::vi2d(wStatus, mHeight), "All good", "status");

    for (unsigned id = 0u ; id < sk_lastMoves ; ++id) {
      m_menus.moves.push_back(
        generateMenu(olc::vi2d(wMove, mHeight), "", "move_" + std::to_string(id))
      );
    }

    // Register the menu in the parent one.
    status->addMenu(m_menus.round);
    status->addMenu(m_menus.player);
    status->addMenu(m_menus.status);

    for (unsigned id = 0u ; id < sk_lastMoves ; ++id) {
      status->addMenu(m_menus.moves[id]);
    }

    // Initialize the current move at one minus the
    // maximum number of move so that the first move
    // can actually be registered.
    m_menus.move = sk_lastMoves - 1u;
    m_menus.checkDate = utils::TimeStamp();
    m_menus.wasInCheck = false;
    m_menus.checkmateDate = utils::TimeStamp();
    m_menus.wasInCheckmate = false;

    // Generate the alert menu indicating that the player
    // is in check or checkmate.
    pge::menu::MenuContentDesc fd = pge::menu::newMenuContent("ALERT", "", olc::vi2d(300, 150));
    fd.color = olc::RED;
    fd.align = pge::menu::Alignment::Center;

    m_menus.check = generateAlertMenu(
      olc::vi2d((width - 300.0f) / 2.0f, (height - 150.0f) / 2.0f),
      olc::vi2d(300, 150),
      "You're in check !",
      "check"
    );
    m_menus.check->setVisible(false);
    m_menus.checkmate = generateAlertMenu(
      olc::vi2d((width - 300.0f) / 2.0f, (height - 150.0f) / 2.0f),
      olc::vi2d(300, 150),
      "You're in checkmate !",
      "check"
    );
    m_menus.checkmate->setVisible(false);

    std::vector<MenuShPtr> menus;
    menus.push_back(status);
    menus.push_back(m_menus.check);
    menus.push_back(m_menus.checkmate);

    return menus;
  }

  void
  Game::performAction(float x, float y) {
    // Only handle actions when the game is not disabled.
    if (m_state.disabled) {
      log("Ignoring action while menu is disabled");
      return;
    }

    chess::CoordinatesShPtr coords = convertCoords(x, y, 1.0f * m_board->w(), 1.0f * m_board->h());
    if (coords == nullptr) {
      // Reset the current selected starting position for
      // a piece move.
      m_start.reset();
      return;
    }

    // Two main possbilities: either we already have a valid
    // starting location, or we don't.
    if (!m_start) {
      const chess::Piece& c = m_board->at(*coords);
      if (c.invalid()) {
        return;
      }

      // Prevent selection of pieces that are not meant to
      // play at the moment.
      if (c.color() != m_board->getPlayer()) {
        return;
      }

      m_start = coords;
      log("Click at " + m_start->toString());
      return;
    }

    // In case the end location is the same as the starting
    // one, consider that we unselect the starting location.
    if (*m_start == *coords) {
      m_start.reset();
      return;
    }

    // Attempt to move this piece: this can fail if the piece
    // is not allowed to move there (for any reason).
    if (!m_board->move(*m_start, *coords)) {
      // Keep the initial position, the user can try again.
      return;
    }

    // Make the AI play as the move was valid if we reach this
    // point: this will make sure that the user can play again
    // immediately.
    m_ai->play(*m_board);

    // Reset starting location after the move.
    m_start.reset();
  }

  bool
  Game::step(float /*tDelta*/) {
    // When the game is paused it is not over yet.
    if (m_state.paused) {
      return true;
    }

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
    // Fetch properties to update.
    chess::Color p = m_board->getPlayer();
    chess::Round r = m_board->getCurrentRound();
    chess::Round m = m_board->getLastRound();

    m_menus.round->setText("Round: " + std::to_string(r.id() + 1u));
    m_menus.player->setText(chess::colorToString(p));

    std::string st = "All good";
    if (m_board->isInCheck(p)) {
      st = "Check";

      if (!m_menus.wasInCheck) {
        m_menus.checkDate = utils::now();
        m_menus.wasInCheck = true;
        m_menus.check->setVisible(true);
      }
      else if (utils::now() > m_menus.checkDate + utils::toMilliseconds(ALERT_DURATION_MS)) {
        m_menus.check->setVisible(false);
      }
      else {
        float d = utils::diffInMs(m_menus.checkDate, utils::now()) / ALERT_DURATION_MS;
        uint8_t alpha = static_cast<uint8_t>(
          std::clamp((1.0f - d) * pge::alpha::Opaque, 0.0f, 255.0f)
        );
        m_menus.check->setBackground(pge::menu::newColoredBackground(olc::Pixel(64, 0, 0, alpha)));
      }
    }
    else if (m_menus.wasInCheck) {
      m_menus.check->setVisible(false);
      m_menus.wasInCheck = false;
    }
    if (m_board->isInCheckmate(p)) {
      st = "Checkmate";
      if (!m_menus.wasInCheckmate) {
        m_menus.checkmateDate = utils::now();
        m_menus.wasInCheckmate = true;
        m_menus.checkmate->setVisible(true);
      }
      else if (utils::now() > m_menus.checkmateDate + utils::toMilliseconds(500)) {
        m_menus.checkmate->setVisible(false);
      }
    }
    else if (m_menus.wasInCheckmate) {
      m_menus.checkmate->setVisible(false);
      m_menus.wasInCheckmate = false;
    }
    m_menus.status->setText(st);

    if (m.id() != m_menus.move && m.valid()) {
      unsigned pos = m.id() % m_menus.moves.size();
      log("Setting move " + std::to_string(m.id()) + " at " + std::to_string(pos) + " to " + m.toString());
      m_menus.moves[pos]->setText(m.toString());
      m_menus.move = m.id();
    }
  }

}
