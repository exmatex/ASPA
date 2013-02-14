/* DO-NOT-DELETE revisionify.begin() */
/*
Copyright (c) 2007-2008 Lawrence Livermore National Security LLC

This file is part of the mdef package (version 0.1) and is free software: 
you can redistribute it and/or modify it under the terms of the GNU
Lesser General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this program.  If not, see
<http://www.gnu.org/licenses/>.

                              DISCLAIMER

This work was prepared as an account of work sponsored by an agency of
the United States Government. Neither the United States Government nor
Lawrence Livermore National Security, LLC nor any of their employees,
makes any warranty, express or implied, or assumes any liability or
responsibility for the accuracy, completeness, or usefulness of any
information, apparatus, product, or process disclosed, or represents
that its use would not infringe privately-owned rights. Reference
herein to any specific commercial products, process, or service by
trade name, trademark, manufacturer or otherwise does not necessarily
constitute or imply its endorsement, recommendation, or favoring by
the United States Government or Lawrence Livermore National Security,
LLC. The views and opinions of authors expressed herein do not
necessarily state or reflect those of the United States Government or
Lawrence Livermore National Security, LLC, and shall not be used for
advertising or product endorsement purposes.
*/
/* DO-NOT-DELETE revisionify.end() */
//
// File:        Vector.I
// Package:     MPTCOUPLER kriging algorithm
// 
// 
// 
// Description: Class implementing MTL-aware vector.
//
// $Id$
//
// $Log$
//

#include <mtl/mtl.h>
#include <algorithm>

#include <cmath>

#ifdef DEBUG_NO_INLINE
#define inline
#endif // DEBUG_NO_INLINE

namespace MPTCOUPLER {
  namespace krigalg {

    inline
    Point::Point()
    {
      
      return;
      
    }
    
    inline
    Point::Point(int n)
      : Vector(n)
    {
      
      return;
      
    }
    
    inline
    Point::Point(int n, double init)
      : Vector(n, init)
    {
      
      return;
      
    }

    inline
    Point::Point(int n, const double * coordinates)
      : Vector(n, coordinates)
    {

      return;
      
    }

    //
    // addition
    //

    inline Point &
    operator+=(Point & point, const Vector & vector)
    {
      
      mtl::add(point,
	       vector,
	       point);

      return point;

    }

    inline Point
    operator+(const Point & point, const Vector & vector)
    {
    
      Point result(point);
      return result += vector;

    }
  
    inline Point
    operator+(const Vector & vector, const Point & point)
    {

      return point + vector;

    }

    //
    // subtraction
    //

    inline Vector
    operator-(const Point & point1, const Point & point2)
    {

      return static_cast<Vector>(point1) - static_cast<Vector>(point2);

    }

    //
    // distance
    //
#if 0
    inline double
    distance(const Point & point1, const Point & point2)
    {
#ifdef DEBUG_CHECK_ASSERTIONS
      assert(point1.size() == point2.size());
#endif 
      return mtl::two_norm(point1 - point2);

    }

#endif

    inline double
    distance(const Point & point1, const Point & point2)
    {
#ifdef DEBUG_CHECK_ASSERTIONS
      assert(point1.size() == point2.size());
#endif       
      Point::size_type i;
      const Point::size_type dimension = point1.size();

      double  distanceSqr = 0.0;

      for (i = 0; i < dimension; ++i) {

	const double diff = point1[i] - point2[i];
	distanceSqr += diff*diff;

      }
	
      return std::sqrt(distanceSqr);

    }

  }

}



