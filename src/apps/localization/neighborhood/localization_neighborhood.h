/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_NEIGHBORHOOD_NEIGHBORHOOD_H
#define __SHAWN_APPS_LOCALIZATION_NEIGHBORHOOD_NEIGHBORHOOD_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include <map>
#include "apps/localization/neighborhood/localization_neighbor_info.h"
#include "apps/localization/math/localization_statistic.h"
#include "apps/localization/util/localization_defutils.h"
#include "sys/vec.h"
#include "sys/node.h"


namespace localization
{

   /// Basic neighborhood operations
   /** This class combines \ref LocalizationNeighborInfo "neighbors" to a
    *  neighborhood. In addition to basic methods such like adding an anchor
    *  or neighbor, there are operations on the neighborhood like counting
    *  anchors or something alike.
    *
    *  At first, there are the above mentioned basic methods like
    *  adding/updating anchors or neighbors, methods for searching certain
    *  anchors/neighbors in the neighborhood, same with distances, and simply
    *  methods which return iterators.\n
    *  To add more information to a neighbor, you have to direct access the
    *  LocalizationNeighborInfo via find_w() and resulting iterator.
    *
    *  Second there is a chance to decide, whether the neighborhood is sound
    *  or not. This means, that the received information is unique. The
    *  neighborhood is sound, if there are at least three/four valid anchors,
    *  for which the data is received by different neighbors. Moreover, e.g.
    *  the LocalizationIterLaterationModule does this, you can add so called
    *  sound neighbors, so that the intersection of unique anchors and sound
    *  nodes has to be greater or equal than three/four.\n
    *  The sound nodes are stored in separate set
    *
    *  At last there are operations on the neighborhood. On the one hand
    *  the varying like reassign_twins(), on the other hand just \em const
    *  ones like valid_anchor_cnt() or confident_neighbor_cnt().
    *
    */
   class LocalizationNeighborhood
   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      LocalizationNeighborhood();
      ///
      virtual ~LocalizationNeighborhood();
      ///@}


      ///@name owner info
      ///@{
      /** Set the node this neighborhood belongs to.
       *
       *  \param Node belonging Node
       */
      virtual void set_source( shawn::Node& ) throw();
      /** \return Node this neighborhood belongs to
       */
      virtual const shawn::Node& source( void ) const throw();
      ///@}


      ///@name add/update nodes to neighborhood
      ///@{
      /** Update an anchor with given data. If the anchor does not exist, it
       *  is created and inserted in the neighborhood.
       *
       *  \param Node anchor to insert/update
       *  \param Vec position of anchor
       *  \param double distance to anchor. if distance is not known, the
       *    default value is localization::UNKNOWN_DISTANCE
       */
      virtual void update_anchor(
            const shawn::Node&, shawn::Vec, double = localization::UNKNOWN_DISTANCE )
         throw();
      /** Update a neighbor with given data. If the neighbor does not exist,
       *  it is created and inserted in the neighborhood.
       *
       *  \param Node neighbor to insert/update
       *  \param double distance to neighbor
       */
      virtual void update_neighbor( const shawn::Node&, double ) throw();
      /** Update a neighbor of a neighbor with given data, so that the
       *  neighbor's neighborhood is filled/updated. If both Nodes exists
       *  in source neighborhood, the update happens in both ways.
       *
       *  \param Node neighbor to update
       *  \param Node neighbor of above mentioned neighbor
       *  \param double distance between Nodes
       *  \sa update_nneighbors()
       */
      virtual void update_nneighbor( const shawn::Node&, const shawn::Node&, double )
         throw();
      /** Update neighbors of a neighbor with given data, so that the
       *  neighbor's neighborhood is filled/updated.
       *
       *  \param Node neighbor to update
       *  \param localization::DistanceMap neighbors of above mentioned
       *    neighbor
       *  \sa update_nneighbor()
       */
      virtual void update_nneighbors(
            const shawn::Node&, const localization::DistanceMap& )
         throw();
      ///@}


      ///@name reference nodes and sound-check
      ///@{
      /** Set a reference node of a neighbor. Old one(s) will be deleted
       *  and new one set.
       *
       *  \param Node neighbor to update
       *  \param Node reference node
       *  \sa update_ref_node(), update_ref_nodes()
       */
      virtual void set_ref_node( const shawn::Node&, const shawn::Node& ) throw();
      /** Set a reference node of a neighbor. Old one(s) will be deleted
       *  and new one set.
       *
       *  Till now this method has the same functionality as set_ref_node().
       *  Maybe later there will be a check, if the update results in a better
       *  \ref is_sound() "sound-check" following of this it is decided,
       *  whether the update happens or not.
       *
       *  \param Node neighbor to update
       *  \param Node reference node
       *  \sa set_ref_node(), update_ref_nodes()
       */
      virtual void update_ref_node( const shawn::Node&, const shawn::Node& ) throw();
      /** Set reference nodes of a neighbor. Old one(s) will be deleted
       *  and new one(s) set.
       *
       *  \param Node neighbor to update
       *  \param Node reference nodes
       *  \sa set_ref_node(), update_ref_node()
       */
      virtual void update_ref_nodes(
            const shawn::Node&, const localization::NodeList& )
         throw();
      /** \return Set of all reference nodes
       */
      virtual const localization::NodeSet ref_nodes( void ) const throw();
      /** Add sound neighbor. The sound nodes are stored in separate set.
       */
      virtual void add_sound( const shawn::Node& ) throw();
      /** To decide, whether the neighborhood is sound or not, the number of
       *  intersection of reference and \ref add_sound() "sound" nodes is
       *  taken. If this number is greater or eual than three/four,
       *  neighborhood is sound.
       *
       *  \return \c true, if neighborhood is sound. \c false otherwise
       */
      virtual bool is_sound( void ) const throw();
      ///@}


      ///@name information about neighborhood
      ///@{
      /** \param Node anchor to search for
       *
       *  \return \true if neighbor exists and is an anchor
       */
      virtual bool has_anchor( const shawn::Node& ) const throw();
      /** \param Node neighbor to search for
       *
       *  \return \true if neighbor exists
       *  \sa has_valid_neighbor()
       */
      virtual bool has_neighbor( const shawn::Node& ) const throw();
      /** \param Node source neighbor
       *  \param Node neighbor of neighbor to search for
       *
       *  \return \true if neighbor of neighbor exists
       *  \sa has_valid_nneighbor()
       */
      virtual bool has_nneighbor( const shawn::Node&, const shawn::Node& )
         const throw();
      /** This method checks, whether a neighbor exits or not, and is valid.
       *  Unlike LocalizationNeighborInfo::is_valid \em valid in this case
       *  just means, that there exists a distance to this neighbor.
       *
       *  \param Node neighbor to search for
       *
       *  \return \true if neighbor exists and is valid
       *  \sa has_neighbor()
       */
      virtual bool has_valid_neighbor( const shawn::Node& ) const throw();
      /** This method checks, whether a neighbor of neighbor exits or not, and
       *  is valid. Unlike LocalizationNeighborInfo::is_valid \em valid in
       *  this case just means, that there exists a distance of given neighbor
       *  to his neighbor.
       *
       *  \param Node source neighbor
       *  \param Node neighbor of neighbor to search for
       *
       *  \return \true if neighbor of neighbor exists and is valid
       *  \sa has_nneighbor()
       */
      virtual bool has_valid_nneighbor( const shawn::Node&, const shawn::Node& )
         const throw();
      ///@}


      ///@name distance information
      ///@{
      /** \param Node neighbor to search for
       *
       *  \return Distance to given neighbor
       */
      virtual double neighbor_distance( const shawn::Node& ) const throw();
      /** Returns the distance between given neighbors. If both distances are
       *  known, the mean of both is returned. If none is known,
       *  UNKNOWN_DISTANCE is returned.
       *
       *  \param Node source neighbor
       *  \param Node neighbor of neighbor to search for
       *
       *  \return Distance from given neighbor to its neighbor
       */
      virtual double nneighbor_distance( const shawn::Node&, const shawn::Node& )
         const throw();
      /** \return localization::DistanceMap of complete neighborhood
       */
      virtual const localization::DistanceMap neighbor_distance_map( void )
         const throw();
      ///@}


      ///@name special information
      ///@{
      /** This method count the number of valid anchors. Valid means, that
       *  there is a distance to this anchor known and it has a position.
       *
       *  \return Number of valid anchors
       *  \sa anchor_cnt(), LocalizationNeighborInfo::is_valid()
       */
      virtual int valid_anchor_cnt( void ) const throw();
      /** This method count the number of anchors, whether they are valid or
       *  not.
       *
       *  \return Number of anchors
       *  \sa valid_anchor_cnt(), LocalizationNeighborInfo::is_valid()
       */
      virtual int anchor_cnt() const throw();
      /** This method count the number of confident neighbors. Confident
       *  means, that there is a position and distance known, the neighbor is
       *  not a twin and has a confidence != 0.
       *
       *  \return Number of confident neighbors
       *  \sa valid_anchor_cnt(), LocalizationNeighborInfo::is_confident()
       */
      virtual int confident_neighbor_cnt( void ) const throw();
      /** This method computes the average confidence of all confident
       *  neighbors. Confident means, that there is a position and distance
       *  known, the neighbor is not a twin and has a confidence != 0.
       *
       *  \return Average confidence of all confident neighbors
       *  \sa confident_neighbor_cnt(),
       *    LocalizationNeighborInfo::is_confident()
       */
      virtual double avg_neighbor_confidence( void ) const throw();
      ///@}


      ///@name work on the neighborhood
      ///@{
      /** This method passes all pairs of neighbors and decide, whether they
       *  are twins or not on the basis of given measure, and set the second
       *  neighbor to a twin in positive cases.\n
       *  The result is, that there is only one neighbor on one position in
       *  the neighborhood, which is not a twin.
       *
       *  \param double Measure to decide, whether a neighbor is near enough
       *    to be a twin
       */
      virtual void reassign_twins( double ) throw();
      ///@}


      ///@name iterators and general functions on neighboorhood
      ///@{
      /** \return writable iterator to begin of neighborhood
       */
      virtual localization::NeighborhoodIterator
      begin_neighborhood_w( void )
         throw();
      /** \return writable iterator to end of neighborhood
       */
      virtual localization::NeighborhoodIterator
      end_neighborhood_w( void )
         throw();
      /** \return iterator to begin of neighborhood
       */
      virtual localization::ConstNeighborhoodIterator
      begin_neighborhood( void )
         const throw();
      /** \return iterator to end of neighborhood
       */
      virtual localization::ConstNeighborhoodIterator
      end_neighborhood( void )
         const throw();
      /** \return size of neighborhood
       */
      virtual size_t size( void ) const throw();
      /** \param Node neighbor to search for
       *
       *  \return writable iterator to given neighbor
       */
      virtual localization::NeighborhoodIterator find_w( const shawn::Node& )
         throw();
      /** \param Node neighbor to search for
       *
       *  \return iterator to given neighbor
       */
      virtual localization::ConstNeighborhoodIterator find(
            const shawn::Node& )
         const throw();
      ///@}


      ///@name write out
      ///@{
      /** Print out IDs of all neighbors in neighborhood
       */
      friend std::ostream& operator<<( std::ostream&, const LocalizationNeighborhood& ) throw();
      ///@}


   private:

      localization::NeighborInfoMap neighborhood_;
      localization::NodeSet sounds_;

      shawn::Node* source_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/neighborhood/localization_neighborhood.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_neighborhood.h,v $
 *-----------------------------------------------------------------------*/
