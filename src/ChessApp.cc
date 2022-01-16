
# include "ChessApp.hh"
# include <maths_utils/ComparisonUtils.hh>

namespace pge {

  ChessApp::ChessApp(const AppDesc& desc):
    PGEApp(desc),

    m_game(nullptr),
    m_menus(),

    m_packs(std::make_shared<TexturePack>()),
    m_piecesPackID(),
    m_currentPiece(0u)
  {}

  void
  ChessApp::loadData() {
    // Create the texture pack.
    sprites::Pack pack;
    pack.file = "data/img/pieces.png";
    pack.sSize = olc::vi2d(170, 170);
    pack.layout = olc::vi2d(6, 2);

    m_piecesPackID = m_packs->registerPack(pack);
  }

  bool
  ChessApp::onFrame(float fElapsed) {
    // Handle case where no game is defined.
    if (m_game == nullptr) {
      return false;
    }

    if (!m_game->step(fElapsed)) {
      log("This is game over", utils::Level::Info);
    }

    return m_game->terminated();
  }

  void
  ChessApp::onInputs(const controls::State& c,
                     const CoordinateFrame& cf)
  {
    if (c.keys[controls::keys::N]) {
      m_currentPiece = (m_currentPiece + 1u) % 12u;
    }

    // Handle case where no game is defined.
    if (m_game == nullptr) {
      return;
    }

    // Handle menus update and process the
    // corresponding actions.
    std::vector<ActionShPtr> actions;
    bool relevant = false;

    for (unsigned id = 0u ; id < m_menus.size() ; ++id) {
      menu::InputHandle ih = m_menus[id]->processUserInput(c, actions);
      relevant = (relevant || ih.relevant);
    }

    for (unsigned id = 0u ; id < actions.size() ; ++id) {
      actions[id]->apply(*m_game);
    }

    bool lClick = (c.buttons[controls::mouse::Left] == controls::ButtonState::Released);
    if (lClick && !relevant) {
      olc::vf2d it;
      olc::vi2d tp = cf.pixelCoordsToTiles(olc::vi2d(c.mPosX, c.mPosY), &it);

      m_game->performAction(tp.x + it.x, tp.y + it.y);
    }

    if (c.keys[controls::keys::P]) {
      m_game->togglePause();
    }
  }

  void
  ChessApp::drawDecal(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(125, 154, 255));


    SpriteDesc sd;
    sd.radius = 1.0f;
    sd.x = 0.0f;
    sd.y = 0.0f;
    sd.loc = RelativePosition::Center;

    sd.sprite.pack = m_piecesPackID;
    sd.sprite.sprite = m_currentPiece;
    sd.sprite.id = 0;
    sd.sprite.tint = olc::WHITE;

    drawSprite(sd, res.cf);

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  ChessApp::draw(const RenderDesc& /*res*/) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  ChessApp::drawUI(const RenderDesc& /*res*/) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

    // Render the game menus.
    for (unsigned id = 0u ; id < m_menus.size() ; ++id) {
      m_menus[id]->render(this);
    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  ChessApp::drawDebug(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

    // Draw cursor's position.
    olc::vi2d mp = GetMousePos();
    olc::vf2d it;
    olc::vi2d mtp = res.cf.pixelCoordsToTiles(mp, &it);

    int h = GetDrawTargetHeight();
    int dOffset = 15;
    DrawString(olc::vi2d(0, h / 2), "Mouse coords      : " + toString(mp), olc::CYAN);
    DrawString(olc::vi2d(0, h / 2 + 1 * dOffset), "World cell coords : " + toString(mtp), olc::CYAN);
    DrawString(olc::vi2d(0, h / 2 + 2 * dOffset), "Intra cell        : " + toString(it), olc::CYAN);

    SetPixelMode(olc::Pixel::NORMAL);
  }

}
