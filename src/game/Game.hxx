#ifndef    GAME_HXX
# define   GAME_HXX

# include "Game.hh"

namespace pge {

  inline
  void
  Game::terminate() noexcept {
    log("Game has been terminated", utils::Level::Info);
    m_state.terminated = true;
  }

  inline
  bool
  Game::terminated() const noexcept {
    return m_state.terminated;
  }

  inline
  void
  Game::pause() {
    // Do nothing in case the game is already paused.
    if (m_state.paused) {
      return;
    }

    log("Game is now paused", utils::Level::Info);
    m_state.paused = true;
  }

  inline
  chess::CoordinatesShPtr
  Game::getSelectedPosition() const noexcept {
    return m_start;
  }

  inline
  chess::Round
  Game::getCurrentRound() const noexcept {
    return m_board->getCurrentRound();
  }

  inline
  chess::Round
  Game::getLastRound() const noexcept {
    return m_board->getLastRound();
  }

}

#endif    /* GAME_HXX */
