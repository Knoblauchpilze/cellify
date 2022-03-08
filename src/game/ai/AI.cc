
# include "AI.hh"

namespace cellify {

  AI::AI(const std::string& name) noexcept:
    utils::CoreObject(name)
  {
    setService("ai");
  }

  void
  AI::merge(const AI& rhs) {
    error("Unsupported merge operation with " + rhs.getName());
  }

}
