
# include "GameState.hh"

/// @brief - Ratio of the size of the menus compared
/// to the total size of the window.
# define RATIO_MENU_TO_WINDOW 0.6

namespace {

  pge::MenuShPtr
  generateDefaultScreen(const olc::vi2d& dims,
                        const olc::Pixel& color)
  {
    olc::vi2d size(dims.x * RATIO_MENU_TO_WINDOW, dims.y * RATIO_MENU_TO_WINDOW);
    olc::vi2d pos(dims.x / 2.0f - size.x / 2.0f, dims.y / 2.0f - size.y / 2.0f);

    pge::menu::BackgroundDesc bg = pge::menu::newColoredBackground(color);
    pge::menu::MenuContentDesc fg = pge::menu::newTextContent("");

    return std::make_shared<pge::Menu>(
      pos,
      size,
      "goMenu",
      bg,
      fg,
      pge::menu::Layout::Vertical,
      false,
      false
    );
  }

  pge::MenuShPtr
  generateScreenOption(const olc::vi2d& dims,
                       const std::string& text,
                       const olc::Pixel& bgColor,
                       const std::string& name,
                       bool selectable)
  {
    pge::menu::BackgroundDesc bg = pge::menu::newColoredBackground(bgColor);
    bg.hColor = olc::GREY;

    pge::menu::MenuContentDesc fd = pge::menu::newMenuContent(text, "", dims);
    fd.color = (bgColor == olc::WHITE ? olc::BLACK : olc::WHITE);
    fd.hColor = olc::BLACK;
    fd.align = pge::menu::Alignment::Center;

    return std::make_shared<pge::Menu>(
      olc::vi2d(),
      dims,
      name,
      bg,
      fd,
      pge::menu::Layout::Horizontal,
      selectable,
      false
    );
  }

}

namespace pge {

  GameState::GameState(const olc::vi2d& dims,
                       const Screen& screen):
    utils::CoreObject("state"),

    // Assign a different screen so that we can use the
    // `setScreen` routine to initialize the visibility
    // status of screens.
    m_screen(screen == Screen::Home ? Screen::Exit : Screen::Home),

    m_home(nullptr),
    m_side(nullptr),
    m_gameOver(nullptr)
  {
    setService("chess");

    generateHomeScreen(dims);
    generateSideSelectionScreen(dims);
    generateGameOverScreen(dims);

    // Assign the screen, which will handle the visibility
    // update.
    setScreen(screen);
  }

  Screen
  GameState::getScreen() const noexcept {
    return m_screen;
  }

  void
  GameState::setScreen(const Screen& screen) {
    if (m_screen == screen) {
      return;
    }

    // Assign the state.
    m_screen = screen;

    // Update screens' visibility.
    m_home->setVisible(m_screen == Screen::Home);
    m_side->setVisible(m_screen == Screen::SideSelection);
    m_gameOver->setVisible(m_screen == Screen::GameOver);
  }

  void
  GameState::render(olc::PixelGameEngine* pge) const {
    m_home->render(pge);
    m_side->render(pge);
    m_gameOver->render(pge);
  }

  menu::InputHandle
  GameState::processUserInput(const controls::State& c,
                              std::vector<ActionShPtr>& actions)
  {
    menu::InputHandle res{false, false};

    // Propagate the user input to each screen.
    menu::InputHandle cur = m_home->processUserInput(c, actions);
    res.relevant = (res.relevant || cur.relevant);
    res.selected = (res.selected || cur.selected);

    cur = m_side->processUserInput(c, actions);
    res.relevant = (res.relevant || cur.relevant);
    res.selected = (res.selected || cur.selected);

    cur = m_gameOver->processUserInput(c, actions);
    res.relevant = (res.relevant || cur.relevant);
    res.selected = (res.selected || cur.selected);

    return res;
  }

  void
  GameState::generateHomeScreen(const olc::vi2d& dims) {
    // Generate the main screen.
    m_home = generateDefaultScreen(dims, olc::DARK_PINK);

    // Add each option to the screen.
    MenuShPtr m = generateScreenOption(dims, "New game", olc::VERY_DARK_PINK, "new_game", true);
    m->setSimpleAction(
      [this](Game& /*g*/) {
        setScreen(Screen::SideSelection);
      }
    );
    m_home->addMenu(m);

    m = generateScreenOption(dims, "Quit", olc::VERY_DARK_PINK, "quit", true);
    m->setSimpleAction(
      [this](Game& g) {
        setScreen(Screen::Exit);
        g.terminate();
      }
    );
    m_home->addMenu(m);
  }

  void
  GameState::generateSideSelectionScreen(const olc::vi2d& dims) {
    // Generate the main screen.
    m_side = generateDefaultScreen(dims, olc::DARK_CYAN);

    // Add each option to the screen.
    MenuShPtr m = generateScreenOption(dims, "White", olc::WHITE, "white", true);
    m->setSimpleAction(
      [this](Game& g) {
        g.setPlayer(chess::Color::White);
        setScreen(Screen::Game);
      }
    );
    m_side->addMenu(m);

    m = generateScreenOption(dims, "Black", olc::BLACK, "black", true);
    m->setSimpleAction(
      [this](Game& g) {
        g.setPlayer(chess::Color::Black);
        setScreen(Screen::Game);
      }
    );
    m_side->addMenu(m);
  }

  void
  GameState::generateGameOverScreen(const olc::vi2d& dims) {
    // Generate the main screen.
    m_gameOver = generateDefaultScreen(dims, olc::DARK_MAGENTA);

    MenuShPtr m = generateScreenOption(dims, "Back to main screen", olc::VERY_DARK_MAGENTA, "back_to_main", true);
    m->setSimpleAction(
      [this](Game& /*g*/) {
        setScreen(Screen::Home);
      }
    );
    m_gameOver->addMenu(m);

    m = generateScreenOption(dims, "Restart", olc::VERY_DARK_MAGENTA, "restart", true);
    m->setSimpleAction(
      [this](Game& /*g*/) {
        setScreen(Screen::SideSelection);
      }
    );
    m_gameOver->addMenu(m);

    m = generateScreenOption(dims, "Quit", olc::VERY_DARK_MAGENTA, "quit", true);
    m->setSimpleAction(
      [this](Game& g) {
        setScreen(Screen::Exit);
        g.terminate();
      }
    );
    m_gameOver->addMenu(m);
  }

}
