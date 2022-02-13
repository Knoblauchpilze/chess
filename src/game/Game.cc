
# include "Game.hh"
# include <cxxabi.h>
# include "Menu.hh"

# define ALERT_DURATION_MS 3000

namespace {

  pge::MenuShPtr
  generateMenu(const olc::vi2d& pos,
               const olc::vi2d& size,
               const std::string& text,
               const std::string& name)
  {
    pge::menu::MenuContentDesc fd = pge::menu::newMenuContent(text, "", size);
    fd.color = olc::WHITE;

    return std::make_shared<pge::Menu>(
      pos,
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
    pge::MenuShPtr status = generateMenu(olc::vi2d(), olc::vi2d(width, 50), "", "root");

    static const unsigned sk_lastMoves = 5u;
    // And additional menus: we will display 3 menus
    // and the number of moves. The percentage are
    // arbitrary.
    int wRound = static_cast<int>(width * 0.1f);
    int wPlayer = static_cast<int>(width * 0.2f);
    int wStatus = static_cast<int>(width * 0.2f);
    int wMove = static_cast<int>(width * 0.5f / sk_lastMoves);

    int mHeight = 30;

    m_menus.round = generateMenu(olc::vi2d(), olc::vi2d(wRound, mHeight), "Round: N/A", "round");
    m_menus.player = generateMenu(olc::vi2d(), olc::vi2d(wPlayer, mHeight), "White", "player");
    m_menus.status = generateMenu(olc::vi2d(), olc::vi2d(wStatus, mHeight), "All good", "status");

    for (unsigned id = 0u ; id < sk_lastMoves ; ++id) {
      m_menus.moves.push_back(
        generateMenu(olc::vi2d(), olc::vi2d(wMove, mHeight), "", "move_" + std::to_string(id))
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
    m_menus.check.date = utils::TimeStamp();
    m_menus.check.wasActive = false;
    m_menus.check.duration = ALERT_DURATION_MS;
    m_menus.checkmate.date = utils::TimeStamp();
    m_menus.checkmate.wasActive = false;
    m_menus.checkmate.duration = ALERT_DURATION_MS;
    m_menus.stalemate.date = utils::TimeStamp();
    m_menus.stalemate.wasActive = false;
    m_menus.stalemate.duration = ALERT_DURATION_MS;

    // Generate the alert menu indicating that the player
    // is in check or checkmate.
    pge::menu::MenuContentDesc fd = pge::menu::newMenuContent("ALERT", "", olc::vi2d(300, 150));
    fd.color = olc::RED;
    fd.align = pge::menu::Alignment::Center;

    m_menus.check.menu = generateAlertMenu(
      olc::vi2d((width - 300.0f) / 2.0f, (height - 150.0f) / 2.0f),
      olc::vi2d(300, 150),
      "You're in check !",
      "check"
    );
    m_menus.check.menu->setVisible(false);

    m_menus.checkmate.menu = generateAlertMenu(
      olc::vi2d((width - 300.0f) / 2.0f, (height - 150.0f) / 2.0f),
      olc::vi2d(300, 150),
      "You're in checkmate !",
      "checkmate"
    );
    m_menus.checkmate.menu->setVisible(false);

    m_menus.stalemate.menu = generateAlertMenu(
      olc::vi2d((width - 300.0f) / 2.0f, (height - 150.0f) / 2.0f),
      olc::vi2d(300, 150),
      "You're in stalemate !",
      "stalemate"
    );
    m_menus.stalemate.menu->setVisible(false);

    // Generate the menu indicating the pieces that
    // were taken during the game.
    pge::MenuShPtr taken = generateMenu(olc::vi2d(0, height - 50), olc::vi2d(width, 50), "", "taken_title");

    olc::vi2d size(width / 2, 50);
    pge::MenuShPtr wPieces = generateMenu(olc::vi2d(0, height - 50), size, "root", "");
    wPieces->addMenu(generateMenu(olc::vi2d(), size, "whites", "label"));
    m_menus.wCaptured = generatePiecesMenu(chess::Color::White, size.x, size.y);

    wPieces->addMenu(m_menus.wCaptured.pMenu);
    wPieces->addMenu(m_menus.wCaptured.kMenu);
    wPieces->addMenu(m_menus.wCaptured.bMenu);
    wPieces->addMenu(m_menus.wCaptured.rMenu);
    wPieces->addMenu(m_menus.wCaptured.qMenu);

    pge::MenuShPtr bPieces = generateMenu(olc::vi2d(width / 2, height - 50), size, "root", "");
    bPieces->addMenu(generateMenu(olc::vi2d(), size, "blacks", "label"));
    m_menus.bCaptured = generatePiecesMenu(chess::Color::Black, size.x, size.y);

    bPieces->addMenu(m_menus.bCaptured.pMenu);
    bPieces->addMenu(m_menus.bCaptured.kMenu);
    bPieces->addMenu(m_menus.bCaptured.bMenu);
    bPieces->addMenu(m_menus.bCaptured.rMenu);
    bPieces->addMenu(m_menus.bCaptured.qMenu);

    taken->addMenu(wPieces);
    taken->addMenu(bPieces);

    // Regenerate the menus.
    std::vector<MenuShPtr> menus;
    menus.push_back(status);
    menus.push_back(m_menus.check.menu);
    menus.push_back(m_menus.checkmate.menu);
    menus.push_back(m_menus.stalemate.menu);

    menus.push_back(taken);

    return menus;
  }

  void
  Game::performAction(float x, float y) {
    // Only handle actions when the game is not disabled.
    if (m_state.disabled) {
      log("Ignoring action while menu is disabled");
      return;
    }

    chess::CoordinatesShPtr coords = convertCoords(x, y, 1.0f * m_board->w(), 1.0f * m_board->h(), getPlayer());
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
  Game::resume() {
    // Do nothing in case the game is already running.
    if (!m_state.paused) {
      return;
    }

    log("Game is now resumed", utils::Level::Info);
    m_state.paused = false;

    // Also, make the AI play: this will make sure that
    // in case it's needed the player will be able to
    // play right away.
    m_ai->play(*m_board);
  }

  void
  Game::setPlayer(const chess::Color& color) noexcept {
    // Create the AI with the oppostie color as the player.
    m_ai = std::make_shared<chess::AI>(color == chess::Color::White ? chess::Color::Black : chess::Color::White);
    log("Player will be " + colorToString(color), utils::Level::Info);
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

    // Update the state of the game.
    updateStateMenu();

    // Update captured pieces.
    updateCapturedPieces();

    if (m.id() != m_menus.move && m.valid()) {
      unsigned pos = m.id() % m_menus.moves.size();
      m_menus.moves[pos]->setText(m.toString());
      m_menus.move = m.id();
    }
  }

  void
  Game::TimedMenu::update(bool active) noexcept {
      // In case the menu should be active
    if (active) {
      if (!wasActive) {
        // Make it active if it's the first time that
        // we detect that it should be active.
        date = utils::now();
        wasActive = true;
        menu->setVisible(true);
      }
      else if (utils::now() > date + utils::toMilliseconds(duration)) {
        // Deactivate the menu in case it's been active
        // for too long.
        menu->setVisible(false);
      }
      else {
        // Update the alpha value in case it's active
        // for not long enough.
        float d = utils::diffInMs(date, utils::now()) / duration;
        uint8_t alpha = static_cast<uint8_t>(
          std::clamp((1.0f - d) * pge::alpha::Opaque, 0.0f, 255.0f)
        );
        menu->setBackground(pge::menu::newColoredBackground(olc::Pixel(64, 0, 0, alpha)));
      }
    }
    // Or if the menu shouldn't be active anymore and
    // it's the first time we detect that.
    else if (wasActive) {
      // Deactivate the menu.
      menu->setVisible(false);
      wasActive = false;
    }
  }

  void
  Game::updateStateMenu() noexcept {
    chess::Color p = m_board->getPlayer();

    // Start with checkmate as it's 'stronger' than
    // regular check.
    bool cm = m_board->isInCheckmate(p);
    bool c = m_board->isInCheck(p);
    bool sm = m_board->isInStalemate(p);

    m_menus.checkmate.update(cm);
    m_menus.check.update(c && !cm);
    m_menus.stalemate.update(sm);

    std::string st = (cm ? "Checkmate" : (cm ? "Check" : (sm ? "Stalemate" : "All good")));
    m_menus.status->setText(st);
  }

  void
  Game::updateCapturedPieces() {
    // Fetch pieces for each color
    std::vector<chess::Piece> wp = m_board->pieces(chess::Color::White);
    std::vector<chess::Piece> bp = m_board->pieces(chess::Color::Black);

    auto count = [](const std::vector<chess::Piece>& pieces, Captured& cap) {
      cap.pawns = 8u;
      cap.knights = 2u;
      cap.bishops = 2u;
      cap.rooks = 2u;
      cap.queens = 1u;

      for (unsigned id =  0u ; id < pieces.size() ; ++id) {
        // Accumulate pieces count.
        if (pieces[id].pawn()) {
          --cap.pawns;
        }
        if (pieces[id].knight()) {
          --cap.knights;
        }
        if (pieces[id].bishop()) {
          --cap.bishops;
        }
        if (pieces[id].rook()) {
          --cap.rooks;
        }
        if (pieces[id].queen()) {
          --cap.queens;
        }
      }
    };

    count(wp, m_menus.wCaptured);

    m_menus.wCaptured.pMenu->setText(std::to_string(m_menus.wCaptured.pawns));
    m_menus.wCaptured.kMenu->setText(std::to_string(m_menus.wCaptured.knights));
    m_menus.wCaptured.bMenu->setText(std::to_string(m_menus.wCaptured.bishops));
    m_menus.wCaptured.rMenu->setText(std::to_string(m_menus.wCaptured.rooks));
    m_menus.wCaptured.qMenu->setText(std::to_string(m_menus.wCaptured.queens));

    count(bp, m_menus.bCaptured);

    m_menus.bCaptured.pMenu->setText(std::to_string(m_menus.bCaptured.pawns));
    m_menus.bCaptured.kMenu->setText(std::to_string(m_menus.bCaptured.knights));
    m_menus.bCaptured.bMenu->setText(std::to_string(m_menus.bCaptured.bishops));
    m_menus.bCaptured.rMenu->setText(std::to_string(m_menus.bCaptured.rooks));
    m_menus.bCaptured.qMenu->setText(std::to_string(m_menus.bCaptured.queens));
  }

  Game::Captured
  Game::generatePiecesMenu(const chess::Color& c,
                           int width,
                           int height) const noexcept
  {
    Captured out = {};

    std::string cl = chess::colorToString(c);

    auto generatePiece = [width, height, &cl](const chess::Type& t, unsigned count) {
      std::string icon = "data/img/" + cl + "_" + chess::pieceToString(t) + ".png";
      pge::menu::MenuContentDesc fg = pge::menu::newMenuContent(std::to_string(count), icon, olc::vi2d());
      fg.color = olc::WHITE;
      // We have 6 menus: 5 for the pieces and one for the
      // title. For only the piece menu, we want the icon
      // to use 2/3 of the available space.
      int desired = static_cast<int>(std::min(2.0f * width / (6.0f * 3.0f), 1.0f * height));
      fg.size = olc::vi2d(desired, desired);

      return std::make_shared<pge::Menu>(
        olc::vi2d(),
        olc::vi2d(),
        chess::pieceToString(t),
        pge::menu::newColoredBackground(olc::VERY_DARK_GREEN),
        fg,
        pge::menu::Layout::Horizontal,
        false,
        false
      );
    };

    out.pawns = 0u;
    out.pMenu = generatePiece(chess::Type::Pawn, out.pawns);
    out.knights = 0u;
    out.kMenu = generatePiece(chess::Type::Knight, out.knights);
    out.bishops = 0u;
    out.bMenu = generatePiece(chess::Type::Bishop, out.bishops);
    out.rooks = 0u;
    out.rMenu = generatePiece(chess::Type::Rook, out.rooks);
    out.queens = 0u;
    out.qMenu = generatePiece(chess::Type::Queen, out.queens);

    return out;
  }

}
