
# include "AStar.hh"
# include <deque>
# include <iterator>
# include <maths_utils/LocationUtils.hh>
# include "Node.hh"
# include "AStarNodes.hh"

namespace {

  bool
  pathTooFar(const cellify::Path& path,
             const utils::Point2i& p,
             float d) noexcept
  {
    // In case the `radius` is negative, consider
    // that there's no limit.
    if (d < 0.0f) {
      return false;
    }

    bool bounded = false;
    unsigned id = 0u;

    while (id < path.size() && bounded) {
      bounded = (utils::d(p, path[id]) > d);
      ++id;
    }

    return bounded;
  }

}

namespace cellify {

  AStar::AStar(const utils::Point2i& s,
               const utils::Point2i& e,
               const Locator& loc):
    utils::CoreObject("algo"),

    m_start(s),
    m_end(e),

    m_loc(loc)
  {
    setService("astar");
  }

  bool
  AStar::findPath(Path& path,
                  float radius,
                  bool allowLog) const noexcept
  {
    // The code for this algorithm has been taken from the
    // below link:
    // https://en.wikipedia.org/wiki/A*_search_algorithm
    Path out;
    path.clear();

    if (allowLog) {
      log(
        "Starting a* at " + std::to_string(m_start.x()) + "x" + std::to_string(m_start.y()) +
        " to reach " + std::to_string(m_end.x()) + "x" + std::to_string(m_end.y()),
        utils::Level::Verbose
      );
    }

    // The list of nodes that are currently being explored.
    AStarNodes nodes;
    nodes.seed(m_start, utils::d(m_start, m_end));

    while (!nodes.stuck()) {
      // Fetch the best node.
      Node current = nodes.pickBest(true);

      if (allowLog) {
        log(
          "Picked node " + std::to_string(current.p().x()) + "x" + std::to_string(current.p().y()) +
          " with c " + std::to_string(current.cost()) +
          " h is " + std::to_string(current.heuristic()) +
          " (nodes: " + std::to_string(nodes.opened()) + ")",
          utils::Level::Verbose
        );
      }

      // In case we reached the end, stop the algorithm.
      if (current.contains(m_end)) {
        return reconstruct(path, nodes, radius, allowLog);
      }

      // Generate neighbors for the current node and try
      // to register each of them.
      Nodes neighbors = current.generateNeighbors(m_end);
      for (unsigned id = 0u ; id < neighbors.size() ; ++id) {
        const Node& neighbor = neighbors[id];

        // Only consider the node if it is not obstructed.
        // Note that if the node is the target, we also do
        // not consider whether it's obstructed.
        if (m_loc.obstructed(neighbor.p()) && !neighbor.contains(m_end)) {
          continue;
        }

        // Make sure that we don't consider nodes farther
        // away from the source than defined by the user.
        if (radius > 0.0f && utils::d(m_start, neighbor.p()) >= radius) {
          continue;
        }

        // Explore the neighboring node.
        nodes.explore(neighbor, current.p(), allowLog);
      }
    }

    // We couldn't reach the goal, the algorithm failed.
    return false;
  }

  bool
  AStar::reconstruct(Path& path, const AStarNodes& nodes, float radius, bool allowLog) const noexcept {
    // Reconstruct the path and reverse it as we start
    // from the end.
    path.clear();
    Path out = nodes.reconstruct(m_end, allowLog);
    out.reverse();

    // Make sure that we reached the starting point. If
    // this is the case we can copy the path we just built
    // to the output argument.
    if (out.begin() != m_start) {
      return false;
    }

    // Check whether the path goes beyong the input
    // limit at any point: if this is the case we
    // will prevent it from being returned as we do
    // not consider it valid.
    if (pathTooFar(out, m_start, radius)) {
      return false;
    }

    // Copy the path.
    std::swap(path, out);

    return true;
  }

}
