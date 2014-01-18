// ========================================================================== //
// This file is part of DO++, a basic set of libraries in C++ for computer 
// vision.
//
// Copyright (C) 2013 David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public 
// License v. 2.0. If a copy of the MPL was not distributed with this file, 
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //

#ifndef DO_GEOMETRY_BBOX_HPP
#define DO_GEOMETRY_BBOX_HPP

#include <DO/Core/EigenExtension.hpp>

namespace DO {

  class BBox
  {
    Point2d tl_, br_;
  public:
    BBox() {}
    BBox(const Point2d& topLeft, const Point2d& bottomRight)
      : tl_(topLeft), br_(bottomRight)
    {
      if ( tl_.x() > br_.x() ||
           tl_.y() > br_.y() )
      {
        const char *msg = "Top-left and bottom-right corners are wrong!";
        throw std::logic_error(msg);
      }
    }
    template <typename PointIterator>
    BBox(PointIterator begin, PointIterator end);
    BBox(const std::vector<Point2d>& points);
    
    Point2d& topLeft()     { return tl_; }
    Point2d& bottomRight() { return br_; }
    double& x1() { return  tl_.x(); }
    double& y1() { return  tl_.y(); }
    double& x2() { return  br_.x(); }
    double& y2() { return  br_.y(); }

    const Point2d& topLeft()     const { return tl_; }
    const Point2d& bottomRight() const { return br_; }
    Point2d        topRight()    const { return tl_+Point2d(width(), 0); }
    Point2d        bottomLeft()  const { return br_-Point2d(width(), 0); }

    double x1() const { return  tl_.x(); }
    double y1() const { return  tl_.y(); }
    double x2() const { return  br_.x(); }
    double y2() const { return  br_.y(); }

    double width() const  { return std::abs(br_.x() - tl_.x()); }
    double height() const { return std::abs(br_.y() - tl_.y()); }
    double area() const   { return width()*height(); }
    
    friend std::ostream& operator<<(std::ostream& os, const BBox& bbox);

    static BBox infinite() {
      BBox b;
      b.topLeft().fill(-std::numeric_limits<double>::infinity());
      b.bottomRight().fill(std::numeric_limits<double>::infinity());
      return b;
    }
    static BBox zero() {
        BBox b(Point2d::Zero(), Point2d::Zero());
        return b;
    }
  };

  // Very fast.
  BBox intersection(const BBox& bbox1, const BBox& bbox2);
  bool intersect(const BBox& bbox1, const BBox& bbox2);

  bool similar(const BBox& bbox1, const BBox& bbox2, double overlap);
  bool degenerate(const BBox& bbox, double eps = 1e-3);
  bool inside(const Point2d& p, const BBox& bbox);
  
  void drawBBox(const BBox& bbox, const Color3ub& c, double scale = 1.,
                int penWidth = 1);

} /* namespace DO */

#endif /* DO_GEOMETRY_BBOX_HPP */
