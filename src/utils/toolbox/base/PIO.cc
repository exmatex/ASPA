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
// File:	PIO.cc
// Package:	MPTCOUPLER toolbox
// 
// 
// 
// Description:	Parallel I/O classes pout, perr, and plog and control class
//

#include "toolbox/base/PIO.h"

#include <stdio.h>
#include "toolbox/parallel/MPI.h"
#include "toolbox/base/ParallelBuffer.h"

#ifndef NULL
#define NULL 0
#endif

namespace MPTCOUPLER {
   namespace toolbox {

int       PIO::s_rank       = -1;
ofstream* PIO::s_filestream = NULL;

/*
*************************************************************************
*									*
* Define the parallel buffers and the associated ostream objects.	*
*									*
*************************************************************************
*/

static ParallelBuffer pout_buffer;
static ParallelBuffer perr_buffer;
static ParallelBuffer plog_buffer;

ostream pout(&pout_buffer);
ostream perr(&perr_buffer);
ostream plog(&plog_buffer);

/*
*************************************************************************
*									*
* Initialie the parallel I/O streams.  This routine must be called	*
* before pout, perr, and plog are used for output but after MPI has	*
* been initialized.  By default, logging is disabled.			*
*									*
*************************************************************************
*/

void PIO::initialize()
{
   s_rank       = MPI::getRank();
   s_filestream = NULL;
   
   /*
    * Initialize the standard parallel output stream
    */

   pout_buffer.setActive(s_rank == 0);
   pout_buffer.setPrefixString(string());
   pout_buffer.setOutputStream1(&cout);
   pout_buffer.setOutputStream2(NULL);

   /*
    * Initialize the error parallel output stream
    */

   char buffer[16];
   sprintf(buffer, "P=%05d:", s_rank);

   perr_buffer.setActive(true);
   perr_buffer.setPrefixString(buffer);
   perr_buffer.setOutputStream1(&cerr);
   perr_buffer.setOutputStream2(NULL);

   /*
    * Initialize the parallel log file (disabled by default)
    */

   plog_buffer.setActive(false);
   plog_buffer.setPrefixString(string());
   plog_buffer.setOutputStream1(NULL);
   plog_buffer.setOutputStream2(NULL);
}

/*
*************************************************************************
*									*
* Close the output streams.  Flush both cout and cerr.  If logging,	*
* then flush and close the log stream.					*
*									*
*************************************************************************
*/

void PIO::finalize()
{
   cout.flush();
   cerr.flush();
   shutdownFilestream();
}

/*
*************************************************************************
*									*
* If the log file stream is open, then shut down the filestream.  Close	*
* and flush the channel and disconnect the output stream buffers.	*
*									*
*************************************************************************
*/

void PIO::shutdownFilestream()
{
   if (s_filestream) {
      s_filestream->flush();
      s_filestream->close();

      delete s_filestream;
      s_filestream = NULL;

      pout_buffer.setOutputStream2(NULL);
      perr_buffer.setOutputStream2(NULL);
      plog_buffer.setOutputStream1(NULL);
      plog_buffer.setActive(false);
   }
}

/*
*************************************************************************
*									*
* Log messages for node zero only.  If a log stream was open, close	*
* it.  If this is node zero, then open a new log stream and set the	*
* appropriate buffer streams to point to the log file.			*
*									*
*************************************************************************
*/

void PIO::logOnlyNodeZero(const string &filename)
{
   /*
    * If the filestream was open, then close it and reset streams
    */

   shutdownFilestream();

   /*
    * If this is node zero, then open the log stream and redirect output
    */

   if (s_rank == 0) {
      s_filestream = new ofstream(filename.c_str());
      if (!(*s_filestream)) {
         delete s_filestream;
         s_filestream = NULL;
         perr << "PIO: Could not open log file ``" << filename.c_str() << "''\n";
      } else {
         pout_buffer.setOutputStream2(s_filestream);
         perr_buffer.setOutputStream2(s_filestream);
         plog_buffer.setOutputStream1(s_filestream);
         plog_buffer.setActive(true);
      }
   }
}

/*
*************************************************************************
*									*
* Log messages for all nodes.  If a log stream was open, the close it.	*
* Open a log stream on every processor.  The filename for the log file	*
* will be appended with the processor number.				*
*									*
*************************************************************************
*/

void PIO::logAllNodes(const string &filename)
{
   /*
    * If the filestream was open, then close it and reset streams
    */

   shutdownFilestream();

   /*
    * Open the log stream and redirect output
    */

   char *buffer = new char[filename.length() + 16];
   sprintf(buffer, "%s.%05d", filename.c_str(), s_rank);
   s_filestream = new ofstream(buffer);

   if (!(*s_filestream)) {
      delete s_filestream;
      s_filestream = NULL;
      perr << "PIO: Could not open log file ``" << buffer << "''\n";
   } else {
      pout_buffer.setOutputStream2(s_filestream);
      perr_buffer.setOutputStream2(s_filestream);
      plog_buffer.setOutputStream1(s_filestream);
      plog_buffer.setActive(true);
   }

   delete [] buffer;
}

/*
*************************************************************************
*									*
* Suspend logging of data to the file stream.  This does not close the	*
* filestream (assuming it is open) but just disables logging.		*
*									*
*************************************************************************
*/

void PIO::suspendLogging()
{
   pout_buffer.setOutputStream2(NULL);
   perr_buffer.setOutputStream2(NULL);
   plog_buffer.setOutputStream1(NULL);
   plog_buffer.setActive(false);
}

/*
*************************************************************************
*									*
* Resume logging of the file stream (assuming it was open).  If the	*
* file stream is NULL, then do nothing.					*
*									*
*************************************************************************
*/

void PIO::resumeLogging()
{
   if (s_filestream) {
      pout_buffer.setOutputStream2(s_filestream);
      perr_buffer.setOutputStream2(s_filestream);
      plog_buffer.setOutputStream1(s_filestream);
      plog_buffer.setActive(true);
   }
}


}
}



