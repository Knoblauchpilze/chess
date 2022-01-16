#ifndef    CHESS_APP_HH
# define   CHESS_APP_HH

# include "PGEApp.hh"
# include "TexturePack.hh"
# include "Menu.hh"
# include "Game.hh"

namespace pge {

  class ChessApp: public PGEApp {
    public:

      /**
       * @brief - Create a new default pixel game engine app.
       * @param desc - contains all the needed information to
       *               create the canvas needed by the app and
       *               set up base properties.
       */
      ChessApp(const AppDesc& desc);

      /**
       * @brief - Desctruction of the object.
       */
      ~ChessApp() = default;

    protected:

      void
      loadData() override;

      void
      loadResources() override;

      void
      loadMenuResources() override;

      void
      cleanResources() override;

      void
      cleanMenuResources() override;

      void
      drawDecal(const RenderDesc& res) override;

      void
      draw(const RenderDesc& res) override;

      void
      drawUI(const RenderDesc& res) override;

      void
      drawDebug(const RenderDesc& res) override;

      bool
      onFrame(float fElapsed) override;

      void
      onInputs(const controls::State& c,
               const CoordinateFrame& cf) override;

    private:

      /**
       * @brief - Convenience structure regrouping needed props
       *          to draw a sprite.
       */
      struct SpriteDesc {
        float x;
        float y;

        float radius;
        RelativePosition loc;

        sprites::Sprite sprite;
      };

      /**
       * @brief - Describe a possible orientation for a graphic
       *          component (e.g. a healthbar, etc.).
       */
      enum class Orientation {
        Horizontal,
        Vertical
      };

      /**
       * @brief - Used to draw the tile referenced by the input
       *          struct to the screen using the corresponding
       *          visual representation.
       * @param t - the description of the tile to draw.
       * @param cf - the coordinate frame to use to perform the
       *             conversion from tile position to pixels.
       */
      void
      drawSprite(const SpriteDesc& t, const CoordinateFrame& cf);

      /**
       * @brief - Used to draw a simple rect at the specified
       *          location. Note that we reuse the sprite desc
       *          but don't actually use the sprite.
       * @param t - the description of the tile to draw.
       * @param cf - the coordinate frame to use to perform the
       *             conversion from tile position to pixels.
       */
      void
      drawRect(const SpriteDesc& t,
               const CoordinateFrame& cf);

    private:

      /**
       * @brief - The game managed by this application.
       */
      GameShPtr m_game;

      /**
       * @brief - Defines the list of menus available for
       *          this app.
       */
      std::vector<MenuShPtr> m_menus;

      /**
       * @brief - A description of the textures used to represent
       *          the elements of the game.
       */
      TexturePackShPtr m_packs;

      /**
       * @brief - The identifier of the pack containing the pieces.
       */
      unsigned m_piecesPackID;

      unsigned m_currentPiece;
  };

}

# include "ChessApp.hxx"

#endif    /* CHESS_APP_HH */
