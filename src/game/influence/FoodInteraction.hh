#ifndef    FOOD_INTERACTION_HH
# define   FOOD_INTERACTION_HH

# include "Influence.hh"

namespace cellify {

  class FoodInteraction: public Influence {
    public:

      /**
       * @brief - Creates a new food interaction influence.
       * @param deposit - the deposit from which the food
       *                  needs to be taken from.
       * @param amount - the amount of food to take from the
       *                 deposit.
       * @param carrier - the carrier of the food to take
       *                  from the deposit.
       */
      FoodInteraction(Element* deposit,
                      float amount,
                      Element* carrier);

      /**
       * @brief - Interface method allowing to perfirm the
       *          application of the influence in the world.
       *          This method will make the required changes
       *          in the emitter and receiver.
       * @return - whether or not applying the influence was
       *           a success.
       */
      bool
      apply() const noexcept override;

      /**
       * @brief - Returns the amount of food that should be
       *          considered by the input element. We use
       *          this method in order to provide the amount
       *          based on whether the element is a receiver
       *          or an emitter.
       * @param elem - the element for which the amount of
       *               food should be retrieved. In case the
       *               element is not the emitter nor the
       *               receiver, `0` is returned.
       * @return - the amount of food or `0`.
       */
      float
      amount(const Element* item) const noexcept;

    private:

      /**
       * @brief - The amount of food to withdraw.
       */
      float m_amount;
  };

  using InfluenceShPtr = std::shared_ptr<Influence>;
}

#endif    /* FOOD_INTERACTION_HH */
