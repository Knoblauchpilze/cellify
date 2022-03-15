
# include "FoodInteraction.hh"
# include "Element.hh"

namespace cellify {

  FoodInteraction::FoodInteraction(Element* deposit,
                                   float amount,
                                   Element* carrier):
    Influence(deposit, carrier),

    m_amount(amount)
  {}

  bool
  FoodInteraction::apply() const noexcept {
    if (!m_emitter->influence(this)) {
      return false;
    }

    return m_receiver->influence(this);
  }

  float
  FoodInteraction::amount(const Element* item) const noexcept {
    // The emitter should get minus the amount as we
    // take from it.
    if (item == m_emitter) {
      return -m_amount;
    }

    // The receiver should receive the amount of food.
    if (item == m_receiver) {
      return m_amount;
    }

    // No food for unknown element.
    return 0.0f;
  }

}
