// DO-NOT-DELETE revisionify.begin() 
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
// DO-NOT-DELETE revisionify.end() 
//
// File:        Matrix.cc
// Package:     MPTCOUPLER kriging algorithm
// 
// 
// 
// Description: Class implementing matrix.
//
// $Id$
//
// $Log$
//

#include "Matrix.h"

#include <mtl/mtl2lapack.h>

#include <cassert>

//
//
//

#ifdef DEBUG_NO_INLINE
#include "Matrix.I"
#endif // DEBUG_NO_INLINE

namespace MPTCOUPLER {
    namespace krigalg {

  //
  // compute inverse of R;
  // second argument is true of inverse succeeded
  //

  std::pair<Matrix, bool>
  inverse(const Matrix & R)
  {

    //
    // firewalls
    //

    assert(R.major() == R.minor());

    //
    //
    //

    int size = R.major();

    //
    // make a copy of R as it gets invalidated via dgesv
    //

    Matrix copyR(size,
		 size);
    
    mtl::copy(R, copyR);

    //
    // use LAPACK's DGESV; here we assume that R is NOT symmteric
    //
      
    //
    // instantiate inverse
    //

    mtl2lapack::lapack_matrix<double>::type lapackInverseR(size,
							   size);
    mtl::set_diagonal(lapackInverseR, 
		      1.0);

    //
    // instantiate pivot vector
    //
      
    mtl::dense1D<int> pivot(size, 0);
 
    //
    // call DGESV
    //

    int dgesvExitValue = mtl2lapack::gesv(copyR, 
					  pivot,
					  lapackInverseR);

    //
    // instead of:
    //    assert(dgesvExitValue == 0);
    //
    if (dgesvExitValue != 0) {

       Matrix inverseR(size,size); // make_pair does not compile with Matrix inverseR()
       return std::make_pair(inverseR,
			     false);

    }

    //
    // copy data from lapack_matrix to Matrix
    //

    Matrix inverseR(size,
		    size);
    mtl::copy(lapackInverseR, 
	      inverseR);

    //
    //
    //

    return std::make_pair(inverseR,
			  true);
      
  }

}
}


