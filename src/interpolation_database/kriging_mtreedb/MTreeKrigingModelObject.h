//
// File:        MTreeKrigingModelObject.h
// Package:     MPTCOUPLER kriging coupler
// 
// Revision:    $Revision$
// Modified:    $Date$
// Description: Specialization of the MTreeModelObject to kriging.
//

#ifndef included_krigcpl_MTreeKrigingModelObject_h
#define included_krigcpl_MTreeKrigingModelObject_h

#ifndef included_MPTCOUPLER_config
#include "asf_config.h"
#endif

#include "base/MTreeModelObject.h"

#include <base/InterpolationModel.h>

namespace MPTCOUPLER {
  namespace krigcpl {

    typedef MTreeModelObject<krigalg::InterpolationModelPtr> MTreeKrigingModelObject;

  }
}
  


#endif // included_krigcpl_MTreeKrigingModelObject_h
