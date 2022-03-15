#ifndef    AI_HH
# define   AI_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include <core_utils/Uuid.hh>
# include <maths_utils/Point2.hh>
# include <core_utils/RNG.hh>
# include <core_utils/TimeUtils.hh>
# include "Info.hh"

namespace cellify {

  /// @brief - Forward declaration of an influence.
  class Influence;

  /// @brief - Forward declaration of an element.
  class Element;

  class AI: public utils::CoreObject {
    public:

      /**
       * @brief - The identifier of this AI.
       * @return - the identifier of the AI.
       */
      const utils::Uuid&
      uuid() const noexcept;

      /**
       * @brief - Interface method caled before the first
       *          execution of this agent with the info
       *          on the step at the moment of the spawn.
       *          It allows to initialize some values.
       * @param info - the info of the step.
       */
      virtual void
      init(Info& info) = 0;

      /**
       * @brief - Interface method allowing an agent to evolve
       *          based on its surroundings. We use a struct
       *          gathering the current state of the world to
       *          take decisions based on it.
       *          Inheriting classes can subclass this method
       *          to provide custom behavior.
       * @param info - the information to get data on the step.
       */
      virtual void
      step(Info& info) = 0;

      /**
       * @brief - Interface method allowing to merge the content
       *          of an AI with another. Depending on the concrete
       *          type it can mean various things.
       *          The default implementation is to raise an error.
       * @param rhs - the AI to merge.
       */
      virtual void
      merge(const AI& rhs);

      /**
       * @brief - Handles the application of an influence on the
       *          AI's properties.
       * @param influence - the influence to process.
       * @param body - the body of the agent.
       * @return - `true` if the influence could be processed.
       */
      virtual bool
      influence(const Influence* inf,
                const Element* body) noexcept = 0;

    protected:

      /**
       * @brief - Build a new AI with the specified name.
       * @param name - the name of the AI.
       * @param uuid - the identifier of the AI. Will be
       *               generated automatically in case it
       *               is invalid.
       */
      AI(const std::string& name,
         const utils::Uuid& uuid = utils::Uuid()) noexcept;

    private:

      /**
       * @brief - The identifier of the AI.
       */
      utils::Uuid m_uuid;
  };

  using AIShPtr = std::shared_ptr<AI>;
}

#endif    /* AI_HH */
