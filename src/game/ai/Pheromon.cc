
# include "Pheromon.hh"

namespace cellify {

  Pheromon::Pheromon(const Scent& scent,
                     float amount,
                     float evaporation):
    AI("pheromon"),

    m_scent(scent),

    m_amount(amount),
    m_evaporation(evaporation)
  {}

  Scent
  Pheromon::kind() const noexcept {
    return m_scent;
  }

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

    /// TODO: Include some spreading of the pheromons ?
  }

}
