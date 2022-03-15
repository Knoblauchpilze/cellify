
# include "Game.hh"
# include <cxxabi.h>
# include "Menu.hh"

/// @brief - The height of the status menu in pixels.
# define STATUS_MENU_HEIGHT 50

/// @brief - The height of the building menu in pixels.
# define BUILDING_MENU_HEIGHT 50

/// @brief - The maximum speed for the simulation.
# define MAX_SIMULATION_SPEED 8.0f

namespace {

  pge::MenuShPtr
  generateMenu(const olc::vi2d& pos,
               const olc::vi2d& size,
               const std::string& text,
               const std::string& name,
               bool clickable = false,
               bool selectable = false)
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
      selectable
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

    m_menus(),

    // By default, add a food item.
    m_itemToAdd(cellify::Tile::Obstacle)
  {
    setService("game");
  }

  Game::~Game() {}

  std::vector<MenuShPtr>
  Game::generateMenus(float width,
                      float height)
  {
    // Generate the status menu.
    MenuShPtr status = generateMenu(olc::vi2d(), olc::vi2d(width, STATUS_MENU_HEIGHT), "", "status");

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

    // Generate the building menu.
    MenuShPtr building = generateMenu(olc::vi2d(0, height - BUILDING_MENU_HEIGHT), olc::vi2d(width, BUILDING_MENU_HEIGHT), "", "building");

    dims = olc::vi2d(50, BUILDING_MENU_HEIGHT);
    MenuShPtr label = generateMenu(pos, dims, "Add element:", "label");
    m_menus.food = generateMenu(pos, dims, "Food", "food", true, true);
    m_menus.obstacle = generateMenu(pos, dims, "Obstacle", "obstacle", true, true);

    // Register menus in the parent.
    building->addMenu(label);
    building->addMenu(m_menus.food);
    building->addMenu(m_menus.obstacle);
    m_menus.food->setSimpleAction(
      [this](Game& g) {
        g.setItemToAdd(cellify::Tile::Food);
      }
    );
    m_menus.obstacle->setSimpleAction(
      [this](Game& g) {
        g.setItemToAdd(cellify::Tile::Obstacle);
      }
    );

    // Package menus for output.
    std::vector<MenuShPtr> menus;

    menus.push_back(status);
    menus.push_back(building);

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

    // Step the world with a speed adapted to the
    // current selected speed for the game.
    m_world->step(m_state.speed * tDelta);

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
  Game::setItemToAdd(const cellify::Tile& tile) noexcept {
    log("Activated " + cellify::tileToString(tile) + " item to be placed in the world", utils::Level::Info);

    m_itemToAdd = tile;
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
