/* DO-NOT-DELETE revisionify.begin() */
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
/* DO-NOT-DELETE revisionify.end() */
//
// File:	MessageStream.I
// Package:	MPTCOUPLER toolbox
// 
// 
// 
// Description:	Fixed-size message buffer used in interprocessor communication
//

#ifdef DEBUG_NO_INLINE
#define inline
#endif

#ifndef included_toolbox_Utilities
#include "toolbox/base/Utilities.h"
#endif

namespace MPTCOUPLER {
   namespace toolbox {


inline
void MessageStream::useXDR(const bool flag)
{
#ifdef HAVE_XDR
   s_use_xdr_translation = flag;
#else
   if ( flag == true )
      TBOX_ERROR("MessageStream: Error in XDR was not used in this compile...\n");
#endif
}

inline
void *MessageStream::getBufferStart()
{
   return((void *) d_buffer);
}

inline
int MessageStream::getCurrentSize() const
{
   return(d_current_size);
}

inline
int MessageStream::getCurrentIndex() const
{
   return(d_buffer_index);
}

inline
void MessageStream::setCurrentIndex(const int index)
{
   d_buffer_index = index;
	
   // SGS this is broken, in fact this whole class 
   // looks broken, if xdr is not used how is d_buffer_index 
   // moved?	

#ifdef HAVE_XDR
   if (d_use_xdr) {
#ifndef LACKS_PROPER_XDR_HEADER
      xdr_setpos(&d_xdr_stream, d_buffer_index);
#else
      (*(void(*)(XDR*,unsigned int))(d_xdr_stream.x_ops->x_setpostn))(
         &d_xdr_stream, d_buffer_index);
#endif
   }
#endif

}

inline
void MessageStream::resetIndex()
{
   setCurrentIndex(0);
}

#ifdef DEBUG_NO_INLINE
#undef inline
#endif

}
}



