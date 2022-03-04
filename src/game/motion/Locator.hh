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
  };

}

#endif    /* LOCATOR_HH */
