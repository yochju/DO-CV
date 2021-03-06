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


#ifndef DO_FEATURES_FEATURE_HPP
#define DO_FEATURES_FEATURE_HPP

#include <stdexcept>

namespace DO {

  /*!
    \ingroup Features
    @{
  */

  //! Abstract 'VisualFeature' class.
  class VisualFeature {
  public:
    VisualFeature() {}
    virtual ~VisualFeature() {}
    virtual std::ostream& print(std::ostream& os) const = 0;
    virtual std::istream& read(std::istream& in) = 0;
    friend std::ostream& operator<<(std::ostream& out, const VisualFeature& f)
    { return f.print(out); }
    friend std::istream& operator<<(std::istream& in, VisualFeature& f)
    { return f.read(in); }
  };

  //! PointFeature for interest points
  class InterestPoint : public VisualFeature
  {
  public:
    //! ID for each point feature type.
    enum Type { Harris, HarAff, HarLap, FAST, SUSAN,
                DoG, LoG, DoH, MSER, HesAff, HesLap };
    enum ExtremumType { Min = -1, Saddle = 0,  Max = 1 };
    //! Constructors.
    InterestPoint() : VisualFeature() {}
    InterestPoint(const Point2f& coords) : coords_(coords) {}
    //! Destructor.
    virtual ~InterestPoint() {}
    //! Constant getters.
    float x() const { return coords_(0); }
    float y() const { return coords_(1); }
    const Point2f& coords() const { return coords_; }
    const Point2f& center() const { return coords(); }
    Type type() const { return type_; }
    ExtremumType extremumType() const { return extremum_type_; }
    float extremumValue() const { return extremum_value_; }
    //! Mutable getters.
    float& x() { return coords_(0); }
    float& y() { return coords_(1); }
    Point2f& coords() { return coords_; }
    Point2f& center() { return coords(); }
    Type& type() { return type_; }
    ExtremumType& extremumType() { return extremum_type_; }
    float& extremumValue() { return extremum_value_; }
    //! Equality operator.
    bool operator==(const InterestPoint& f) const
    { return coords() == f.coords(); }
    //! Drawing.
    void draw(const Color3ub& c, float scale = 1.f,
              const Point2f& offset = Point2f::Zero()) const;
    //! I/O.
    std::ostream& print(std::ostream& os) const;
    std::istream& read(std::istream& in);
  private:
    Point2f coords_;
    Type type_;
    ExtremumType extremum_type_;
    float extremum_value_;
  };

  /*!
    The 'OERegion' class stands for 'Oriented Elliptic Region' and is 
    dedicated to store important geometric features such as:
    - DoG
    - Harris-Affine
    - Hessian-Affine features and so on...
   */
  class OERegion : public InterestPoint
  {
  public:
    //! Default constructor
    OERegion() : InterestPoint() {}
    //! Constructor for circular region.
    OERegion(const Point2f& coords, float scale)
      : InterestPoint(coords)
      , shape_matrix_(Matrix2f::Identity()*(pow(scale,-2))) {}
    //! Destructor.
    virtual ~OERegion() {}
    //! Constant/mutable shape matrix getters.
    //! The shape matrix is the matrix $M$ that describes the ellipse 
    //! $\varepsilon$, i.e.: 
    //! $$ \varepsilon = \{ x \in R^2 : (x-c)^T M (x-c) = 1 \} $$
    //! where $c$ is the center of the region.
    const Matrix2f& shapeMat() const { return shape_matrix_; }
    Matrix2f& shapeMat() { return shape_matrix_; }
    //! Constant/mutable orientation (in radian) getters.
    //! This completely determines the affine transformation that transforms the
    //! unit circle to the elliptic shape of the region.
    float orientation() const { return orientation_; }
    float& orientation() { return orientation_; }
    //! Returns the anisotropic radius at a given angle in radians.
    float radius(float radian = 0.f) const;
    //! Returns the anisotropic scale at a given angle in radians.
    float scale(float radian = 0.f) const { return radius(radian); }
    //! Get the affine transform $A$ that transforms the unit circle to that 
    //! oriented ellipse of the region.
    //! We compute $A$ from a QR decomposition and by observing
    //! $M = (A^{-1})^T A^{-1}$ where $M$ is the shape matrix.
    Matrix3f affinity() const;
    //! Equality operator.
    bool operator==(const OERegion& f) const
    {
      return (coords() == f.coords() &&
              shapeMat() == f.shapeMat() &&
              orientation() == f.orientation() &&
              type() == f.type());
    };
    //! Drawing.
    void draw(const Color3ub& c, float scale = 1.f,
              const Point2f& offset = Point2f::Zero()) const;
    //! I/O
    std::ostream& print(std::ostream& os) const;
    std::istream& read(std::istream& in);

  private:
    Matrix2f shape_matrix_;
    float orientation_;
  };

  //! @file
}

#endif /* DO_FEATURES_FEATURE_HPP */
