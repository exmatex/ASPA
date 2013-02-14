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

#include <algorithm>

#ifndef included_mtl_mtl_h
#define included_mtl_mtl_h
#include <mtl/mtl.h>
#endif

#ifndef included_mtl_blas_interface_h
#define included_mtl_blas_interface_h
#include <mtl/blas_interface.h>
#endif 

#ifdef DEBUG_NO_INLINE
#define inline
#endif // DEBUG_NO_INLINE

namespace MPTCOUPLER {
  namespace krigalg {

    //
    // construction
    //

    inline
    Vector::Vector()
    {

      return;

    }

    inline
    Vector::Vector(int n)
      : mtl::dense1D<double>(n)
    {

      return;

    }
  
    inline
    Vector::Vector(int n, double init)
      : mtl::dense1D<double>(n, init)
    {

      return;

    }

    inline
    Vector::Vector(int n, const double * value)
      : mtl::dense1D<double>(n) 
    {

      std::copy(&(value[0]),
		&(value[n]),
		&((*this)[0]));

      return;

    }

    //
    // addition
    //

    inline Vector & 
    operator+=(Vector       & vector1,
	       const Vector & vector2)
    {

      mtl::add(vector1,
	       vector2,
	       vector1);

      return vector1;

    }
  

    inline Vector 
    operator+(const Vector & vector1,
	      const Vector & vector2)
    {

      Vector result(vector1.size());
      mtl::copy(vector1, result);
      return result += vector2;
	     
    }

    //
    // subtraction
    //

    inline Vector &
    operator-=(Vector       & vector1,
	       const Vector & vector2)
    {

      mtl::add(vector1,
	       mtl::scaled(vector2, -1.0),
	       vector1);
    
      return vector1;

    }

    inline Vector
    operator-(const Vector & vector1,
	      const Vector & vector2)
    {

      Vector result(vector1.size());
      mtl::copy(vector1, result);
      return result -= vector2;

    }
     
     //
     // scalar product
     //

     inline Vector &
     operator*=(Vector & vector1,
		double alpha)
     {
	
	mtl::scale(vector1, alpha);
	return vector1;

     }

     inline Vector
     operator*(const Vector & vector1,
	       double alpha)
     {
	
	Vector result(vector1.size());
	mtl::copy(vector1, result);
	mtl::scale(result, alpha);
	return result;

     }

    //
    // compute dot product of two vectors
    //

    inline double
    dot(const Vector & vector1,
	const Vector & vector2)
    {

      //
      // firewalls
      //

      assert(vector1.size() == vector2.size());
      
      //
      // setup parameters
      //

      int n = static_cast<int>(vector1.size());
      double * x = const_cast<double *>(&(vector1[0]));
      int incx = 1;
      double * y = const_cast<double *>(&(vector2[0]));
      int incy = 1;

      //
      //
      //

      return ddot_(&n,
		   x,
		   &incx,
		   y,
		   &incy);
      
    }

  }
}


