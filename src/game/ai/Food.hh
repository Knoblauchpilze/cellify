#ifndef    FOOD_HH
# define   FOOD_HH

# include <memory>
# include "AI.hh"

namespace cellify {

  class Food: public AI {
    public:

      /**
       * @brief - Creates a new food deposit with the specified
       *          amount of food available.
       * @param amount - the amount of food available.
       */
      Food(float amount);

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
       * @brief - Implementation of the interface method to apply
       *          an influence to this food deposit.
       * @param influence - the influence to process.
       * @param body - the deposit.
       * @return - `true` if the influence could be processed.
       */
      bool
      influence(const Influence* inf,
                const Element* body) noexcept override;

    private:

      /**
       * @brief - The amount of food still available.
       */
      float m_stock;
  };

  using FoodShPtr = std::shared_ptr<Food>;
}

#endif    /* FOOD_HH */
