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
// File:	IEEE.cc
// Package:	MPTCOUPLER toolbox
// 
// 
// 
// Description:	IEEE routines to set up handlers and get signaling NaNs
//

#include "toolbox/base/IEEE.h"
#include "toolbox/parallel/MPI.h"
#include <float.h>
#include <math.h>
#include <limits.h>

/*
 * Floating point exception handling.  
 * 
 * The following lines setup exception handling header files.
 */
#if defined(HAVE_EXCEPTION_HANDLING)
#include <stdlib.h>
#include <stdio.h>
#include <fpu_control.h>
#include <signal.h>
#endif

/*
 * The following lines setup exception handling headers on the Sun.  If we
 * use Sun's native compiler, just pull in the <sunmath.h> include file.
 * If we are under solaris but use a different compiler (e.g. KCC, g++)
 * we have to explicitly define the functions that <sunmath.h> defines,
 * since we don't have access to this file.
 */
#ifdef __SUNPRO_CC
#include <sunmath.h>
#endif

#ifdef DEBUG_NO_INLINE
#include "toolbox/base/IEEE.I"
#endif

namespace MPTCOUPLER {
   namespace toolbox {


/*
 * Create the function invoked when an exception is tripped. 
 */
#if defined(HAVE_EXCEPTION_HANDLING)
static void error_action(int error) 
{
   fprintf(stderr, "floating point exception -- program abort!\n");
   abort();
   MPI::abort();
}
#endif

/*
 *  Settings for the various signaling NaNs on different systems
 */

#if !defined(FLT_SNAN_IS_BROKEN)  
float  IEEE::s_signaling_nan_float  = FLT_SNAN;
#elif !defined(FLT_MAX_IS_BROKEN)
float  IEEE::s_signaling_nan_float  = FLT_MAX;
#else
float  IEEE::s_signaling_nan_float  = NAN;
#endif

#if !defined(DBL_SNAN_IS_BROKEN)
double  IEEE::s_signaling_nan_double  = DBL_SNAN;
#elif !defined(DBL_MAX_IS_BROKEN)
double  IEEE::s_signaling_nan_double  = DBL_MAX;
#else
double  IEEE::s_signaling_nan_double  = NAN;
#endif

int    IEEE::s_int_max = INT_MAX;
int    IEEE::s_int_min = INT_MIN;
float  IEEE::s_flt_max = FLT_MAX;
float  IEEE::s_flt_min = FLT_MIN;
float  IEEE::s_flt_epsilon = FLT_EPSILON;
double IEEE::s_dbl_max = DBL_MAX;
double IEEE::s_dbl_min = DBL_MIN;
double IEEE::s_dbl_epsilon = DBL_EPSILON;
   


/*
*************************************************************************
*									*
* Set up the IEEE exception handlers so that normal IEEE exceptions	*
* will cause a program abort.  How this is done varies wildly from	*
* architecture to architecture. 					*
*************************************************************************
*/

void IEEE::setupExceptionHandlers()
{

#if defined(HAVE_EXCEPTION_HANDLING)
   unsigned short fpu_flags = _FPU_DEFAULT;          
   fpu_flags &= ~_FPU_MASK_IM;  /* Execption on Invalid operation */
   fpu_flags &= ~_FPU_MASK_ZM;  /* Execption on Division by zero  */
   fpu_flags &= ~_FPU_MASK_OM;  /* Execption on Overflow */
   _FPU_SETCW(fpu_flags);
   signal(SIGFPE, error_action);
#endif
}

/*
*************************************************************************
*									*
* Initialize float and double values to the signaling nan.              *
* Initialize int to INT_MAX.                                            *
*									*
*************************************************************************
*/

void IEEE::setNaN(float &f)
{  
   f = s_signaling_nan_float;
}

void IEEE::setNaN(double &d)
{  
   d = s_signaling_nan_double;
}

/*
*************************************************************************
*									*
* Initialize float and double arrays to signaling NaNs.			*
* Initialize int array to INT_MAX.                                      *
*									*
*************************************************************************
*/

void IEEE::initializeArrayToSignalingNaN(float *data, const int n)
{
   for (int i = 0; i < n; i++) {
      data[i] = s_signaling_nan_float;
   }
}

void IEEE::initializeArrayToSignalingNaN(double *data, const int n)
{
   for (int i = 0; i < n; i++) {
      data[i] = s_signaling_nan_double;
   }
}

void IEEE::initializeArrayToINT_MAX(int *data, const int n)
{
   for (int i = 0; i < n; i++) {
      data[i] = s_int_max;
   }
}

void IEEE::initializeArrayToINT_MIN(int *data, const int n)
{
   for (int i = 0; i < n; i++) {
      data[i] = s_int_min;
   }
}

void IEEE::initializeArrayToFLT_MAX(float *data, const int n)
{
   for (int i = 0; i < n; i++) {
      data[i] = s_flt_max;
   }
}

void IEEE::initializeArrayToFLT_MIN(float *data, const int n)
{
   for (int i = 0; i < n; i++) {
      data[i] = s_flt_min;
   }
}

void IEEE::initializeArrayToDBL_MAX(double *data, const int n)
{
   for (int i = 0; i < n; i++) {
      data[i] = s_dbl_max;
   }
}

void IEEE::initializeArrayToDBL_MIN(double *data, const int n)
{
   for (int i = 0; i < n; i++) {
      data[i] = s_dbl_min;
   }
}

/*
*************************************************************************
*									*
* Return whether or not the value is a NaN.     	                *
*									*
*************************************************************************
*/

bool IEEE::isNaN(const float &f) 
{
   int i = isnan(f);
   if (i != 0) {
     return(true);
   } else {
     return(false);
   }
}

bool IEEE::isNaN(const double &d) 
{
   int i = isnan(d);
   if (i != 0) {
     return(true);
   } else {
     return(false);
   }
}


}
}



