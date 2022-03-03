#ifndef    ELEMENT_HH
# define   ELEMENT_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include <maths_utils/Point2.hh>
# include "Tiles.hh"
# include "StepInfo.hh"

namespace cellify {

  class Element: public utils::CoreObject {
    public:

      /**
       * @brief - Creates a new tile.
       * @param t - the type of the tile.
       * @param pos - the position of the element.
       */
      Element(const Tile& t,
              const utils::Point2i& pos);

      /**
       * @brief - Return the type of the element.
       * @return - the type of the element.
       */
      const Tile&
      type() const noexcept;

      /**
       * @brief - The position of the element.
       * @return - the position of the element.
       */
      const utils::Point2i&
      pos() const noexcept;

      /**
       * @brief - Interface method caled before the first
       *          execution of this element with the info
       *          on the step at the moment of the spawn.
       *          It allows to initialize some values.
       * @param info - the info of the step.
       */
      virtual void
      init(StepInfo& info);

      /**
       * @brief - Interface method allowing for a world element
       *          to evolve based on its surroundings. We use a
       *          struct gathering the current state of the
       *          world to take decisions based on it.
       *          Inheriting classes can subclass this method
       *          to provide custom behavior.
       * @param info - the information to get data on the step.
       */
      virtual void
      step(StepInfo& info);

      /**
       * @brief - Interface method allowing to pause this world
       *          element: all internal behavior that depend on
       *          time should be stopped so that they can be
       *          later resumed.
       * @param t - the timestamp at which the pause occur.
       */
      virtual void
      pause(const utils::TimeStamp& t);

      /**
       * @brief - Opposite operation to the `pause`, this method
       *          is called to indicate that internal processes
       *          for this world element can be resumed.
       * @param t - the timestamp at which the resume occur.
       */
      virtual void
      resume(const utils::TimeStamp& t);

    private:

      /**
       * @brief - The type of the element.
       */
      Tile m_tile;

      /**
       * @brief - The position of the element.
       */
      utils::Point2i m_pos;
  };

  using ElementShPtr = std::shared_ptr<Element>;
}

#endif    /* ELEMENT_HH */
