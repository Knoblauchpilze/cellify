#ifndef    ASTAR_NODES_HH
# define   ASTAR_NODES_HH

# include <string>
# include <deque>
# include <unordered_map>
# include <core_utils/CoreObject.hh>
# include "Node.hh"
# include "Path.hh"

namespace cellify {

  /// @brief - A convenience structure to manage the list
  /// of nodes explored by the AStar.
  class AStarNodes: public utils::CoreObject {
    public:

      /**
       * @brief - Generate a new empty set of nodes.
       */
      AStarNodes();

      /**
       * @brief - Whether there are no more nodes to be explored.
       * @return - `true` in case there are no more nodes to be
       *           explored.
       */
      bool
      stuck() const noexcept;

      /**
       * @brief - The amount of opened nodes to explore still.
       * @return - the amount of open nodes.
       */
      bool
      opened() const noexcept;

      /**
       * @brief - Seed the nodes with a single open node with no
       *          parent.
       * @param p - the position of the node.
       * @param heuristic - the heuristic for the initial node.
       */
      void
      seed(const utils::Point2i& p, float heuristic) noexcept;

      /**
       * @brief - Explore the child node from its parent. We will be
       *          updating internal values based on the cost of the
       *          exploration and update the existing links.
       * @param child - the child node which is currently explored.
       * @param parent - the parent of the child node.
       * @param allowLog - whether or not logs are allowed.
       * @return - `true` if the relation was updated.
       */
      bool
      explore(const Node& child,
              const utils::Point2i& parent,
              bool allowLog) noexcept;

      /**
       * @brief - Picks the best node based on the list of explorable
       *          ones. The node is then removed from the list of nodes
       *          available if needed.
       * @param pop - `true` if the best node should be popped from the
       *              list or not.
       * @return - the best node to explore.
       */
      Node
      pickBest(bool pop) noexcept;

      /**
       * @brief - Reconstruct the list of nodes that were traversed
       *          to reach the input location.
       * @param end - the end location to which the path should be
       *              reconstructed.
       * @param allowLog - whether or not the logs are allowed.
       * @return - the reconstructed path.
       */
      Path
      reconstruct(const utils::Point2i& end, bool allowLog) const;

    private:

      /// @brief - A convenience definition allowing to keep track
      /// of a node and a cost.
      using Link = std::pair<std::string, float>;

      /// @brief - An association map where each element has as a key
      /// the child hash and as a value the hash of the parent which
      /// allowed to reach the child along with the cost to go from
      // the parent to the child.
      using Ancestors = std::unordered_map<std::string, Link>;

      /// @brief - A list of nodes where one can add and remove from
      /// both ends.
      using OpenNodes = std::deque<Node>;

      /**
       * @brief - The ancestors table explored by the algorithm.
       */
      Ancestors m_ancestors;

      /**
       * @brief - The list of nodes that are currently open to be
       *          explored by the algorithm.
       */
      OpenNodes m_openNodes;

      /**
       * @brief - Whether or not the open nodes have been sorted by
       *          relevance already or not.
       */
      bool m_sorted;
  };

}

#endif    /* ASTAR_NODES_HH */
