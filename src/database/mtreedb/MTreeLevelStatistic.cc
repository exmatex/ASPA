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
// File:        MTreeLevelStatistic.cc
// Package:     MPTCOUPLER MTree database
// 
// 
// 
// Description: Simple class for holding MTree statistic for a level in the tree
//

#ifndef included_mtreedb_MtreeLevelStatistic_C
#define included_mtreedb_MTreeLevelStatistic_C

#include "MTreeLevelStatistic.h"

#ifdef DEBUG_CHECK_ASSERTIONS
#ifndef included_cassert
#define included_cassert
#include <cassert>
#endif
#endif

namespace MPTCOUPLER {
    namespace mtreedb {

/*
*************************************************************************
*                                                                       *
* Ctor for MTree level statistic object.                                *
*                                                                       *
*************************************************************************
*/

MTreeLevelStatistic::MTreeLevelStatistic(
   int level_number,
   int max_num_nodes) 
:
   d_level_number(level_number)
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(level_number >= 0);
   assert(max_num_nodes >= 0);
#endif
   d_num_nodes_added = 0;
   d_node_stats.reserve(max_num_nodes);
}


/*
*************************************************************************
*                                                                       *
* Dtor for MTree level statistic object.                                *
*                                                                       *
*************************************************************************
*/

MTreeLevelStatistic::~MTreeLevelStatistic()
{
   d_level_number = -1;
   d_node_stats.clear();
}

/*
*************************************************************************
*                                                                       *
* Public member functions to access level statistic data.               *
*                                                                       *
*************************************************************************
*/

int MTreeLevelStatistic::getLevelNumber() const
{
   return(d_level_number);
}

int MTreeLevelStatistic::getNumberNodesOnLevel() const
{
   return( static_cast<int>(d_node_stats.size()) );
}

const MTreeNodeStat& MTreeLevelStatistic::getNodeStat(int indx) const
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(0 <= indx && static_cast<unsigned int>(indx) < d_node_stats.size());
#endif
   return(d_node_stats[indx]);
}

void MTreeLevelStatistic::printClassData(ostream& stream) const
{
   stream << "\nNode statistic data for tree level " 
          << d_level_number << endl;
   stream << "   " << d_num_nodes_added << " nodes..." << endl;
   for (int n = 0; n < d_num_nodes_added; ++n) {
      stream << "\n     Node entry # " << n << endl;
      d_node_stats[n].printClassData(stream);
   }
}

/*
*************************************************************************
*                                                                       *
* Private function to add MTreeNodeStat to this level statistic object. *
*                                                                       *
*************************************************************************
*/

void MTreeLevelStatistic::addNodeStat(MTreeNodePtr node)
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(node);
   assert(node->getLevelInTree() == d_level_number);
   assert(d_node_stats.capacity() > static_cast<unsigned int>(d_num_nodes_added));
#endif
   MTreeNodeStat nstat(node);
   d_node_stats.push_back(nstat);
   d_num_nodes_added++;
}

/*
*************************************************************************
*                                                                       *
* Private functions for setting number of objects in subtree of         *
* MTreeNodeStat objects.                                                *
*                                                                       *
*************************************************************************
*/

void MTreeLevelStatistic::getSubtreeNodeRealIds(
   int node_indx,
   vector<int>& sub_indx)
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(node_indx < d_num_nodes_added);
   assert(d_level_number > 0);
#endif
   const MTreeNodeStat& nstat = d_node_stats[node_indx];
   const int n_entries = nstat.d_node->getNumberEntries();
    
   sub_indx.clear();
   sub_indx.reserve(n_entries);

   for (int ie = 0; ie < n_entries; ++ie) {
      sub_indx.push_back( nstat.d_node->
                             getEntry(ie)->
                                getSubtreeNode()->
                                   getNodeId() );
   }

}

int 
MTreeLevelStatistic::getTotalNumberDataObjectsInSubtreeForRealNodeId(
   int real_node_id)
{
   int n_objects = 0;

   bool found = false;
   int n = 0; 
   while ( !found && (n < d_num_nodes_added) ) { 
      if ( d_node_stats[n].getNodeId() == real_node_id ) {
         n_objects = 
            d_node_stats[n].getTotalNumberDataObjectsInSubtree(); 
         found = true;
      }
      n++;
   }
   return( n_objects );
}

void MTreeLevelStatistic::setTotalNumberDataObjectsInSubtree(
   int node_indx,
   int num_objects)
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(node_indx < d_num_nodes_added);
#endif
    d_node_stats[node_indx].
       setTotalNumberDataObjectsInSubtree(num_objects);
 
}

}
}
#endif




