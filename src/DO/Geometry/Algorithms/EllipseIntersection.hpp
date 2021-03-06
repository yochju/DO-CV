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

#pragma once

#define _USE_MATH_DEFINES
#include <DO/Geometry/Objects/Ellipse.hpp>
#include <vector>
#include <cmath>


namespace DO {

  //! Compute the intersection union ratio approximately
  std::vector<Point2d> approxIntersection(const Ellipse& e1, const Ellipse& e2, int n);
  double approxJaccardSimilarity(const Ellipse& e1, const Ellipse& e2,
                                 int n = 36, double limit = 1e9);

  //! Check polynomial solvers.
  int computeIntersectionPoints(Point2d intersections[],
                                const Ellipse& e1,
                                const Ellipse& e2);

  /*!
    Compute the intersection union ratio exactly.
    CAUTION: Numerical issues are not totally solved. We are almost there...
    Investigation is still ongoing.
   */
  double analyticIntersection(const Ellipse& e1, const Ellipse& e2);
  double analyticJaccardSimilarity(const Ellipse& e1, const Ellipse& e2);


} /* namespace DO */