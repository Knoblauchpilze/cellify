#ifndef    ELEMENT_HH
# define   ELEMENT_HH

# include <vector>
# include <memory>
# include <core_utils/CoreObject.hh>
# include <core_utils/Uuid.hh>
# include <maths_utils/Point2.hh>
# include "Tiles.hh"
# include "StepInfo.hh"
# include "AI.hh"

namespace cellify {

  class Element: public utils::CoreObject {
    public:

      /**
       * @brief - Creates a new tile.
       * @param t - the type of the tile.
       * @param pos - the position of the element.
       * @param brain - the brain attached to this element.
       * @param data - the specific data for this element.
       * @param uuid - the identifier for this element.
       */
      Element(const Tile& t,
              const utils::Point2i& pos,
              AIShPtr brain = nullptr,
              const std::vector<char>& data = std::vector<char>(),
              const utils::Uuid& uuid = utils::Uuid());

      /**
       * @brief - Return the type of the element.
       * @return - the type of the element.
       */
      const Tile&
      type() const noexcept;

      /**
       * @brief - Whether this element has specific data.
       * @return - `true` if the element has specific data.
       */
      bool
      hasData() const noexcept;

      /**
       * @brief - Get the specific data for this element. It
       *          corresponds to a segment of data that is
       *          dependent on the type of the element. It
       *          can be interpreted based on the type and
       *          provide more information about the actual
       *          type of an element.
       * @return - the constant data segment for this element.
       */
      const char*
      data() const noexcept;

      /**
       * @brief - The position of the element.
       * @return - the position of the element.
       */
      const utils::Point2i&
      pos() const noexcept;

      /**
       * @brief - Defines whether this element has been marked
       *          for deletion.
       * @return - `true` if the element has been deleted.
       */
      bool
      tobeDeleted() const noexcept;

      /**
       * @brief - Assign a new brain to the element. Note
       *          that the brain can be null which means
       *          that the element won't move anymore.
       * @param brain - the new brain for this element.
       */
      void
      plug(AIShPtr brain) noexcept;

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
      pause(const TimeStamp& t);

      /**
       * @brief - Opposite operation to the `pause`, this method
       *          is called to indicate that internal processes
       *          for this world element can be resumed.
       * @param t - the timestamp at which the resume occur.
       */
      virtual void
      resume(const TimeStamp& t);

    private:

      /// @brief - A convenience define for the specific data
      /// segement attached to this element.
      using SpecificData = std::vector<char>;

      /**
       * @brief - The identifier for this element.
       */
      utils::Uuid m_uuid;

      /**
       * @brief - The type of the element.
       */
      Tile m_tile;

      /**
       * @brief - The specific data for this element.
       */
      SpecificData m_data;

      /**
       * @brief - The position of the element.
       */
      utils::Point2i m_pos;

      /**
       * @brief - The brain of this element.
       */
      AIShPtr m_brain;

      /**
       * @brief - Defines that this element has been marked
       *          for deletion.
       */
      bool m_deleted;

      /**
       * @brief - The current path followed by the element.
       */
      Path m_path;

      /**
       * @brief - The last time the element moved.
       */
      TimeStamp m_last;

      /**
       * @brief - The elapsed time compared to the last time
       *          the element moved at the moment of the pause.
       */
      Duration m_elapsedSinceLast;
  };

  using ElementShPtr = std::shared_ptr<Element>;
}

#endif    /* ELEMENT_HH */
