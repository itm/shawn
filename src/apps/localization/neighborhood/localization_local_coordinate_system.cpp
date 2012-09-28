/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include <cmath>
#include "apps/localization/neighborhood/localization_local_coordinate_system.h"
#include "apps/localization/math/localization_triangulation.h"
#include "apps/localization/math/localization_general_math.h"


namespace localization
{

   LocalizationLocalCoordinateSystem::
   LocalizationLocalCoordinateSystem()
      : src_pos_  ( shawn::Vec( 0.0, 0.0, 0.0 ) ),
         valid_   ( false ),
         epsilon_ ( 0.0001 )
   {}
   // ----------------------------------------------------------------------
   LocalizationLocalCoordinateSystem::
   LocalizationLocalCoordinateSystem( const LocalizationLocalCoordinateSystem& llcs )
      : p_             ( llcs.p_ ),
         q_            ( llcs.q_ ),
         src_node_     ( llcs.src_node_ ),
         neighborhood_ ( llcs.neighborhood_ ),
         src_pos_      ( llcs.src_pos_ ),
         locations_    ( llcs.locations_ ),
         valid_        ( llcs.valid_ ),
         epsilon_      ( llcs.epsilon_ )
   {}
   // ----------------------------------------------------------------------
   LocalizationLocalCoordinateSystem::
   ~LocalizationLocalCoordinateSystem()
   {}
   // ----------------------------------------------------------------------
   void
   LocalizationLocalCoordinateSystem::
   update_basic_nodes( const shawn::Node& p,
            const shawn::Node& q,
            const LocalizationNeighborhood& nbrh )
      throw()
   {
      p_ = &p;
      q_ = &q;
      neighborhood_ = &nbrh;

      double src_p = neighborhood_->neighbor_distance( *p_ );
      double src_q = neighborhood_->neighbor_distance( *q_ );
      double p_q = neighborhood_->nneighbor_distance( *p_, *q_ );

      shawn::Vec pos_p, pos_q;

      if ( !compute_basis_coords( src_p, src_q, p_q, pos_p, pos_q ) )
      {
         if ( !valid_ )
            clear();
         return;
      }

      locations_[p_] = pos_p;
      locations_[q_] = pos_q;

      valid_ = true;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationLocalCoordinateSystem::
   update_node( const shawn::Node& j )
      throw()
   {
      if ( !valid_ )
         return;

      double src_p = neighborhood_->neighbor_distance( *p_ );
      double src_q = neighborhood_->neighbor_distance( *q_ );
      double src_j = neighborhood_->neighbor_distance( j );
      double p_j = neighborhood_->nneighbor_distance( *p_, j );
      double q_j = neighborhood_->nneighbor_distance( *q_, j );
      double p_q = neighborhood_->nneighbor_distance( *p_, *q_ );

      shawn::Vec pos_j;

      if ( !compute_rel_coord_triangulisation( src_p, src_q, src_j, p_j, q_j, p_q, pos_j, epsilon_ ) )
         return;

      locations_[&j] = pos_j;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationLocalCoordinateSystem::
   set_src_node( const shawn::Node& src_node )
      throw()
   {
      src_node_ = &src_node;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationLocalCoordinateSystem::
   set_position( const shawn::Vec& new_src_pos )
      throw()
   {
      for ( LocationMapIterator
               it = locations_.begin();
               it != locations_.end();
               ++it )
         it->second += new_src_pos - src_pos_;

      src_pos_ = new_src_pos;
   }
   // ----------------------------------------------------------------------
   double
   LocalizationLocalCoordinateSystem::
   epsilon( void )
      const throw()
   {
      return epsilon_;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationLocalCoordinateSystem::
   set_epsilon( double epsilon )
      throw()
   {
      epsilon_ = epsilon;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationLocalCoordinateSystem::
   is_valid( void )
      const throw()
   {
      return valid_;
   }
   // ----------------------------------------------------------------------
   int
   LocalizationLocalCoordinateSystem::
   size( void )
      const throw()
   {
      return locations_.size();
   }
   // ----------------------------------------------------------------------
   const shawn::Node&
   LocalizationLocalCoordinateSystem::
   src_node( void )
      const throw()
   {
      assert( src_node_ != NULL );
      return *src_node_;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationLocalCoordinateSystem::
   has_neighbor( const shawn::Node& node )
      const throw()
   {
      return ( locations_.find( &node ) != locations_.end() );
   }
   // ----------------------------------------------------------------------
   const shawn::Vec&
   LocalizationLocalCoordinateSystem::
   node_position( const shawn::Node& node )
      const throw()
   {
      if ( locations_.find( &node ) != locations_.end() )
         return locations_.find( &node )->second;

      return UNKNOWN_POSITION;
   }
   // ----------------------------------------------------------------------
   const shawn::Vec&
   LocalizationLocalCoordinateSystem::
   src_position( void )
      const throw()
   {
      return src_pos_;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationLocalCoordinateSystem::
   perform_correction( const CorrectionData& cd )
      throw()
   {
      rotate( cd.correction_angle );
      if ( cd.mirror )
         mirror_x();
      set_position( cd.pos );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationLocalCoordinateSystem::
   rotate( double angle )
      throw()
   {
      for ( LocationMapIterator
               it = locations_.begin();
               it != locations_.end();
               ++it )
         rotate_2D( angle, it->second, src_pos_ );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationLocalCoordinateSystem::
   mirror_x( void )
      throw()
   {
      for ( LocationMapIterator
               it = locations_.begin();
               it != locations_.end();
               ++it )
         it->second = shawn::Vec( -it->second.x(), it->second.y(), it->second.z() );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationLocalCoordinateSystem::
   mirror_y( void )
      throw()
   {
      for ( LocationMapIterator
               it = locations_.begin();
               it != locations_.end();
               ++it )
         it->second = shawn::Vec( it->second.x(), -it->second.y(), it->second.z() );
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationLocalCoordinateSystem::
   correct_lcs( const LocalizationLocalCoordinateSystem& lcs, CorrectionData& cd )
      const throw()
   {
      if ( !is_valid() || !lcs.is_valid() ||
            lcs.node_position( src_node() ) == UNKNOWN_POSITION ||
            node_position( lcs.src_node() ) == UNKNOWN_POSITION )
         return false;

      for ( ConstLocationMapIterator
               it = locations_.begin();
               it != locations_.end();
               ++it )
         if ( lcs.has_neighbor( *it->first ) )
         {
            const shawn::Node *bas_node = &lcs.src_node();
            const shawn::Node *ref_node = it->first;
            const shawn::Node *me = &src_node();

            if ( lcs.node_position( *ref_node ) == UNKNOWN_POSITION ||
                  node_position( *ref_node ) == UNKNOWN_POSITION ||
                  bas_node == ref_node || me == ref_node )
               continue;

            double angle_bas_me = angle_vec( lcs.node_position( *me ) - lcs.src_position() );
            double angle_bas_ref = angle_vec( lcs.node_position( *ref_node ) - lcs.src_position() );
            double angle_me_bas = angle_vec( node_position( *bas_node ) - src_position() );
            double angle_me_ref = angle_vec( node_position( *ref_node ) - src_position() );

            double me_bas = angle_me_ref - angle_me_bas;
            double bas_me = angle_bas_ref - angle_bas_me;

            me_bas = normalize_angle( me_bas );
            bas_me = normalize_angle( bas_me );

            if ( ( bas_me < M_PI && me_bas < M_PI ) ||
               ( bas_me > M_PI && me_bas > M_PI ) )
            {
               double correction_angle = angle_bas_me + angle_me_bas;
               cd.correction_angle = -correction_angle;
               cd.mirror = true;
            }
            else // if ( ( bas_me < M_PI && me_bas > M_PI ) ||
                 //       ( bas_me > M_PI && me_bas < M_PI ) )
            {
               double correction_angle = angle_bas_me - angle_me_bas + M_PI;
               cd.correction_angle = correction_angle;
               cd.mirror = false;
            }

            cd.correction_angle = normalize_angle( cd.correction_angle );
            cd.pos = lcs.node_position( src_node() );

            return true;
         }

      return false;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationLocalCoordinateSystem::
   correct_lcs_to_real_ncs( CorrectionData& cd )
      const throw()
   {
      if ( !is_valid() ) return false;

      const shawn::Node *bas_node = q_;
      const shawn::Node *ref_node = p_;
      const shawn::Node *me = &src_node();

      double angle_bas_me = angle_vec( me->real_position() - bas_node->real_position() );
      double angle_bas_ref = angle_vec( ref_node->real_position() - bas_node->real_position() );
      double angle_me_bas = angle_vec( node_position( *bas_node ) - src_position() );
      double angle_me_ref = angle_vec( node_position( *ref_node ) - src_position() );

      double me_bas = angle_me_ref - angle_me_bas;
      double bas_me = angle_bas_ref - angle_bas_me;

      me_bas = normalize_angle( me_bas );
      bas_me = normalize_angle( bas_me );

      if ( ( bas_me < M_PI && me_bas < M_PI ) ||
            ( bas_me > M_PI && me_bas > M_PI ) )
      {
         double correction_angle = angle_bas_me + angle_me_bas;
         cd.correction_angle = -correction_angle;
         cd.mirror = true;
      }
      else // if ( ( bas_me < M_PI && me_bas > M_PI ) ||
           //       ( bas_me > M_PI && me_bas < M_PI ) )
      {
         double correction_angle = angle_bas_me - angle_me_bas + M_PI;
         cd.correction_angle = correction_angle;
         cd.mirror = false;
      }

      cd.correction_angle = normalize_angle( cd.correction_angle );
      cd.pos = me->real_position();

      return true;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationLocalCoordinateSystem::
   adapt_to_real_positions( void )
      throw()
   {
      src_pos_ = src_node().real_position();

      for ( LocationMapIterator
               it = locations_.begin();
               it != locations_.end();
               ++it )
         it->second = it->first->real_position();
   }
   // ----------------------------------------------------------------------
   void
   LocalizationLocalCoordinateSystem::
   clear( void )
      throw()
   {
      p_ = NULL;
      q_ = NULL;
      neighborhood_ = NULL;

      src_pos_ = shawn::Vec( 0.0, 0.0, 0.0 );
      locations_.clear();
      valid_ = false;
   }
   // ----------------------------------------------------------------------
   std::ostream&
   operator<<(
         std::ostream& os,
         const LocalizationLocalCoordinateSystem& llcs )
      throw()
   {
      os << "LCS Members : " << std::endl;

      for ( ConstLocationMapIterator
               it = llcs.locations_.begin();
               it != llcs.locations_.end();
               ++it )
         os << "\t" << it->first->label();

      return os;
   }

}// namespace localization
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/neighborhood/localization_local_coordinate_system.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_local_coordinate_system.cpp,v $
 *-----------------------------------------------------------------------*/
