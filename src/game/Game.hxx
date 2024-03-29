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

    m_world->pause();

    info("Game is now paused");
    m_state.paused = true;
  }

  inline
  void
  Game::resume() {
    // Do nothing in case the game is already running.
    if (!m_state.paused) {
      return;
    }

    m_world->resume();

    info("Game is now resumed");
    m_state.paused = false;
  }

}

#endif    /* GAME_HXX */
