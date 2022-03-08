
# include "Element.hh"
# include <cstring>
# include "Grid.hh"
# include "Ant.hh"
# include "Colony.hh"
# include "Pheromon.hh"

/// @brief - The interval defining two consecutive
/// moves of an ant in milliseconds.
# define IDLE_TIME 200

namespace {

  cellify::Tile
  tileFromBrain(cellify::AIShPtr brain) noexcept {
    // Based on the type of the AI, assign the correct
    // type of tile.
    if (std::dynamic_pointer_cast<cellify::Colony>(brain)) {
      return cellify::Tile::Colony;
    }
    if (std::dynamic_pointer_cast<cellify::Pheromon>(brain)) {
      return cellify::Tile::Pheromon;
    }

    // Assume it's an ant as the food doesn't have a
    // brain at all.
    return cellify::Tile::Ant;
  }

  std::vector<char>
  dataFromBrain(cellify::AIShPtr brain) noexcept {
    std::vector<char> out;

    // Based on the type of the AI, assign the correct
    // property to the data.
    cellify::AntShPtr a = std::dynamic_pointer_cast<cellify::Ant>(brain);
    if (a) {
      out.resize(sizeof(cellify::Behavior));
      cellify::Behavior b = a->mode();

      std::memcpy(
        out.data(),
        reinterpret_cast<const char*>(&b),
        sizeof(cellify::Behavior)
      );
    }
    cellify::PheromonShPtr p = std::dynamic_pointer_cast<cellify::Pheromon>(brain);
    if (p) {
      out.resize(sizeof(cellify::Scent));
      cellify::Scent s = p->kind();

      std::memcpy(
        out.data(),
        reinterpret_cast<const char*>(&s),
        sizeof(cellify::Scent)
      );
    }

    // Assume there's no data.
    return out;
  }

}

namespace cellify {

  Element::Element(const Tile& t,
                   const utils::Point2i& pos,
                   AIShPtr brain,
                   const std::vector<char>& data,
                   const utils::Uuid& uuid):
    utils::CoreObject(tileToString(t)),

    m_uuid(uuid),

    m_tile(t),
    m_data(),
    m_pos(pos),

    m_brain(brain),

    m_deleted(false),

    m_path(),
    m_last(0.0f),
    m_elapsedSinceLast(zero())
  {
    setService("world");

    // Generate valid uuid if needed.
    if (!m_uuid.valid()) {
      m_uuid = utils::Uuid::create();
    }

    // Copy the specific data.
    if (!data.empty()) {
      m_data.insert(m_data.end(), data.begin(), data.end());
    }
  }

  const Tile&
  Element::type() const noexcept {
    return m_tile;
  }

  bool
  Element::hasData() const noexcept {
    return !m_data.empty();
  }

  const char*
  Element::data() const noexcept {
    // An empty vector doesn't have any data only when
    // it has not been allocated.
    if (!hasData()) {
      return nullptr;
    }

    return m_data.data();
  }

  const utils::Point2i&
  Element::pos() const noexcept {
    return m_pos;
  }

  bool
  Element::tobeDeleted() const noexcept {
    return m_deleted;
  }

  void
  Element::plug(AIShPtr brain) noexcept {
    m_brain = brain;
  }

  void
  Element::init(StepInfo& info) {
    // No brain means no action.
    if (m_brain == nullptr) {
      return;
    }

    // Initialize the brain.
    Info i = {
      m_pos,
      info.rng,
      info.moment,
      info.elapsed,
      m_path,
      info.grid,
      m_deleted,
      Animats()
    };
    m_brain->init(i);

    // Persist the information.
    if (!m_path.empty()) {
      m_pos = m_path.begin();
    }
    m_deleted = i.selfDestruct;

    // Copy the spawned elements.
    for (unsigned id = 0u ; id < i.spawned.size() ; ++id) {
      const Animat& a = i.spawned[id];

      // Generate the type of the element from its brain.
      Tile t = tileFromBrain(a.brain);
      std::vector<char> d = dataFromBrain(a.brain);

      ElementShPtr e = std::make_shared<Element>(t, a.pos, a.brain, d);

      info.spawned.push_back(e);
    }

    m_last = info.moment;
  }

  void
  Element::step(StepInfo& info) {
    // No brain means no action.
    if (m_brain == nullptr) {
      return;
    }

    // Advance the brain.
    Info i = {
      m_pos,
      info.rng,
      info.moment,
      info.elapsed,
      m_path,
      info.grid,
      m_deleted,
      Animats()
    };
    m_brain->step(i);

    // Pick the next position in the path and advance
    // to this location if we moved long enough in the
    // past.
    if (!m_path.empty()) {
      Duration d = info.moment - m_last;
      if (d >= millisecondsToDuration(IDLE_TIME)) {
        m_pos = m_path.advance();
        m_last = info.moment;
      }
    }

    // Persist the information.
    m_deleted = i.selfDestruct;

    // And copy the spawned elements.
    for (unsigned id = 0u ; id < i.spawned.size() ; ++id) {
      const Animat& a = i.spawned[id];

      // Generate the type of the element from its brain.
      Tile t = tileFromBrain(a.brain);
      std::vector<char> d = dataFromBrain(a.brain);

      ElementShPtr e = std::make_shared<Element>(t, a.pos, a.brain, d);

      /// TODO: Associate data.

      info.spawned.push_back(e);
    }
  }

  void
  Element::pause(const TimeStamp& t) {
    // Update the elapsed time since the last moment
    // the agent moved.
    m_elapsedSinceLast = t - m_last;
  }

  void
  Element::resume(const TimeStamp& t) {
    // Restore the last time the agent move so that
    // it is as in the past as indicated by the value
    // of the dedicated attribute.
    m_last = t - m_elapsedSinceLast;
    m_elapsedSinceLast = zero();
  }

}
