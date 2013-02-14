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
// File:        MTree.I
// Package:     MPTCOUPLER MTree database
// Description: Main Mtree index structure class.
//

#ifdef DEBUG_NO_INLINE
#define inline
#endif

namespace MPTCOUPLER {
    namespace mtreedb {

inline
string MTree::getName() const
{
   return(d_tree_name);
}

/*
*************************************************************************
*                                                                       *
* inline methods to set node parameters.                                * 
*                                                                       *
*************************************************************************
*/

inline 
void MTree::setNodePromotionMethod(
   MTreeNode::MTreeNodePromotionMethod method)
{
   d_node_promotion_method = method;
}

inline 
void MTree::setRootNodePromotionMethod(
   MTreeNode::MTreeNodePromotionMethod method)
{
   switch(method) {

      case MTreeNode::RANDOM_PROMOTION:
      case MTreeNode::MIN_RADII_PROMOTION:
      case MTreeNode::MIN_OVERLAP_PROMOTION: {
         d_root_node_promotion_method = method;
         break;
      }  // valid root promotion options

      default: {
      }  // use default method if not acceptable

   }
}

inline 
void MTree::setNodePartitionMethod(
   MTreeNode::MTreeNodePartitionMethod method,
   double min_utilization)
{
   d_node_partition_method = method;
   if ( (0.0 < min_utilization) && (min_utilization <= 0.5) ) {
      d_min_node_utilization = min_utilization;
   }
}

/*
*************************************************************************
*                                                                       *
* inline methods to get insert operation count information.             *
*                                                                       *
*************************************************************************
*/

inline
int MTree::getTotalInsertCount() const
{
   return(d_num_inserts);
}

inline
int MTree::getTotalInsertDistanceCount() const
{
   return(d_total_distance_comps_in_inserts);
}

inline
int MTree::getLastInsertDistanceCount() const
{
   return(d_num_distance_comps_in_last_insert);
}

/*
*************************************************************************
*                                                                       *
* inline methods to get delete operation count information.             *
*                                                                       *
*************************************************************************
*/

inline
int MTree::getTotalDeleteCount() const
{
   return(d_num_deletes);
}

inline
int MTree::getTotalDeleteDistanceCount() const
{
   return(d_total_distance_comps_in_deletes);
}

inline
int MTree::getLastDeleteDistanceCount() const
{
   return(d_num_distance_comps_in_last_delete);
}

/*
*************************************************************************
*                                                                       *
* inline methods to get KNN search operation count information.         *
*                                                                       *
*************************************************************************
*/

inline
int MTree::getTotalKNNSearchCount() const
{
   return(d_num_knn_queries);
}

inline
int MTree::getTotalKNNSearchDistanceCount() const
{
   return(d_total_distance_comps_in_knn_queries);
}

inline
int MTree::getLastKNNSearchDistanceCount() const
{
   return(d_num_distance_comps_in_last_knn_query);
}

/*
*************************************************************************
*                                                                       *
* inline methods to get range search operation count information.       *
*                                                                       *
*************************************************************************
*/

inline
int MTree::getTotalRangeSearchCount() const
{
   return(d_num_range_queries);
}

inline
int MTree::getTotalRangeSearchDistanceCount() const
{
   return(d_total_distance_comps_in_range_queries);
}

inline
int MTree::getLastRangeSearchDistanceCount() const
{
   return(d_num_distance_comps_in_last_range_query);
}


/*
*************************************************************************
*                                                                       *
* Private inline methods to get node parameters.                        * 
*                                                                       *
*************************************************************************
*/

inline
MTreeNode::MTreeNodePromotionMethod 
MTree::getRootNodePromotionMethod() const
{
   return( d_root_node_promotion_method );
}

inline
MTreeNode::MTreeNodePromotionMethod 
MTree::getNodePromotionMethod() const
{
   return( d_node_promotion_method );
}

inline
MTreeNode::MTreeNodePartitionMethod 
MTree::getNodePartitionMethod() const
{
   return( d_node_partition_method );
}

inline
double MTree::getMinNodeUtilization() const
{
   return( d_min_node_utilization );
}

/*
*************************************************************************
*                                                                       *
* Private inline members for incrementing and decrementing              *
* level node counts.                                                    *
*                                                                       *
*************************************************************************
*/

inline
void MTree::addNodeToLevelCount(int level)
{
   d_number_nodes_in_level[level]++;
}

inline
void MTree::removeNodeFromLevelCount(int level)
{
   d_number_nodes_in_level[level]--;
}


}
}

#ifdef DEBUG_NO_INLINE
#undef inline
#endif




