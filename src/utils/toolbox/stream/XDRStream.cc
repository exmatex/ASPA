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
// File:	XDRStream.cc
// Package:	MPTCOUPLER toolbox
// 
// 
// 
// Description:	Stream class that converts into XDR for portable communication
//

#include "toolbox/stream/XDRStream.h"

#include <string>
using namespace std;
#include "toolbox/base/Utilities.h"
#ifdef DEBUG_CHECK_ASSERTIONS
#include <assert.h>
#endif

#ifdef DEBUG_NO_INLINE
#include "toolbox/stream/XDRStream.I"
#endif

namespace MPTCOUPLER {
   namespace toolbox {



/*
*************************************************************************
*									*
* Some macros to simplify the implementation for the XDR streams	*
*									*
*************************************************************************
*/

#ifdef HAVE_XDR

#define XDR_PACK_OPAQUE(m_data,m_size)					\
   ((d_xdr_stream->x_op != XDR_ENCODE) ||				\
    (!xdr_opaque(d_xdr_stream, (caddr_t) m_data, m_size)))
#define XDR_UNPACK_OPAQUE(m_data,m_size)				\
   ((d_xdr_stream->x_op != XDR_DECODE) ||				\
    (!xdr_opaque(d_xdr_stream, (caddr_t) m_data, m_size)))
#define XDR_PACK_VECTOR(m_data,m_size,m_type)				\
   ((d_xdr_stream->x_op != XDR_ENCODE) ||				\
    (!xdr_vector(d_xdr_stream, (char *) m_data, m_size,			\
                 sizeof(m_type), (xdrproc_t) xdr_##m_type)))
#define XDR_UNPACK_VECTOR(m_data,m_size,m_type)				\
   ((d_xdr_stream->x_op != XDR_DECODE) ||				\
    (!xdr_vector(d_xdr_stream, (char *) m_data, m_size,			\
                 sizeof(m_type), (xdrproc_t) xdr_##m_type)))

#else

#define XDR_PACK_OPAQUE(m_data,m_size) 0
#define XDR_UNPACK_OPAQUE(m_data,m_size) 0
#define XDR_PACK_VECTOR(m_data,m_size,m_type) 0
#define XDR_UNPACK_VECTOR(m_data,m_size,m_type) 0

#endif


/*
*************************************************************************
*									*
* The virtual destructor for XDRStream does nothing.		*
*									*
*************************************************************************
*/

XDRStream::~XDRStream()
{
}

/*
*************************************************************************
*									*
* Packing and unpacking member functions for booleans.  Note that since	*
* the boolean representation is non-standard, boolean arrays are copied	*
* into character arrays and then packed using the character routines.	*
*									*
*************************************************************************
*/

AbstractStream& XDRStream::operator<<(const bool& data)
{
   pack(&data, 1);
   return(*this);
}

AbstractStream& XDRStream::operator>>(bool& data)
{
   unpack(&data, 1);
   return(*this);
}

void XDRStream::pack(const bool *data, const int n)
{
   char *flags = new char[n];
   for (int i = 0; i < n; i++) {
      flags[i] = (data[i] ? 1 : 0);
   }
   if (XDR_PACK_OPAQUE(flags, n)) {
      TBOX_ERROR("XDRStream: Error in encoding bool...\n");
   }
   delete [] flags;
}

void XDRStream::unpack(bool *data, const int n)
{
   char *flags = new char[n];
   if (XDR_UNPACK_OPAQUE(flags, n)) {
      TBOX_ERROR("XDRStream: Error in decoding bool...\n");
   }
   for (int i = 0; i < n; i++) {
      data[i] = (flags[i] ? true : false);
   }
   delete [] flags;
}

/*
*************************************************************************
*									*
* Packing and unpacking member functions for characters			*
*									*
*************************************************************************
*/

AbstractStream& XDRStream::operator<<(const char& data)
{
   pack(&data, 1);
   return(*this);
}

AbstractStream& XDRStream::operator>>(char& data)
{
   unpack(&data, 1);
   return(*this);
}

void XDRStream::pack(const char *data, const int n)
{
   if (XDR_PACK_OPAQUE(data, n)) {
      TBOX_ERROR("XDRStream: Error in encoding char...\n");
   }
}

void XDRStream::unpack(char *data, const int n)
{
   if (XDR_UNPACK_OPAQUE(data, n)) {
      TBOX_ERROR("XDRStream: Error in decoding char...\n");
   }
}

void XDRStream::writeString(const char *data)
{
#ifdef HAVE_XDR
   if (!xdr_string(d_xdr_stream, (char **) &data, strlen(data))) {
      TBOX_ERROR("XDRStream: Error in writing string...\n");
   }
#endif

}

#if 0
/*
*************************************************************************
*									*
* Packing and unpacking member functions for double complex		*
*									*
*************************************************************************
*/

AbstractStream& XDRStream::operator<<(const dcomplex& data)
{
   pack(&data, 1);
   return(*this);
}

AbstractStream& XDRStream::operator>>(dcomplex& data)
{
   unpack(&data, 1);
   return(*this);
}

void XDRStream::pack(const dcomplex *data, const int n)
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(sizeof(dcomplex) == 2*sizeof(double));
#endif
   if (XDR_PACK_VECTOR((double *) data, 2*n, double)) {
      TBOX_ERROR("XDRStream: Error in encoding double complex...\n");
   }
}

void XDRStream::unpack(dcomplex *data, const int n)
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(sizeof(dcomplex) == 2*sizeof(double));
#endif
   if (XDR_UNPACK_VECTOR((double *) data, 2*n, double)) {
      TBOX_ERROR("XDRStream: Error in decoding double complex...\n");
   }
}
#endif

/*
*************************************************************************
*									*
* Packing and unpacking member functions for doubles			*
*									*
*************************************************************************
*/

AbstractStream& XDRStream::operator<<(const double& data)
{
   pack(&data, 1);
   return(*this);
}

AbstractStream& XDRStream::operator>>(double& data)
{
   unpack(&data, 1);
   return(*this);
}

void XDRStream::pack(const double *data, const int n)
{
   if (XDR_PACK_VECTOR(data, n, double)) {
      TBOX_ERROR("XDRStream: Error in encoding double...\n");
   }
}

void XDRStream::unpack(double *data, const int n)
{
   if (XDR_UNPACK_VECTOR(data, n, double)) {
      TBOX_ERROR("XDRStream: Error in decoding double...\n");
   }
}

/*
*************************************************************************
*									*
* Packing and unpacking member functions for floats			*
*									*
*************************************************************************
*/

AbstractStream& XDRStream::operator<<(const float& data)
{
   pack(&data, 1);
   return(*this);
}

AbstractStream& XDRStream::operator>>(float& data)
{
   unpack(&data, 1);
   return(*this);
}

void XDRStream::pack(const float *data, const int n)
{
   if (XDR_PACK_VECTOR(data, n, float)) {
      TBOX_ERROR("XDRStream: Error in encoding float...\n");
   }
}

void XDRStream::unpack(float *data, const int n)
{
   if (XDR_UNPACK_VECTOR(data, n, float)) {
      TBOX_ERROR("XDRStream: Error in decoding float...\n");
   }
}

/*
*************************************************************************
*									*
* Packing and unpacking member functions for integers			*
*									*
*************************************************************************
*/

AbstractStream& XDRStream::operator<<(const int& data)
{
   pack(&data, 1);
   return(*this);
}

AbstractStream& XDRStream::operator>>(int& data)
{
   unpack(&data, 1);
   return(*this);
}

void XDRStream::pack(const int *data, const int n)
{
   if (XDR_PACK_VECTOR(data, n, int)) {
      TBOX_ERROR("XDRStream: Error in encoding integer...\n");
   }
}

void XDRStream::unpack(int *data, const int n)
{
   if (XDR_UNPACK_VECTOR(data, n, int)) {
      TBOX_ERROR("XDRStream: Error in decoding integer...\n");
   }
}

}
}


