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
// File:        MTreeSearchResult.I
// Package:     MPTCOUPLER MTree database
// 
// 
// 
// Description: Container for single data object resulting from MTree search
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
* Default ctor, copy ctor, and copy assignment operator.                *
*                                                                       *
*************************************************************************
*/

inline
MTreeSearchResult::MTreeSearchResult()
:
   d_distance_to_query_point( -MTreePoint::getMaxDistance() ),
   d_data_object_id( MTreeObject::getUndefinedId() ),
   d_data_object_radius( -MTreePoint::getMaxDistance() ),
   d_is_valid_result(false)
{
}

inline
MTreeSearchResult::MTreeSearchResult(const MTreeSearchResult& result)
:
   d_query_point(result.d_query_point),
   d_distance_to_query_point(result.d_distance_to_query_point),
   d_data_object(result.d_data_object),
   d_data_object_id(result.d_data_object_id),
   d_data_object_point(result.d_data_object_point),
   d_data_object_radius(result.d_data_object_radius),
   d_is_valid_result(result.d_is_valid_result)
{
}

inline 
MTreeSearchResult& 
MTreeSearchResult::operator=(const MTreeSearchResult& rhs)
{
   d_query_point = rhs.d_query_point;
   d_distance_to_query_point = rhs.d_distance_to_query_point;
   d_data_object = rhs.d_data_object;
   d_data_object_id = rhs.d_data_object_id;
   d_data_object_point = rhs.d_data_object_point;
   d_data_object_radius = rhs.d_data_object_radius;
   d_is_valid_result = rhs.d_is_valid_result;

   return(*this);
}

/*
*************************************************************************
*                                                                       *
* Accessory functions to get data members.                              * 
*                                                                       *
*************************************************************************
*/

inline
const MTreeObject& MTreeSearchResult::getDataObject() const
{
   return( *(d_data_object.get()) );
}

inline 
double MTreeSearchResult::getDistanceToQueryPoint() const
{
   return( d_distance_to_query_point );
}

inline
const MTreePoint& MTreeSearchResult::getDataObjectPoint() const
{
   return( *(d_data_object_point.get()) );
}

inline 
double MTreeSearchResult::getDataObjectRadius() const
{
   return( d_data_object_radius );
}

inline
const MTreePoint& MTreeSearchResult::getQueryPoint() const
{
   return( *(d_query_point.get()) );
}

/*
*************************************************************************
*                                                                       *
* Less-than comparison operator for STL list sort() method.             *
*                                                                       *
*************************************************************************
*/

inline
int MTreeSearchResult::operator< (const MTreeSearchResult& rhs) const
{
   int ret_val = 0;
   if ( d_distance_to_query_point < rhs.d_distance_to_query_point ) {
      ret_val = 1;
   }
   return( ret_val );
}

/*
*************************************************************************
*                                                                       *
* Private ctor for MTree search result.                                 *
*                                                                       *
*************************************************************************
*/

inline 
MTreeSearchResult::MTreeSearchResult(MTreePointPtr query_point,
                                     MTreeEntryPtr entry)
:
   d_query_point(query_point),
   d_distance_to_query_point(MTreePoint::getMaxDistance()),
   d_data_object_id(entry->getDataObjectId()),
   d_data_object_point(entry->getPoint()),
   d_data_object_radius(entry->getRadius()),
   d_is_valid_result(true)
{
#ifdef DEBUG_CHECK_ASSERTIONS
   assert(query_point.get());
   assert(entry.get());
#endif
}

/*
*************************************************************************
*                                                                       *
* Private accessory functions to set data members.                      * 
*                                                                       *
*************************************************************************
*/

inline
void MTreeSearchResult::setQueryPoint(MTreePointPtr query_point) 
{
   d_query_point = query_point;
}

inline
void MTreeSearchResult::setDistanceToQueryPoint(double distance)
{
   d_distance_to_query_point = distance;
}

inline
bool MTreeSearchResult::isValidResult() const
{
   return( d_is_valid_result );
}


}
}

#ifdef DEBUG_NO_INLINE
#undef inline
#endif




