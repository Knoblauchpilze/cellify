
# include "Game.hh"
# include <cxxabi.h>
# include "Menu.hh"

/// @brief - The height of the status menu in pixels.
# define STATUS_MENU_HEIGHT 50

/// @brief - The width of the probe menu in pixels.
# define PROBE_MENU_WIDTH 150

/// @brief - The maximum speed for the simulation.
# define MAX_SIMULATION_SPEED 8.0f

namespace {

  pge::MenuShPtr
  generateMenu(const olc::vi2d& pos,
               const olc::vi2d& size,
               const std::string& text,
               const std::string& name,
               bool clickable = false)
  {
    pge::menu::MenuContentDesc fd = pge::menu::newMenuContent(text, "", size);
    fd.color = olc::WHITE;
    fd.hColor = olc::GREY;
    fd.align = pge::menu::Alignment::Center;

    return std::make_shared<pge::Menu>(
      pos,
      size,
      name,
      pge::menu::newColoredBackground(olc::VERY_DARK_BLUE),
      fd,
      pge::menu::Layout::Horizontal,
      clickable,
      false
    );
  }

}

namespace pge {

  Game::Game(cellify::WorldShPtr world):
    utils::CoreObject("game"),

    m_state(
      State{
        true,  // paused
        true,  // disabled
        false, // terminated
        1.0f,  // speed
      }
    ),

    m_world(world),

    m_menus()
  {
    setService("game");
  }

  Game::~Game() {}

  std::vector<MenuShPtr>
  Game::generateMenus(float width,
                      float height)
  {
    // Generate the status menu.
    pge::MenuShPtr status = generateMenu(olc::vi2d(), olc::vi2d(width, STATUS_MENU_HEIGHT), "", "root");

    olc::vi2d pos;
    olc::vi2d dims(50, STATUS_MENU_HEIGHT);
    m_menus.count = generateMenu(pos, dims, "N/A agent(s)", "count");
    m_menus.speed = generateMenu(pos, dims, "Speed: x1", "speed", true);

    // Register menus in the parent.
    status->addMenu(m_menus.count);
    status->addMenu(m_menus.speed);
    m_menus.speed->setSimpleAction(
      [this](Game& g) {
        g.speedUpSimulation();
      }
    );

    // Generate the probe menu.
    pos = olc::vi2d(width - PROBE_MENU_WIDTH, STATUS_MENU_HEIGHT);
    dims = olc::vi2d(PROBE_MENU_WIDTH, height - STATUS_MENU_HEIGHT);
    m_menus.probe = generateMenu(pos, dims, "PROBE", "prove", true);

    // Package menus for output.
    std::vector<MenuShPtr> menus;

    menus.push_back(status);

    menus.push_back(m_menus.probe);

    return menus;
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
  Game::speedUpSimulation() noexcept {
    // Only available when the game is not paused.
    if (m_state.paused) {
      return;
    }

    float s = m_state.speed;

    m_state.speed *= 2.0f;
    if (m_state.speed > MAX_SIMULATION_SPEED) {
      m_state.speed = 1.0f;
    }

    log(
      "Simulation speed updated from " + std::to_string(s) +
      " to " + std::to_string(m_state.speed),
      utils::Level::Info
    );
  }

  void
  Game::updateUI() {
    // Update the speed of the simulation.
    int sp = static_cast<int>(std::round(m_state.speed));
    m_menus.speed->setText("Speed: x" + std::to_string(sp));

    unsigned c = m_world->count(cellify::Tile::Ant);
    std::string str = std::to_string(c) + " agent";
    if (c != 1u) {
      str += "s";
    }
    m_menus.count->setText(str);
  }

}
