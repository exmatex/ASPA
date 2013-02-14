//
// File:        Dimension.h
// Package:     MPTCOUPLER kriging algorithm
// 
// 
// 
// Description: Base class for the concept of dimension.
//
// $Id$
//
// $Log$
//

#if !defined(included_krigalg_Dimension)
#define included_krigalg_Dimension

#ifndef included_config
#include "asf_config.h"
#endif // included_config

#ifndef included_mtl_dimension
#define included_mtl_dimension
#include <mtl/dimension.h>
#endif

namespace MPTCOUPLER {
    namespace krigalg {

  typedef mtl::dimension<int> Dimension;

}
}

#endif // included_krigalg_Dimension
