#ifndef    PHEROMON_HH
# define   PHEROMON_HH

# include <memory>
# include "AI.hh"

namespace cellify {

  /// @brief - The possible types for a pheromon.
  enum class Scent {
    Home,
    Food
  };

  class Pheromon: public AI {
    public:

      /**
       * @brief - Creates a new pheromon with the specified amount.
       * @param scent - the type for this pheromon.
       * @param amount - the amount of pheromon initially.
       * @param evaporation - the evaporation rate in unit per second.
       */
      Pheromon(const Scent& scent,
               float amount,
               float evaporation);

      /**
       * @brief - Return the type of the pheromon.
       * @return - the kind of the pheromon.
       */
      Scent
      kind() const noexcept;

      /**
       * @brief - Implementation of the initialization method.
       * @param info - the info of the step.
       */
      void
      init(Info& info) override;

      /**
       * @brief - Implementation of the step method.
       * @param info - the information to get data on the step.
       */
      void
      step(Info& info) override;

      /**
       * @brief - Implementation of the interface method allowing
       *          to merge the content of two pheromons. In this
       *          case it means adding the amounts.
       * @param rhs - the pheromon to merge.
       */
      void
      merge(const AI& rhs) override;

    private:

      /**
       * @brief - The type of the pheromon.
       */
      Scent m_scent;

      /**
       * @brief - The amount of pheromon available.
       */
      float m_amount;

      /**
       * @brief - The evaporation of the pheromon in units per
       *          second.
       */
      float m_evaporation;
  };

  using PheromonShPtr = std::shared_ptr<Pheromon>;
}

#endif    /* PHEROMON_HH */
