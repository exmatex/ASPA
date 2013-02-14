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
// File:        GaussianDerivativeCorrelationModel.cc
// Package:     MPTCOUPLER kriging algorithm
// 
// 
// 
// Description: Class implementing gaussian correlation model with derivatives.
//
// $Id: GaussianDerivativeCorrelationModel.cc,v 1.1 2005/09/01 16:14:19 knap2 Exp $
//
// $Log: GaussianDerivativeCorrelationModel.cc,v $
// Revision 1.1  2005/09/01 16:14:19  knap2
// Initial source.
//
//

#include "GaussianDerivativeCorrelationModel.h"

#include <mtl/mtl.h>

#include <cassert>
#include <cmath>
 
//
//
//

namespace MPTCOUPLER {
  namespace krigalg {
    
  //
  // construction/destruction
  //
    
    GaussianDerivativeCorrelationModel::GaussianDerivativeCorrelationModel(const std::vector<double> & thetas)
      : DerivativeCorrelationModel(thetas)
  {

    return;

  }

  GaussianDerivativeCorrelationModel::~GaussianDerivativeCorrelationModel()
  {

    return;

  }

  //
  // get value of the correlation function between two points.
  // 
  // Z(s)={Y, D_1 Y, D_2 Y, ..., D_n Y}(s)
  //

  Matrix 
  GaussianDerivativeCorrelationModel::getValue(const Point & firstPoint,
					       const Point & secondPoint) const
  {

    //
    // compute distance between firstPoint and secondPoint
    //

    const Vector distance = firstPoint - secondPoint;


    //
    // compute L2-norm of the firstPoint-secondPoint distance
    //

    const double distanceNorm = mtl::two_norm(distance);

    //
    // allocate array 
    //

    const int arrayDimension = firstPoint.size() + 1;

    Matrix covarianceArray(arrayDimension,
			   arrayDimension);
    
    //
    // fill in values; for now the values are normalized wrt. the
    // gaussian correlation
    //

    const double theta    = _thetas.front();
    const double thetaSqr = theta*theta;
    const double covarianceScaling = exp(-theta*distanceNorm*
					 distanceNorm);

    //
    // correlation of function with itself
    //

    covarianceArray[0][0] = covarianceScaling;

    //
    // correlation of function and derivative
    //

    for (int i = 1; i < arrayDimension; ++i) {

      //
      // get covariance magnitude
      //

      const double covarianceValue = -2.0*theta*distance[i - 1];

      //
      // Cov[ D_{i-1}Y(firstPoint), Y(secondPoint) ]
      //
      
      covarianceArray[0][i] = -covarianceValue*covarianceScaling;

      //
      // Cov[ Y(firstPoint), D_{i-1} Y(secondPoint) ]
      //

      covarianceArray[i][0] = covarianceValue*covarianceScaling;

    }

    //
    // correlation of derivatives
    //

    for (int i = 1; i < arrayDimension; ++i) 
      for (int j = i; j < arrayDimension; ++j)
	if (i == j)	  
	  //
	  // self correlation
	  //
	  covarianceArray[i][i] = covarianceScaling*
	    (2.0*theta - 
	     4.0*thetaSqr*distance[i - 1]*distance[i - 1]);
	else
	  covarianceArray[i][j] = covarianceArray[j][i] =
	    covarianceScaling*(-4.0*thetaSqr*distance[i - 1]*distance[j - 1]);

    //
    // return covarianceArray
    //

    return covarianceArray;
    
  }


  //
  // get a string representation of the class name
  //

  std::string
  GaussianDerivativeCorrelationModel::getClassName() const
  {

    return std::string("MPTCOUPLER::krigalg::GaussianDerivativeCorrelationModel");

  }

  //
  // Database output
  //
  
  void
  GaussianDerivativeCorrelationModel::putToDatabase(toolbox::Database & db) const
  {

    //
    // store base-class data
    //
    
    DerivativeCorrelationModel::putToDatabase(db);

    //
    //
    //

    return;

  }

  //
  // Database input
  //

  void
  GaussianDerivativeCorrelationModel::getFromDatabase(toolbox::Database & db)
  {

    //
    // get base-class data
    //

    DerivativeCorrelationModel::getFromDatabase(db);

    //
    //
    //
    
    return;
    
  }

}
}


