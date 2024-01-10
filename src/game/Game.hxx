#ifndef    GAME_HXX
# define   GAME_HXX

# include "Game.hh"

namespace pge {

  inline
  void
  Game::terminate() noexcept {
    info("Game has been terminated");
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

    info("Game is now paused");
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
    chess::Rounds rs = m_board->getRounds();
    if (rs.empty()) {
      return m_board->getCurrentRound();
    }

    return rs.back();
  }

  inline
  chess::Color
  Game::getPlayer() const noexcept {
    return m_ai->side() == chess::Color::White ? chess::Color::Black : chess::Color::White;
  }

}

#endif    /* GAME_HXX */
