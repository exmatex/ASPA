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
// File:        MTreeKey.I
// Package:     MPTCOUPLER MTree database
// 
// 
// 
// Description: Description of metric space region associated with each MTree entry
//

#ifndef included_toolbox_MathUtilities
#include "toolbox/base/MathUtilities.h"
#endif

#ifdef DEBUG_NO_INLINE
#define inline
#endif

namespace MPTCOUPLER {
    namespace mtreedb {

/*
*************************************************************************
*                                                                       *
* Constructors for MTree key objects.                                   *
*                                                                       *
*************************************************************************
*/

inline 
MTreeKey::MTreeKey()
:
   d_radius(toolbox::MathUtilities<double>::getMax()),
   d_distance_to_parent( getUndefinedDistanceToParent() )
{
}

inline
MTreeKey::MTreeKey(const MTreeKey& key)
:
   d_point(key.d_point),
   d_radius(key.d_radius),
   d_distance_to_parent(key.d_distance_to_parent)
{
}

inline
MTreeKey::MTreeKey(
   const MTreePointPtr point,
   double radius,
   double dist2parent)
:
   d_point(point),
   d_radius(radius),
   d_distance_to_parent(dist2parent)
{
}

/*
*************************************************************************
*                                                                       *
* Copy assignment operatoy for key.                                     *
*                                                                       *
*************************************************************************
*/

inline
MTreeKey& MTreeKey::operator=(const MTreeKey& rhs)
{
   d_point              = rhs.d_point;
   d_radius             = rhs.d_radius;
   d_distance_to_parent = rhs.d_distance_to_parent;

   return(*this);
}

/*
*************************************************************************
*                                                                       *
* Accessory function to get center point of key region.                 *
*                                                                       *
*************************************************************************
*/

inline 
const MTreePointPtr MTreeKey::getPoint() const 
{
   return(d_point);
}

/*
*************************************************************************
*                                                                       *
* Accessory functions to get and set radii of key region.               *
*                                                                       *
*************************************************************************
*/

inline
double MTreeKey::getRadius() const
{
   return(d_radius);
}

inline
void MTreeKey::setRadius(double radius)
{
   d_radius = radius;
}

/*
*************************************************************************
*                                                                       *
* Accessory functions to get and set distance to center of parent       *
* region and to compute distance between the centers of two regions.    *
*                                                                       *
*************************************************************************
*/

inline
double MTreeKey::getDistanceToParent() const
{
   return(d_distance_to_parent);
}

inline
void MTreeKey::setDistanceToParent(double distance)
{
   d_distance_to_parent = distance;
}

inline
double MTreeKey::computeDistanceTo(const MTreeKey& other) const
{
   return( d_point->computeDistanceTo( other.d_point ) );
}


}
}

#ifdef DEBUG_NO_INLINE
#undef inline
#endif




