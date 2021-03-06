
# include "App.hh"
# include <algorithm>
# include <maths_utils/ComparisonUtils.hh>

/// @brief - Size of the tiles.
# define TILE_SIZE 170

namespace {

  int
  spriteIDFromPiece(const chess::Piece& p) noexcept {
    if (p.knight()) {
      return 3;
    }
    else if (p.bishop()) {
      return 2;
    }
    else if (p.rook()) {
      return 4;
    }
    else if (p.queen()) {
      return 1;
    }
    else if (p.king()) {
      return 0;
    }
    else {
      // Assume pawn.
      return 5;
    }
  }

}

namespace chess {

  App::App(const pge::AppDesc& desc):
    pge::App(desc),

    m_game(nullptr),
    m_state(nullptr),
    m_menus(),

    m_packs(std::make_shared<pge::TexturePack>()),
    m_piecesPackID(),

    m_board(std::make_shared<ChessGame>())
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

  void
  App::loadResources() {
    // Assign a specific tint to the regular
    // drawing layer so that we have a built
    // in transparency.
    // We can't do it directly when drawing
    // in the rendering function because as
    // the whole layer will be drawn as one
    // quad in opengl with an opaque alpha,
    // we will lose this info.
    // This means that everything is indeed
    // transparent but that's the only way
    // for now to achieve it.
    setLayerTint(Layer::Draw, olc::Pixel(255, 255, 255, pge::alpha::SemiOpaque));

    // Create the game and its state.
    m_game = std::make_shared<pge::Game>(m_board);
  }

  void
  App::loadMenuResources() {
    m_state = std::make_shared<pge::GameState>(
      olc::vi2d(ScreenWidth(), ScreenHeight()),
      pge::Screen::Home
    );
    m_menus = m_game->generateMenus(ScreenWidth(), ScreenHeight());
  }

  bool
  App::onFrame(float fElapsed) {
    // Handle case where no game is defined.
    if (m_game == nullptr) {
      return false;
    }

    // Make the game evolve and display game over
    // screen if needed.
    if (!m_game->step(fElapsed)) {
      m_state->setScreen(pge::Screen::GameOver);
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

    if (m_state != nullptr) {
      pge::menu::InputHandle ih = m_state->processUserInput(c, actions);
      relevant = (relevant || ih.relevant);
    }

    for (unsigned id = 0u ; id < actions.size() ; ++id) {
      actions[id]->apply(*m_game);
    }

    pge::controls::ButtonState be = c.buttons[pge::controls::mouse::Left];
    if (!relevant && be == pge::controls::ButtonState::Pressed) {
      olc::vf2d it;
      olc::vi2d tp = cf.pixelCoordsToTiles(olc::vi2d(c.mPosX, c.mPosY), &it);

      m_game->performAction(tp.x + it.x, tp.y + it.y);
    }

    if (c.keys[pge::controls::keys::P]) {
      m_game->togglePause();
    }
  }

  void
  App::cleanResources() {
    if (m_packs != nullptr) {
      m_packs.reset();
    }
  }

  void
  App::cleanMenuResources() {
    m_menus.clear();
  }

  void
  App::drawDecal(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(0, 100, 0));

    // In case we're not in the game screen, do nothing.
    if (m_state->getScreen() != pge::Screen::Game) {
      SetPixelMode(olc::Pixel::NORMAL);
      return;
    }

    SpriteDesc sd = {};
    sd.loc = pge::RelativePosition::Center;
    sd.radius = 8.4f;

    sd.x = 3.5f;
    sd.y = 3.5f;

    sd.sprite.tint = olc::Pixel(123, 63, 0);

    // The wooden layer.
    drawRect(sd, res.cf);

    drawBoard(res);
    drawPieces(res);
    drawOverlays(res);

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

    // In case we're not in game mode, just render
    // the state.
    if (m_state->getScreen() != pge::Screen::Game) {
      m_state->render(this);
      SetPixelMode(olc::Pixel::NORMAL);
      return;
    }

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

    // Disable rendering if we're not in the game screen.
    if (m_state->getScreen() != pge::Screen::Game) {
      SetPixelMode(olc::Pixel::NORMAL);
      return;
    }

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

  void
  App::drawBoard(const RenderDesc& res) noexcept {
    // Colors for the board.
    olc::Pixel bright(238, 238, 213);
    olc::Pixel dark(149, 69, 53);

    SpriteDesc sd = {};
    sd.loc = pge::RelativePosition::Center;

    // Assume that the tile size is a square and scale
    // the tiles so that they occupy one tile.
    sd.radius = 1.0f;

    // Draw the board.
    for (unsigned y = 0u ; y < 8u ; ++y) {
      for (unsigned x = 0u ; x < 8u ; ++x) {
        unsigned det = (y % 2u + x) % 2u;

        sd.x = x;
        sd.y = m_game->getPlayer() == Color::White ? y : 7.0f - y;

        sd.sprite.tint = (det == 1u ? dark : bright);

        drawRect(sd, res.cf);
      }
    }

    // Draw the indications about files and rows.
    for (unsigned id = 0u ; id < 8u ; ++id) {
      Coordinates cFile(id, 0);
      std::string file = cells::file(cFile.asValue());

      Coordinates cRow(0, m_game->getPlayer() == Color::White ? 7u - id : id);
      std::string row = cells::row(cRow.asValue());

      // Draw files and row on both sides of the
      // board.
      olc::vf2d pFile = res.cf.tileCoordsToPixels(0.35f + id, -0.15f, pge::RelativePosition::Center, 1.0f);
      DrawStringDecal(pFile, file, olc::BLACK);
      pFile = res.cf.tileCoordsToPixels(0.35f + id, 8.05f, pge::RelativePosition::Center, 1.0f);
      DrawStringDecal(pFile, file, olc::BLACK);

      olc::vf2d pRow = res.cf.tileCoordsToPixels(-0.15f, 0.5f + id, pge::RelativePosition::Center, 1.0f);
      DrawStringDecal(pRow, row, olc::BLACK);
      pRow = res.cf.tileCoordsToPixels(8.05f, 0.5f + id, pge::RelativePosition::Center, 1.0f);
      DrawStringDecal(pRow, row, olc::BLACK);
    }
  }

  void
  App::drawPieces(const RenderDesc& res) noexcept {
    SpriteDesc sd = {};
    sd.loc = pge::RelativePosition::Center;
    sd.radius = 0.9f;

    const Board& b = (*m_board)();

    for (unsigned y = 0u ; y < 8u ; ++y) {
      for (unsigned x = 0u ; x < 8u ; ++x) {
        // Check if something is at this position.
        const Piece& p = b.at(x, y);
        if (p.invalid()) {
          continue;
        }

        sd.x = x;
        // Note that the board is upside down when drawn
        // on screen. This leads to displaying it in the
        // reverse direction for black.
        sd.y = m_game->getPlayer() == Color::White ? 7.0f - y : y;

        sd.sprite.pack = m_piecesPackID;
        sd.sprite.id = 0;
        sd.sprite.tint = olc::WHITE;
        sd.sprite.sprite = olc::vi2d(
          spriteIDFromPiece(p),
          p.color() == Color::White ? 0 : 1
        );

        drawSprite(sd, res.cf);
      }
    }
  }

  void
  App::drawOverlays(const RenderDesc& res) noexcept {
    SpriteDesc sd = {};
    sd.loc = pge::RelativePosition::Center;
    sd.radius = 1.0f;

    const Board& b = (*m_board)();

    // Draw the overlay in case the mouse is over
    // a cell with a piece.
    olc::vi2d mp = GetMousePos();
    olc::vi2d mtp = res.cf.pixelCoordsToTiles(mp, nullptr);
    if (mtp.x >= 0 && mtp.x < b.w() && mtp.y >= 0 && mtp.y < b.h()) {
      unsigned x = std::clamp(mtp.x, 0, b.w());
      unsigned y = std::clamp(mtp.y, 0, b.h());

      sd.x = 1.0f * x;
      sd.y = 1.0f * y;

      sd.sprite.tint = olc::Pixel(0, 255, 0, pge::alpha::AlmostTransparent);
      drawRect(sd, res.cf);
    }

    // Draw overlay for the last move.
    Coordinates s(0, 0), e(0, 0);
    bool valid = false;

    // Start with the current rount.
    Round r = m_game->getCurrentRound();
    if (!r.whitePlayed()) {
      // In case white didn't play yet it means that
      // we should use the last round to get meaningful
      // information.
      r = m_game->getLastRound();
    }

    // Start by consider black: if they played it means
    // that we should display this information as it is
    // always the second thing happening in a move.
    if (r.blackPlayed()) {
      s = r.getMoveStart(Color::Black);
      e = r.getMoveEnd(Color::Black);

      valid = true;
    }
    // Otherwise display white information if any.
    else if (r.whitePlayed()) {
      s = r.getMoveStart(Color::White);
      e = r.getMoveEnd(Color::White);

      valid = true;
    }

    // If one move is to be displayed, then display as
    // an overlay the starting and ending positions.
    if (valid) {
      sd.sprite.tint = olc::Pixel(128, 128, 0, pge::alpha::SemiOpaque);

      sd.x = 1.0f * s.x();
      sd.y = m_game->getPlayer() == Color::White ? 7.0f - s.y() : s.y();
      drawRect(sd, res.cf);

      sd.x = 1.0f * e.x();
      sd.y = m_game->getPlayer() == Color::White ? 7.0f - e.y() : e.y();
      drawRect(sd, res.cf);
    }

    // Draw the overlay for the selected piece.
    CoordinatesShPtr c = m_game->getSelectedPosition();
    if (c != nullptr) {
      sd.radius = 1.0f;

      sd.x = 1.0f * c->x();
      // Note that the board is upside down when drawn
      // on screen.
      sd.y = m_game->getPlayer() == Color::White ? 7.0f - c->y() : c->y();

      sd.sprite.tint = olc::Pixel(0, 0, 128, pge::alpha::AlmostTransparent);
      drawRect(sd, res.cf);

      // Also, not the possible positions for this piece.
      CoordinatesSet ps = b.availablePositions(*c);

      for (CoordinatesSet::const_iterator it = ps.cbegin() ; it != ps.cend() ; ++it) {
        sd.x = 1.0f * it->x();
        // Note that the board is upside down when drawn
        // on screen.
        sd.y = m_game->getPlayer() == Color::White ? 7.0f - it->y() : it->y();

        sd.sprite.tint = olc::Pixel(0, 0, 255, pge::alpha::AlmostTransparent);
        drawRect(sd, res.cf);
      }
    }
  }

}
