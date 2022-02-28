
# include <Grid.hh>

namespace cellify {

  Grid::Grid():
    utils::CoreObject("grid"),

    m_min(),
    m_max(),

    m_cells()
  {
    setService("cellify");

    initialize();
  }

  utils::Point2i
  Grid::min() const noexcept {
    return m_min;
  }

  utils::Point2i
  Grid::max() const noexcept {
    return m_max;
  }

  void
  Grid::initialize() noexcept {
    /// TODO: Handle this.
    warn("Should generate grid");
  }

}
