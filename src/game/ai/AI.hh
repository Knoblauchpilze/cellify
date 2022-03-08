#ifndef    AI_HH
# define   AI_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include <maths_utils/Point2.hh>
# include <core_utils/RNG.hh>
# include <core_utils/TimeUtils.hh>
# include "Info.hh"

namespace cellify {

  class AI: public utils::CoreObject {
    public:

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

    protected:

      /**
       * @brief - Build a new AI with the specified name.
       */
      AI(const std::string& name) noexcept;
  };

  using AIShPtr = std::shared_ptr<AI>;
}

#endif    /* AI_HH */
