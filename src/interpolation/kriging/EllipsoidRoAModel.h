//
// File:        EllipsoidRoAModel.h
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

#if !defined(included_ellalg_EllipsoidRoAModel)
#define included_ellalg_EllipsoidRoAModel

#ifndef included_config
#include "asf_config.h"
#endif // included_config

#ifndef included_krigalg_Point
#include <base/Point.h>
#endif // included_krigalg_Point

#ifndef included_krigalg_Value
#include <base/Value.h>
#endif // included_krigalg_Value

#ifndef included_krigalg_Matrix
#include <base/Matrix.h>
#endif // included_krigalg_Matrix

//#ifndef included_boost_shared_ptr
//#define included_boost_shared_ptr
//#include <boost/shared_ptr.hpp>
//#endif
#include <memory>

#ifndef included_vector
#define included_vector
#include <vector>
#endif // included_vector

#ifndef included_iosfwd
#define included_iosfwd
#include <iosfwd>
#endif // included_iosfwd

//
// 
//

namespace MPTCOUPLER {

  //
  // forward declarations
  //

  namespace toolbox {

    class Database;
    
  }

  namespace ellalg {

    //
    // forward declarations
    //

    class EllipsoidRoAModel;

    //
    // local types
    //

    //    typedef boost::shared_ptr<EllipsoidRoAModel> 
    //      EllipsoidRoAModelPtr;
    typedef std::shared_ptr<EllipsoidRoAModel> 
      EllipsoidRoAModelPtr;

    //
    // class definition
    //

    class EllipsoidRoAModel {
    
    public:
      //
      // construction
      //

      EllipsoidRoAModel(
			const MPTCOUPLER::krigalg::Matrix & GJ,
			const MPTCOUPLER::krigalg::Point & point, 
			const MPTCOUPLER::krigalg::Value & value
			);
      ~EllipsoidRoAModel();

      //
      // meta-methods
      //
    
      //
      // set parameters
      //
      static void setParams(double shM, double grH, double grM, double interpPDistMax, double epsA) ;

      //
      // get point dimension
      //

      int getPointDimension() const;

      //
      // get value dimension
      //

      int getValueDimension() const;

      //
      // interpolate value at a point
      //

      MPTCOUPLER::krigalg::Value interpolate(const MPTCOUPLER::krigalg::Point & point) const;
      MPTCOUPLER::krigalg::Value interpolate(const MPTCOUPLER::krigalg::Value & valueDiff ) const;

      //
      // do ellipsoid growth
      //
      void doEllipsoidGrowth(
			     const MPTCOUPLER::krigalg::Value & valueDiff,
			     const MPTCOUPLER::krigalg::Point & pointNew,
			     const double errorRatio,
			     bool & shifted,
			     double & shiftFactor);

      //
      // test to see if can interpolate
      //
      void testInterpG(
		       const MPTCOUPLER::krigalg::Point & inputPoint,
		       bool & canInterp,
		       bool & hitLimitIDist,
		       MPTCOUPLER::krigalg::Value & estOutputDiff);

      //
      // get current ellipsoid center
      //

      MPTCOUPLER::krigalg::Point getCenter() const;

      //
      // get GJ
      //

      void getGJ(double * gradient) const;

      //
      // output
      //

      void putToDatabase(toolbox::Database & db) const;
      void getFromDatabase(toolbox::Database & db);

      friend std::ostream & 
	operator<<(std::ostream                             & outputStream,
		   const EllipsoidRoAModel & ellipsoidModel);

    private:
      //
      // copy construction/assignment
      //
      //     EllipsoidRoAModel(const EllipsoidRoAModel &);
      //     const EllipsoidRoAModel & operator=(const EllipsoidRoAModel &);
    

      //
      // set precomputed private stuff
      //
      static void computePrivateStuff();

      double determineGrowthFactor(const double errorRatio) const;

      

    public:

    private:
    
      MPTCOUPLER::krigalg::Point        _point; // reference input
      MPTCOUPLER::krigalg::Value        _value; // reference output

      
      MPTCOUPLER::krigalg::Matrix   _GJ; // generalized Jacobian
      MPTCOUPLER::krigalg::Matrix   _A; // used in output distance measure

      static bool _haveComputedPrivateStuff;

      // for ellipsoid growth control:
      static double _grH, _grK, _grM, _grMInv;
      static bool   _grExtend; 

      // for ellipsoid shape control:
      static double _shM; // NOT dimension of space; this is exponent on minor axis in volume expression
      static bool   _shZero, _shInf;

      // for extra interpolation control
      static double _interpPDistMax;

      // for initial A matrix size
      static double _epsA;


    };

  }

}
#endif // included_ellalg_EllipsoidRoAModel_h
