
# include "App.hh"
# include <maths_utils/ComparisonUtils.hh>

/// @brief - Size of the tiles.
# define TILE_SIZE 170

namespace chess {

  App::App(const pge::AppDesc& desc):
    pge::App(desc),

    m_game(nullptr),
    m_menus(),

    m_packs(std::make_shared<pge::TexturePack>()),
    m_piecesPackID(),

    m_board()
  {}

  void
  App::loadData() {
    // Create the texture pack.
    pge::sprites::Pack pack;
    pack.file = "data/img/pieces.png";
    pack.sSize = olc::vi2d(TILE_SIZE, TILE_SIZE);
    pack.layout = olc::vi2d(6, 2);

    m_piecesPackID = m_packs->registerPack(pack);
  }

  bool
  App::onFrame(float fElapsed) {
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
  App::onInputs(const pge::controls::State& c,
                const pge::CoordinateFrame& cf)
  {
    // Handle case where no game is defined.
    if (m_game == nullptr) {
      return;
    }

    // Handle menus update and process the
    // corresponding actions.
    std::vector<pge::ActionShPtr> actions;
    bool relevant = false;

    for (unsigned id = 0u ; id < m_menus.size() ; ++id) {
      pge::menu::InputHandle ih = m_menus[id]->processUserInput(c, actions);
      relevant = (relevant || ih.relevant);
    }

    for (unsigned id = 0u ; id < actions.size() ; ++id) {
      actions[id]->apply(*m_game);
    }

    bool lClick = (c.buttons[pge::controls::mouse::Left] == pge::controls::ButtonState::Released);
    if (lClick && !relevant) {
      olc::vf2d it;
      olc::vi2d tp = cf.pixelCoordsToTiles(olc::vi2d(c.mPosX, c.mPosY), &it);

      m_game->performAction(tp.x + it.x, tp.y + it.y);
    }

    if (c.keys[pge::controls::keys::P]) {
      m_game->togglePause();
    }
  }

  void
  App::drawDecal(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(125, 154, 255));

    // Colors for the board.
    olc::Pixel bright(238, 238, 213);
    olc::Pixel dark(128, 148, 93);

    // Colors for the pieces.
    olc::Pixel white = olc::Pixel(128, 0, 0, pge::alpha::SemiOpaque);
    olc::Pixel black = olc::Pixel(0, 0, 128, pge::alpha::SemiOpaque);

    // Draw the board.
    for (unsigned y = 0u ; y < 8u ; ++y) {
      for (unsigned x = 0u ; x < 8u ; ++x) {
        unsigned det = (y % 2u + x) % 2u;

        SpriteDesc sd = {};
        sd.loc = pge::RelativePosition::Center;
        // Assume that the tile size is a square and scale
        // the tiles so that they occupy one tile.
        sd.radius = 1.0f;

        sd.x = x;
        sd.y = y;

        sd.sprite.tint = (det == 0u ? dark : bright);

        drawRect(sd, res.cf);
      }
    }

    // Draw the pieces.
    for (unsigned y = 0u ; y < 8u ; ++y) {
      for (unsigned x = 0u ; x < 8u ; ++x) {
        // Check if something is at this position.
        pieces::Cell p = m_board.at(x, y);
        if (p.type == pieces::None) {
          continue;
        }

        SpriteDesc sd = {};
        sd.loc = pge::RelativePosition::Center;
        sd.radius = 0.9f;

        sd.x = x;
        sd.y = y;

        sd.sprite.pack = m_piecesPackID;
        sd.sprite.id = 0;
        sd.sprite.tint = (p.color == pieces::White ? white : black);
        sd.sprite.sprite = 0u;

        // if (x == 0 && y == 0) {
        //   log(
        //     "Sprite at " + std::to_string(x) + "x" + std::to_string(y) +
        //     " is at " + std::to_string(sd.x) + "x" + std::to_string(sd.y) +
        //     " tile: " + std::to_string(res.cf.tileSize().x) + "x" + std::to_string(res.cf.tileSize().y) +
        //     " scale: " + std::to_string(res.cf.tileScale().x) + "x" + std::to_string(res.cf.tileScale().y)
        //   );
        // }

        drawSprite(sd, res.cf);
      }
    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  App::draw(const RenderDesc& /*res*/) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, pge::alpha::Transparent));

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  App::drawUI(const RenderDesc& /*res*/) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, pge::alpha::Transparent));

    // Render the game menus.
    for (unsigned id = 0u ; id < m_menus.size() ; ++id) {
      m_menus[id]->render(this);
    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  App::drawDebug(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, pge::alpha::Transparent));

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
