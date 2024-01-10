
# include "Food.hh"
# include <cxxabi.h>
# include "FoodInteraction.hh"

namespace cellify {

  Food::Food(float amount):
    AI("food"),

    m_stock(amount)
  {}

  void
  Food::init(Info& /*info*/) {}

  void
  Food::step(Info& info) {
    // In case we don't have a valid stock, we have
    // to self-destruct.
    if (m_stock <= 0.0f) {
      this->info("Deposit " + info.pos.toString() + " is now empty");
      info.selfDestruct = true;
    }
  }

  bool
  Food::influence(const Influence* inf,
                  const Element* body) noexcept
  {
    // In case the influence is not a food interaction,
    // we can't process it.
    const FoodInteraction* fi = dynamic_cast<const FoodInteraction*>(inf);
    if (fi == nullptr) {
      int status;
      std::string it = abi::__cxa_demangle(typeid(*inf).name(), 0, 0, &status);

      error(
        "Failed to process influence",
        "Unsupported influence with kind " + it
      );
    }

    float a = fi->amount(body);
    if (a > 0.0f) {
      info("Piled up " + std::to_string(a) + " food (" + std::to_string(m_stock) + " available)");
    }
    else {
      info(
        "Withdrew " + std::to_string(-a) + " food (" + std::to_string(m_stock + a) +
        " remaining)"
      );
    }

    m_stock += a;

    return true;
  }

}

