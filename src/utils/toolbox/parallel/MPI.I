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
// File:	MPI.I
// Package:	MPTCOUPLER toolbox
// 
// 
// 
// Description:	Simple utility class for interfacing with MPI
//

#ifdef DEBUG_NO_INLINE
#define inline
#endif

namespace MPTCOUPLER {
   namespace toolbox {


inline
void MPI::init(int* argc, char** argv[]) 
{
#ifdef HAVE_MPI
   int test;
   // Determine if MPI has been initialized and init if needed
   MPI_Initialized(&test);
   if (!test) {
      MPI_Init(argc,argv);
   }
#else
   (void) argc;
   (void) argv;
#endif
   initialize();
}

inline
void MPI::finalize()
{
#ifdef HAVE_MPI
   MPI_Finalize();
#endif
}

inline
void MPI::setCommunicator(MPI::comm communicator)
{
   s_communicator = communicator;
}

inline
MPI::comm MPI::getCommunicator()
{
   return(s_communicator);
}

inline
int MPI::getRank()
{
   int myid = 0;
#ifdef HAVE_MPI
   MPI_Comm_rank(s_communicator, &myid);
#endif
   return(myid);
}

inline
int MPI::getNodes()
{
   int nodes = 1;
#ifdef HAVE_MPI
   MPI_Comm_size(s_communicator, &nodes);
#endif
   return(nodes);
}

inline
void MPI::updateOutgoingStatistics(const int messages, const int bytes)
{
   s_outgoing_messages += messages;
   s_outgoing_bytes    += bytes;
}

inline
void MPI::updateIncomingStatistics(const int messages, const int bytes)
{
   s_incoming_messages += messages;
   s_incoming_bytes    += bytes;
}

inline
int MPI::getOutgoingMessages()
{
   return(s_outgoing_messages);
}

inline
int MPI::getOutgoingBytes()
{
   return(s_outgoing_bytes);
}

inline
int MPI::getIncomingMessages()
{
   return(s_incoming_messages);
}

inline
int MPI::getIncomingBytes()
{
   return(s_incoming_bytes);
}

#ifdef DEBUG_NO_INLINE
#undef inline
#endif

}
}


