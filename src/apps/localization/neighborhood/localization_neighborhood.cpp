/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/neighborhood/localization_neighborhood.h"
#include "apps/localization/math/localization_statistic.h"
#include "apps/localization/util/localization_defutils.h"
#include <cmath>
#include <algorithm>
#include <limits>


using namespace shawn;

namespace localization
{

   LocalizationNeighborhood::
   LocalizationNeighborhood()
   {}
   // ----------------------------------------------------------------------
   LocalizationNeighborhood::
   ~LocalizationNeighborhood()
   {}
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborhood::
   set_source( Node& source )
      throw()
   {
      source_ = &source;
   }
   // ----------------------------------------------------------------------
   const Node&
   LocalizationNeighborhood::
   source( void )
      const throw()
   {
      return *source_;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborhood::
   update_anchor( const Node& node, Vec pos, double distance )
      throw()
   {
      NeighborhoodIterator it = find_w( node );
      if ( it != end_neighborhood_w() )
      {
         it->second->set_pos( pos );
         if ( distance != UNKNOWN_DISTANCE )
            it->second->set_distance( distance );
         it->second->set_anchor( true );
         return;
      }

      LocalizationNeighborInfoHandle nih = new LocalizationNeighborInfo( node, true );
      nih->set_pos( pos );
      if ( distance != UNKNOWN_DISTANCE )
         nih->set_distance( distance );

      neighborhood_[&node] = nih;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborhood::
   update_neighbor( const Node& node, double distance )
      throw()
   {
      NeighborhoodIterator it = find_w( node );
      if ( it != end_neighborhood_w() )
      {
         it->second->set_distance( distance );
         return;
      }

      LocalizationNeighborInfoHandle nih = new LocalizationNeighborInfo( node, false );
      nih->set_distance( distance );
      neighborhood_[&node] = nih;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborhood::
   update_nneighbor( const Node& node, const Node& neighbor, double distance )
      throw()
   {
      if ( node == neighbor )
         return;

      NeighborhoodIterator it;

      if ( ( it = find_w( node ) ) != end_neighborhood_w() )
         it->second->update_neighbor( neighbor, distance );

      if ( ( it = find_w( neighbor ) ) != end_neighborhood_w() )
         it->second->update_neighbor( node, distance );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborhood::
   update_nneighbors( const Node& node, const DistanceMap& dm )
      throw()
   {
      NeighborhoodIterator it = find_w( node );
      if ( it == end_neighborhood_w() )
         return;

      it->second->update_neighbors( dm );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborhood::
   set_ref_node( const Node& node, const Node& ref )
      throw()
   {
      NeighborhoodIterator it = find_w( node );
      if ( it == end_neighborhood_w() )
         return;

      it->second->clear_ref_nodes();
      it->second->add_ref_node( ref );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborhood::
   update_ref_node( const Node& node, const Node& ref )
      throw()
   {
      NeighborhoodIterator it = find_w( node );
      if ( it == end_neighborhood_w() )
         return;

//FIXME: check, if 'set of ref-nodes' is getting better/greater by adding the node.
//  -> Too much computing???
      it->second->clear_ref_nodes();
      it->second->add_ref_node( ref );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborhood::
   update_ref_nodes( const Node& node, const localization::NodeList& nl )
      throw()
   {
      NeighborhoodIterator it = find_w( node );
      if ( it == end_neighborhood_w() )
         return;

      it->second->clear_ref_nodes();
      it->second->ref_nodes_w().insert( nl.begin(), nl.end() );
   }
   // ----------------------------------------------------------------------
   const localization::NodeSet
   LocalizationNeighborhood::
   ref_nodes( void )
      const throw()
   {
      NodeSet temp;

      for ( ConstNeighborhoodIterator
               it = begin_neighborhood();
               it != end_neighborhood();
               ++it )
         if ( it->second->is_anchor() && it->second->is_valid() )
            temp.insert(
               it->second->ref_nodes().begin(),
               it->second->ref_nodes().end() );

      return temp;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborhood::
   add_sound( const Node& node )
      throw()
   {
      sounds_.insert( &node );
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationNeighborhood::
   is_sound( void )
      const throw()
   {
      NodeSet temp = ref_nodes();
      temp.insert( sounds_.begin(), sounds_.end() );

      // if there is no info about ref-nodes of the anchors, ignore this
      // check return true anyway
      if ( anchor_cnt() > 0 && temp.size() == 0 )
         return true;

//TODO: Dimension anpassen !!!!
      return (int)temp.size() >= 3;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationNeighborhood::
   has_anchor( const Node& node )
      const throw()
   {
      ConstNeighborhoodIterator it = neighborhood_.find( &node );

      return ( it != end_neighborhood() && it->second->is_anchor() );
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationNeighborhood::
   has_neighbor( const Node& node )
      const throw()
   {
      return ( neighborhood_.find( &node ) != end_neighborhood() );
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationNeighborhood::
   has_nneighbor( const Node& node, const Node& neighbor )
      const throw()
   {
      if ( node == neighbor )
         return false;

      ConstNeighborhoodIterator it = neighborhood_.find( &node );
      if ( it == end_neighborhood() )
         return false;

      return it->second->has_neighbor( neighbor );
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationNeighborhood::
   has_valid_neighbor( const Node& node )
      const throw()
   {
      ConstNeighborhoodIterator it = neighborhood_.find( &node );
      if ( it == end_neighborhood() ) return false;

      return ( it->second->has_distance() );
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationNeighborhood::
   has_valid_nneighbor( const Node& node, const Node& neighbor )
      const throw()
   {
      if ( node == neighbor )
         return false;

      ConstNeighborhoodIterator it = neighborhood_.find( &node );
      if ( it == end_neighborhood() ) return false;

      return ( it->second->has_neighbor( neighbor ) &&
                  it->second->has_neighbor_distance( neighbor ) );
   }
   // ----------------------------------------------------------------------
   double
   LocalizationNeighborhood::
   neighbor_distance( const Node& node )
      const throw()
   {
      ConstNeighborhoodIterator it = neighborhood_.find( &node );
      if ( it == end_neighborhood() )
         return UNKNOWN_DISTANCE;

      return ( it->second->distance() );
   }
   // ----------------------------------------------------------------------
   double
   LocalizationNeighborhood::
   nneighbor_distance( const Node& node, const Node& neighbor )
      const throw()
   {
      if ( node == neighbor )
         return 0;

      ConstNeighborhoodIterator it1 = neighborhood_.find( &node );
      ConstNeighborhoodIterator it2 = neighborhood_.find( &neighbor );

      LocalizationStatistic stat;

      if ( it1 != end_neighborhood() &&
            it1->second->has_neighbor_distance( neighbor ) )
         stat += it1->second->neighbor_distance( neighbor );

      if ( it2 != end_neighborhood() &&
            it2->second->has_neighbor_distance( node ) )
         stat += it2->second->neighbor_distance( node );

      if ( stat.size() == 0 )
         return UNKNOWN_DISTANCE;

      return ( stat.mean() );
   }
   // ----------------------------------------------------------------------
   const DistanceMap
   LocalizationNeighborhood::
   neighbor_distance_map( void )
      const throw()
   {
      DistanceMap temp;

      for ( ConstNeighborhoodIterator
               it = begin_neighborhood();
               it != end_neighborhood();
               ++it )
         temp[&it->second->node()] = it->second->distance();

      return temp;
   }
   // ----------------------------------------------------------------------
   int
   LocalizationNeighborhood::
   valid_anchor_cnt( void )
      const throw()
   {
      int cnt = 0;

      for ( ConstNeighborhoodIterator
               it = begin_neighborhood();
               it != end_neighborhood();
               ++it )
         if ( it->second->is_anchor() && it->second->is_valid() )
            ++cnt;

      return cnt;
   }
   // ----------------------------------------------------------------------
   int
   LocalizationNeighborhood::
   anchor_cnt()
      const throw()
   {
      int cnt = 0;

      for ( ConstNeighborhoodIterator
               it = begin_neighborhood();
               it != end_neighborhood();
               ++it )
         if ( it->second->is_anchor() )
            ++cnt;

      return cnt;
   }
   // ----------------------------------------------------------------------
   int
   LocalizationNeighborhood::
   confident_neighbor_cnt( void )
      const throw()
   {
      int cnt = 0;

      for ( ConstNeighborhoodIterator
               it = begin_neighborhood();
               it != end_neighborhood();
               ++it )
         if ( it->second->is_confident() )
            ++cnt;

      return cnt;
   }
   // ----------------------------------------------------------------------
   double
   LocalizationNeighborhood::
   avg_neighbor_confidence( void )
      const throw()
   {
      double conf = 0;
      int count = 0;

      for ( ConstNeighborhoodIterator
               it = begin_neighborhood();
               it != end_neighborhood();
               ++it )
         if ( it->second->is_confident() )
         {
            ++count;
            conf += it->second->confidence();
         }

      return ( conf / count );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborhood::
   reassign_twins( double twin_measure )
      throw()
   {
      // main task of this cycle is to unset all twins in the neighborhood.
      // additionally it is checked, whether the neighbor is a twin of the
      // source node or not.
      for ( NeighborhoodIterator
               it = begin_neighborhood_w();
               it != end_neighborhood_w();
               ++it )
         if ( euclidean_distance(
                  source().est_position(), it->second->node().est_position() )
               <= twin_measure )
            it->second->set_twin( true );
         else
            it->second->set_twin( false );

      // this cycle sorts out all neighbors that are twins to each other, so
      // that after this check there is only one neighbor on one position.
      for ( NeighborhoodIterator
               it1 = begin_neighborhood_w();
               it1 != end_neighborhood_w();
               ++it1 )
         for ( NeighborhoodIterator
                  it2 = it1;
                  it2 != end_neighborhood_w();
                  ++it2 )
         {
            if ( it1 == it2 )
               continue;

            if ( !it1->second->has_pos() || !it2->second->has_pos() )
               continue;

            if ( euclidean_distance( it1->second->pos(), it2->second->pos() )
                  <= twin_measure )
               it2->second->set_twin( true );
         }
   }
   // ----------------------------------------------------------------------
   NeighborhoodIterator
   LocalizationNeighborhood::
   begin_neighborhood_w( void ) throw()
   {
      return neighborhood_.begin();
   }
   // ----------------------------------------------------------------------
   NeighborhoodIterator
   LocalizationNeighborhood::
   end_neighborhood_w( void )
      throw()
   {
      return neighborhood_.end();
   }
   // ----------------------------------------------------------------------
   ConstNeighborhoodIterator
   LocalizationNeighborhood::
   begin_neighborhood( void )
      const throw()
   {
      return neighborhood_.begin();
   }
   // ----------------------------------------------------------------------
   ConstNeighborhoodIterator
   LocalizationNeighborhood::
   end_neighborhood( void )
      const throw()
   {
      return neighborhood_.end();
   }
   // ----------------------------------------------------------------------
   size_t
   LocalizationNeighborhood::
   size( void )
      const throw()
   {
      return neighborhood_.size();
   }
   // ----------------------------------------------------------------------
   NeighborhoodIterator
   LocalizationNeighborhood::
   find_w( const Node& node )
      throw()
   {
      return neighborhood_.find( &node );
   }
   // ----------------------------------------------------------------------
   ConstNeighborhoodIterator
   LocalizationNeighborhood::
   find( const Node& node )
      const throw()
   {
      return neighborhood_.find( &node );
   }
   // ----------------------------------------------------------------------
   std::ostream&
   operator<<(
         std::ostream& os,
         const LocalizationNeighborhood& ln )
      throw()
   {
      os << "Neighborhood: " << std::endl;

      for ( ConstNeighborhoodIterator
               it = ln.begin_neighborhood();
               it != ln.end_neighborhood();
               ++it )
         os << "\t" << it->second->label();

      return os;
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/neighborhood/localization_neighborhood.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_neighborhood.cpp,v $
 *-----------------------------------------------------------------------*/
