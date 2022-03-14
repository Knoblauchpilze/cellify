#ifndef    ANT_HH
# define   ANT_HH

# include <memory>
# include <core_utils/Uuid.hh>
# include "AI.hh"
# include "Time.hh"
# include "Element.hh"
# include "Pheromon.hh"

namespace cellify {

  /// @brief - The potential mode for the ant. Depends on whether
  /// some food was already found or not.
  enum class Behavior {
    Wander,
    Food,
    Return,
    Deposit
  };

  /**
   * @brief - Generate a human readable string for a behavior.
   * @param b - the behavior.
   * @return - a string representing this behavior.
   */
  std::string
  behaviorToString(const Behavior& b) noexcept;

  class Ant: public AI {
    public:

      /**
       * @brief - Creates a new ant.
       * @param uuid - the identifier of the body of the ant.
       */
      Ant(const utils::Uuid& uuid);

      /**
       * @brief - Return the current behavior for the ant.
       * @return - the behavior for this ant.
       */
      Behavior
      mode() const noexcept;

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

      /**
       * @brief - Used to perform the spawning of a pheromon by
       *          this ant, at the current position.
       * @param info - the info to use to spawn the pheromon.
       */
      void
      spawnPheromon(Info& info) noexcept;

      /**
       * @brief - Handle the wandering behavior.
       * @param info - the info to use to perform the behavior.
       * @param items - the items that are visible to the ant.
       */
      void
      wander(Info& info, const std::vector<int>& items);

      /**
       * @brief - Handle the go to food behavior.
       * @param info - the info to use to perform the behavior.
       * @param items - the items that are visible to the ant.
       */
      void
      food(Info& info, const std::vector<int>& items);

      /**
       * @brief - Handle the return to home behavior.
       * @param info - the info to use to perform the behavior.
       * @param items - the items that are visible to the ant.
       */
      void
      returnHome(Info& info, const std::vector<int>& items);

      /**
       * @brief - Handle the deposit behavior.
       * @param info - the info to use to perform the behavior.
       * @param items - the items that are visible to the ant.
       */
      void
      deposit(Info& info, const std::vector<int>& items);

      /**
       * @brief - Find the closest target of the input type and
       *          return its position.
       * @param info - the info to find a target of the input type.
       * @param items - the list of items from which some of them
       *                might be of the input type.
       * @param tile - the type of target to find.
       * @param out - the output position of the target. Should
       *              be ignored if the return value is `false`.
       * @return - `true` in case a target of the input kind has
       *           been found.
       */
      bool
      findClosest(Info& info,
                  const std::vector<int>& items,
                  const Tile& tile,
                  utils::Point2i& out) const noexcept;

      /**
       * @brief - Used to aggregate a path from the pheromons
       *          with a type matching the input one.
       * @param info - the info to aggregate pheromons.
       * @param items - the list of items from which some of
       *                them are pheromons.
       * @param scent - the scent of the pheromon to aggregate.
       * @param out - the output position aggregating from the
       *              available pheromons. Should be ignored if
       *              the return value is `false`.
       * @return - `true` if at least one pheromon could be
       *           found (and so the position is valid).
       */
      bool
      aggregatePheromomns(Info& info,
                          const std::vector<int>& items,
                          const Scent& scent,
                          utils::Point2i& out) const noexcept;

    private:

      /// @brief - Convenience representation of an optional target.
      using OptTarget = std::shared_ptr<utils::Point2i>;

      /**
       * @brief - The beahvior currently active for the ant.
       */
      Behavior m_behavior;

      /**
       * @brief - The last time a pheromon was emitted by this
       *          entity.
       */
      TimeStamp m_lastPheromon;

      /**
       * @brief - The current target of the ant. Might be empty
       *          in which case we have to generate a new random
       *          one.
       */
      OptTarget m_target;

      /**
       * @brief - The last position of the ant. Allows to compute
       *          some sort of 'forward' direction.
       */
      utils::Point2i m_lastPos;
  };

  using AntShPtr = std::shared_ptr<Ant>;
}

#endif    /* ANT_HH */
