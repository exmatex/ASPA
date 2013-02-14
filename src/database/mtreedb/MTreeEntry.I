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
// File:        MTreeEntry.I
// Package:     MPTCOUPLER MTree database
// 
// 
// 
// Description: Representation of data entry or routing entry in MTree
//

#ifdef DEBUG_CHECK_ASSERTIONS
#ifndef included_cassert
#define included_cassert
#include <cassert>
#endif
#endif

#ifdef DEBUG_NO_INLINE
#define inline
#endif

namespace MPTCOUPLER {
    namespace mtreedb {

/*
*************************************************************************
*                                                                       *
* Accessory routines to get type of entry and check whether defined.    *
*                                                                       *
*************************************************************************
*/

inline
bool MTreeEntry::isDefined() const
{
   return( isDataEntry() || isRoutingEntry() );
}

inline
bool MTreeEntry::isDataEntry() const
{
   return( d_entry_type == DATA_ENTRY );
}

inline
bool MTreeEntry::isRoutingEntry() const
{
   return( d_entry_type == ROUTING_ENTRY );
}

/*
*************************************************************************
*                                                                       *
* Accessory routine to get entry key.                                   *
*                                                                       *
*************************************************************************
*/

inline
const MTreeKey& MTreeEntry::getKey() const
{
   return( d_key );
}

/*
*************************************************************************
*                                                                       *
* Accessory routines to get node owning entry and                       *
* entry position in node owning entry.                                  *
*                                                                       *
*************************************************************************
*/

inline
MTreeNodePtr MTreeEntry::getNode() const
{
   return( d_my_node );
}

inline
int MTreeEntry::getPositionInNode() const
{
   return( d_my_position_in_node );
}

/*
*************************************************************************
*                                                                       *
* Accessory routines to get subtree node (if routing entry)             *
* and to get data object (if data entry).                               *
*                                                                       *
*************************************************************************
*/

inline
MTreeNodePtr MTreeEntry::getSubtreeNode() const
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(d_entry_type == ROUTING_ENTRY);
#endif
   return( d_subtree_node );
}

inline
int MTreeEntry::getDataObjectId() const
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(d_entry_type == DATA_ENTRY);
#endif
   return( d_data_object_id );
}

/*
*************************************************************************
*                                                                       *
* Accessory routines to facilitate operations on the entry key.         *
*                                                                       *
*************************************************************************
*/

inline
double MTreeEntry::computeDistanceTo(MTreeEntryPtr other) const
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(other.get());
#endif
   return( d_key.computeDistanceTo(other->getKey()) );
}

inline
void MTreeEntry::setDistanceToParent(double distance)
{
   d_key.setDistanceToParent(distance);
}

inline
double MTreeEntry::getDistanceToParent() const
{
   return( d_key.getDistanceToParent() );
}

inline
void MTreeEntry::setRadius(double radius)
{
   d_key.setRadius(radius);
}

inline
double MTreeEntry::getRadius() const
{
   return( d_key.getRadius() );
}

inline
MTreePointPtr MTreeEntry::getPoint() const
{
   return( d_key.getPoint() );
}


}
}

#ifdef DEBUG_NO_INLINE
#undef inline
#endif




