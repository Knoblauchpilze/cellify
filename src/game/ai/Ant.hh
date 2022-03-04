#ifndef    ANT_HH
# define   ANT_HH

# include "AI.hh"
# include "Path.hh"
# include "AStar.hh"

namespace cellify {

  class Ant: public AI {
    public:

      /**
       * @brief - Creates a new ant.
       */
      Ant();

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
       * @brief - Generate a new path to a random location. It
       *          overrides any existing path.
       * @param info - data to generate the path.
       * @return - whether a path was generated.
       */
      bool
      generatePath(Info& info);

    private:

      /**
       * @brief - The current path followed by the ant.
       */
      Path m_path;

      /**
       * @brief - The last time the agent moved.
       */
      utils::TimeStamp m_last;
  };

}

#endif    /* ANT_HH */
