#ifndef    INFLUENCE_HH
# define   INFLUENCE_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include "StepInfo.hh"

namespace cellify {

  /// @brief - Forward declaration of the AI class.
  class Element;

  class Influence: public utils::CoreObject {
    public:

      /**
       * @brief - Creates a new influence.
       * @param emitter - the brain that emitted the influence.
       * @param receiver - the brain that receives the influence.
       */
      Influence(Element* emitter,
                Element* receiver);

      /**
       * @brief - Interface method allowing to perfirm the
       *          application of the influence in the world.
       *          This method will make the required changes
       *          in the emitter and receiver.
       * @return - whether or not applying the influence was
       *           a success.
       */
      virtual bool
      apply() const noexcept = 0;

    protected:

      /**
       * @brief - The emitter of the influence.
       */
      Element* m_emitter;

      /**
       * @brief - The receiver of the influence.
       */
      Element* m_receiver;
  };

  using InfluenceShPtr = std::shared_ptr<Influence>;
}

#endif    /* INFLUENCE_HH */
