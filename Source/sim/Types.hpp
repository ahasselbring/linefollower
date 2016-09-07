#pragma once

#include <cmath>

class Point2D {
public:
  /**
   * @brief Point2D constructs a point from two coordinates
   * @param x the x coordinate of the point
   * @param y the y coordinate of the point
   */
  Point2D(const float x = 0, const float y = 0) :
    x(x),
    y(y)
  {
  }
  /// the x coordinate of the point
  float x;
  /// the y coordinate of the point
  float y;
};

class Pose2D {
public:
  /**
   * @brief Pose2D constructs a pose from a point and a heading
   * @param position the position of the pose
   * @param heading the heading of the pose
   */
  Pose2D(const Point2D& position = Point2D(), const float heading = 0) :
    position(position),
    heading(heading)
  {
  }
  /**
   * @brief Pose2D constructs a pose from two coordinates and a heading
   * @param x the x coordinate of the position
   * @param y the y coordinate of the position
   * @param heading the heading of the pose
   */
  Pose2D(const float x, const float y = 0, const float heading = 0) :
    position(x, y),
    heading(heading)
  {
  }
  /**
   * @brief operator* transforms a point from the pose-relative coordinate system to the global coordinate system
   * @param other a point in pose-relative coordinates
   * @return the transformed point
   */
  Point2D operator*(const Point2D& other)
  {
    const float c = std::cos(heading), s = std::sin(heading);
    return Point2D(c * other.x - s * other.y + position.x, s * other.x + c * other.y + position.y);
  }
  /// the position of the pose
  Point2D position;
  /// the heading of the pose
  float heading;
};

class Line2D {
public:
  /**
   * @brief Line2D constructs a line from two points
   * @param p1 the first point defining the line
   * @param p2 the second point defining the line
   */
  Line2D(const Point2D& p1 = Point2D(), const Point2D& p2 = Point2D()) :
    p1(p1),
    p2(p2)
  {
  }
  /**
   * @brief Line2D constructs a line from four coordinates (representing two points)
   * @param x1 the x coordinate of the first point
   * @param y1 the y coordinate of the first point
   * @param x2 the x coordinate of the second point
   * @param y2 the y coordinate of the second point
   */
  Line2D(const float x1, const float y1 = 0, const float x2 = 0, const float y2 = 0) :
    p1(x1, y1),
    p2(x2, y2)
  {
  }
  /**
   * @brief distance_to_point computes the distance between the line segment from p1 to p2 to another point
   * @param point an aribitrary point
   * @return the distance between the line segment to a point
   */
  float distance_to_point(const Point2D& point) const
  {
    // The vector pointing from p1 to p2.
    const Point2D wv(p2.x - p1.x, p2.y - p1.y);
    // The vector pointing from p1 to point.
    const Point2D pv(point.x - p1.x, point.y - p1.y);
    // The square of the distance between p1 and p2.
    const float l2 = wv.x * wv.x + wv.y * wv.y;
    if (l2 == 0) {
      // This is the case if p1 == p2.
      return std::sqrt(pv.x * pv.x + pv.y * pv.y);
    }
    float t = (pv.x * wv.x + pv.y * wv.y) / l2;
    if (t < 0) {
      t = 0;
    } else if (t > 1) {
      t = 1;
    }
    // x is the projection of point onto the line from p1 to p2, but clipped to the ends of the line.
    const Point2D x(p1.x + t * wv.x, p1.y + t * wv.y);
    // px is the vector from x (on the line) to point.
    const Point2D px(point.x - x.x, point.y - x.y);
    // The result is the length of px.
    return std::sqrt(px.x * px.x + px.y * px.y);
  }
  /// the first point defining the line
  Point2D p1;
  /// the second point defining the line
  Point2D p2;
};
