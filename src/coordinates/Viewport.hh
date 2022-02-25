#ifndef    VIEWPORT_HH
# define   VIEWPORT_HH

# include "olcEngine.hh"

namespace pge {

  /**
   * @brief - Defines a viewport from its top left
   *          corner and the associated dimensions.
   */
  struct Viewport {
    // `p` defines the origin of the viewport: it
    // represents the top left corner of the view
    // window.
    olc::vf2d p;

    // `dims` represents the dimensions of the view
    // window along each axis.
    olc::vf2d dims;
  };

  /**
   * @brief - Defines a new viewport with the specified
   *          origin and dimensions.
   * @param origin - the top left corner of the viewport.
   * @param size - the dimensions of the viewport.
   * @return - the created viewport.
   */
  Viewport
  newViewport(const olc::vf2d& origin,
              const olc::vf2d& size) noexcept;

}

#endif    /* VIEWPORT_HH */
