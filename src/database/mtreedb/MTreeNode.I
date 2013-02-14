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
// File:        MTreeNode.I
// Package:     MPTCOUPLER MTree database
// 
// 
// 
// Description: Representation of node in an MTree.
//

#ifdef DEBUG_CHECK_ASSERTIONS
#ifndef included_cassert
#define included_cassert
#include <cassert>
#endif
#endif

#ifndef included_toolbox_Utilities
#include "toolbox/base/Utilities.h"
#endif

#ifndef included_mtreedb_MTree
#include "MTree.h"
#endif

#ifdef DEBUG_NO_INLINE
#define inline
#endif

namespace MPTCOUPLER {
    namespace mtreedb {

/*
*************************************************************************
*                                                                       *
* Accessory functions to get and set node id and get shared pointer     *
* to this object.                                                       *
*                                                                       *
*************************************************************************
*/

inline
int MTreeNode::getNodeId() const
{
   return(d_node_id);
}

/*
*************************************************************************
*                                                                       *
* Accessory routines to check type of node and whether it is defined,   * 
* and also to set node level in tree.                                   *
*                                                                       *
*************************************************************************
*/

inline
bool MTreeNode::isDefined() const
{
   return( d_node_id > MTreeNode::getUndefinedId() );
}

inline
bool MTreeNode::isRoot() const
{
   return( d_is_root_node );
}

inline
bool MTreeNode::isLeaf() const
{
   return( d_level_in_tree == 0 );
}

inline
int MTreeNode::getMaxEntries() const
{
   return( d_max_entries );
}

inline
void MTreeNode::setRootNode(bool is_root)
{
   d_is_root_node = is_root;
   d_parent_entry.reset();
}

inline
int MTreeNode::getLevelInTree() const
{
   return( d_level_in_tree );
}

/*
*************************************************************************
*                                                                       *
* Accessory functions to check number of entries in node and to         *
* retrieve the entry at a given position.                               *
*                                                                       *
*************************************************************************
*/

inline
int MTreeNode::getNumberEntries() const
{
   return( d_entries.size() );
}

inline
bool MTreeNode::isOverfull() const
{
   const int num_entries = d_entries.size();
   return( num_entries > d_max_entries );
}

inline
MTreeEntryPtr MTreeNode::getEntry(int position) const
{
   MTreeEntryPtr ret_entry;
   if ( (position >= 0) && (position < getNumberEntries()) ) {
      ret_entry = d_entries[position];
   }
   return( ret_entry );
}

/*
*************************************************************************
*                                                                       *
* Accessory function to get parent node of this node if it exists.      *
*                                                                       *
*************************************************************************
*/

inline
MTreeNodePtr MTreeNode::getParentNode() const
{
   MTreeNodePtr ret_node;
   if ( d_parent_entry.get() ) {
      ret_node = d_parent_entry->getNode(); 
   }
   
   return( ret_node );
}

/*
*************************************************************************
*                                                                       *
* Accessory function to set and get parent entry of this node.          *
*                                                                       *
*************************************************************************
*/

inline
void MTreeNode::setParentEntry(MTreeEntryPtr parent)
{
   if ( isRoot() ) {
      TBOX_ERROR("MTreeNode::setParentEntry() error..."
                 << " Cannot set parent entry of root node!" << endl);
   }
   d_parent_entry = parent;
}

inline
MTreeEntryPtr MTreeNode::getParentEntry() const
{
   return( d_parent_entry );
}

/*
*************************************************************************
*                                                                       *
* Private accessory function to set and get leaf node id.               *
*                                                                       *
*************************************************************************
*/

inline
int MTreeNode::getLeafNodeId() const
{
   return(d_leaf_node_id);
}

inline
void MTreeNode::setLeafNodeId(int id)
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(d_leaf_node_id == MTreeNode::getUndefinedId());
   assert(id >= 0);
#endif
   d_leaf_node_id = id;
}


}
}

#ifdef DEBUG_NO_INLINE
#undef inline
#endif




