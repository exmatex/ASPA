//
// File:        DerivativeRegressionModel.h
// Package:     MPTCOUPLER kriging algorithm
// 
// 
// 
// Description: Class implementing regression model that includes derivative 
// Description: data.
//
// $Id: DerivativeRegressionModel.h,v 1.1 2005/09/01 16:13:55 knap2 Exp $
//
// $Log: DerivativeRegressionModel.h,v $
// Revision 1.1  2005/09/01 16:13:55  knap2
// Initial source.
//
//

#if !defined(included_krigalg_DerivativeRegressionModel)
#define included_krigalg_DerivativeRegressionModel

#ifndef included_config
#include "asf_config.h"
#endif // included_config

//#ifndef included_boost_shared_ptr
//#define included_boost_shared_ptr
//#include <boost/shared_ptr.hpp>
//#endif

#ifndef included_krigalg_RegressionModel
#include "RegressionModel.h"
#endif

namespace MPTCOUPLER {

  //
  // forward declarations
  //

  namespace toolbox {
    
    class Database;

  }

  //
  //
  //

  namespace krigalg {
    
  //
  // forward declarations
  //

  class DerivativeRegressionModel;
  //  typedef boost::shared_ptr<DerivativeRegressionModel> DerivativeRegressionModelPointer;
  typedef std::shared_ptr<DerivativeRegressionModel> DerivativeRegressionModelPointer;

  //
  // class definition
  //

  class DerivativeRegressionModel : public RegressionModel {

    //
    // methods
    //

  public:
    
    //
    // construction/destruction
    //
    
    DerivativeRegressionModel();
    virtual ~DerivativeRegressionModel() = 0;

    //
    // meta-methods
    //
    
    virtual Matrix    getValues(const Point & point) const = 0;
    virtual Dimension getDimension(const Point & point) const = 0;

    //
    // Database input/output
    //

    virtual std::string getClassName() const = 0;
    virtual void putToDatabase(toolbox::Database & db) const;
    virtual void getFromDatabase(toolbox::Database & db);

  private:
    //
    // copy construction/assignment
    //
    
    DerivativeRegressionModel(const DerivativeRegressionModel &);
    const DerivativeRegressionModel & operator=(const DerivativeRegressionModel &);

  };

}
}


#endif // included_krigalg_DerivativeRegressionModel
