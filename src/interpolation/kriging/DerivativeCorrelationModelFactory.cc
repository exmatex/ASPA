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
// File:        DerivativeCorrelationModelFactory.cc
// Package:     MPTCOUPLER kriging algorithm
// 
// 
// 
// Description: Class implementing DerivativeCorrelationModel factory
// 

#include "DerivativeCorrelationModelFactory.h"

#include "GaussianDerivativeCorrelationModel.h"

//
//
//

namespace MPTCOUPLER {
  namespace krigalg {

    //
    //const data
    //
    
    const std::string gaussianDerivativeCorrelationModelKey("MPTCOUPLER::krigalg::GaussianDerivativeCorrelationModel");

    enum {GAUSSIAN_DERIVATIVE_CORRELATION_CLASS};

    //
    // construction/destruction
    //
    DerivativeCorrelationModelFactory::DerivativeCorrelationModelFactory()
    {

      return;
      
    }

    DerivativeCorrelationModelFactory::~DerivativeCorrelationModelFactory()
    {

      return;

    }
    
    //
    // build a model containing empty _thetas
    //
    
    DerivativeCorrelationModelPointer 
    DerivativeCorrelationModelFactory::build(const std::string & classKey) const
    {

      //
      //
      //

      if (classKey == gaussianDerivativeCorrelationModelKey)
	return DerivativeCorrelationModelPointer(new GaussianDerivativeCorrelationModel(std::vector<double>()));

      //
      // cannot be reached
      //
      assert(false);
      
      return  DerivativeCorrelationModelPointer(new GaussianDerivativeCorrelationModel(std::vector<double>()));

    }

    //
    // build model
    //

    DerivativeCorrelationModelPointer
    DerivativeCorrelationModelFactory::build(const std::string & classKey,
					     const std::vector<double> & thetas) const
    {

      //
      //
      //

      if (classKey == gaussianDerivativeCorrelationModelKey)
	return DerivativeCorrelationModelPointer(new GaussianDerivativeCorrelationModel(thetas));

      //
      // cannot be reached
      //
      
      assert(false);
      
      return DerivativeCorrelationModelPointer(new GaussianDerivativeCorrelationModel(thetas));;

    }

    //
    // build model containing empty _thetas
    //

    DerivativeCorrelationModelPointer 
    DerivativeCorrelationModelFactory::build(ClassIdentifier classId) const
    {

      //
      //
      //

      switch(classId) {

      case GAUSSIAN_DERIVATIVE_CORRELATION_CLASS:
	return DerivativeCorrelationModelPointer(new GaussianDerivativeCorrelationModel(std::vector<double>()));
	break;

      default:
	assert(false);
	break;

      }
      
      //
      // not reached
      //

      return  DerivativeCorrelationModelPointer(new GaussianDerivativeCorrelationModel(std::vector<double>()));

    }

    //
    // build model
    //

    DerivativeCorrelationModelPointer
    DerivativeCorrelationModelFactory::build(ClassIdentifier classId,
					     const std::vector<double> & thetas) const
    {

      switch(classId) {

      case GAUSSIAN_DERIVATIVE_CORRELATION_CLASS:
	return DerivativeCorrelationModelPointer(new GaussianDerivativeCorrelationModel(thetas));
	break;

      default:
	assert(false);
	break;

      }

      //
      // not reached
      //
      
      return DerivativeCorrelationModelPointer(new GaussianDerivativeCorrelationModel(thetas));;

    }

    //
    // return class identifier
    //

    DerivativeCorrelationModelFactory::ClassIdentifier
    DerivativeCorrelationModelFactory::getClassId(const std::string & classKey)
    {

      if (classKey == gaussianDerivativeCorrelationModelKey)
	return GAUSSIAN_DERIVATIVE_CORRELATION_CLASS;
      
      //
      // not reached
      //

      assert(false);
      
      return GAUSSIAN_DERIVATIVE_CORRELATION_CLASS;

    }

  }
}


