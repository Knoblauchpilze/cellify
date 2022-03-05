
# include "Element.hh"
# include "Grid.hh"

/// @brief - The interval defining two consecutive
/// moves of an ant in milliseconds.
# define IDLE_TIME 200

namespace cellify {

  Element::Element(const Tile& t,
                   const utils::Point2i& pos,
                   AIShPtr brain,
                   const utils::Uuid& uuid):
    utils::CoreObject(tileToString(t)),

    m_uuid(uuid),

    m_tile(t),
    m_pos(pos),

    m_brain(brain),

    m_deleted(false),

    m_path(),
    m_last(utils::now()),
    m_elapsedSinceLast(utils::Duration::zero())
  {
    setService("world");

    // Generate valid uuid if needed.
    if (!m_uuid.valid()) {
      m_uuid = utils::Uuid::create();
    }
  }

  const Tile&
  Element::type() const noexcept {
    return m_tile;
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
    m_pos = m_path.begin();
    m_deleted = i.selfDestruct;

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
    utils::Duration d = info.moment - m_last;
    if (utils::toMilliseconds(d) >= IDLE_TIME) {
      m_pos = m_path.advance();
      m_last = info.moment;
    }

    // Persist the information.
    m_deleted = i.selfDestruct;

    // And copy the spawned elements.
    for (unsigned id = 0u ; id < i.spawned.size() ; ++id) {
      const Animat& a = i.spawned[id];

      ElementShPtr e = std::make_shared<Element>(
        /* TODO: Type of the spawned animat */
        Tile::Ant,
        a.pos,
        a.brain
      );

      info.spawned.push_back(e);
    }
  }

  void
  Element::pause(const utils::TimeStamp& t) {
    // Update the elapsed time since the last moment
    // the agent moved.
    m_elapsedSinceLast = t - m_last;
  }

  void
  Element::resume(const utils::TimeStamp& t) {
    // Restore the last time the agent move so that
    // it is as in the past as indicated by the value
    // of the dedicated attribute.
    m_last = t - m_elapsedSinceLast;
    m_elapsedSinceLast = utils::Duration::zero();
  }

}
