// ========================================================================== //
// This file is part of DO++, a basic set of libraries in C++ for computer
// vision.
//
// Copyright (C) 2014 David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //

#pragma once

#include <DO/Core/EigenExtension.hpp>

namespace DO { namespace P2 {

    typedef Vector3d Line, Point;
    typedef Point2d Point2;
    typedef Vector2d Vector2;

    inline Point2 euclidean(const Point& p)
    { return (p/p(2)).head(2); }

    inline Point homogeneous(const Point2& p)
    { return Point(p.x(), p.y(), 1.); }

    inline Point intersection(const Line& l1, const Line& l2)
    { return l1.cross(l2); }

    inline Line line(const Point& p, const Point& q)
    { return p.cross(q); }

    inline Line line(const Point2&a, const Point2& b)
    { return line(homogeneous(a), homogeneous(b)); }

    inline double dist(const Point& p, const Line& l)
    { return std::abs((p/p(2)).dot(l))/l.head(2).norm(); }

} /* namespace P2 */
} /* namespace DO */