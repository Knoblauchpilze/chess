#ifndef    CHESS_APP_HXX
# define   CHESS_APP_HXX

# include "App.hh"
# include "Action.hh"

namespace chess {

  inline
  void
  App::cleanResources() {
    if (m_packs != nullptr) {
      m_packs.reset();
    }
  }

  inline
  void
  App::cleanMenuResources() {
    m_menus.clear();
  }

  inline
  void
  App::drawSprite(const SpriteDesc& t, const pge::CoordinateFrame& cf) {
    olc::vf2d p = cf.tileCoordsToPixels(t.x, t.y, t.loc, t.radius);

    m_packs->draw(this, t.sprite, p, t.radius * cf.tileScale());
  }

  inline
  void
  App::drawRect(const SpriteDesc& t,
                const pge::CoordinateFrame& cf)
  {
    olc::vf2d p = cf.tileCoordsToPixels(t.x, t.y, t.loc, t.radius);
    FillRectDecal(p, t.radius * cf.tileSize(), t.sprite.tint);
  }

}

#endif    /* CHESS_APP_HXX */
