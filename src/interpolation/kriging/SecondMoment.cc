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
// File:        SecondMoment.cc
// Package:     MPTCOUPLER kriging algorithm
// 
// 
// 
// Description: Implement operations related to computing tensor of inertia
// Description: for a collection of Points. The tensor is computed wrt the 
// Description: center of mass.
//

#include "SecondMoment.h"

#include <mtl/mtl.h>
#include <mtl/mtl2lapack.h>

#include <cassert>
#include <complex>

//
//
//

#ifdef DEBUG_NO_INLINE
#include "SecondMoment.I"
#endif // DEBUG_NO_INLINE

namespace MPTCOUPLER {
    namespace krigalg {

  //
  //
  //

  namespace {

    //
    // compute the center of mass
    //

    Point 
    computeCenterOfMass(const std::vector<Point> & points,
			int                        spaceDimension)
    {
      
      //
      // instantiate
      //

      Point centerMass(spaceDimension, 0.0);
      
      //
      // iterate over all points
      //
      
      std::vector<Point>::const_iterator pointsIter;
      std::vector<Point>::const_iterator pointsEnd = points.end();
      
      for (pointsIter  = points.begin();
	   pointsIter != pointsEnd;
	   ++pointsIter) {

	//
	// get Point handle
	//

	const Point & point = *pointsIter;

	//
	// accumulate 
	//

	centerMass += static_cast<Vector>(point);

      }

      //
      // scale
      //

      mtl::scale(centerMass, 
		 1.0/points.size());
      
      //
      //
      //

      return centerMass;

    }

    //
    // compute inetrtia tensor wrt to the coordinate system axis
    // passing through the center of mass
    //

    Matrix
    computeSecondMoment(const Point              & centerMass,
			const std::vector<Point> & points,
			int                        spaceDimension)
    {

      //
      // instantiate
      //

      Matrix intertiaTensor(spaceDimension,
			    spaceDimension);

      //
      // iterate over intertiaTensor entries
      //

      for (int i = 0; i < spaceDimension; ++i)
	for (int j = i; j < spaceDimension; ++j) {
	
	  // 
	  // initialize
	  //

	  intertiaTensor[i][j] = 0.0;

	  //
	  // fill in 
	  //
	  
	  std::vector<Point>::const_iterator pointsIter;
	  std::vector<Point>::const_iterator pointsEnd = points.end();
	  
	  for (pointsIter  = points.begin();
	       pointsIter != pointsEnd;
	       ++pointsIter) {
	    
	    //
	    // get Point handle
	    //
	    
	    const Point & point = *pointsIter;

	    //
	    // subtract center of mass
	    //

	    const Vector relativeDistance = point - centerMass;

	    //
	    // add contribution
	    //

	    intertiaTensor[i][j] += relativeDistance[i]*
	      relativeDistance[j];

// 	    if (i != j)
// 	      intertiaTensor[i][j] -= relativeDistance[i]*
// 		relativeDistance[j];
// 	    else
// 	      intertiaTensor[i][i] += mtl::dot(relativeDistance,
// 					       relativeDistance) - 
// 		relativeDistance[i]*relativeDistance[i];

	  }
	  
	  //	    
	  // symmetric entry
	  //
	  
	  intertiaTensor[j][i] = intertiaTensor[i][j];
	  
	}
      
      //
      //
      //

      return intertiaTensor;

    }


  }

  //
  // construction/destruction
  //

  SecondMoment::SecondMoment(const std::vector<Point> & points)
  {

    //
    // firewalls
    //
    
    assert(points.empty() == false);

    //
    // get the space dimension from the first point
    //

    const int spaceDimension = points.front().size();

    //
    // compute the center of mass
    //

    _centerMass = computeCenterOfMass(points,
				      spaceDimension);

    //
    // compute the moment of inertia
    //

    _data = computeSecondMoment(_centerMass,
				 points,
				 spaceDimension);

    //
    //
    //

    return;

  }
  
  SecondMoment::~SecondMoment()
  {

    return;

  }

  //
  // compute the eigensystem; eigenvalues are stored in Vector; the
  // eigenvector corresponding to i-th eigenvalue is storred as the
  // i-th row of Matrix.
  //

  std::pair<Vector, Matrix>
  SecondMoment::computeEigenSystem() const
  {

    //
    // get the size of _data
    //

    const int size = _data.nrows();
    
    //
    // copy _data into lapack_matrix
    //

    mtl2lapack::lapack_matrix<double>::type lapackData(size,
						       size);

    mtl::copy(_data,
	      lapackData);

    //
    // instantiate output constainers
    //

    mtl::dense1D<std::complex<double> > eigenValues(size);
    Matrix                              eigenVectors(size,
						     size);

    //
    // call geev
    //

    const int geevError = mtl2lapack::geev(mtl2lapack::GEEV_CALC_RIGHT,
					   lapackData,
					   eigenValues,
					   eigenVectors,
					   eigenVectors);
    
    assert(geevError == 0);

    //
    // copy eigenvalues
    //

    Vector outputEigenValues(size);
    
    for (int i = 0; i < size; ++i)
      outputEigenValues[i] = eigenValues[i].real();

    //
    // copy eigenvectors (must be real)
    //

    Matrix outputEigenVectors(size,
			      size);

    mtl::copy(eigenVectors,
	      outputEigenVectors);

    //
    //
    //

    return std::make_pair(outputEigenValues,
			  outputEigenVectors);

  }
  
  //
  // output
  //

  std::ostream & 
  operator<<(std::ostream       & outputStream,
	     const SecondMoment & secondMoment)
  {


    //
    // center of mass
    //

    outputStream << "Center of mass: " << std::endl << 
      secondMoment._centerMass << std::endl;

    //
    // inertia tensor
    //

    outputStream << "Inertia tensor: " << std::endl << secondMoment._data;

    //
    //
    //

    return outputStream;

  }

}
}


