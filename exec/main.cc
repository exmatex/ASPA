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

#ifndef included_config
#include <asf_config.h>
#endif

#include <kriging_mtreedb/KrigingInterpolationDataBase.h>
#include <kriging/LinearDerivativeRegressionModel.h>
#include <kriging/GaussianDerivativeCorrelationModel.h>
#include <kriging/MultivariateDerivativeKrigingModelFactory.h>

#include <kriging/SecondMoment.h>

#include <mtl/mtl.h>
#include <mtl/utils.h>

#include <cmath>
#include <cstdlib>
#if !defined(__INTEL_COMPILER) && defined(_LARGEFILE_SOURCE)
#include <ext/stdio_filebuf.h>
#endif // _LARGEFILE_SOURCE

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>
#include <iterator>
#include <vector>

#if defined(HAVE_MPI)
#include <toolbox/parallel/MPI.h>
#endif // HAVE_MPI

//
//
//

using namespace MPTCOUPLER::krigalg;
using namespace MPTCOUPLER::krigcpl;

//
//
//

namespace {

  //
  // variables
  //

  const int pointDimension = 18;
  const int valueDimension = 11;
  const int valueDataDimension = (pointDimension + 1)*valueDimension;

  int    maxKrigingModelSize   = 4;
  int    maxNumberSearchModels = 1;
  double theta                 = 4.0e2;
  double meanErrorFactor       = 7.07106781186548;
  double tolerance             = 1.0e-4;
  double maxQueryPointModelDistance = 1.0e6;

  const double pointScaling[pointDimension] = {0.10000000e-02, 
					       0.11550000e-02,
					       0.88104231e-03,
					       0.88104231e-03,
					       0.88104231e-03,
					       0.88104231e-03,
					       0.88104231e-03,
					       0.88104231e-03,
					       0.88104231e-03,
					       0.88104231e-03,
					       0.88104231e-03,
					       0.95276326e-03,
					       0.95276326e-03,
					       0.95276326e-03,
					       0.95276326e-03,
					       0.95276326e-03,
					       0.99999998e-02,
					       0.16198347e+03};

  const double valueScaling[valueDimension] = {0.95276326e-03,
					       0.95276326e-03,
					       0.95276326e-03,
					       0.95276326e-03,
					       0.95276326e-03,
					       0.10000000e+01,
					       0.10000000e+01,
					       0.10000000e+01,
					       0.10000000e+01,
					       0.10000000e+01,
					       0.16198347e+03};
  //
  // local types
  //


  //
  // convert value data to make it ready for kriging model
  // insertion 
  //

  std::vector<Value> 
  copyValueData(const double * value,
		const double * gradient,
		int            pointDimension,
		int            valueDimension)
  {

    //
    // instatiate return object
    //

    std::vector<Value> pointValues;

    //
    // iterate over all values
    //

    for (int iValue = 0; iValue < valueDimension; ++iValue) {

      //
      // copy value data
      //

      Value pointValue(pointDimension + 1);
	  
      pointValue[0] = value[iValue];

      //
      // copy gradient data
      //

      for (int i = 0; i < pointDimension; ++i)
	pointValue[1 + i] = gradient[i*valueDimension + iValue];

      //
      // add pointValue
      //

      pointValues.push_back(pointValue);

    }

    return pointValues;

  }


  //
  // read key/data pairs from a file
  //

  template <typename T>
  void
  readKeyDataPairs(const std::string              & fileName,
		   const std::vector<std::string> & keywords,
		   std::vector<T>                 & values)
  {

    //
    // open input stream
    //

    std::ifstream inputStream(fileName.c_str());
    
    if (!inputStream) {
      
      std::perror(fileName.c_str());
      std::exit(EXIT_FAILURE);
      
    }

    //
    // prepare values
    //

    values.resize(keywords.size());
    
    //
    // allocate vector of bool to check if all keywords have been found
    //

    std::vector<bool> keywordsFound(keywords.size(),
				    false);
    
    //
    // read key/data pairs
    //

    std::vector<std::string>::size_type numberKeywords = keywords.size();

    while ( !inputStream.eof() ) {

      //
      // read key
      //

      std::string key;

      inputStream >> key;

      //
      // compare key with keywords
      //

      for (std::vector<std::string>::size_type iKey = 0;
	   iKey < numberKeywords;
	   ++iKey)
	if (key == keywords[iKey]) {
	  
	  //
	  // read data
	  //

	  inputStream >> values[iKey];

	  //
	  // mark keyword as processed
	  //

	  keywordsFound[iKey] = true;

	}

      //
      // check if we are not past the EOF mark
      //
      
      if (inputStream.eof())
	break;
      
    }
    
    //
    // check for errors
    //

    for (std::vector<bool>::size_type iKey = 0;
	 iKey < keywordsFound.size();
	 ++iKey) 
      if (keywordsFound[iKey] == false) {

	std::cerr << "Keyword " << keywords[iKey] << " "
		  << "not found in file " << fileName
		  << std::endl;

      }
    
    //
    //
    //

    return;

  }

  //
  // read input values from a file
  //
  
  void 
  readInputValues(const std::string & fileName)
  {

    //
    // prepare data for readKeyDataPairs
    //

    std::vector<std::string> keywords;
    keywords.push_back("maxKrigingModelSize");
    keywords.push_back("maxNumberSearchModels");
    keywords.push_back("theta");
    keywords.push_back("meanErrorFactor");
    keywords.push_back("tolerance");
    keywords.push_back("maxQueryPointModelDistance");

    std::vector<double> values;

    //
    // call readKeyDataPairs
    //

    readKeyDataPairs(fileName,
		     keywords,
		     values);

    //
    // reload data into variables
    //
    
    maxKrigingModelSize        = static_cast<int>(values[0]);
    maxNumberSearchModels      = static_cast<int>(values[1]);
    theta                      = values[2];
    meanErrorFactor            = values[3];
    tolerance                  = values[4];
    maxQueryPointModelDistance = values[5];

    //
    //
    //

    return;

  }

  void
  readInputValues(const std::string & fileName,
		  bool)
  {

    std::ifstream inputStream(fileName.c_str());

    if (!inputStream) {

      std::perror(fileName.c_str());
      std::exit(EXIT_FAILURE);

    }

    //
    // keywords
    //

    const char * keywords[] = {"maxKrigingModelSize",
			       "maxNumberSearchModels",
			       "theta",
			       "meanErrorFactor",
			       "tolerance",
                               "maxQueryPointModelDistance"};
    const int numberKeywords = 6;

    //
    // read key/data pairs
    //

    int numberKeyDataPairsRead = 0;

    while ( !inputStream.eof() ) {

      //
      // read key
      //

      std::string key;

      inputStream >> key;

      //
      //
      //

      if (key == keywords[0])
	inputStream >> maxKrigingModelSize;

      if (key == keywords[1])
	inputStream >> maxNumberSearchModels;

      if (key == keywords[2])
	inputStream >> theta;

      if (key == keywords[3])
	inputStream >> meanErrorFactor;
      
      if (key == keywords[4])
	inputStream >> tolerance;

      if (key == keywords[5])
	inputStream >> maxQueryPointModelDistance;

      //
      // check if we are not past the EOF mark
      //
      
      if (inputStream.eof())
	break;

      //
      // increment the number of key/data pairs read
      //

      ++numberKeyDataPairsRead;

    }
    
    
    //
    //
    //

    return;

  }

  //
  // read input data from a file
  //

  std::vector<MPTCOUPLER::krigalg::Point> 
  readPointData(const std::string & fileName,
		int                 maxNumberRecords)
  {

    static std::ifstream inputStream(fileName.c_str());
    
    if (!inputStream)
      return std::vector<MPTCOUPLER::krigalg::Point>(0);

    //
    // instantiate point container
    //

    std::vector<MPTCOUPLER::krigalg::Point> points;

    //
    // read lines until EOF
    //

    int iRecord = 0;

    while ( !inputStream.eof() && iRecord < maxNumberRecords) {

      //
      // instantiate point
      //

      MPTCOUPLER::krigalg::Point point(pointDimension);

      //
      // read and scale point coordinates
      //

      for (int i = 0; i < pointDimension; ++i) {
	
	inputStream >> point[i];
	point[i] /= pointScaling[i];

      }

      //
      // check if we are not past the EOF mark
      //
      
      if (inputStream.eof())
	break;

      //
      // store point in points
      //

      points.push_back(point);

      //
      // bump up record number
      //
      
      ++iRecord;

    }

    //
    //
    //

    return points;

  }

  //
  // read values 
  //
  
  std::vector<MPTCOUPLER::krigalg::Value> 
  readValueData(const std::string & fileName,
		int                 maxNumberRecords)
  {

    //
    // open input stream
    //

#if !defined(__INTEL_COMPILER) && defined(_LARGEFILE_SOURCE)
    
    static bool inputStreamInit = false;
    static std::ifstream inputStream;

    if (inputStreamInit == false) {

      FILE * inputStreamFile = std::fopen(fileName.c_str(),
					  "r");

      if (inputStreamFile == NULL) {
	
	std::perror(fileName.c_str());
	std::exit(EXIT_FAILURE);

      }

      //
      // needs to remain static as we take a reference
      //
      static __gnu_cxx::stdio_filebuf<char> filebuf(inputStreamFile,
						    std::ios::in);

      inputStream.std::ios::rdbuf(&filebuf);
      inputStreamInit = true;

    }

#else     
    static std::ifstream inputStream(fileName.c_str());
#endif //  _LARGEFILE_SOURCE

    if (!inputStream)
      return std::vector<MPTCOUPLER::krigalg::Value>(0);
    
    //
    // instantiate value container
    //

    std::vector<MPTCOUPLER::krigalg::Value> values;
    
    //
    // read until EOF
    //

    int iRecord = 0;

    while ( !inputStream.eof() && iRecord < maxNumberRecords) {
      
      //
      // instantiate value
      //

      MPTCOUPLER::krigalg::Value value(valueDataDimension);

      //
      // read and scale function values (1,...,valueDimension)
      //

      for (int i = 0; i < valueDimension; ++i) {

	inputStream >> value[i];
	value[i] /= valueScaling[i];

      }

      //
      // read and scale gradient values

      for (int i = valueDimension; i < valueDataDimension; ++i) {
	
	//
	// read gradient
	//

	inputStream >> value[i];

	//
	// compute the value and point ids; 
	//

	const int pointId = (i - valueDimension)/valueDimension;
	const int valueId = (i - valueDimension) - pointId*valueDimension;
	
	assert(pointId >=0 && pointId < pointDimension);
	assert(valueId >=0 && valueId < valueDimension);

	//
	// scale gradient
	//

	value[i] /= valueScaling[valueId]/pointScaling[pointId];

      }

      //
      // check if we are not past the EOF mark
      //
      
      if (inputStream.eof())
	break;

      //
      // store value in values
      //

      values.push_back(value);

      //
      // bump up record Id
      //

      ++iRecord;

    }
    
    //
    //
    //

    return values;

  }

  //
  // process query points inserting them into the kriging models if
  // the esitmated error is greater than assumed tolerance
  //

  void
  processQueryPoints(double                     tolerance,
		     const std::vector<Point> & queryPoints,
		     const std::vector<Value> & queryValues,
		     InterpolationDataBase    & interpolationDb)
  {
    
    //
    // firewalls
    //
    
    assert(queryPoints.size() == queryValues.size());

    //
    // storage for interpolated values
    //

    Value interpolatedValue(valueDimension);

    //
    // 
    //

    int hint = -1;
    std::vector<bool> interpolateFlags(InterpolationDataBase::NUMBER_FLAGS);

    //
    // iterate over all query points
    //

    for (int iPoint = 0; iPoint < queryPoints.size(); ++iPoint) {

      //
      // output 
      //
      
      std::cout << "Processing point: " << iPoint << std::endl;
      
      //
      // get a handle to a query point
      //
      
      const Point & queryPoint = queryPoints[iPoint];
      // std::cout << queryPoint << std::endl;
      //
      // get a handle to a query value
      //
      
      const Value & queryValue = queryValues[iPoint];

      assert(queryValue.size() == valueDataDimension);
      // std::cout << "Point: " << queryPoint << std::endl;
      // std::cout << "Value: " << queryValue << std::endl;
      //
      // interpolate
      //

      const bool interpolationSuccess = 
	interpolationDb.interpolate(&(interpolatedValue[0]),
				    hint,
				    &(queryPoint[0]),
				    interpolateFlags);
      // std::cout << interpolatedValue << std::endl;
      // std::cout << queryValue << std::endl;
      // std::cout << hint << std::endl;
      //
      // 
      //

      if (interpolationSuccess == false) {

	std::cout << "Adding point :" << iPoint << std::endl;
	interpolationDb.insert(hint,
			       &(queryPoint[0]),
			       &(queryValue[0]),
			       &(queryValue[valueDimension]),
			       interpolateFlags);


      }

      if (interpolationSuccess == true) {

	//
	// compute real error
	//

	for (int iValue = 0; iValue < valueDimension; ++iValue) {

	  const double realError = std::fabs(queryValue[iValue] - 
					     interpolatedValue[iValue]);
	  // std::cout << realError << std::endl;
	  if (realError > tolerance) 
	    std::cout  << "Missed point: " 
		       << "value Id " << iValue << " "
		       << "real value: " << queryValue[iValue] << " "
		       << "interp. value: " << interpolatedValue[iValue] << " "
		       << "error " 
		       << realError << " "
		       << std::endl;

	}
	
      }

    }
    
    //
    //
    //
    
    return;
    
  }

  //
  // test matrix vector multiplication
  //

  void
  testMatrixVectorMultiply()
  {
    
    MPTCOUPLER::krigalg::Matrix A(3,2);
    MPTCOUPLER::krigalg::Vector v(3);

    A[0][0] =-3; A[0][1] = 2; // A[0][2] = -2;
    A[1][0] = 8; A[1][1] = 4; // A[1][2] = -4;
    A[2][0] =-5; A[2][1] = 7; // A[2][2] =  7;

    v[0] = 2; v[1] = 3; v[2] = -5;

    std::cout << MPTCOUPLER::krigalg::mult(transpose(A), v) << std::endl;
    std::cout << MPTCOUPLER::krigalg::mult(A, v, true) << std::endl;

    return;

  }

  //
  // test matrix matrix multiplication
  //

  void
  testMatrixMatrixMultiply()
  {

    MPTCOUPLER::krigalg::Matrix A(3,2);
    MPTCOUPLER::krigalg::Matrix B(2,3);

    A[0][0] = 1; A[0][1] = 2; // A[0][2] = -2;
    A[1][0] = 3; A[1][1] = 4; // A[1][2] = -4;
    A[2][0] = 5; A[2][1] = 6; // A[2][2] =  7;

    B[0][0] =-1; B[0][1] = 6; B[0][2] = -2;
    B[1][0] =-7; B[1][1] = 4; B[1][2] = -4;
    // B[2][0] = 5; B[2][1] = 8; B[2][2] =  7;

    std::cout << MPTCOUPLER::krigalg::mult(transpose(A), transpose(B)) << std::endl;
    std::cout << MPTCOUPLER::krigalg::mult(A, B, true, true) << std::endl;

    return;

  }

  void
  testDotProduct()
  {
    
    MPTCOUPLER::krigalg::Vector x(3);
    MPTCOUPLER::krigalg::Vector y(3);
    
    x[0] = 3; x[1] = 8; x[2] = -4;
    y[0] =-2; y[1] = 6; y[2] = -2;

    std::cout << mtl::dot(x, y) << std::endl;
    std::cout << MPTCOUPLER::krigalg::dot(x,y) << std::endl;
    
    return;

  }

}

int 
main(int   ac,
     char *av[])
{
#if defined(HAVE_MPI)
  MPTCOUPLER::toolbox::MPI::init(&ac,
				 &av);
#endif // HAVE_MPI


  // testMatrixMatrixMultiply(); exit(0);
  // testMatrixVectorMultiply(); exit(0);
  // testDotProduct(); exit(0);

  std::cout << std::setprecision(17) << std::scientific << std::right
	    << std::showpos;
  //
  // check command line arguments
  //

  if (ac != 3) {

    std::cerr << "usage: " << av[0] << " <file1> <file2>" 
	      << std::endl;
    std::exit(EXIT_FAILURE);

  }

  //
  // load input file
  //

  readInputValues(av[0] + std::string(".inp"));

  //
  // parameters
  //
  
  const int    maxPointCache = 100;
  
  //
  // parameter output
  //

  std::cout << "#" << std::endl;
  std::cout << "# maxKrigingModelSize        = " << maxKrigingModelSize << std::endl;
  std::cout << "# maxNumberRecords           = " << maxNumberSearchModels << std::endl;
  std::cout << "# theta                      = " << theta << std::endl;
  std::cout << "# error ampl                 = " << meanErrorFactor << std::endl;
  std::cout << "# tolerance                  = " << tolerance << std::endl;
  std::cout << "# maxQueryPointModelDistance = " << maxQueryPointModelDistance << std::endl;
  std::cout << "#" << std::endl;

  //
  // instantiate correlation and regression models
  //

  DerivativeRegressionModelPointer 
    regressionModel(new LinearDerivativeRegressionModel);
  DerivativeCorrelationModelPointer
    correlationModel(new GaussianDerivativeCorrelationModel(std::vector<double>(1, theta)));

  MultivariateDerivativeKrigingModelFactoryPointer 
    modelFactory(new MultivariateDerivativeKrigingModelFactory(regressionModel,
							       correlationModel));

  //
  // instantiate multivariate kriging model
  //

  MultivariateDerivativeKrigingModel krigingModel(regressionModel,
						  correlationModel);

  //
  // instantiate interpolation db
  //

  KrigingInterpolationDataBase interpolationDb(pointDimension,
					       valueDimension,
					       modelFactory,
					       maxKrigingModelSize,
					       maxNumberSearchModels,
					       true,
					       meanErrorFactor,
					       tolerance,
					       maxQueryPointModelDistance,
					       600000000,
					       ".");
					       

  //
  // keep reading from files until EOF
  //
  
  int totalNumberPoints = 0;
  bool stopFlag = false;
  int hint = -1;
  int numberInsrtedPairs = 0;

  while(stopFlag == false) {

    //
    // read query points from a file
    //
    
    const std::vector<MPTCOUPLER::krigalg::Point> queryPoints = 
      readPointData(av[1],
		    maxPointCache);
    
    //
    // read values at query points from a file
    //
    
    const std::vector<MPTCOUPLER::krigalg::Value> queryValues = 
      readValueData(av[2],
		    maxPointCache);
  
    assert(queryPoints.size() == queryValues.size());
    
    //
    // update totalNumberPoints
    //

    totalNumberPoints += queryPoints.size();

    //
    //
    //
    
    if (queryPoints.empty() == true) {
      stopFlag = true;
      break;
    }
    
    //
    // process all query points and keep building the kriging model
    //
    

//     interpolationDb.insert(hint,
// 			   &(queryPoints[0][0]),
// 			   &(queryValues[0][0]),
// 			   &(queryValues[0][valueDimension]));

    processQueryPoints(tolerance,
		       queryPoints,
		       queryValues,
		       interpolationDb);

    
    //
    // output DB statistics
    //

    //interpolationDb.printDBStats(std::cout);

    
    //
    // try adding point/value-pair into kriging model
    //

#if 0
    const Point point(pointDimension,
		      &(queryPoints[0][0]));

    const std::vector<Value> pointValues = 
      copyValueData(&(queryValues[0][0]),
		    &(queryValues[0][valueDimension]),
		    pointDimension,
		    valueDimension);

     const bool addPairSuccess = 
       krigingModel.addPoint(point,
			     pointValues);
      
     if (addPairSuccess == true) {
       
       ++numberInsrtedPairs;
       std::cout << "Successfully added point " << numberInsrtedPairs
		 << std::endl;

     }

     if (numberInsrtedPairs == maxKrigingModelSize)
       break;
    
#endif

  }

#if 0

  //
  // store kriging model into an array
  //

  std::vector<double> packedKrigingModel;

  krigingModel.pack(packedKrigingModel);

  
  //
  // instantiate new kriging model
  //

  DerivativeRegressionModelPointer 
    regressionModelUP(new LinearDerivativeRegressionModel);
  DerivativeCorrelationModelPointer
    correlationModelUP(new GaussianDerivativeCorrelationModel(std::vector<double>(1, theta)));
  
  MultivariateDerivativeKrigingModel krigingModelUP(regressionModelUP,
						    correlationModelUP);
  
  krigingModelUP.unpack(packedKrigingModel);


  {

    const int maxPointCacheUP = 10;

    const std::vector<MPTCOUPLER::krigalg::Point> queryPoints = 
      readPointData(av[1],
		    maxPointCacheUP);
    
    //
    // read values at query points from a file
    //
    
    const std::vector<MPTCOUPLER::krigalg::Value> queryValues = 
      readValueData(av[2],
		    maxPointCacheUP);
    
    //
    //
    //

    for (int i = 0; i < maxPointCacheUP; ++i) {

      const Value value   = krigingModel.interpolate(valueDimension - 1, queryPoints[i]);
      const Value valueUP = krigingModelUP.interpolate(valueDimension - 1, queryPoints[i]);

      const Value diffValue = valueUP - value;

      std::cout << std::sqrt(MPTCOUPLER::krigalg::dot(diffValue, diffValue))
		<< std::endl;

    }


  }

#endif 

  interpolationDb.printDBStats(std::cout);

  //
  //
  //

  return EXIT_SUCCESS;

}



