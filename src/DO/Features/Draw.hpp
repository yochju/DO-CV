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

//! @file

#ifndef DO_FEATURES_DRAW_HPP
#define DO_FEATURES_DRAW_HPP

namespace DO {

  /*!
    \ingroup Features
    @{
  */

  void drawOERegions(const std::vector<OERegion>& features, const Color3ub& c,
                     float scale = 1.f, const Point2f& off = Point2f::Zero());

  //! @}

} /* namespace DO */

#endif /* DO_AFFINECOVARIANTFEATURES_DRAW_H */
