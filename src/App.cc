
# include "App.hh"
# include "Ant.hh"
# include "Pheromon.hh"

namespace {

  olc::Pixel
  colorFromTile(const cellify::Tile& t,
                const char* data) noexcept
  {
    switch (t) {
      case cellify::Tile::Colony:
        return olc::RED;
      case cellify::Tile::Ant: {
        const cellify::Behavior* b = reinterpret_cast<const cellify::Behavior*>(data);

        if (*b == cellify::Behavior::Return) {
          return olc::ORANGE;
        }
        if (*b == cellify::Behavior::Food) {
          return olc::YELLOW;
        }
        return olc::BLUE;
      }
      case cellify::Tile::Food:
        return olc::GREEN;
      case cellify::Tile::Pheromon: {
        const cellify::Scent* s = reinterpret_cast<const cellify::Scent*>(data);

        if (*s == cellify::Scent::Food) {
          return olc::Pixel(192, 255, 2);
        }
        return olc::Pixel(137, 209, 254);
      }
      case cellify::Tile::Obstacle:
        return olc::DARK_GREY;
      default:
        // Error case.
        return olc::RED;
    }
  }
}

namespace pge {

  App::App(const AppDesc& desc):
    PGEApp(desc),

    m_game(nullptr),
    m_state(nullptr),
    m_menus(),

    m_packs(std::make_shared<TexturePack>()),

    m_world(std::make_shared<cellify::World>())
  {}

  bool
  App::onFrame(float fElapsed) {
    // Handle case where no game is defined.
    if (m_game == nullptr) {
      return false;
    }

    if (!m_game->step(fElapsed)) {
      log("This is game over", utils::Level::Info);
    }

    return m_game->terminated();
  }

  void
  App::onInputs(const controls::State& c,
                const CoordinateFrame& cf)
  {
    // Handle case where no game is defined.
    if (m_game == nullptr) {
      return;
    }

    // Handle menus update and process the
    // corresponding actions.
    std::vector<ActionShPtr> actions;
    bool relevant = false;

    for (unsigned id = 0u ; id < m_menus.size() ; ++id) {
      menu::InputHandle ih = m_menus[id]->processUserInput(c, actions);
      relevant = (relevant || ih.relevant);
    }

    if (m_state != nullptr) {
      menu::InputHandle ih = m_state->processUserInput(c, actions);
      relevant = (relevant || ih.relevant);
    }

    for (unsigned id = 0u ; id < actions.size() ; ++id) {
      actions[id]->apply(*m_game);
    }

    bool lClick = (c.buttons[controls::mouse::Left] == controls::ButtonState::Released);
    if (lClick && !relevant) {
      olc::vf2d it;
      olc::vi2d tp = cf.pixelCoordsToTiles(olc::vi2d(c.mPosX, c.mPosY), &it);

      m_game->performAction(tp.x + it.x, tp.y + it.y);
    }

    if (c.keys[controls::keys::P]) {
      m_game->togglePause();
    }
  }

  void
  App::loadData() {
    log("Load app data as needed", utils::Level::Info);

    // Create the game and its state.
    m_game = std::make_shared<Game>(m_world);
  }

  void
  App::loadResources() {
    // Assign a specific tint to the regular
    // drawing layer so that we have a built
    // in transparency.
    // We can't do it directly when drawing
    // in the rendering function because as
    // the whole layer will be drawn as one
    // quad in opengl with an opaque alpha,
    // we will lose this info.
    // This means that everything is indeed
    // transparent but that's the only way
    // for now to achieve it.
    setLayerTint(Layer::Draw, olc::Pixel(255, 255, 255, alpha::SemiOpaque));

    log("Load app resources in the 'm_packs' attribute", utils::Level::Info);
  }

  void
  App::loadMenuResources() {
    // Generate the game state.
    m_state = std::make_shared<GameState>(
      olc::vi2d(ScreenWidth(), ScreenHeight()),
      Screen::Game
    );

    m_menus = m_game->generateMenus(ScreenWidth(), ScreenHeight());
  }

  void
  App::cleanResources() {
    if (m_packs != nullptr) {
      m_packs.reset();
    }
  }

  void
  App::cleanMenuResources() {
    m_menus.clear();
  }

  void
  App::drawDecal(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::BLACK);

    // In case we're not in the game screen, do nothing.
    if (m_state->getScreen() != Screen::Game) {
      SetPixelMode(olc::Pixel::NORMAL);
      return;
    }

    drawWorld(res);
    drawOverlays(res);
    drawGrid(res);

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  App::draw(const RenderDesc& /*res*/) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

    // In case we're not in game mode, just render
    // the state.
    if (m_state->getScreen() != Screen::Game) {
      m_state->render(this);
      SetPixelMode(olc::Pixel::NORMAL);
      return;
    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  App::drawUI(const RenderDesc& /*res*/) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

    // In case we're not in game mode, just render
    // the state.
    if (m_state->getScreen() != Screen::Game) {
      m_state->render(this);
      SetPixelMode(olc::Pixel::NORMAL);
      return;
    }

    // Render the game menus.
    for (unsigned id = 0u ; id < m_menus.size() ; ++id) {
      m_menus[id]->render(this);
    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  App::drawDebug(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

    // In case we're not in game mode, just render
    // the state.
    if (m_state->getScreen() != Screen::Game) {
      m_state->render(this);
      SetPixelMode(olc::Pixel::NORMAL);
      return;
    }

    // Draw cursor's position.
    olc::vi2d mp = GetMousePos();
    olc::vf2d it;
    olc::vi2d mtp = res.cf.pixelCoordsToTiles(mp, &it);

    int h = GetDrawTargetHeight();
    int dOffset = 15;
    DrawString(olc::vi2d(0, h / 2), "Mouse coords      : " + toString(mp), olc::CYAN);
    DrawString(olc::vi2d(0, h / 2 + 1 * dOffset), "World cell coords : " + toString(mtp), olc::CYAN);
    DrawString(olc::vi2d(0, h / 2 + 2 * dOffset), "Intra cell        : " + toString(it), olc::CYAN);

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  App::drawWorld(const RenderDesc& res) noexcept {
    // We want to draw first the pheromons and then the
    // solid elements, and finally the ants.
    drawWorldLayer(res, std::unordered_set<cellify::Tile>{cellify::Tile::Pheromon});
    drawWorldLayer(res, std::unordered_set<cellify::Tile>{cellify::Tile::Food, cellify::Tile::Colony, cellify::Tile::Obstacle});
    drawWorldLayer(res, std::unordered_set<cellify::Tile>{cellify::Tile::Ant});
  }

  void
  App::drawWorldLayer(const RenderDesc& res,
                      const std::unordered_set<cellify::Tile>& layer) noexcept
  {
    SpriteDesc sd = {};
    sd.loc = pge::RelativePosition::Center;
    sd.radius = 1.0f;

    const cellify::Grid& g = m_world->grid();

    const Viewport& tvp = res.cf.cellsViewport();

    // Traverse the list of elements of the world. We
    // want to draw first the pheromons and then the
    // solid elements, and finally the ants.
    for (unsigned id = 0u ; id < g.size() ; ++id) {
      const cellify::Element& e = g.at(id);

      // Ignore items outside of the view frustum.
      if (!tvp.visible(e.pos(), 0.0f)) {
        continue;
      }

      // Ignore items that are not in the current layer.
      if (layer.count(e.type()) == 0) {
        continue;
      }

      sd.x = 1.0f * e.pos().x();
      sd.y = 1.0f * e.pos().y();

      sd.sprite.tint = colorFromTile(e.type(), e.data());

      // Make pheromons not fully opaque.
      if (e.type() == cellify::Tile::Pheromon) {
        sd.sprite.tint.a = alpha::AlmostOpaque;
      }

      drawRect(sd, res.cf);
    }
  }

  void
  App::drawOverlays(const RenderDesc& res) noexcept {
    SpriteDesc sd = {};
    sd.loc = pge::RelativePosition::Center;
    sd.radius = 1.0f;

    // Draw the overlay of the mouse position.
    olc::vi2d mp = GetMousePos();
    olc::vi2d mtp = res.cf.pixelCoordsToTiles(mp, nullptr);

    sd.x = mtp.x;
    sd.y = mtp.y;

    sd.sprite.tint = olc::Pixel(0, 255, 0, pge::alpha::AlmostTransparent);
    drawRect(sd, res.cf);
  }

  void
  App::drawGrid(const RenderDesc& res) noexcept {
    SpriteDesc sd = {};
    sd.loc = pge::RelativePosition::Center;
    sd.radius = 1.0f;

    // Draw the grid.
    olc::vf2d tl = res.cf.cellsViewport().topLeft();
    olc::vf2d dims = res.cf.cellsViewport().dims();

    olc::vi2d min;
    min.x = static_cast<int>(std::floor(tl.x));
    min.y = static_cast<int>(std::floor(tl.y));

    olc::vi2d max;
    max.x = static_cast<int>(std::floor(tl.x + dims.x));
    max.y = static_cast<int>(std::floor(tl.y + dims.y));

    // Rendering function.
    auto render = [this, &sd, &res, &dims](int x, int y, bool horizontal, olc::Pixel color) {
      sd.x = x;
      sd.y = y;

      olc::vf2d p = res.cf.tileCoordsToPixels(sd.x, sd.y, sd.loc, sd.radius);

      // Add a bit of buffer in order to allow the
      // lines to cover the whole screen.
      float sx, sy;
      if (horizontal) {
        sx = (dims.x + 1.0f) * res.cf.tileSize().x;
        sy = 1.0f;
      }
      else {
        sx = 1.0f;
        sy = (dims.y + 1.0f) * res.cf.tileSize().y;
      }


      FillRectDecal(p, olc::vf2d(sx, sy), color);
    };

    // Render each line
    olc::Pixel norm = olc::VERY_DARK_GREY;
    olc::Pixel bold = olc::DARK_RED;
    olc::Pixel impo = olc::WHITE;

    auto colorize = [&norm, &bold, &impo](int v) {
      if (v % 10 == 0) {
        return impo;
      }
      if (v % 5 == 0) {
        return bold;
      }

      return norm;
    };

    for (int y = min.y ; y < max.y ; ++y) {
      for (int x = min.x ; x < max.x ; ++x) {
        render(x, y, false, colorize(x));
      }

      render(min.x, y, true, colorize(y));
    }
  }

}
