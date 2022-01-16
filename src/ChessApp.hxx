#ifndef    CHESS_APP_HXX
# define   CHESS_APP_HXX

# include "ChessApp.hh"
# include "Action.hh"

namespace pge {

  inline
  void
  ChessApp::loadData() {
    log("Load app data as needed", utils::Level::Info);
  }

  inline
  void
  ChessApp::loadResources() {
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
    setLayerTint(Layer::Draw, olc::Pixel(255, 255, 255, alpha::SemiOpaque));
  }

  inline
  void
  ChessApp::loadMenuResources() {
    log("Generate menus and register them in the 'm_menus' attribute", utils::Level::Info);
  }

  inline
  void
  ChessApp::cleanResources() {
    log("Clean app data as needed", utils::Level::Info);
  }

  inline
  void
  ChessApp::cleanMenuResources() {
    m_menus.clear();
  }

  inline
  void
  ChessApp::drawSprite(const SpriteDesc& t, const CoordinateFrame& cf) {
    olc::vf2d p = cf.tileCoordsToPixels(t.x, t.y, t.loc, t.radius);

    m_packs->draw(this, t.sprite, p, t.radius * cf.tileScale());
  }

  inline
  void
  ChessApp::drawRect(const SpriteDesc& t,
                     const CoordinateFrame& cf)
  {
    olc::vf2d p = cf.tileCoordsToPixels(t.x, t.y, t.loc, t.radius);
    FillRectDecal(p, t.radius * cf.tileSize(), t.sprite.tint);
  }

}

#endif    /* CHESS_APP_HXX */
