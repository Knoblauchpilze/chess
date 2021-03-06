#ifndef    GAME_STATE_HH
# define   GAME_STATE_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include "olcEngine.hh"
# include "Menu.hh"

namespace pge {

  /// @brief - Convenience enumeration defining the current state
  /// of the application: each value roughly corresponds
  /// to a distinct menu in the application.
  enum class Screen {
    Home,
    SideSelection,
    Game,
    GameOver,
    Exit
  };

  class GameState: public utils::CoreObject {
    public:

      /**
       * @brief - Creates a new game state with the specified
       *          state.
       * @param dims - the dimensions of the desired UI.
       * @param screen - the current screen.
       */
      GameState(const olc::vi2d& dims,
                const Screen& screen);

      /**
       * @brief - Retrieves the currently selected screen.
       * @return - the currently selected screen.
       */
      Screen
      getScreen() const noexcept;

      /**
       * @brief - Define a new active screen for this game.
       * @param screen - the new screen to apply.
       */
      void
      setScreen(const Screen& screen);

      /**
       * @brief - Used to render the screen corresponding to
       *          the current state using the provided game
       *          renderer.
       * @param pge - the engine to use to render the game
       *              state.
       */
      void
      render(olc::PixelGameEngine* pge) const;

      /**
       * @brief - Performs the interpretation of the controls
       *          provided as input to update the selected
       *          screen. Actions may be generated through
       *          this mechanism.
       * @param c - the controls and user input for this
       *            frame.
       * @param actions - the list of actions produced by the
       *                  menu while processing the events.
       * @return - the description of what happened when the
       *           inputs has been processed.
       */
      menu::InputHandle
      processUserInput(const controls::State& c,
                       std::vector<ActionShPtr>& actions);

    private:

      void
      generateHomeScreen(const olc::vi2d& dims);

      void
      generateSideSelectionScreen(const olc::vi2d& dims);

      void
      generateGameOverScreen(const olc::vi2d& dims);

    private:

      /**
       * @brief - Defines the current screen selected in this
       *          game. Updated whenever the user takes action
       *          to change it.
       */
      Screen m_screen;

      /**
       * @brief - Defines the screen to display when the game is
       *          on the home screen.
       */
      MenuShPtr m_home;

      /**
       * @brief - Defines the screen to choose a side.
       */
      MenuShPtr m_side;

      /**
       * @brief - Defines the menu to display in case
       *          the game is over.
       */
      MenuShPtr m_gameOver;
  };

  using GameStateShPtr = std::shared_ptr<GameState>;
}

#endif    /* GAME_STATE_HH */
