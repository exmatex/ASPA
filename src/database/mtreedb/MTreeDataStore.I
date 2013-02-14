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
// File:        MTreeDataStore.I
// Package:     MPTCOUPLER MTree database
// 
// 
// 
// Description: Manager class for MTree node allocation and storage of data objects.
//

#ifndef included_mtreedb_MTreeNode
#include "MTreeNode.h"
#endif

#ifdef DEBUG_NO_INLINE
#define inline
#endif

namespace MPTCOUPLER {
    namespace mtreedb {

/*
*************************************************************************
*                                                                       *
* Inline method to check initialization status of data store.           *
*                                                                       *
*************************************************************************
*/

inline
bool MTreeDataStore::isInitialized() const
{
   return( d_is_initialized );
}

/*
*************************************************************************
*                                                                       *
* Inline methods for private leaf node info utility class.              *
*                                                                       *
*************************************************************************
*/

inline
MTreeDataStore::LeafNodeInfo::LeafNodeInfo(MTreeNodePtr node) 
:
   d_node_ptr(node),
   d_in_file(false),
   d_in_memory(false)
{
   d_object_ids.reserve( node->getMaxEntries() );
}

inline
MTreeDataStore::LeafNodeInfo::~LeafNodeInfo()
{
   d_node_ptr.reset();
   d_object_ids.clear();
}

inline
MTreeNodePtr MTreeDataStore::LeafNodeInfo::getNode() const 
{ 
   return d_node_ptr; 
}

inline
vector<int>& MTreeDataStore::LeafNodeInfo::getObjectIds() 
{
   return d_object_ids; 
}

inline 
void MTreeDataStore::LeafNodeInfo::setInFile(bool value) 
{ 
   d_in_file = value; 
}

inline 
bool MTreeDataStore::LeafNodeInfo::getInFile() const 
{ 
   return d_in_file; 
}

inline 
void MTreeDataStore::LeafNodeInfo::setInMemory(bool value) 
{ 
   d_in_memory = value; 
}

inline 
bool MTreeDataStore::LeafNodeInfo::getInMemory() const 
{ 
   return d_in_memory; 
}


/*
*************************************************************************
*                                                                       *
* Inline methods for private data object info utility class.            *
*                                                                       *
*************************************************************************
*/

inline
MTreeDataStore::ObjectInfo::ObjectInfo(MTreeObjectPtr object)
:
   d_object_ptr(object),
   d_owner_leaf_node_id( MTreeNode::getUndefinedId() ),
   d_owner_leaf_node_entry_position(-1),
   d_in_file(false),
   d_in_memory(false),
   d_object_file_index(-1)
{
}

inline
MTreeDataStore::ObjectInfo::~ObjectInfo()
{
   d_object_ptr.reset();
}

inline
MTreeObjectPtr MTreeDataStore::ObjectInfo::getObject() const 
{ 
   return d_object_ptr; 
}

inline 
void MTreeDataStore::ObjectInfo::resetObjectPtr() 
{ 
   d_object_ptr.reset(); 
}

inline 
void MTreeDataStore::ObjectInfo::setObjectPtr(MTreeObjectPtr obj) 
{ 
   d_object_ptr = obj; 
}

inline 
void MTreeDataStore::ObjectInfo::setOwnerLeafNodeInfo(
   int leaf_node_id,
   int entry_position) 
{ 
   d_owner_leaf_node_id = leaf_node_id; 
   d_owner_leaf_node_entry_position = entry_position; 
}

inline 
int MTreeDataStore::ObjectInfo::getOwnerLeafNodeId() const 
{ 
   return d_owner_leaf_node_id; 
}

inline
int MTreeDataStore::ObjectInfo::getOwnerLeafNodeEntryPosition() const
{
   return d_owner_leaf_node_entry_position;
}

inline 
void MTreeDataStore::ObjectInfo::setInFile(bool value) 
{ 
   d_in_file = value; 
}

inline 
bool MTreeDataStore::ObjectInfo::getInFile() const 
{ 
   return d_in_file; 
}

inline 
void MTreeDataStore::ObjectInfo::setInMemory(bool value) 
{ 
   d_in_memory = value; 
}

inline 
bool MTreeDataStore::ObjectInfo::getInMemory() const 
{ 
   return d_in_memory; 
}

inline 
void MTreeDataStore::ObjectInfo::setFileIndex(int index) 
{ 
   d_object_file_index = index; 
}

inline 
int MTreeDataStore::ObjectInfo::getFileIndex() const 
{ 
   return d_object_file_index; 
}

/*
*************************************************************************
*                                                                       *
* Inline methods for private object file info utility class.            *
*                                                                       *
*************************************************************************
*/

inline
MTreeDataStore::ObjectFileInfo::ObjectFileInfo(const string& file_name) 
:
   d_file_name(file_name)
{
   d_object_ids.reserve( MTREE_DATA_STORE_OBJECT_FILE_CAPACITY );
}

inline
MTreeDataStore::ObjectFileInfo::~ObjectFileInfo()
{
   d_object_ids.clear();
}

inline
void MTreeDataStore::ObjectFileInfo::addObjectId(int object_id)
{
   d_object_ids.push_back(object_id);
}

inline
const string& MTreeDataStore::ObjectFileInfo::getFileName() const 
{ 
   return d_file_name; 
}

inline
const vector<int>& 
MTreeDataStore::ObjectFileInfo::getObjectIds() const 
{ 
   return d_object_ids;
}

inline 
bool MTreeDataStore::ObjectFileInfo::fileIsEmpty() const
{ 
   return( d_object_ids.empty() );
}

inline 
bool MTreeDataStore::ObjectFileInfo::fileIsFull() const
{ 
   return( d_object_ids.size() == d_object_ids.capacity() );
}


}
}

#ifdef DEBUG_NO_INLINE
#undef inline
#endif




