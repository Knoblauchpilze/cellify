
# include "Element.hh"

namespace cellify {

  Element::Element(const Tile& t,
                   const utils::Point2i& pos):
    utils::CoreObject(tileToString(t)),

    m_tile(t),
    m_pos(pos)
  {
    setService("world");
  }

  const Tile&
  Element::type() const noexcept {
    return m_tile;
  }

  const utils::Point2i&
  Element::pos() const noexcept {
    return m_pos;
  }

  void
  Element::init(StepInfo& /*info*/) {}

  void
  Element::step(StepInfo& /*info*/) {}

  void
  Element::pause(const utils::TimeStamp& /*t*/) {}

  void
  Element::resume(const utils::TimeStamp& /*t*/) {}

}
