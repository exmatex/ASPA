//
// File:        CorrelationModelFactory.h
// Package:     MPTCOUPLER kriging algorithm
// 
// Revision:    $Revision$
// Modified:    $Date$
// Description: Class implementing CorrelationModel factory
// 

#if !defined(included_krigalg_CorrelationModelFactory_h)
#define included_krigalg_CorrelationModelFactory_h

#ifndef included_config
#include "asf_config.h"
#endif // included_config

#ifndef included_krigalg_CorrelationModel
#include "CorrelationModel.h"
#endif

namespace MPTCOUPLER {
  namespace krigalg {

    //
    // forward declarations
    //

    class CorrelationModelFactory;

    //
    //
    //

    class CorrelationModelFactory {

      //
      // types
      //

    public:

      typedef int ClassIdentifier;
      
      //
      // methods
      //

    public:
      //
      // construction/destruction
      //
      CorrelationModelFactory();
      ~CorrelationModelFactory();

      //
      // build correlation model
      //
      CorrelationModelPointer build(const std::string & classKey) const;
      CorrelationModelPointer build(const std::string & classKey,
				    const std::vector<double> & thetas) const;
      
      CorrelationModelPointer build(ClassIdentifier classId) const;
      CorrelationModelPointer build(ClassIdentifier classId,
				    const std::vector<double> & thetas) const;

      static ClassIdentifier getClassId(const std::string & classKey);

    private:
      //
      // copy construction and assignment operator
      //
      CorrelationModelFactory(const CorrelationModelFactory &);
      const CorrelationModelFactory & operator=(const CorrelationModelFactory &);

    };

  }
}

#endif // included_krigalg_CorrelationModelFactory_h
