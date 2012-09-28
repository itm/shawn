/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MATH_TRIANGULATION_H
#define __SHAWN_APPS_LOCALIZATION_MATH_TRIANGULATION_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "sys/vec.h"
#include "apps/localization/neighborhood/localization_neighborhood.h"
#include "apps/localization/util/localization_defutils.h"

namespace localization
{

   enum LaterationType
   {
      lat_anchors,  ///< only valid anchors
      lat_confident ///< only confident neighbors
   };

   /** Position estimation with distance to anchors and their positions.
    *  Main idea is set a bounding box, defined by position and distance,
    *  around each anchor. The estimated position is the center of the
    *  resulting intersection box.
    *
    *  \param NeighborInfoList Neighbors, for which above mentioned
    *    bounding boxes are builded
    *  \param Vec New, estimated position
    *  \result \c true, if estimation succeeded. \c false otherwise
    *  \sa est_pos_lateration()
    */
   bool est_pos_min_max( const NeighborInfoList&, shawn::Vec& )
      throw();
   /** Position estimation with distance to anchors and their positions.
    *  This method uses lateration for position estimation. Main idea is
    *  to solve a system of equations, using here a least squares approach.
    *
    *  Cause of different requirements, there is a chance to use standard
    *  or weighted least squares approach.
    *
    *  \param NeighborInfoList Neighbors, for which above mentioned
    *    least squares approach is started
    *  \param Vec New, estimated position
    *  \param LaterationType Type of lateration to decide, whether using
    *    standard or weighted least squares approach
    *  \param bool if \c true, given position is used in least squares
    *    approach
    *  \result \c true, if estimation succeeded. \c false otherwise
    *  \sa est_pos_min_max(), collect_neighbors(), LaterationType
    */
   bool est_pos_lateration(
         const NeighborInfoList&,
         shawn::Vec&,
         const LaterationType&,
         bool )
      throw();
   /** This Method checks, whether an estimated position is accepted or
    *  not. It uses the distance residues between given ones and estimated.
    *  Formula looks like following:
    *  \f[residue = \frac{\sum^n_{i=1}\sqrt{(x_i-x)^2+(y_i-y)^2}-d_i}{n}\f]
    *
    *  \param NeighborInfoList Neighbors, for which sanity check is started
    *  \param Vec estimated position
    *  \param LaterationType Type of lateration to decide, whether use
    *    weights in calculation or not
    *  \param double Simulations communication range
    *  \result \c true, if estimated position is accepted. \c false
    *    otherwise
    *  \sa est_pos_lateration(), est_pos_min_max(), collect_neighbors(),
    *    LaterationType
    */
   bool check_residue(
         const NeighborInfoList&,
         const shawn::Vec&,
         const LaterationType&,
         double )
      throw();
   /** This method calculates the distance between two nodes using only
    *  distance information with trilateration.
    *
    *  There are two triangles known. At first, the triangle source-n1-n2,
    *  where as well the distance from source to n1 and n2 as the distance
    *  from n1 to n2 is known. Second, there is the triangle
    *  n1-n2-destination with same distance information.
    *
    *  Result is a choice of two valid distances.
    *
    *  \param double distance source-n1
    *  \param double distance source-n2
    *  \param double distance n1-n2
    *  \param double distance n1-destination
    *  \param double distance n2-destination
    *  \result The two resulting distances
    */
   DistancePair trilateration_distance(
         double, double, double, double, double )
      throw();
   /** This method collects different types of neighbors in a
    *  NeighborInfoList used in est_pos_lateration() and check_residue().
    *  The type of neighbors is given by the lateration type.
    *
    *  \param LocalizationNeighborhood Used neighborhood
    *  \param LaterationType Type of resulting neighbors, e.g. anchors or
    *    confident neighbors
    *  \param NeighborInfoList Resulting list of neighbors
    *  \sa est_pos_lateration(), check_residue(), LaterationType
    */
   void collect_neighbors(
         const LocalizationNeighborhood&,
         const LaterationType&,
         NeighborInfoList& )
      throw();
   /** This method computes relative coordinates of two nodes given
    *  the distances from source to the two nodes and distance
    *  between latter. The coordinates are relative to the source position
    *  (0,0,0).
    *
    *  Till now, there is just 2D supported.
    *
    *  \param double distance source - first node p
    *  \param double distance source - second node q
    *  \param double distance first node p - second node q
    *  \param shawn::Vec& coordinate of first node p
    *  \param shawn::Vec& coordinate of second node q
    *  \result \c true, if computation succeedes. \c false otherwise.
    */
   bool compute_basis_coords(
         double, double, double,
         shawn::Vec&, shawn::Vec& )
      throw();
   /** This method computes the relative coordinate of a node j
    *  depending on the distances to two others p and q.
    *
    *  Up to now, there is just 2D supported.
    *
    *  \param double distance source - first node p
    *  \param double distance source - second node q
    *  \param double distance source - third node j
    *  \param double distance first node p - third node j
    *  \param double distance second node q - third node j
    *  \param double distance first node p - second node q
    *  \param shawn::Vec& coordinate of the node j
    *  \param double epsilon
    *  \result \c true, if computation succeedes. \c false otherwise.
    */
   bool compute_rel_coord_triangulisation(
         double, double, double, double, double, double,
         shawn::Vec&, double )
      throw();

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/math/localization_triangulation.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_triangulation.h,v $
 *-----------------------------------------------------------------------*/
