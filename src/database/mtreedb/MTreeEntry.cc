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
// File:        MTreeEntry.cc
// Package:     MPTCOUPLER MTree database
// 
// 
// 
// Description: Representation of data entry or routing entry in MTree
//

#ifndef included_mtreedb_MtreeEntry_C
#define included_mtreedb_MTreeEntry_C

#include "MTreeEntry.h"

#include "toolbox/base/MathUtilities.h"
#include "MTreeNode.h"

#ifdef DEBUG_NO_INLINE
#include "MTreeEntry.I"
#endif

#include <assert.h>

namespace MPTCOUPLER {
    namespace mtreedb {

/*
*************************************************************************
*                                                                       *
* Ctor for MTreeEntry initializes key and set entry type and position   *
* in node to be undefined.                                              *
*                                                                       *
*************************************************************************
*/
 
MTreeEntry::MTreeEntry(const MTreeKey& key)
: d_entry_type(UNDEFINED_ENTRY),
  d_key(key),
  d_my_node_id(MTreeNode::getUndefinedId()),
  d_my_position_in_node(-1),
  d_subtree_node_id(MTreeNode::getUndefinedId()),
  d_data_object_id(MTreeObject::getUndefinedId())
{
}

/*
*************************************************************************
*                                                                       *
* Dtor for entry.                                                       *
*                                                                       *
*************************************************************************
*/

MTreeEntry::~MTreeEntry()
{
   d_my_node.reset();
   d_subtree_node.reset();
}

/*
*************************************************************************
*                                                                       *
* Accessory routines to set node owning entry, position in node,        *
* subtree node, and object of entry.                                    *
*                                                                       *
*************************************************************************
*/
 
void MTreeEntry::setNode(MTreeNodePtr node)
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(node.get());
#endif
   d_my_node = node;
   d_my_node_id = node->getNodeId();
   if (d_my_node->isRoot()) {
      setDistanceToParent( MTreeKey::getUndefinedDistanceToParent() );
   }
}

void MTreeEntry::setPositionInNode(int pos)
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(d_my_node.get());
   assert(0 <= pos && pos <= d_my_node->getMaxEntries());
#endif
   d_my_position_in_node = pos;
}

void MTreeEntry::setSubtreeNode(MTreeNodePtr subtree)
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(subtree.get());
#endif
   d_entry_type = ROUTING_ENTRY;
   d_subtree_node = subtree;
   d_subtree_node_id = subtree->getNodeId();
   d_data_object_id = MTreeObject::getUndefinedId();
}

void MTreeEntry::setDataObjectId(int object_id)
{
   d_entry_type = DATA_ENTRY;
   d_data_object_id = object_id;
   d_subtree_node.reset();
   d_subtree_node_id = MTreeNode::getUndefinedId();
}

/*
*************************************************************************
*                                                                       *
* Check consistency of this entry.                                      *
*                                                                       *
*************************************************************************
*/
 
bool MTreeEntry::checkConsistency(MTreeNodePtr my_node,
                                  ostream& stream) const
{
   assert(my_node.get());

   bool entry_is_consistent = true;

   if ( d_entry_type == UNDEFINED_ENTRY ) {
      stream << "MTREE ENTRY ERROR: Entry type is undefined!" << endl;
      entry_is_consistent = false;
   }

   if ( !d_my_node.get() ) {
      stream << "MTREE ENTRY ERROR: "
             << "Entry has null pointer to owning node!" << endl;
      entry_is_consistent = false;
   }

   if ( d_my_node.get() != my_node.get() ) {
      stream << "MTREE ENTRY ERROR: "
             << "Entry owning node not same as node "
             << "passed to checkConsistency() method!" << endl;
      entry_is_consistent = false;
   }

   if ( !d_my_node.get() ) {
      stream << "MTREE ENTRY ERROR: "
             << "Entry has null pointer to owning node!" << endl;
      entry_is_consistent = false;
   }
   
   if ( d_my_node->isRoot() ) {
      if ( getDistanceToParent() != 
           MTreeKey::getUndefinedDistanceToParent() ) {
         stream << "MTREE ENTRY ERROR: root node entry distance-to-parent " 
                << " is not undefined!" << endl;
         entry_is_consistent = false;
      } 
   } else {
      double my_dist2par = getDistanceToParent();
      double actual_dist2par = 
         computeDistanceTo( d_my_node->getParentEntry() );
      if ( !toolbox::MathUtilities<double>::equalEps(
            my_dist2par, actual_dist2par) ) {
         stream << "MTREE ENTRY ERROR: distance-to-parent incorrect!" 
                << "\n  my_dist2par = " << my_dist2par << endl
                << "  actual_dist2par = " << actual_dist2par << endl;
         entry_is_consistent = false;
      }
   }

   if ( d_my_node->isLeaf() ) {
      
      if ( d_entry_type != DATA_ENTRY ) {
         stream << "MTREE ENTRY ERROR: "
                << "Leaf node entry type not set to data entry!" << endl;
         entry_is_consistent = false;
      } 

      if ( d_subtree_node.get() ) {
         stream << "MTREE ENTRY ERROR: "
                << "Subtree node set for leaf node entry!" << endl;
         entry_is_consistent = false;
      }

      if ( d_data_object_id == MTreeObject::getUndefinedId() ) {
         stream << "MTREE ENTRY ERROR: "
                << "Data object id not set for leaf node entry!" << endl;
         entry_is_consistent = false;
      }

   } else {  // internal node entry

      if ( d_entry_type != ROUTING_ENTRY ) {
         stream << "MTREE ENTRY ERROR: "
                << "Non-leaf node entry type not set to routing entry!" 
                << endl;
         entry_is_consistent = false;
      } 

      if ( !d_subtree_node.get() ) {
         stream << "MTREE ENTRY ERROR: "
                << "Subtree node not set for non-leaf node entry!"
                << endl;
         entry_is_consistent = false;
      }

      if ( d_data_object_id != MTreeObject::getUndefinedId() ) {
         stream << "MTREE ENTRY ERROR: "
                << "Data object id defined for non-leaf node entry!"
                << endl;
         entry_is_consistent = false;
      }

      double subtree_radius = 0.0;
      for (int ie = 0; ie < d_subtree_node->getNumberEntries(); ++ie) {
         subtree_radius = 
            toolbox::MathUtilities<double>::Max(
               subtree_radius, 
               d_subtree_node->getEntry(ie)->getDistanceToParent() +
               d_subtree_node->getEntry(ie)->getRadius() );
      }
      if ( getRadius() < subtree_radius ) {
         stream << "MTREE ENTRY ERROR: Covering radius too small!" << endl;
         stream << "  my_radius = " << getRadius() << endl;
         stream << "  subtree_radius = " << subtree_radius << endl;
         entry_is_consistent = false;
      }

   }

   if ( !entry_is_consistent ) {
      printClassData(stream);
   }

   return( entry_is_consistent );
}

/*
*************************************************************************
*                                                                       *
* Print entry to given output stream.                                   *
*                                                                       *
*************************************************************************
*/

void MTreeEntry::printClassData(ostream& stream) const
{
   stream << "MTreeEntry::printClassData()\n";
   stream << "--------------------------------------\n";
   stream << "this ptr = " << (MTreeEntry*)this << endl;
   stream << "d_entry_type = " << d_entry_type << endl;
   stream << "isDefined() = " << isDefined() << endl;
   stream << "isDataEntry() = " << isDataEntry() << endl;
   stream << "isRoutingEntry() = " << isRoutingEntry() << endl;
   stream << "d_my_node = " << (MTreeNode*)d_my_node.get() << endl;
   stream << "d_my_node_id = " << d_my_node_id << endl;
   stream << "d_my_position_in_node = " << d_my_position_in_node << endl;
   stream << "d_subtree_node = " << (MTreeNode*)d_subtree_node.get() << endl;
   stream << "d_subtree_node_id = " << d_subtree_node_id << endl;
   stream << "d_data_object_id = " << d_data_object_id << endl;

   stream << "\nPrinting entry key..." << endl;
   d_key.printClassData(stream);

   stream << "\n" << endl;
}


}
}
#endif




