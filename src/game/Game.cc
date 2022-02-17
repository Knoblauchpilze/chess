
# include "Game.hh"
# include <cxxabi.h>
# include "Menu.hh"
# include "MinimaxAI.hh"

/// @brief - The duration of the alert prompting that
/// the current player is in check, in stalemate or
/// won/lost.
# define ALERT_DURATION_MS 3000

/// @brief - Defines the number of moves displayed in
/// the history.
# define MOVES_COUNT 3u

/// @brief - The depth of the tree considered for the
/// AI.
# define AI_TREE_DEPTH 3u

namespace {

  pge::MenuShPtr
  generateMenu(const olc::vi2d& pos,
               const olc::vi2d& size,
               const std::string& text,
               const std::string& name,
               bool clickable = false)
  {
    pge::menu::MenuContentDesc fd = pge::menu::newMenuContent(text, "", size);
    fd.color = olc::WHITE;
    fd.hColor = olc::BLACK;
    fd.align = pge::menu::Alignment::Center;

    return std::make_shared<pge::Menu>(
      pos,
      size,
      name,
      pge::menu::newColoredBackground(olc::VERY_DARK_GREEN),
      fd,
      pge::menu::Layout::Horizontal,
      clickable,
      false
    );
  }

  pge::MenuShPtr
  generateMessageBoxMenu(const olc::vi2d& pos,
                         const olc::vi2d& size,
                         const std::string& text,
                         const std::string& name,
                         bool alert)
  {
    pge::menu::MenuContentDesc fd = pge::menu::newMenuContent(text, "", size);
    fd.color = (alert ? olc::RED : olc::GREEN);
    fd.align = pge::menu::Alignment::Center;

    return std::make_shared<pge::Menu>(
      pos,
      size,
      name,
      pge::menu::newColoredBackground(alert ? olc::VERY_DARK_RED : olc::VERY_DARK_GREEN),
      fd,
      pge::menu::Layout::Horizontal,
      false,
      false
    );
  }

}

namespace pge {

  Game::Game(chess::ChessGameShPtr board):
    utils::CoreObject("game"),

    m_state(
      State{
        true,  // paused
        true,  // disabled
        false, // terminated
        false, // resigned
        false, // lost
      }
    ),

    m_board(board),
    m_start(nullptr),
    m_promote(nullptr),
    m_ai(std::make_shared<chess::MinimaxAI>(chess::Color::Black, AI_TREE_DEPTH)),
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

    // And additional menus: we will display 3 menus
    // and the number of moves. The percentage are
    // arbitrary.
    int wRound = static_cast<int>(width * 0.1f);
    int wPlayer = static_cast<int>(width * 0.15f);
    int wStatus = static_cast<int>(width * 0.15f);
    int wMove = static_cast<int>(width * 0.5f / MOVES_COUNT);
    int wResign = static_cast<int>(width * 0.1f / MOVES_COUNT);

    int mHeight = 30;

    m_menus.round = generateMenu(olc::vi2d(), olc::vi2d(wRound, mHeight), "Round: N/A", "round");
    m_menus.player = generateMenu(olc::vi2d(), olc::vi2d(wPlayer, mHeight), "White", "player");
    m_menus.status = generateMenu(olc::vi2d(), olc::vi2d(wStatus, mHeight), "All good", "status");

    for (unsigned id = 0u ; id < MOVES_COUNT ; ++id) {
      m_menus.moves.push_back(
        generateMenu(olc::vi2d(), olc::vi2d(wMove, mHeight), "", "move_" + std::to_string(id))
      );
    }

    m_menus.resign = generateMenu(olc::vi2d(), olc::vi2d(wResign, mHeight), "Resign", "resign", true);
    m_menus.resign->setSimpleAction(
      [this](Game& g) {
        g.resign();
      }
    );

    // Register the menu in the parent one.
    status->addMenu(m_menus.round);
    status->addMenu(m_menus.player);
    status->addMenu(m_menus.status);

    for (unsigned id = 0u ; id < MOVES_COUNT ; ++id) {
      status->addMenu(m_menus.moves[id]);
    }

    status->addMenu(m_menus.resign);

    // Initialize the current move at one minus the
    // maximum number of move so that the first move
    // can actually be registered.
    m_menus.check.date = utils::TimeStamp();
    m_menus.check.wasActive = false;
    m_menus.check.duration = ALERT_DURATION_MS;
    m_menus.checkmate.date = utils::TimeStamp();
    m_menus.checkmate.wasActive = false;
    m_menus.checkmate.duration = ALERT_DURATION_MS;
    m_menus.stalemate.date = utils::TimeStamp();
    m_menus.stalemate.wasActive = false;
    m_menus.stalemate.duration = ALERT_DURATION_MS;
    m_menus.oStalemate.date = utils::TimeStamp();
    m_menus.oStalemate.wasActive = false;
    m_menus.oStalemate.duration = ALERT_DURATION_MS;
    m_menus.win.date = utils::TimeStamp();
    m_menus.win.wasActive = false;
    m_menus.win.duration = ALERT_DURATION_MS;
    m_menus.resigned.date = utils::TimeStamp();
    m_menus.resigned.wasActive = false;
    m_menus.resigned.duration = ALERT_DURATION_MS;

    // Generate the alert menu indicating that the player
    // is in check or checkmate.
    pge::menu::MenuContentDesc fd = pge::menu::newMenuContent("ALERT", "", olc::vi2d(300, 150));
    fd.color = olc::RED;
    fd.align = pge::menu::Alignment::Center;

    m_menus.check.menu = generateMessageBoxMenu(
      olc::vi2d((width - 300.0f) / 2.0f, (height - 150.0f) / 2.0f),
      olc::vi2d(300, 150),
      "You're in check !",
      "check",
      true
    );
    m_menus.check.menu->setVisible(false);

    m_menus.checkmate.menu = generateMessageBoxMenu(
      olc::vi2d((width - 300.0f) / 2.0f, (height - 150.0f) / 2.0f),
      olc::vi2d(300, 150),
      "You're in checkmate !",
      "checkmate",
      true
    );
    m_menus.checkmate.menu->setVisible(false);

    m_menus.stalemate.menu = generateMessageBoxMenu(
      olc::vi2d((width - 300.0f) / 2.0f, (height - 150.0f) / 2.0f),
      olc::vi2d(300, 150),
      "You're in stalemate !",
      "stalemate",
      true
    );
    m_menus.stalemate.menu->setVisible(false);

    m_menus.oStalemate.menu = generateMessageBoxMenu(
      olc::vi2d((width - 300.0f) / 2.0f, (height - 150.0f) / 2.0f),
      olc::vi2d(300, 150),
      "Your opponent is in stalemate !",
      "oStalemate",
      true
    );
    m_menus.oStalemate.menu->setVisible(false);

    m_menus.win.menu = generateMessageBoxMenu(
      olc::vi2d((width - 300.0f) / 2.0f, (height - 150.0f) / 2.0f),
      olc::vi2d(300, 150),
      "Your checkmated your opponent !",
      "win",
      false
    );
    m_menus.win.menu->setVisible(false);

    m_menus.resigned.menu = generateMessageBoxMenu(
      olc::vi2d((width - 300.0f) / 2.0f, (height - 150.0f) / 2.0f),
      olc::vi2d(300, 150),
      "You resigned !",
      "resign",
      true
    );
    m_menus.resigned.menu->setVisible(false);

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

    // Generate the promotion menu.
    m_menus.wPromotion = generatePromotionMenu(width, height, chess::Color::White);
    m_menus.wPromotion->setVisible(false);
    m_menus.bPromotion = generatePromotionMenu(width, height, chess::Color::Black);
    m_menus.bPromotion->setVisible(false);

    // Regenerate the menus.
    std::vector<MenuShPtr> menus;
    menus.push_back(status);
    menus.push_back(m_menus.check.menu);
    menus.push_back(m_menus.checkmate.menu);
    menus.push_back(m_menus.stalemate.menu);
    menus.push_back(m_menus.oStalemate.menu);
    menus.push_back(m_menus.win.menu);
    menus.push_back(m_menus.resigned.menu);

    menus.push_back(taken);

    menus.push_back(m_menus.wPromotion);
    menus.push_back(m_menus.bPromotion);

    return menus;
  }

  void
  Game::performAction(float x, float y) {
    // Only handle actions when the game is not disabled.
    if (m_state.disabled) {
      log("Ignoring action while menu is disabled");
      return;
    }

    // Also prevent action in case the player resigned.
    if (m_state.resigned) {
      log("Ignoring action while player resigned");
      return;
    }

    const chess::Board& b = (*m_board)();

    chess::CoordinatesShPtr coords = convertCoords(x, y, 1.0f * b.w(), 1.0f * b.h(), getPlayer());
    if (coords == nullptr) {
      // Reset the current selected starting position for
      // a piece move.
      m_start.reset();
      return;
    }

    // In case a promotion coordinates is available, disable
    // the selection of a new one.
    if (m_promote) {
      log("Waiting for promoting");
      return;
    }

    // Two main possbilities: either we already have a valid
    // starting location, or we don't.
    if (!m_start) {
      const chess::Piece& c = b.at(*coords);
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

    // Handle pawn promotion.
    if (coords->y() == 0 || coords->y() == b.h() - 1) {
      const chess::Piece& p = b.at(*coords);
      if (p.pawn()) {
        if (p.color() == chess::Color::White) {
          m_menus.wPromotion->setVisible(true);
        }
        else {
          m_menus.bPromotion->setVisible(true);
        }

        m_promote = coords;

        return;
      }
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

    // Disable UI in case the game is done.
    if (m_state.done) {
      pause();
      enable(!m_state.paused);
    }

    return !m_state.done;
  }

  void
  Game::togglePause() {
    // Call the correct method based on the current state
    // and the desired one. This might mean not changing
    // the state at all.
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
    m_ai = std::make_shared<chess::MinimaxAI>(oppositeColor(color), AI_TREE_DEPTH);
    log("Player will be " + colorToString(color), utils::Level::Info);

    // Reset the board.
    m_board->initialize();

    // Prevent the game from being done from the start.
    m_state.done = false;

    resume();
    enable(!m_state.paused);
  }

  void
  Game::setPromotion(const chess::Type& promotion, const chess::Color& c) noexcept {
    if (!m_promote) {
      warn(
        "Failed to handle pawn promotion to " + chess::pieceToString(promotion),
        "Invalid promotion coordinate"
      );

      return;
    }

    // Handle promoting.
    m_board->promote(*m_promote, promotion);
    m_promote.reset();

    // Deactivate the menu to pick the promotion.
    if (c == chess::Color::White) {
      m_menus.wPromotion->setVisible(false);
    }
    else {
      m_menus.bPromotion->setVisible(false);
    }

    // Resume the course of the move: the AI should play
    // play and we can reset the starting location.
    m_ai->play(*m_board);
    m_start.reset();
  }

  void
  Game::resign() noexcept {
    chess::Color p = chess::oppositeColor(m_ai->side());
    log(colorToString(p) + " resigned", utils::Level::Info);

    m_state.resigned = true;
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
    chess::Round m = getLastRound();

    m_menus.round->setText("Round: " + std::to_string(r.id() + 1u));
    m_menus.player->setText(chess::colorToString(p));

    // Update the state of the game.
    updateStateMenu();

    // Update captured pieces.
    updateCapturedPieces();

    chess::Rounds rs = m_board->getRounds();
    unsigned id = 0u;
    if (rs.size() > MOVES_COUNT) {
      id = rs.size() - MOVES_COUNT;
    }

    unsigned lid = 0u;
    while (id < rs.size()) {
      if (rs[id].valid()) {
        m_menus.moves[lid]->setText(rs[id].toString());
        ++lid;
      }

      ++id;
    }
  }

  bool
  Game::TimedMenu::update(bool active) noexcept {
    // In case the menu should be active.
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
        olc::Pixel c = menu->getBackgroundColor();

        float d = utils::diffInMs(date, utils::now()) / duration;
        c.a = static_cast<uint8_t>(
          std::clamp((1.0f - d) * pge::alpha::Opaque, 0.0f, 255.0f)
        );
        menu->setBackground(pge::menu::newColoredBackground(c));
      }
    }
    // Or if the menu shouldn't be active anymore and
    // it's the first time we detect that.
    else if (wasActive) {
      // Deactivate the menu.
      menu->setVisible(false);
      wasActive = false;
    }

    return menu->visible();
  }

  void
  Game::updateStateMenu() noexcept {
    chess::Color p = chess::oppositeColor(m_ai->side());

    // Start with checkmate as it's 'stronger' than
    // regular check.
    bool cm = m_board->isInCheckmate(p);
    bool c = m_board->isInCheck(p);
    bool sm = m_board->isInStalemate(p);

    bool ocm = m_board->isInCheckmate(oppositeColor(p));
    bool osm = m_board->isInStalemate(oppositeColor(p));

    bool cmDone = m_menus.checkmate.menu->visible();
    cmDone &= !m_menus.checkmate.update(cm);
    m_menus.check.update(c && !cm);
    bool sDone = m_menus.stalemate.menu->visible();
    sDone &= !m_menus.stalemate.update(sm);
    bool osDone = m_menus.oStalemate.menu->visible();
    osDone &= !m_menus.oStalemate.update(osm);
    bool wDone = m_menus.win.menu->visible();
    wDone &= !m_menus.win.update(ocm);
    bool rDone = m_menus.resigned.menu->visible();
    rDone &= !m_menus.resigned.update(m_state.resigned);

    if (cmDone || sDone || osDone || wDone || rDone) {
      // The game is lost and the display screen has been
      // reached, so we can indicate it.
      m_state.done = true;
    }

    // Disable the UI in case we reached the end of the game.
    if (cm || sm || osm || ocm) {
      enable(false);
    }

    std::string st = "All good";
    if (cm) {
      st = "Checkmate";
    }
    else if (sm) {
      st = "Stalemate";
    }
    else if (ocm) {
      st = "Victory !";
    }
    else if (osm) {
      st = "Draw";
    }
    else if (c) {
      st = "Check";
    }
    m_menus.status->setText(st);
  }

  void
  Game::updateCapturedPieces() {
    // Fetch pieces for each color
    chess::Pieces wp = (*m_board)().pieces(chess::Color::White);
    chess::Pieces bp = (*m_board)().pieces(chess::Color::Black);

    auto count = [](const chess::Pieces& pieces, Captured& cap) {
      cap.pawns = 8u;
      cap.knights = 2u;
      cap.bishops = 2u;
      cap.rooks = 2u;
      cap.queens = 1u;

      for (unsigned id =  0u ; id < pieces.size() ; ++id) {
        // Accumulate pieces count.
        if (pieces[id].first.pawn() && cap.pawns > 0u) {
          --cap.pawns;
        }
        if (pieces[id].first.knight() && cap.knights > 0u) {
          --cap.knights;
        }
        if (pieces[id].first.bishop() && cap.bishops > 0u) {
          --cap.bishops;
        }
        if (pieces[id].first.rook() && cap.rooks > 0u) {
          --cap.rooks;
        }
        if (pieces[id].first.queen() && cap.queens > 0u) {
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

  MenuShPtr
  Game::generatePromotionMenu(int width, int height, const chess::Color& c) const noexcept {
    MenuShPtr out = std::make_shared<pge::Menu>(
      olc::vi2d((width - 300.0f) / 2.0f, (height - 150.0f) / 2.0f),
      olc::vi2d(300, 150),
      "root",
      pge::menu::newColoredBackground(olc::VERY_DARK_GREEN),
      pge::menu::newTextContent(""),
      pge::menu::Layout::Vertical,
      false,
      false
    );

    MenuShPtr title = generateMenu(olc::vi2d(), olc::vi2d(), "Choose a promotion:", "title");
    std::string n = chess::colorToString(c);

    MenuShPtr promotions = generateMenu(olc::vi2d(), olc::vi2d(300, 150), "", "title");
    pge::menu::BackgroundDesc bg = pge::menu::newColoredBackground(olc::VERY_DARK_GREEN);

    // Generate knight promotion.
    pge::menu::MenuContentDesc fd = pge::menu::newImageContent("data/img/" + n + "_knight.png", olc::vi2d(64, 64));
    MenuShPtr prom = std::make_shared<pge::Menu>(olc::vi2d(), olc::vi2d(64, 64), "knight", bg, fd);
    prom->setSimpleAction(
      [c](Game& g) {
        g.setPromotion(chess::Type::Knight, c);
      }
    );

    promotions->addMenu(prom);

    // Generate bishop promotion.
    fd = pge::menu::newImageContent("data/img/" + n + "_bishop.png", olc::vi2d(64, 64));
    prom = std::make_shared<pge::Menu>(olc::vi2d(), olc::vi2d(64, 64), "bishop", bg, fd);
    prom->setSimpleAction(
      [c](Game& g) {
        g.setPromotion(chess::Type::Bishop, c);
      }
    );

    promotions->addMenu(prom);

    // Generate rook promotion.
    fd = pge::menu::newImageContent("data/img/" + n + "_rook.png", olc::vi2d(64, 64));
    prom = std::make_shared<pge::Menu>(olc::vi2d(), olc::vi2d(64, 64), "rook", bg, fd);
    prom->setSimpleAction(
      [c](Game& g) {
        g.setPromotion(chess::Type::Rook, c);
      }
    );

    promotions->addMenu(prom);

    // Generate queen promotion.
    fd = pge::menu::newImageContent("data/img/" + n + "_queen.png", olc::vi2d(64, 64));
    prom = std::make_shared<pge::Menu>(olc::vi2d(), olc::vi2d(64, 64), "queen", bg, fd);
    prom->setSimpleAction(
      [c](Game& g) {
        g.setPromotion(chess::Type::Queen, c);
      }
    );

    promotions->addMenu(prom);

    out->addMenu(title);
    out->addMenu(promotions);

    return out;
  }

}
