
# include "Path.hh"

namespace cellify {

  Path::Path() noexcept:
    utils::CoreObject("path"),

    m_points()
  {
    setService("astar");
  }

  Path::Path(const utils::Point2i& start) noexcept:
    utils::CoreObject("path"),

    m_points()
  {
    setService("astar");

    add(start, false);
  }

  void
  Path::add(const utils::Point2i& p, bool duplicate) noexcept {
    if (m_points.empty()) {
      m_points.push_back(p);
      return;
    }

    const utils::Point2i& l = m_points.back();
    if (duplicate && l == p) {
      return;
    }

    m_points.push_back(p);
  }

  const utils::Point2i&
  Path::operator[](unsigned id) const {
    if (id > m_points.size()) {
      error(
        "Failed to access point at " + std::to_string(id),
        "Path only defines " + std::to_string(m_points.size()) + " value(s)"
      );
    }

    return m_points[id];
  }

  void
  Path::clear() noexcept {
    m_points.clear();
  }

  void
  Path::reverse() noexcept {
    std::reverse(m_points.begin(), m_points.end());
  }

  const utils::Point2i&
  Path::begin() const {
    if (m_points.empty()) {
      error(
        "Failed to get starting point of the path",
        "Path is empty"
      );
    }

    return m_points.front();
  }

  const utils::Point2i&
  Path::end() const {
    if (m_points.empty()) {
      error(
        "Failed to get ending point of the path",
        "Path is empty"
      );
    }

    return m_points.back();
  }

  unsigned
  Path::length() const noexcept {
    return m_points.size();
  }

}
