
# include "Game.hh"
# include <cxxabi.h>
# include "Menu.hh"

namespace pge {

  Game::Game(cellify::WorldShPtr world):
    utils::CoreObject("game"),

    m_state(
      State{
        true,  // paused
        true,  // disabled
        false, // terminated
      }
    ),

    m_world(world)
  {
    setService("game");
  }

  Game::~Game() {}

  std::vector<MenuShPtr>
  Game::generateMenus(float /*width*/,
                      float /*height*/)
  {
    log("Generate UI menus here", utils::Level::Info);
    return std::vector<MenuShPtr>();
  }

  void
  Game::performAction(float /*x*/, float /*y*/) {
    // Only handle actions when the game is not disabled.
    if (m_state.disabled) {
      log("Ignoring action while menu is disabled");
      return;
    }
  }

  bool
  Game::step(float tDelta) {
    // When the game is paused it is not over yet.
    if (m_state.paused) {
      return true;
    }

    // Step the world.
    m_world->step(tDelta);

    updateUI();

    return true;
  }

  void
  Game::togglePause() {
    if (m_state.paused) {
      resume();
    }
    else {
      pause();
    }

    enable(!m_state.paused);
  }

  void
  Game::enable(bool enable) {
    m_state.disabled = !enable;

    if (m_state.disabled) {
      log("Disabled game UI", utils::Level::Verbose);
    }
    else {
      log("Enabled game UI", utils::Level::Verbose);
    }
  }

  void
  Game::updateUI() {
    /// TODO: Handle menus.
  }

}
