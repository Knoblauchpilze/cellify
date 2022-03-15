
# include "AI.hh"

namespace cellify {

  AI::AI(const std::string& name,
         const utils::Uuid& uuid) noexcept:
    utils::CoreObject(name),

    m_uuid(uuid)
  {
    setService("ai");

    // Generate valid uuid if needed.
    if (!m_uuid.valid()) {
      m_uuid = utils::Uuid::create();
    }
  }

  const utils::Uuid&
  AI::uuid() const noexcept {
    return m_uuid;
  }

  void
  AI::merge(const AI& rhs) {
    error("Unsupported merge operation with " + rhs.getName());
  }

}
