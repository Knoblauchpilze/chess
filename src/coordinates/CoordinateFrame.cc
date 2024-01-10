
# include "CoordinateFrame.hh"
# include "utils.hh"

namespace pge {

  CoordinateFrame::CoordinateFrame(const Viewport& cvp,
                                   const Viewport& pvp,
                                   const olc::vi2d& tileSize):
    utils::CoreObject("frame"),

    m_cViewport(cvp),
    m_pViewport(pvp),

    m_ts(tileSize),
    m_scale(1.0f, 1.0f),
    m_tScaled(m_ts),

    m_translationOrigin(),
    m_cachedPOrigin()
  {
    setService("coordinate");

    // Update the tiles scale based on the viewport.
    updateTileScale();
  }

  const olc::vf2d&
  CoordinateFrame::tileScale() const noexcept {
    return m_scale;
  }

  olc::vf2d
  CoordinateFrame::tileSize() const noexcept {
    return m_scale * m_ts;
  }

  void
  CoordinateFrame::zoomIn(const olc::vf2d& pos) {
    zoom(0.5f, pos);
  }

  void
  CoordinateFrame::zoomOut(const olc::vf2d& pos) {
    zoom(2.0f, pos);
  }

  void
  CoordinateFrame::beginTranslation(const olc::vi2d& origin) {
    m_translationOrigin = origin;
    m_cachedPOrigin = m_pViewport.p;
  }

  void
  CoordinateFrame::translate(const olc::vi2d& pos) {
    // We need to deduce the translation added by
    // the input `pos` assuming that this will be
    // the final position of the viewport.
    olc::vf2d translation = pos - m_translationOrigin;
    m_pViewport.p = m_cachedPOrigin + translation;
  }

  void
  CoordinateFrame::updateTileScale() {
    m_tScaled = m_pViewport.dims / m_cViewport.dims;
    m_scale = m_tScaled / m_ts;

    verbose(
      "Tile size is " + toString(m_ts) + ", scale is " + toString(m_scale)
    );
  }

  void
  CoordinateFrame::zoom(float factor, const olc::vf2d& pos) {
    // What we know is that the top left cell of
    // the viewport is at the top left corner of
    // the pixels viewport.
    // We can compute the distance from the `pos`
    // to the pixels viewport origin: it should
    // be applied the *inverse* of the `factor`
    // and we should be good to go: indeed if a
    // viewport has its span reduced, distances
    // are lengthened (and conversely).
    olc::vf2d d = m_pViewport.p - pos;
    d /= factor;

    m_pViewport.p = pos + d;

    // Also update the dimensions of the cells
    // viewport by `factor`.
    m_cViewport.dims *= factor;

    updateTileScale();
  }

}
