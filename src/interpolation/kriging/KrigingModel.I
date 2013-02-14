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
// $Id: KrigingModel.i.cc,v 1.2 2005/08/24 18:33:37 knap2 Exp $
//
// $Log: KrigingModel.i.cc,v $
// Revision 1.2  2005/08/24 18:33:37  knap2
// Added computeTranspose() function. Initial implementation of
// KrigingModel::getError().
//
// Revision 1.1  2005/08/23 21:12:40  knap2
// Initial source.
//
//

#ifndef included_krigalg_CorrelationModel
#include "CorrelationModel.h"
#endif

#ifndef included_krigalg_RegressionModel
#include "RegressionModel.h"
#endif

#ifndef included_mtl_mtl
#define included_mtl_mtl
#include <mtl/mtl.h>
#endif

#ifndef included_cassert
#define included_cassert
#include <cassert>
#endif

#ifndef included_numeric
#define included_numeric
#include <numeric>
#endif

#ifdef DEBUG_NO_INLINE
#define inline
#endif // DEBUG_NO_INLINE

namespace MPTCOUPLER {
  namespace krigalg {
  
    //
    // interpolate
    //

    inline Value 
    KrigingModel::interpolate(const Point & point) const
    {

      //
      // firewalls
      //

      assert(_isValid == true);

      //
      //
      //

      Value interpolatedValue(1);

      //
      // obtain regression model values at point
      //

      const Matrix regressionModelValuesTemp = 
	_regressionModel.getValues(point);

      assert(regressionModelValuesTemp.ncols() == 1);

      Vector regressionModelValues(regressionModelValuesTemp.nrows());

      for (int i = 0; i < regressionModelValuesTemp.nrows(); ++i)
	regressionModelValues[i] = regressionModelValuesTemp[i][0];

      //
      // obtain correlation model values between point and points
      // stored in _pointData
      //

      std::vector<double> correlationModelValues;
      correlationModelValues.reserve(_Bu.size());

      std::vector<std::pair<Point, Value> >::const_iterator pointDataIter;
      std::vector<std::pair<Point, Value> >::const_iterator pointDataEnd =
	_pointData.end();

      for (pointDataIter  = _pointData.begin();
	   pointDataIter != pointDataEnd;
	   ++pointDataIter)
	correlationModelValues.push_back((_correlationModel.getValue((*pointDataIter).first,
								     point))[0][0]);

      //
      //
      //

      assert(regressionModelValues.size()  == _Au.size());
      assert(correlationModelValues.size() == _Bu.size());

      interpolatedValue[0] = 
	std::inner_product(regressionModelValues.begin(),
			   regressionModelValues.end(),
			   _Au.begin(),
			   0.0) +
	std::inner_product(correlationModelValues.begin(),
			   correlationModelValues.end(),
			   _Bu.begin(),
			   0.0);

      //
      //
      //

      return interpolatedValue;

    }
  
    //
    // get (estimated) interpolation error
    //

    inline Value
    KrigingModel::getError(const Point & point) const
    {

      //
      // firewalls
      //

      assert(_isValid == true);
    
      //
      // obtain correlation model values between point and points
      // stored in _pointData
      // 

      Vector r(_pointData.size());

      std::vector<std::pair<Point, Value> >::const_iterator pointDataIter;
      std::vector<std::pair<Point, Value> >::const_iterator pointDataEnd =
	_pointData.end();
      int valuesOffset = 0;

      for (pointDataIter  = _pointData.begin();
	   pointDataIter != pointDataEnd;
	   ++pointDataIter, ++valuesOffset)
	r[valuesOffset] = (_correlationModel.getValue((*pointDataIter).first,
						      point))[0][0];

      //
      // obtain regression model values at point
      //

      const Matrix regressionModelValuesArrayTemp = 
	_regressionModel.getValues(point);

      assert(regressionModelValuesArrayTemp.ncols() == 1);

      Vector regressionModelValuesArray(regressionModelValuesArrayTemp.nrows()); 
      
      for (int i = 0; i < regressionModelValuesArrayTemp.nrows(); ++i)
	regressionModelValuesArray[i] = regressionModelValuesArrayTemp[i][0];
      

      //
      // compute u
      // 

      const Matrix transposeMatrixP = transpose(_matrixP);
      const Matrix tmpMatrix = mult(transposeMatrixP, 
				    _matrixInverseR);
      Vector tmpVector = mult(tmpMatrix,
			      r);

      const Vector u(tmpVector.size());
      mtl::copy(tmpVector - regressionModelValuesArray, u);

      //
      // compute u.Inverse[Transpose[P].Inverse[R].P].u 
      // 

      tmpVector = mult(_matrixInversePRP, u);
      const double part1 = mtl::dot(u, tmpVector);

      //
      // compute r.Inverse[R].r
      //
    
      tmpVector = mult(_matrixInverseR, r);
      const double part2 = mtl::dot(r, tmpVector);

      const double errorPart = part1 - part2;

      //
      // instantiate return value
      //
    
      Value error(1);

      //
      // compute error estimate
      //

      error[0] = _sigmaSqr*(1.0 + errorPart);
     
      //
      //
      //
    
      return error;

    }

  }
}

#ifdef DEBUG_NO_INLINE
#undef inline
#endif // DEBUG_NO_INLINE


