
# include "AI.hh"

namespace cellify {

  AI::AI(const std::string& name) noexcept:
    utils::CoreObject(name)
  {
    setService("ai");
  }

}
