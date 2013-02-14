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
// File:        KrigingModel.cc
// Package:     MPTCOUPLER kriging algorithm
// 
// 
// 
// Description: Class implementing kriging interpolation.
//
// $Id: KrigingModel.cc,v 1.2 2005/08/24 18:31:37 knap2 Exp $
//
// $Log: KrigingModel.cc,v $
// Revision 1.2  2005/08/24 18:31:37  knap2
// Added function to compute sigmaSqr.
//
// Revision 1.1  2005/08/23 21:12:40  knap2
// Initial source.
//
//

#include "KrigingModel.h"

#include <mtl/dense1D.h>

#include <cassert>
#include <iostream>

//
//
//

#ifdef DEBUG_NO_INLINE
#include "KrigingModel.I"
#endif // DEBUG_NO_INLINE

namespace MPTCOUPLER {
    namespace krigalg {      
  
  //
  //
  //

  namespace {

    template<typename T>
    T ** 
    allocateArray(size_t n,
		  size_t m)
    {

      T ** array = new T*[n];
      array[0]   = new T[n*m];

      for (int i = 1; i < n; ++i)
	array[i] = array[i-1] + m;
      
      return array;

    }

    template<typename T>
    void
    deallocateArray(T ** array)
    {

      delete [] array[0];
      delete [] array;

      return;

    }	   

    //
    // compute values of RegressionModel at points
    //

    Matrix
    evaluateRegressionModelAtPoints(const std::vector<std::pair<Point, Value> >  & _pointData,
				    const RegressionModel                        & _regressionModel)
    {
      
      //
      // firewalls
      //

      assert(_pointData.empty() == false);

      //
      // get the number of points
      //

      const std::vector<std::pair<Point, Value> >::size_type numberPoints = 
	_pointData.size();

      //
      // get the size of RegressionModel
      //

      const Dimension regressionModelDimensionTemp = 
	_regressionModel.getDimension(_pointData.front().first);

      assert(regressionModelDimensionTemp.second() == 1);

      const int regressionModelDimension = 
	regressionModelDimensionTemp.first();

      //
      // instantiate array
      //


      Matrix P(numberPoints,
	       regressionModelDimension);

      //
      // iterate over points
      //

      std::vector<std::pair<Point, Value> >::const_iterator 
	pointsDataIter;
      std::vector<std::pair<Point, Value> >::const_iterator 
	pointsDataBegin = _pointData.begin();
      std::vector<std::pair<Point, Value> >::const_iterator 
	pointsDataEnd = _pointData.end();
      int pointOffset =0;
      

      for (pointsDataIter  = pointsDataBegin; 
	   pointsDataIter != pointsDataEnd;
	   ++pointsDataIter, ++pointOffset) {

	//
	// get values at the point
	//

	const Matrix valuesTemp = 
	  _regressionModel.getValues((*pointsDataIter).first);
	
	assert(valuesTemp.ncols() == 1);

	Vector values(valuesTemp.nrows());

	for (int i = 0; i < valuesTemp.nrows(); ++i)
	  values[i] = valuesTemp[i][0];

	//
	// copy values into array P
	//
	
	std::copy(values.begin(),
		  values.end(),
		  &(P[pointOffset][0]));

      }
      
      return P;

    }

    //
    // compute values of CorrelationModel at points
    //

    Matrix
    evaluateCorrelationModelAtPoints(const std::vector<std::pair<Point, Value> >  & _pointData,
				     const CorrelationModel                       & _correlationModel)
    {

      //
      // firewalls
      //

      assert(_pointData.empty() == false);

      //
      // get the number of points
      //

      const std::vector<std::pair<Point, Value> >::size_type numberPoints = 
	_pointData.size();

      //
      // instantiate R array
      //

      Matrix R(numberPoints,
	       numberPoints);

      //
      // iterate through R
      //

      for (int i = 0; i < numberPoints; ++i)
	for (int j = 0; j < numberPoints; ++j)
	  R[i][j] = (_correlationModel.getValue(_pointData[i].first,
						_pointData[j].first))[0][0];

      //
      //
      //

      return R;

    }

    //
    // multiply Matrix and point values
    //

    Vector 
    multiplyArrayAndPointValues(const Matrix & A,
				const std::vector<std::pair<Point, Value> > & _pointData)
    {

      //
      // firewalls
      //

      assert(A.minor() == _pointData.size());

      //
      // reload values into Vector
      //

      Vector values(_pointData.size());
      std::vector<std::pair<Point, Value> >::const_iterator pointDataIter;
      std::vector<std::pair<Point, Value> >::const_iterator pointDataEnd = 
	_pointData.end();
      int valuesOffset = 0;

      for (pointDataIter  = _pointData.begin();
	   pointDataIter != pointDataEnd;
	   ++pointDataIter, ++valuesOffset)
	values[valuesOffset] = (*pointDataIter).second[0];

      //
      // get A values
      //

      Vector result(A.major());
      mtl::mult(A,
		values,
		result);

      //
      //
      //

      return result;

    }

    //
    // compute sigma^2
    //

    double 
    computeSigmaSqr(const std::vector<std::pair<Point, Value> > & _pointData,
		    const Matrix                                & P,
		    const Vector                                & _Au,
		    const Matrix                                & inverseR)
    {

      //
      // reload values into an array
      //

      Vector values(_pointData.size());
      
      std::vector<std::pair<Point, Value> >::const_iterator pointDataIter;
      std::vector<std::pair<Point, Value> >::const_iterator pointDataEnd = 
	_pointData.end();
      int valuesOffset = 0;

      for (pointDataIter  = _pointData.begin();
	   pointDataIter != pointDataEnd;
	   ++pointDataIter, ++valuesOffset) 
	values[valuesOffset] = (*pointDataIter).second[0];
      
      //
      // compute values - P._Au
      //

      Vector valuesDiffrence(values);
      Vector tmpVector(values.size());

      mtl::mult(P, _Au, tmpVector);
      valuesDiffrence -= tmpVector;

      //
      // compute sigmaSqr
      //

      mtl::mult(inverseR, valuesDiffrence, tmpVector);
      const double sigmaSqr = mtl::dot(valuesDiffrence, tmpVector);

      return sigmaSqr/_pointData.size();

    }
		    
  }

  //
  // construction.destruction
  //

  KrigingModel::KrigingModel(const RegressionModel  & regressionModel,
			     const CorrelationModel & correlationModel)
    : _isValid(false),
      _regressionModel(regressionModel),
      _correlationModel(correlationModel)
  {

    return;

  }
  
  KrigingModel::~KrigingModel()
  {

    return;

  }

  //
  // add point/value pair to the model
  //

  void
  KrigingModel::addPoint(const Point & point,
			 const Value & value)
  {

    _pointData.push_back(std::make_pair(point, value));
    return;

  }
    
  //
  // build the model using accumulated points
  //
  
  void 
  KrigingModel::build()
  {

    //
    // compute values of RegressionModel at points
    //

    _matrixP = evaluateRegressionModelAtPoints(_pointData,
					       _regressionModel);

    //
    // compute values of CorrelationModel at points
    //

    Matrix R =
      evaluateCorrelationModelAtPoints(_pointData,
				       _correlationModel);

    //
    // compute the inverse of R
    //

    std::pair<Matrix, bool> inverseData = inverse(R);

    assert(inverseData.second == true);
    _matrixInverseR = inverseData.first;

    //
    // compute P^T
    //

    const Matrix transposeP = transpose(_matrixP);

    //
    // compute P^T R^(-1) P
    //

    Matrix transposePinverseRP = 
      mult(transposeP, 
	   mult(_matrixInverseR, 
		_matrixP));

    //
    // compute (P^T R^(-1) P)^-1
    //

    inverseData = inverse(transposePinverseRP);
    
    assert(inverseData.second == true);
    _matrixInversePRP = inverseData.first;

    //
    // compute A
    //

    const Matrix A = mult(_matrixInversePRP,
			  mult(transposeP,
			       _matrixInverseR));

    //
    // compute B
    //

    const Matrix PA = mult(_matrixP,
			   A);

    const Matrix B = mult(_matrixInverseR, 
			  identity(_pointData.size()) - PA);

    //
    // compute A u
    //

    _Au = multiplyArrayAndPointValues(A,
				      _pointData);

    //
    // compute B u
    //
    
    _Bu = multiplyArrayAndPointValues(B,
				      _pointData);

    //
    // compute sigma^2
    //

    _sigmaSqr = computeSigmaSqr(_pointData,
				_matrixP,
				_Au,
				_matrixInverseR);

    //
    // set valid flag for the model
    //

    _isValid = true;

    return;

  }

  //
  // output
  //

  std::ostream & 
  operator<<(std::ostream       & outputStream,
	     const KrigingModel & krigingModel)
  {
    
    outputStream << "_isValid: " << krigingModel._isValid << std::endl;
    
    std::vector<std::pair<Point, Value> >::const_iterator pointDataIter;
    std::vector<std::pair<Point, Value> >::const_iterator pointDataEnd = 
      krigingModel._pointData.end();
    
    outputStream << "Point data: " << std::endl;

    for (pointDataIter  = krigingModel._pointData.begin();
	 pointDataIter != krigingModel._pointData.end();
	 ++pointDataIter)
      outputStream << (*pointDataIter).first  << " " 
		   << (*pointDataIter).second << std::endl;

    outputStream << "A.u: ";
    std::copy(krigingModel._Au.begin(),
	      krigingModel._Au.end(),
	      std::ostream_iterator<double>(outputStream, " "));
    outputStream << std::endl;

    outputStream << "B.u: ";
    std::copy(krigingModel._Bu.begin(),
	      krigingModel._Bu.end(),
	      std::ostream_iterator<double>(outputStream, " "));
    outputStream << std::endl;

    return outputStream;

  }

}
}


