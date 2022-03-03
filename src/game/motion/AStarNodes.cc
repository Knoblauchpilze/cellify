
# include "AStarNodes.hh"
# include "Node.hh"

namespace cellify {

  AStarNodes::AStarNodes():
    utils::CoreObject("nodes"),

    m_ancestors(),

    m_openNodes(),
    m_sorted(true)
  {
    setService("astar");
  }

  bool
  AStarNodes::stuck() const noexcept {
    return m_openNodes.empty();
  }

  bool
  AStarNodes::opened() const noexcept {
    return m_openNodes.size();
  }

  void
  AStarNodes::seed(const utils::Point2i& p, float heuristic) noexcept {
    // Clear data.
    m_openNodes.clear();
    m_ancestors.clear();

    // Register the node as an opened node.
    m_openNodes.push_back(Node(p, 0.0f, heuristic));

    // And register this node as its own ancestor.
    std::string h = hash(p);
    m_ancestors[h] = std::make_pair(h, 0.0f);
  }

  bool
  AStarNodes::explore(const Node& child,
                      const utils::Point2i& parent,
                      bool allowLog) noexcept
  {
    std::string cHash = hash(child.p());
    std::string pHash = hash(parent);

    Ancestors::iterator anc = m_ancestors.find(cHash);
    bool exist = (anc != m_ancestors.end());

    // In case the node doesn't exist, we always register
    // it as it's the first time that we can reach it.
    if (!exist) {
      m_ancestors[cHash] = std::make_pair(pHash, child.cost());
      m_openNodes.push_back(child);
      m_sorted = false;
    }
    // Otherwise the new association should have a better
    // cost than the currently registered one to be used
    // as the new best link.
    else if (child.cost() < anc->second.second) {
      anc->second = std::make_pair(pHash, child.cost());

      if (allowLog) {
        log(
          "Updating " + std::to_string(child.p().x()) + "x" + std::to_string(child.p().y()) +
          " from (c " + std::to_string(anc->second.second) +
          " parent: " + anc->first + ")" +
          " to (c: " + std::to_string(child.cost()) +
          " parent is " + pHash + ")",
          utils::Level::Verbose
        );
      }
    }

    return exist;
  }

  Node
  AStarNodes::pickBest(bool pop) noexcept {
    // Sort the nodes if needed.
    if (!m_sorted) {
      auto cmp = [](const Node& lhs, const Node& rhs) {
        return lhs.cost() + lhs.heuristic() < rhs.cost() + rhs.heuristic();
      };

      std::sort(m_openNodes.begin(), m_openNodes.end(), cmp);

      m_sorted = true;
    }


    Node best = m_openNodes.front();

    if (pop) {
      m_openNodes.pop_front();
    }

    return best;
  }

  Path
  AStarNodes::reconstruct(const utils::Point2i& end, bool allowLog) const {
    Path out(end);

    // Start from the end position and continue until we
    // don't have any parents for the node anymore.
    std::string h = hash(end);

    Ancestors::const_iterator it = m_ancestors.find(h);
    bool foundRoot = false;

    while (it != m_ancestors.cend() && !foundRoot) {
      // Unhash the current position.
      utils::Point2i p = unhash(h);

      if (allowLog) {
        log(
          "Registering point " + std::to_string(p.x()) + "x" + std::to_string(p.y()) +
          " with hash " + h +
          ", parent is " + it->second.first,
          utils::Level::Verbose
        );
      }

      // Register this as part of the path.
      out.add(p, false);

      // Move to the parents if any.
      foundRoot = (h == it->second.first);
      h = it->second.first;
      it = m_ancestors.find(h);
    }

    return out;
  }

}
