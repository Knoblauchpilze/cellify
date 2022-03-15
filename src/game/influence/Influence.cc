
# include "Influence.hh"

namespace cellify {

  Influence::Influence(Element* emitter,
                       Element* receiver):
    utils::CoreObject("influence"),

    m_emitter(emitter),
    m_receiver(receiver)
  {
    setService("world");

    if (m_emitter == nullptr) {
      error(
        "Failed to create influence",
        "Invalid null emitter"
      );
    }

    if (m_receiver == nullptr) {
      error(
        "Failed to create influence",
        "Invalid null receiver"
      );
    }
  }

}
