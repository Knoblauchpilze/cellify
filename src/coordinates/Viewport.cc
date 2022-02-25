
# include "Viewport.hh"

namespace pge {

  Viewport
  newViewport(const olc::vf2d& origin,
              const olc::vf2d& size) noexcept
  {
    return Viewport{origin, size};
  }

}
