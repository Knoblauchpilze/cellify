
# include "Node.hh"
# include <maths_utils/LocationUtils.hh>

namespace cellify {

  Node::Node(const utils::Point2i& p,
             float c,
             float h) noexcept:
    m_p(p),

    m_c(c),
    m_h(h)
  {}

  const utils::Point2i&
  Node::p() const noexcept {
    return m_p;
  }

  float
  Node::cost() const noexcept {
    return m_c;
  }

  float
  Node::heuristic() const noexcept {
    return m_h;
  }

  bool
  Node::contains(const utils::Point2i& m) const noexcept {
    return m_p.x() == m.x() && m_p.y() == m.y();
  }

  Nodes
  Node::generateNeighbors(const utils::Point2i& target) const noexcept {
    std::vector<Node> neighbors(Count);

    utils::Point2i np;

    int x = m_p.x();
    int y = m_p.y();

    // Right neighbor.
    np.x() = x + 1; np.y() = y;
    neighbors[East] = Node(
      np,
      m_c + utils::d(m_p, np),
      utils::d(np, target)
    );

    // Up neighbor.
    np.x() = x; np.y() = y + 1;
    neighbors[North] = Node(
      np,
      m_c + utils::d(m_p, np),
      utils::d(np, target)
    );

    // Left neighbor.
    np.x() = x - 1; np.y() = y;
    neighbors[West] = Node(
      np,
      m_c + utils::d(m_p, np),
      utils::d(np, target)
    );

    // Down neighbor.
    np.x() = x; np.y() = y - 1;
    neighbors[South] = Node(
      np,
      m_c + utils::d(m_p, np),
      utils::d(np, target)
    );

    return neighbors;
  }

  std::string
  hash(const utils::Point2i& p) noexcept {
    // We first tried to use the Cantor pairing function but as
    // usual the fact that the generated keys are usually quite
    // large was not really suited to our needs: as we can have
    // large coordinates in large worlds the hashing should stay
    // bounded.
    // The string approach (by basically just concatenating the
    // two coordinates) has the advantage to work no matter the
    // dimensions of the world.
    return std::to_string(p.x()) + "#" + std::to_string(p.y());
  }

  utils::Point2i
  unhash(const std::string& hash) noexcept {
    // Deconcatenate the input hash and offset
    // with half a cell the produced coords.
    std::size_t id = hash.find('#');

    if (id == std::string::npos) {
      // This is very weird, assume a default
      // node.
      return utils::Point2i(0, 0);
    }

    std::string xStr = hash.substr(0, id);
    std::string yStr = hash.substr(id + 1);

    int x = std::stof(xStr);
    int y = std::stof(yStr);

    return utils::Point2i(x, y);
  }

}
