
# include "Pheromon.hh"

namespace cellify {

  Pheromon::Pheromon(float amount,
                     float evaporation):
    AI("pheromon"),

    m_amount(amount),
    m_evaporation(evaporation)
  {}

  void
  Pheromon::init(Info& /*info*/) {}

  void
  Pheromon::step(Info& info) {
    // Decrease the amount.
    m_amount -= (m_evaporation * info.elapsed);

    // Self-destruct if needed.
    if (m_amount <= 0.0f) {
      info.selfDestruct = true;
    }
  }

}
