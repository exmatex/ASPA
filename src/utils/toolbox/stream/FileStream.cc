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
// File:	FileStream.cc
// Package:	MPTCOUPLER communication and data transfer package
// 
// 
// 
// Description:	Simple class to read/write files in XDR format for portability
//

#include "toolbox/stream/FileStream.h"
#include "toolbox/base/Utilities.h"

#ifdef DEBUG_NO_INLINE
#include "toolbox/stream/FileStream.I"
#endif

namespace MPTCOUPLER {
   namespace toolbox {


/*
*************************************************************************
*									*
* Constructor and destructor for the file stream.  The constructor	*
* probably should not throw an exception if the file cannot be opened,	*
* but this class will disappear later, so this behavior is OK for now.	*
*									*
*************************************************************************
*/

FileStream::FileStream(const string& filename, const StreamMode mode)
{
   d_close_on_exit = true;

   const char* fmode = ((mode == FileStream::Read)  ? "rb" :
                        (mode == FileStream::Write) ? "wb" : "ab");
   if (!(d_FILE = fopen(filename.c_str(), fmode))) {
      TBOX_ERROR("FileStream: Unable to open file ``" 
                 << filename << "''...\n");
   }

#ifdef HAVE_XDR
   const xdr_op xop = ((mode==FileStream::Read) ? XDR_DECODE : XDR_ENCODE);
   xdrstdio_create(&d_xdr_stream, d_FILE, xop);
   setXDRStream(&d_xdr_stream);
#endif

}

FileStream::FileStream(FILE* file, const StreamMode mode)
{
   d_close_on_exit = false;

   d_FILE = file;

#ifdef HAVE_XDR
   const xdr_op xop = ((mode==FileStream::Read) ? XDR_DECODE : XDR_ENCODE);
   xdrstdio_create(&d_xdr_stream, d_FILE, xop);
   setXDRStream(&d_xdr_stream);
#endif
}

FileStream::~FileStream()
{
#ifdef HAVE_XDR
#ifndef LACKS_PROPER_XDR_HEADER
   xdr_destroy(&d_xdr_stream);
#else
   if (d_xdr_stream.x_ops->x_destroy)
      (*(void(*)(XDR*))(d_xdr_stream.x_ops->x_destroy))(&d_xdr_stream);
#endif
#endif

   if (d_close_on_exit) {
      (void) fclose(d_FILE);
   }
}

}
}


