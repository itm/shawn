/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/neighborhood/localization_neighbor_info.h"


using namespace shawn;

namespace localization
{

   LocalizationNeighborInfo::
   LocalizationNeighborInfo( const Node& node, bool is_anchor )
      : node_        ( node ),
         hops_       ( localization::UNKNOWN_HOP_CNT ),
         distance_   ( localization::UNKNOWN_DISTANCE ),
         confidence_ ( 0 ),
         is_anchor_  ( is_anchor ),
         is_twin_    ( false ),
         has_pos_    ( false )
   {}
   // ----------------------------------------------------------------------
   LocalizationNeighborInfo::
   ~LocalizationNeighborInfo()
   {}
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborInfo::
   update_neighbor( const Node& node, double distance )
      throw()
   {
      neighbors_[&node] = distance;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborInfo::
   update_neighbors( const localization::DistanceMap& dm )
      throw()
   {
      for ( localization::ConstNeighborIterator
               it = dm.begin();
               it != dm.end();
               ++it )
         neighbors_[it->first] = it->second;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborInfo::
   add_ref_node( const Node& node )
      throw()
   {
      reference_points_.insert( &node );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborInfo::
   clear_ref_nodes( void )
      throw()
   {
      reference_points_.clear();
   }
   // ----------------------------------------------------------------------
   const localization::NodeSet&
   LocalizationNeighborInfo::
   ref_nodes( void )
      const throw()
   {
      return reference_points_;
   }
   // ----------------------------------------------------------------------
   localization::NodeSet&
   LocalizationNeighborInfo::
   ref_nodes_w( void )
      throw()
   {
      return reference_points_;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborInfo::
   set_pos( const Vec& pos )
      throw()
   {
      pos_ = pos;
      has_pos_ = true;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborInfo::
   clear_pos()
      throw()
   {
      pos_ = Vec(0,0);
      has_pos_ = false;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborInfo::
   set_hops( int hops )
      throw()
   {
      hops_ = hops;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborInfo::
   set_distance( double distance )
      throw()
   {
      distance_ = distance;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborInfo::
   set_confidence( double confidence )
      throw()
   {
      confidence_ = confidence;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborInfo::
   set_twin( bool twin )
      throw()
   {
      is_twin_ = twin;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborInfo::
   set_anchor( bool anchor )
      throw()
   {
      is_anchor_ = anchor;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationNeighborInfo::
   convert_hops( double hop_dist )
      throw()
   {
      if ( hops_ != localization::UNKNOWN_HOP_CNT )
         distance_ = hops_ * hop_dist;
   }
   // ----------------------------------------------------------------------
   const Node&
   LocalizationNeighborInfo::
   node( void )
      const throw()
   {
      return node_;
   }
   // ----------------------------------------------------------------------
   std::string
   LocalizationNeighborInfo::
   label( void )
      const throw()
   {
      return node_.label();
   }
   // ----------------------------------------------------------------------
   const Vec&
   LocalizationNeighborInfo::
   pos( void )
      const throw()
   {
      return pos_;
   }
   // ----------------------------------------------------------------------
   int
   LocalizationNeighborInfo::
   hops( void )
      const throw()
   {
      return hops_;
   }
   // ----------------------------------------------------------------------
   double
   LocalizationNeighborInfo::
   distance( void )
      const throw()
   {
      return distance_;
   }
   // ----------------------------------------------------------------------
   double
   LocalizationNeighborInfo::
   confidence( void )
      const throw()
   {
      return confidence_;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationNeighborInfo::
   is_anchor( void )
      const throw()
   {
      return is_anchor_;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationNeighborInfo::
   is_valid( void )
      const throw()
   {
      return ( has_pos() && has_distance() );
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationNeighborInfo::
   is_twin( void )
      const throw()
   {
      return is_twin_;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationNeighborInfo::
   has_pos( void )
      const throw()
   {
      return has_pos_;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationNeighborInfo::
   has_distance( void )
      const throw()
   {
      return distance_ != localization::UNKNOWN_DISTANCE;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationNeighborInfo::
   is_confident( void )
      const throw()
   {
      return confidence() != 0 && !is_twin() && has_pos() && has_distance();
   }
   // ----------------------------------------------------------------------
   const localization::DistanceMap&
   LocalizationNeighborInfo::
   neighbors( void )
      const throw()
   {
      return neighbors_;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationNeighborInfo::
   has_neighbor( const Node& node )
      const throw()
   {
      return neighbors_.find( &node ) != neighbors_.end();
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationNeighborInfo::
   has_neighbor_distance( const Node& node )
      const throw()
   {
      return neighbor_distance( node ) != localization::UNKNOWN_DISTANCE;
   }
   // ----------------------------------------------------------------------
   double
   LocalizationNeighborInfo::
   neighbor_distance( const Node& node )
      const throw()
   {
      localization::ConstNeighborIterator it = neighbors_.find( &node );

      if ( it != neighbors_.end() )
         return it->second;
      else
         return localization::UNKNOWN_DISTANCE;
   }
   // ----------------------------------------------------------------------
   localization::NeighborIterator
   LocalizationNeighborInfo::
   begin_neighbors_w( void )
      throw()
   {
      return neighbors_.begin();
   }
   // ----------------------------------------------------------------------
   localization::NeighborIterator
   LocalizationNeighborInfo::
   end_neighbors_w( void )
      throw()
   {
      return neighbors_.end();
   }
   // ----------------------------------------------------------------------
   localization::ConstNeighborIterator
   LocalizationNeighborInfo::
   begin_neighbors( void )
      const throw()
   {
      return neighbors_.begin();
   }
   // ----------------------------------------------------------------------
   localization::ConstNeighborIterator
   LocalizationNeighborInfo::
   end_neighbors( void )
      const throw()
   {
      return neighbors_.end();
   }
   // ----------------------------------------------------------------------
   std::ostream&
   operator<<(
         std::ostream& os,
         const LocalizationNeighborInfo& lni )
      throw()
   {
      os << "NeighborInfo about : " << lni.node().label() << std::endl;

      for ( localization::ConstNeighborIterator
               it = lni.neighbors_.begin();
               it != lni.neighbors_.end();
               ++it )
         os << "\t" << it->first->label();

      return os;
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/neighborhood/localization_neighbor_info.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_neighbor_info.cpp,v $
 *-----------------------------------------------------------------------*/
