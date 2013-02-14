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
// File:        EllipsoidRoAModel.cc
// Package:     MPTCOUPLER kriging algorithm
// 
// 
// 
// Description: Class implementing ellipsoid RoA model 
//
// $Id$
//
// $Log$
//

#include "EllipsoidRoAModel.h"

// #include "SecondMoment.h"

#include <mtl/mtl.h>
#include <mtl/mtl2lapack.h>

#include <algorithm>
#include <cassert>
#include <iostream>
// #include <iterator>
// #include <limits>

//
//
// 

namespace MPTCOUPLER {
  namespace ellalg {

     //
     //
     //

     bool EllipsoidRoAModel::_haveComputedPrivateStuff = false;

     // for ellipsoid growth control:
     double EllipsoidRoAModel::_grH = 1.2;
     double EllipsoidRoAModel::_grM = 64.0;
     double EllipsoidRoAModel::_grK = -1.; // computed from _grH and _grM
     double EllipsoidRoAModel::_grMInv = -1.; // computed from _grH and _grM
     bool   EllipsoidRoAModel::_grExtend = false; 

     // for ellipsoid shape control:
     double EllipsoidRoAModel::_shM = -1.;
     bool   EllipsoidRoAModel::_shZero = false;
     bool   EllipsoidRoAModel::_shInf = false;

     // for extra interpolation control
     double EllipsoidRoAModel::_interpPDistMax = 1.0;

    // for initial A matrix size
    double EllipsoidRoAModel::_epsA = 1.0e-5;
     

    //
    // construction/destruction
    //
    EllipsoidRoAModel::EllipsoidRoAModel(
					 const MPTCOUPLER::krigalg::Matrix & GJ,
					 const MPTCOUPLER::krigalg::Point & point, 
					 const MPTCOUPLER::krigalg::Value & value
					 )
      : _GJ(GJ),
	_point(point), 
	_value(value),
	_A(value.size(), value.size()) // initializes entries to zero
    {

      if (!(EllipsoidRoAModel::_haveComputedPrivateStuff)) {
	computePrivateStuff();
      }

      mtl::set_diagonal(_A, 1./EllipsoidRoAModel::_epsA);

      return;

    }

    EllipsoidRoAModel::~EllipsoidRoAModel()
    {

      return;

    }

    //
    // set precomputed private stuff
    //
    void
    EllipsoidRoAModel::computePrivateStuff(){

       if (EllipsoidRoAModel::_grH < 0.) {
	  EllipsoidRoAModel::_grExtend = false;
	  _haveComputedPrivateStuff = true;
	  return;
       }

       assert(EllipsoidRoAModel::_grH >= 1.);
       
       EllipsoidRoAModel::_grK = pow(EllipsoidRoAModel::_grH, EllipsoidRoAModel::_grM) -1.; 
       EllipsoidRoAModel::_grMInv = 1. / EllipsoidRoAModel::_grM;

       EllipsoidRoAModel::_grExtend = true;

       if (EllipsoidRoAModel::_shM < 0.) {
	  EllipsoidRoAModel::_shInf  = true;
	  EllipsoidRoAModel::_shZero = false;
       } else if (EllipsoidRoAModel::_shM < 1e-8) {
	  EllipsoidRoAModel::_shInf = false;
	  EllipsoidRoAModel::_shZero = true;
       } 

       _haveComputedPrivateStuff = true;

      return;
    }

    //
    // set parameters
    //
    void
    EllipsoidRoAModel::setParams(double shM, double grH, double grM, double interpPDistMax, double epsA) 
    {
       EllipsoidRoAModel::_shM = shM;
       EllipsoidRoAModel::_grH = grH;
       EllipsoidRoAModel::_grM = grM;
       EllipsoidRoAModel::_interpPDistMax = interpPDistMax;
       EllipsoidRoAModel::_epsA = epsA;
       computePrivateStuff();
    }

    //
    // get point dimension
    //
  
    int 
    EllipsoidRoAModel::getPointDimension() const
    {

      return _point.size();

    }
  
    //
    // get value dimension
    //
  
    int 
    EllipsoidRoAModel::getValueDimension() const
    {

      return _value.size();

    }

    MPTCOUPLER::krigalg::Value
    EllipsoidRoAModel::interpolate(const MPTCOUPLER::krigalg::Point & point) const
    {

      const MPTCOUPLER::krigalg::Vector inputDiff = point - _point;

      const MPTCOUPLER::krigalg::Vector estValueDiff = mult(_GJ, inputDiff);
      
      const MPTCOUPLER::krigalg::Vector interpolatedValueVector = _value + estValueDiff;
      return interpolatedValueVector;

    }

    MPTCOUPLER::krigalg::Value
    EllipsoidRoAModel::interpolate(const MPTCOUPLER::krigalg::Value & valueDiff) const
    {

      const MPTCOUPLER::krigalg::Vector interpolatedValueVector = _value + valueDiff;
      return interpolatedValueVector;

    }

    double
    EllipsoidRoAModel::determineGrowthFactor(const double errorRatio) const {
      assert(errorRatio <= 1.);
      assert(errorRatio >= 0.);

      double growthFactor;
	  
      if (_grExtend) {
	growthFactor = _grH * pow( (1./(1.+_grK * errorRatio)), _grMInv); 
      } else {
	growthFactor = 1.;
      }
	  
      return growthFactor;
    }

    void
    EllipsoidRoAModel::doEllipsoidGrowth(
		      const MPTCOUPLER::krigalg::Value & valueDiff, // to be enclosed
		      const MPTCOUPLER::krigalg::Point & pointNew, // point at which new evaluation was done
		      const double errorRatio, // ratio of actual to acceptable error
		      bool & shifted, // whether or not shifted center
		      double & shiftFactor // amount by which shifted
		      ){

	  const int valueDimension = getValueDimension();

	  MPTCOUPLER::krigalg::Vector nMap = mult(_A, valueDiff);

	  const double dSq = mtl::dot( nMap, nMap );

	  if (dSq < 1.) {
	     // may be possible under certain circumstances -- checking for growth of a whole bunch
	     // of models, using a point that may be inside some of them already
	     shifted = false ;
	     shiftFactor = 0. ;
	     return;
	  }
	  
	  double d = sqrt(dSq); // = mtl::two_norm(nMap);
	  mtl::scale(nMap, 1./d);

	  const double growthFactor = determineGrowthFactor(errorRatio);
	  // with GrowthFactor > 1, can grow the ellipsoid a little more than is strictly needed to touch the new value
	  d *= growthFactor ;

	  if (_shInf) {
	     // in limit m->inf, no lateral growth

	     const double factor = (1. / d - 1.);

	     for (int jA = 0; jA < valueDimension; jA++){
	       const double tempknA = factor * mtl::dot( nMap, columns(_A)[jA] ) ;
	       // factor * SUM(n_map(:)*_A(:,j_A)); // from f90 notation

	       // columns(_A)[jA] += nMap * tempknA;
	       mtl::add(columns(_A)[jA],
			mtl::scaled(nMap, tempknA),
			columns(_A)[jA]);
	       // columns(_A)[jA] += nMap * tempknA; // does not work
	       // A_matx(:,j_A) = A_matx(:,j_A) + n_map(:)*temp_knA // from f90 notation
	     }

	     shifted = false ;
	     shiftFactor = 0. ;

	  } else {

	     double a, b;

	     if (_shZero) {
		// in limit m->0, no backside growth
		a = 0.5 * (d + 1.);
		b = sqrt(a);
	     } else {
		const double dd = d*d;
		a = ((2.+_shM)*d + sqrt(4.*(1.+_shM) + dd*_shM*_shM ))/(4. + 2.*_shM);
		const double twoad = 2.*a*d;
		const double factor_b = 1. - dd + twoad;
		const double temp = factor_b*factor_b - 4.*a*a;
		if (temp <= 0.) {
		   b = sqrt( factor_b * 0.5) ;
		} else {
		   b = sqrt(( factor_b - sqrt(temp) )*0.5);
		}
	     }

	     const double capX = d - a;

	     const double bInv =   1. / b;
	     const double factor   = ((1. / a) - bInv);
	     for (int jA = 0; jA < valueDimension; jA++){
	       const double tempknA = factor * mtl::dot( nMap, columns(_A)[jA] ) ;
	       // factor * SUM(n_map(:)*_A(:,j_A)); // from f90 notation
	       // columns(_A)[jA] *= bInv;
	       mtl::scale(columns(_A)[jA], bInv) ;
	       mtl::add(columns(_A)[jA],
			mtl::scaled(nMap, tempknA),
			columns(_A)[jA]);
	       // columns(_A)[jA] += nMap * tempknA;  // does not work
	       // _A(:,j_A) = bInv * _A(:,j_A) + n_map(:)*tempknA; // from f90 notation
	     }

	     shifted    = true ;
	     shiftFactor = capX/d;

	  }

	  if (shifted) {

	    // shift intput and output (point and value) vectors;
	    // can do this because using a linear approximation;
	    // all the mappings from input vector (point) through to get shiftFactor
	    // are linear, so shiftFactor can be applied to estValueDiff and to
	    // inputDiff;

	    // shift point toward point at which new evaluation was done
	    _point *= (1. - shiftFactor);
	    _point += pointNew * shiftFactor;

	    // shift reference value by the amount dictated by GJ
	    _value += valueDiff * shiftFactor;

	  }

	  return ;
       }

       void
       EllipsoidRoAModel::testInterpG(
		   const MPTCOUPLER::krigalg::Point & inputPoint,
		   bool & canInterp,
		   bool & hitLimitIDist,
		   MPTCOUPLER::krigalg::Value & estOutputDiff
		   ){

	  canInterp = false ;
	  hitLimitIDist = false ;

	  const MPTCOUPLER::krigalg::Vector inputDiff      =  inputPoint - _point;
	  estOutputDiff  = mult(_GJ, inputDiff); // _GJ is in normalized spaces
	  const double inputPDist = mtl::two_norm(inputDiff); // already normalized

	  if (inputPDist <= _interpPDistMax) {
	    // check ellipsoid;
	    // estOutputDiff already normalized;
	    const MPTCOUPLER::krigalg::Vector outputInterpDistA = mult(_A, estOutputDiff);
	     const double oDistA = mtl::two_norm(outputInterpDistA);
	     if (oDistA <= 1.) {
		canInterp = true;
	     }
	  } else {
	     hitLimitIDist = true;
	     canInterp = false;
	  }

	  return ;
       }


    //
    // get current ellipsoid center
    //

    MPTCOUPLER::krigalg::Point 
    EllipsoidRoAModel::getCenter() const
    {

      return _point;

    }

    //
    // get GJ
    //

     void
     EllipsoidRoAModel::getGJ(double * gradient) const
     {

	const krigalg::Matrix gjTranspose = krigalg::transpose(_GJ);

	const double * gjData = gjTranspose.data();
	std::copy(&(gjData[0]), &(gjData[_value.size()*_point.size()]), gradient);

	return;

    }

    //
    // output object data to database; store only point-value pairs and
    // rely in the getFromDatabase() to properly build the model
    //

    void
    EllipsoidRoAModel::putToDatabase(toolbox::Database& db) const
    {

      assert(false); // Needs to be implemented
      return;

    }

    //
    // fill in the object from Database
    //

    void
    EllipsoidRoAModel::getFromDatabase(toolbox::Database& db)
    {

      assert(false); // Needs to be implemented
      return;

    }
      

    //
    // output operator
    //

    std::ostream &
    operator<<(std::ostream                             & outputStream,
	       const EllipsoidRoAModel & ellipsoidModel)
    {

      outputStream << "point: " << std::endl;
      outputStream << ellipsoidModel._point << std::endl;

      outputStream << "value: " << std::endl;
      outputStream << ellipsoidModel._value << std::endl;

      outputStream << "GJ: " << std::endl;
      //outputStream << ellipsoidModel._GJ;
      MPTCOUPLER::krigalg::operator<<(outputStream, ellipsoidModel._GJ);
      outputStream << std::endl;

      outputStream << "A: " << std::endl;
      // outputStream << ellipsoidModel._A;
      MPTCOUPLER::krigalg::operator<<(outputStream, ellipsoidModel._A);
      outputStream << std::endl;

      return outputStream;

    }

  }
  
}


