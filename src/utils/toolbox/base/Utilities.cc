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
// File:        Utilities.cc
// Package:     MPTCOUPLER toolbox
// 
// 
// 
// Description: A collection of trivial utility functions such as min and max
//

#include "toolbox/base/Utilities.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <assert.h>

#include "toolbox/parallel/MPI.h"
#include "toolbox/base/PIO.h"


namespace MPTCOUPLER {
   namespace toolbox {


void Utilities::renameFile(const string& old_filename,
                           const string& new_filename)
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(!old_filename.empty());
   assert(!new_filename.empty());
#endif
   rename(old_filename.c_str(), new_filename.c_str());
}

void Utilities::recursiveMkdir(
   const string& path, 
   mode_t mode,
   bool only_node_zero_creates)
{

#ifdef _MSC_VER
   const char seperator = '/';
#define mkdir(path, mode) mkdir(path)
#else
   const char seperator = '/';
#endif


   if ( (!only_node_zero_creates) || (MPI::getRank() == 0)) {
      int length = path.length();
      char *path_buf= new char[length+1];
      sprintf(path_buf,"%s",path.c_str());
      struct stat status;
      int pos = length - 1;
   
      /* find part of path that has not yet been created */
      while ( (stat(path_buf,&status) != 0) && (pos >= 0) ) {
   
         /* slide backwards in string until next slash found */
         bool slash_found = false;
         while ( (!slash_found) && (pos >= 0) ) {
           if (path_buf[pos] == seperator) {
              slash_found = true;
              if (pos >= 0) path_buf[pos] = '\0';
           } else pos--;
         }
      } 

      /* 
       * if there is a part of the path that already exists make sure
       * it is really a directory
       */
      if (pos >= 0) {
         if ( !S_ISDIR(status.st_mode) ) {
            TBOX_ERROR("Error in Utilities::recursiveMkdir...\n"
               << "    Cannot create directories in path = " << path
               << "\n    because some intermediate item in path exists and"
               << "is NOT a directory" << endl);
         }
      }
   
      /* make all directories that do not already exist */
   
      /* 
       * if (pos < 0), then there is no part of the path that
       * already exists.  Need to make the first part of the 
       * path before sliding along path_buf.
       */
      if (pos < 0) {
	 if(mkdir(path_buf,mode) != 0) {
	    TBOX_ERROR("Error in Utilities::recursiveMkdir...\n"
		       << "    Cannot create directory  = " 
		       << path_buf << endl);
	 }
	 pos = 0;
      }
   
      /* make rest of directories */
      do {
   
         /* slide forward in string until next '\0' found */
         bool null_found = false;
         while ( (!null_found) && (pos < length) ) {
           if (path_buf[pos] == '\0') {
              null_found = true;
              path_buf[pos] = seperator;
           }
           pos++;
         }
   
         /* make directory if not at end of path */
	 if (pos < length) {
	    if(mkdir(path_buf,mode) != 0) {
	       TBOX_ERROR("Error in Utilities::recursiveMkdir...\n"
			  << "    Cannot create directory  = " 
			  << path_buf << endl);
	    }
	 }
      } while (pos < length);

      delete [] path_buf;
   }

   /* 
    * Make sure all processors wait until node zero creates 
    * the directory structure.
    */
   if (only_node_zero_creates) {
      MPI::barrier();
   }
}

string Utilities::intToString(int num, int min_width)
{
   int tmp_width = ( min_width > 0 ? min_width : 1 );
   ostringstream os;
   if ( num < 0 ) {
      os << '-' << setw(tmp_width-1) << setfill('0') << -num;
   } else {
      os << setw(tmp_width) << setfill('0') << num;
   }
   os << flush;

   return(os.str()); //returns the string form of the stringstream object
}

void Utilities::abort(const string &message, 
		           const string &filename, 
			   const int line) 
{
   perr << "Program abort called in file ``" << filename
        << "'' at line " << line << endl;
   perr << "ERROR MESSAGE: " << endl << message.c_str() << endl;
   perr << flush;

   MPI::abort();
}

void Utilities::printWarning(const string &message, 
   	                     const string &filename, 
        		     const int line) 
{
   plog << "Warning in file ``" << filename
        << "'' at line " << line << endl;
   plog << "WARNING MESSAGE: " << endl << message.c_str() << endl;
   plog << flush;
}


}
}




