#ifndef    GAME_HH
# define   GAME_HH

# include <vector>
# include <memory>
# include <core_utils/CoreObject.hh>
# include <core_utils/TimeUtils.hh>
# include "Board.hh"
# include "AI.hh"

namespace pge {

  // Forward declaration of the `Menu` class to be able
  // to use it right away.
  class Menu;
  using MenuShPtr = std::shared_ptr<Menu>;

  class Game: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new game with default parameters.
       * @param board - the board managed by this game.
       */
      Game(chess::BoardShPtr board);

      ~Game();

      /**
       * @brief - Used to perform the creation of the menus
       *          allowing to control the world wrapped by
       *          this game.
       * @param width - the zidth of the window in pixels into
                        which this menu will be inserted.
       * @param height - the height of the window in pixels
       *                 into which this menu will be inserted.
       * @return - the list of menus representing this game.
       */
      virtual std::vector<MenuShPtr>
      generateMenus(float width,
                    float height);

      /**
       * @brief - Used to create a tower with the specified
       *          type (as defined by the `setTowerType`)
       *          method at the specified position. Note
       *          that we will only perform the action if
       *          no elements exist here.
       *          Also note that the coordinates are used as
       *          is and should thus correspond to values that
       *          interpretable by the underlying game data.
       * @param x - the abscissa of the position at which the
       *            action should be taking place.
       * @param y - the ordinate of the position at which the
       *            action will be taking place.
       */
      void
      performAction(float x, float y);

      /**
       * @brief - Requests the game to be terminated. This is
       *          applied to the next iteration of the game
       *          loop.
       */
      void
      terminate() noexcept;

      /**
       * @brief - Returns whether or not the game has been
       *          terminated. The game is terminated when
       *          the user wants to exit the app (usually).
       */
      bool
      terminated() const noexcept;

      /**
       * @brief - Forward the call to step one step ahead
       *          in time to the internal world.
       * @param tDelta - the duration of the last frame in
       *                 seconds.
       * @param bool - `true` in case the game continues,
       *               and `false` otherwise (i.e. if the
       *               game is ended).
       */
      bool
      step(float tDelta);

      /**
       * @brief - Performs the needed operation to handle
       *          the pause and resume operation for this
       *          game. It will automatically disable the
       *          menu if needed or make it visible again.
       */
      void
      togglePause();

      /**
       * @brief - Used to indicate that the world should be
       *          paused. Time based entities and actions
       *          should take actions to correctly resume at
       *          a later time.
       */
      void
      pause();

      /**
       * @brief - Used to indicate that the world should be
       *          resuming its activity. Time based entities
       *          should take actions to be resuming their
       *          pathes, motions, etc.
       */
      void
      resume();

      /**
       * @brief - Returns the selected position (may be `null`
       *          in case nothing is selected).
       * @return - the selected coordinate if any.
       */
      chess::CoordinatesShPtr
      getSelectedPosition() const noexcept;

      /**
       * @brief - Returns the current move being played. May be
       *          incomplete if the current player is black.
       * @return - the current round being played.
       */
      chess::Round
      getCurrentRound() const noexcept;

      /**
       * @brief - Returns the last complete move that was played.
       *          Always complete unless it's the first move.
       * @return - the last round played.
       */
      chess::Round
      getLastRound() const noexcept;

    private:

      /**
       * @brief - Used to enable or disable the menus that
       *          compose the game. This allows to easily
       *          hide any game related component.
       * @param enable - `true` if the menus are enabled.
       */
      void
      enable(bool enable);

      /**
       * @brief - Used during the step function and by any process
       *          that needs to update the UI and the text content
       *          of menus.
       */
      virtual void
      updateUI();

    private:

      /// @brief - Convenience information defining the state of the
      /// game. It includes information about whether the
      /// menus should be displayed and if the user actions
      /// should be interpreted or not.
      struct State {
        // Defines whether this world is paused (i.e.
        // internal attributes of the mobs/blocks/etc
        // have already been updated to reflect the
        // pause status) or not. This allows to react
        // to consecutive pause requests and prevent
        // weird behaviors to occur.
        bool paused;

        // Whether or not the UI is disabled.
        bool disabled;

        // Used to hold whether or not the game has been shut
        // down. It usuallu indicates that no simulation will
        // be performed anymore and usually indicates that a
        // termination request has been received.
        bool terminated;
      };

      /// @brief - Convenience structure allowing to display the info
      /// about the current game.
      struct Menus {
        /// The current round.
        MenuShPtr round;

        /// Who is next to play.
        MenuShPtr player;

        /// The check mate status.
        MenuShPtr status;

        /// Information about when the check state was reached.
        utils::TimeStamp checkDate;

        /// Keep track of whether the player was in check in the last
        /// frame.
        bool wasInCheck;

        /// The alert menu indicating that the player is now in check.
        MenuShPtr check;

        /// Information about when the checkmate state was reached.
        utils::TimeStamp checkmateDate;

        /// Keep track of whether the player was in checkmate in the
        /// last frame.
        bool wasInCheckmate;

        /// The alert menu indicating that the player is now in check.
        MenuShPtr checkmate;

        /// The last moves.
        std::vector<MenuShPtr> moves;

        /// The current menu to update for the next move.
        unsigned move;
      };

      /**
       * @brief - The definition of the game state.
       */
      State m_state;

      /**
       * @brief - The board managed by this game.
       */
      chess::BoardShPtr m_board;

      /**
       * @brief - The first coordinate that was clicked on
       *          by the user. Defines the starting point of
       *          a potential motion.
       *          No location selected if null.
       */
      chess::CoordinatesShPtr m_start;

      /**
       * @brief - The AI used to play the other color compared
       *          to what the user chose.
       */
      chess::AIShPtr m_ai;

      /**
       * @brief - The menus registered to display information to
       *          the user about the current state of the game.
       */
      Menus m_menus;
  };

  using GameShPtr = std::shared_ptr<Game>;
}

# include "Game.hxx"

#endif    /* GAME_HH */
