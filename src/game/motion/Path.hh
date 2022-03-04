#ifndef    PATH_HH
# define   PATH_HH

# include <deque>
# include <maths_utils/Point2.hh>
# include <core_utils/CoreObject.hh>

namespace cellify {

  /// @brief - Convenience define for a vector of points.
  using Points = std::deque<utils::Point2i>;

  /// @brief - A convenience structure to define a path as a list
  /// of point.
  class Path: public utils::CoreObject {
    public:

      /**
       * @brief - Generate an empty path.
       */
      Path() noexcept;

      /**
       * @brief - Generate an empty path starting at the specified
       *          location.
       * @param start - the starting point of the path.
       */
      Path(const utils::Point2i& start) noexcept;

      /**
       * @brief - Add a control point to the path.
       * @param p - the position to add.
       * @param duplicate - `true` in case it's okay to have more
       *                    than once a position in the path.
       */
      void
      add(const utils::Point2i& p, bool duplicate) noexcept;

      /**
       * @brief - Return the point at the specified index. Raise
       *          an exception in case the index is out of bounds.
       * @return - the point at the input index.
       */
      const utils::Point2i&
      operator[](unsigned id) const;

      /**
       * @brief - Clear the path: will end up with an empty path.
       */
      void
      clear() noexcept;

      /**
       * @brief - Reverse the path, making the end the starting
       *          point and vice-versa.
       */
      void
      reverse() noexcept;

      /**
       * @brief - Returns the beginning of the path. In case the
       *          path is emtpy an error is raised.
       * @return - the starting point of the path.
       */
      const utils::Point2i&
      begin() const;

      /**
       * @brief - Returns the ending of the path. In case the
       *          path is emtpy an error is raised.
       * @return - the ending point of the path.
       */
      const utils::Point2i&
      end() const;

      /**
       * @brief - The length of the path.
       * @return - the number of points in the path.
       */
      unsigned
      size() const noexcept;

      /**
       * @brief - Whether the path is empty.
       * @return - `true` if the path is empty.
       */
      bool
      empty() const noexcept;

      /**
       * @brief - Advance to the next step in the path. This
       *          method will remove the first element of
       *          the path and return it.
       *          In case the path is empty, an error is raised.
       * @return - the next position in the path.
       */
      utils::Point2i
      advance();

    private:

      /**
       * @brief - The list of points defining this path.
       */
      Points m_points;
  };

}

#endif    /* PATH_HH */
