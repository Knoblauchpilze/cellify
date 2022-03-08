#ifndef    PHEROMON_HH
# define   PHEROMON_HH

# include "AI.hh"

namespace cellify {

  class Pheromon: public AI {
    public:

      /**
       * @brief - Creates a new pheromon with the specified amount.
       * @param amount - the amount of pheromon initially.
       * @param evaporation - the evaporation rate in unit per second.
       */
      Pheromon(float amount,
               float evaporation);

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

    private:

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

}

#endif    /* PHEROMON_HH */
