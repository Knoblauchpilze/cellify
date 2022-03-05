
# include "Element.hh"
# include "Grid.hh"

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

    m_deleted(false)
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
      info.grid,
      m_deleted,
      Animats()
    };
    m_brain->init(i);

    // Persist the information.
    m_pos = i.pos;
    m_deleted = i.selfDestruct;
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
      info.grid,
      m_deleted,
      Animats()
    };
    m_brain->step(i);

    // Persist the information.
    m_pos = i.pos;
    m_deleted = i.selfDestruct;

    // And also copy the spawned elements.
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
  Element::pause(const utils::TimeStamp& /*t*/) {}

  void
  Element::resume(const utils::TimeStamp& /*t*/) {}

}
