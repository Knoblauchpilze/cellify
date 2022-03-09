#ifndef    LOCATOR_HH
# define   LOCATOR_HH

# include <maths_utils/Point2.hh>

namespace cellify {

  class Locator {
    public:

      /**
       * @brief - Interface method allowing to determine whether
       *          the input position is occupied or not. In most
       *          cases we can distinguish between 'solid' and
       *          'non solid' elements (typically entities). It
       *          is provided as an hint to the method to define
       *          whether the obstruction should consider these
       *          non solid objects or not.
       * @param p - the coordinates to check for obstruction.
       * @param includeNonSolid - `true` in case the obstruction
       *                          should also include entities
       *                          and other non-solid elements.
       * @return - `true` if the location is occupied.
       */
      virtual bool
      obstructed(const utils::Point2i& p,
                 bool includeNonSolid = false) const noexcept = 0;

      /**
       * @brief - Interface method allowing to fetch all the items
       *          that are within a certain radius of a position.
       *          The resulting list of items is expected as a set
       *          of identifiers that can be accessed through the
       *          companion method of this interface.
       * @param p - the position to consider.
       * @param d - the radius around the position.
       * @return - a list of the visible objects.
       */
      virtual std::vector<int>
      visible(const utils::Point2i& p,
              float d) const noexcept = 0;

      /**
       * @brief - Fetch the element at the specified index. The
       *          concrete type of the element return is left to
       *          the calling method so that we can have a generic
       *          package to handle the AStar.
       * @param id - the index of the element to fetch.
       * @return - the element or null in case it doesn't exist.
       */
      virtual const void*
      get(unsigned id) const noexcept = 0;
  };

}

#endif    /* LOCATOR_HH */
