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
// File:        MTreeNodeStat.cc
// Package:     MPTCOUPLER MTree database
// 
// 
// 
// Description: Simple class for holding MTree statistic data for a node in tree
//

#ifndef included_mtreedb_MtreeNodeStat_C
#define included_mtreedb_MTreeNodeStat_C

#include "MTreeNodeStat.h"

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
* Ctors for MTreeNodeStat object.                                       *
*                                                                       *
*************************************************************************
*/

MTreeNodeStat::MTreeNodeStat()
{
}

MTreeNodeStat::MTreeNodeStat(MTreeNodePtr node)
:
   d_node(node),
   d_total_objects_in_subtree(0)
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(node);
#endif

   if ( d_node->isLeaf() ) {
      const int nentries = d_node->getNumberEntries();
      d_node_object_ids.reserve(nentries);
      for (int ie = 0; ie < nentries; ++ie) {
         d_node_object_ids.push_back(d_node->getEntry(ie)->getDataObjectId());
      }
      d_total_objects_in_subtree = nentries;
   }
}

MTreeNodeStat::MTreeNodeStat(const MTreeNodeStat& nstat)
:
   d_node(nstat.d_node),
   d_total_objects_in_subtree(nstat.d_total_objects_in_subtree),
   d_node_object_ids(nstat.d_node_object_ids)
{
}

/*
*************************************************************************
*                                                                       *
* Dtor for MTreeNodeStat object.                                        *
*                                                                       *
*************************************************************************
*/

MTreeNodeStat::~MTreeNodeStat()
{
   d_node.reset();
   d_node_object_ids.clear();
}

/*
*************************************************************************
*                                                                       *
* Copy assignment operatoy for MTreeNodeStat object.                    *
*                                                                       *
*************************************************************************
*/

MTreeNodeStat& MTreeNodeStat::operator=(const MTreeNodeStat& rhs)
{
   d_node                     = rhs.d_node;
   d_total_objects_in_subtree = rhs.d_total_objects_in_subtree;
   d_node_object_ids          = rhs.d_node_object_ids;
   return(*this);
}

/*
*************************************************************************
*                                                                       *
* Public member functions to access node statistic data.                *
*                                                                       *
*************************************************************************
*/

int MTreeNodeStat::getNodeId() const 
{
   return( d_node->getNodeId() );
}

int MTreeNodeStat::getLevelNumber() const
{
   return( d_node->getLevelInTree() );
}

int MTreeNodeStat::getNumberEntries() const
{
   return( d_node->getNumberEntries() );
}

bool MTreeNodeStat::isRoot() const
{
   return( d_node->isRoot() );
}

bool MTreeNodeStat::isLeaf() const
{
   return( d_node->isLeaf() );
}

double MTreeNodeStat::getCoveringRadius() const
{
   double radius = -1.0;
   if ( !d_node->isRoot() ) {
      radius = d_node->getParentEntry()->getRadius();
   }
   return( radius );
}

MTreePointPtr MTreeNodeStat::getCenterPoint() const
{
   MTreePointPtr point;
   if ( !d_node->isRoot() ) {
      point = d_node->getParentEntry()->getPoint();
   }
   return( point );
}

int MTreeNodeStat::getTotalNumberDataObjectsInSubtree() const
{
   return( d_total_objects_in_subtree );
}

const vector<int>& MTreeNodeStat::getDataObjectIds() const
{
   return( d_node_object_ids ); 
}

void MTreeNodeStat::printClassData(ostream& stream) const
{
   stream << "        Node id = " << getNodeId() << endl;
   stream << "        Num entries = " << getNumberEntries() << endl;
   stream << "        Num data objects in subtree = "
          << getTotalNumberDataObjectsInSubtree() << endl;
   stream << "        Root node = " << isRoot() << endl;
   stream << "        Leaf node = " << isLeaf() << endl;

   if ( !isRoot() ) {
      stream << "        Covering radius = " 
             << getCoveringRadius() << endl;
      stream << "        Center point = "; 
                getCenterPoint()->print(stream);
   }

   if ( isLeaf() ) {
      stream << "        Owns objects ";
      unsigned int ie = 0;
      for ( ; ie < d_node_object_ids.size() - 1; ++ie) {
         stream << d_node_object_ids[ie] << " , ";
      }
      if (ie > 0) {
         stream << d_node_object_ids[ie] << endl;
      }
   }
}

void MTreeNodeStat::setTotalNumberDataObjectsInSubtree(int n)
{
   d_total_objects_in_subtree = n;
}


}
}
#endif




