/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_NEIGHBORHOOD_NEIGHBOR_INFO_H
#define __SHAWN_APPS_LOCALIZATION_NEIGHBORHOOD_NEIGHBOR_INFO_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include <map>
#include <list>
#include <limits>
#include "sys/util/refcnt_pointable.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"
#include "sys/vec.h"
#include "sys/node.h"
#include "apps/localization/util/localization_defutils.h"


namespace localization
{

   /// Store information about a single neighbor
   /** This class stores information about a single neighbor which is again
    *  collected in LocalizationNeighborhood.
    *
    *  In addition to storing general information about position, hops,
    *  distance and if it is a anchor or not, there is a lot of extra data
    *  collected.
    *
    *  At first, there is information about confidence and twin. Confidence is
    *  used for a weighted least squares approach. A twin is defined as a node
    *  which is (or thinks it is) nearly on the same position as the neighbor
    *  itself. Both parameters are used, e.g., in
    *  LocalizationIterLaterationModule.
    *
    *  Moreover the data of neighborhood is collected. This means, that there
    *  is a chance to store a two-hop neighborhood.
    *
    *  At least there are reference nodes. These are defined as nodes
    *  neighbors get their information about anchors from, and used by
    *  LocalizationNeighborhood to decide whether a node is
    *  \ref LocalizationNeighborhood::is_sound "sound" or not.
    */
   class LocalizationNeighborInfo
      : public shawn::RefcntPointable
   {

   public:

      ///@name construction / destruction
      ///@{
      /** Constructor of new neighbor info.
       *
       *  \param Node Neighbor
       *  \param bool \c true, if anchor. \c false otherwise
       */
      LocalizationNeighborInfo( const shawn::Node&, bool );
      ///
      virtual ~LocalizationNeighborInfo();
      ///@}


      ///@name update/insert neighborhood
      ///@{
      /** Update neighborhood of this neighbor. If neighbor exists, it is just
       *  updated, else inserted.
       *
       *  \param Node neighbor to insert/update
       *  \param double distance to this neighbor
       *  \sa update_neighbors()
       */
      virtual void update_neighbor( const shawn::Node&, double ) throw();
      /** Update neighborhood of this neighbor. If a neighbor exists, it is
       *  just updated, else inserted.
       *
       *  \param DistanceMap neighbors and distances to insert/update
       *  \sa update_neighbor()
       */
      virtual void update_neighbors( const localization::DistanceMap& )
         throw();
      ///@}


      ///@name reference nodes
      ///@{
      /** Add a reference node.
       *
       *  \param Node reference node
       *  \sa LocalizationNeighborhood::is_sound
       */
      virtual void add_ref_node( const shawn::Node& ) throw();
      /** Clear reference nodes.
       *
       *  \sa LocalizationNeighborhood::is_sound
       */
      virtual void clear_ref_nodes( void ) throw();
      /** \return set of reference nodes
       *  \sa LocalizationNeighborhood::is_sound
       */
      virtual const localization::NodeSet& ref_nodes( void )
         const throw();
      /** \return writable set of reference nodes
       *  \sa LocalizationNeighborhood::is_sound
       */
      virtual localization::NodeSet& ref_nodes_w( void ) throw();
      ///@}


      ///@name set data
      ///@{
      /** Set position.
       *
       *  \param Vec new position
       */
      virtual void set_pos( const shawn::Vec& ) throw();
      /** Clear position.
       */
      virtual void clear_pos() throw();
      /** Set hop count.
       *
       *  \param int hop count
       */
      virtual void set_hops( int ) throw();
      /** Set distance.
       *
       *  \param double distance
       */
      virtual void set_distance( double ) throw();
      /** Set confidence.
       *
       *  \param double confidence
       */
      virtual void set_confidence( double ) throw();
      /** Set twin property.
       *
       *  \param bool if it is twin or not
       */
      virtual void set_twin( bool ) throw();
      /** Set anchor property.
       *
       *  \param bool if it is anchor or not
       */
      virtual void set_anchor( bool ) throw();
      ///@}


      ///@name special work on data
      ///@{
      /** This method converts hop count into distance by multiplying the
       *  hop count by given average hop distance.
       *
       *  \param double average hop distance
       */
      virtual void convert_hops( double ) throw();
      ///@}


      ///@name get data
      ///@{
      /** \return Node of this neighbor
       */
      virtual const shawn::Node& node( void ) const throw();
      /** \return Node id of this neighbor
       *  \sa Node::label()
       */
      virtual std::string label( void ) const throw();
      /** \return Position of this neighbor
       */
      virtual const shawn::Vec& pos( void ) const throw();
      /** This method returns the hop count. If there is no hop count known,
       *  it returns localization::UNKNOWN_HOP_CNT
       *
       *  \return Hop count of this neighbor
       */
      virtual int hops( void ) const throw();
      /** \return Distance to this neighbor
       */
      virtual double distance( void ) const throw();
      /** \return Confidence of this neighbor
       */
      virtual double confidence( void ) const throw();
      ///@}


      ///@name get special data
      ///@{
      /** \return \c true, if this neighbor is an anchor. \c false otherwise
       */
      virtual bool is_anchor( void ) const throw();
      /** A valid neighbor has a position and distance.
       *
       *  \return \c true, if this neighbor is valid. \c false otherwise
       */
      virtual bool is_valid( void ) const throw();
      /** \return \c true, if this neighbor is a twin. \c false otherwise
       */
      virtual bool is_twin( void ) const throw();
      /** \return \c true, if this neighbor has a position. \c false otherwise
       */
      virtual bool has_pos( void ) const throw();
      /** \return \c true, if there is distance to this neighbor known.
       *    \c false otherwise
       */
      virtual bool has_distance( void ) const throw();
      /** A confident neighbor has a position and distance. Moreover it is not
       *  a twin and has a confidence != 0.
       *
       *  \return \c true, if this neighbor is confident. \c false otherwise
       */
      virtual bool is_confident( void ) const throw();
      ///@}


      ///@name neighbor information
      ///@{
      /** \return \ref localization::DistanceMap "DistanceMap" of neighborhood
       */
      virtual const localization::DistanceMap& neighbors( void )
         const throw();
      /** \param Node Node for which is searched in neighborhood
       *  \return \c true, if given neighbor is part of neighborhood. \c false
       *    otherwise
       */
      virtual bool has_neighbor( const shawn::Node& ) const throw();
      /** \param Node Node for which is searched in neighborhood
       *  \return \c true, if there is distance for given neighbor known.
       *    \c false otherwise
       */
      virtual bool has_neighbor_distance( const shawn::Node& ) const throw();
      /** This method returns the distance to given neighbor. If there is no
       *  distance known, method returns localization::UNKNOWN_DISTANCE.
       *
       *  \param Node Node for which is searched in neighborhood
       *  \return distance to given neighbor
       */
      virtual double neighbor_distance( const shawn::Node& ) const throw();
      ///@}


      ///@name iterators on neighbors
      ///@{
      /** \return writable iterator to begin of neighbors
       */
      virtual localization::NeighborIterator begin_neighbors_w( void )
         throw();
      /** \return writable iterator to end of neighbors
       */
      virtual localization::NeighborIterator end_neighbors_w( void )
         throw();
      /** \return iterator to begin of neighbors
       */
      virtual localization::ConstNeighborIterator begin_neighbors(
            void )
         const throw();
      /** \return iterator to end of neighbors
       */
      virtual localization::ConstNeighborIterator end_neighbors( void )
         const throw();
      ///@}


      ///@name write out
      ///@{
      /** Print out IDs of self and neighborhood.
       */
      friend std::ostream& operator<<( std::ostream&, const LocalizationNeighborInfo& ) throw();
      ///@}


   private:

      const shawn::Node& node_;
      shawn::Vec pos_;
      int hops_;
      double distance_;
      double confidence_;

      bool is_anchor_;
      bool is_twin_;

      bool has_pos_;

      localization::NodeSet reference_points_;
      localization::DistanceMap neighbors_;
   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/neighborhood/localization_neighbor_info.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_neighbor_info.h,v $
 *-----------------------------------------------------------------------*/
