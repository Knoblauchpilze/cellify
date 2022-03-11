
# include "Pheromon.hh"

namespace cellify {

  Pheromon::Pheromon(const Scent& scent,
                     const TimeStamp& created,
                     float amount,
                     float evaporation):
    AI("pheromon"),

    m_scent(scent),

    m_created(created),

    m_amount(amount),
    m_evaporation(evaporation)
  {}

  Scent
  Pheromon::kind() const noexcept {
    return m_scent;
  }

  const TimeStamp&
  Pheromon::created() const noexcept {
    return m_created;
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

  void
  Pheromon::merge(const AI& rhs) {
    // Consistency check: the cast will prevent merging
    // invalid types.
    const Pheromon& p = dynamic_cast<const Pheromon&>(rhs);

    m_amount += p.m_amount;
    // Avergare the evaporation rate.
    m_evaporation = 0.5f * (m_evaporation + p.m_evaporation);
  }

}
