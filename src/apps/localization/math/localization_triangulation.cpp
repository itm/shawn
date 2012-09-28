/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/math/localization_triangulation.h"
#include "apps/localization/math/localization_simple_matrix.h"
#include <algorithm>
#include <cmath>
#include <limits>


#define SQR(n) (n*n)

#ifdef WIN32
	#include <float.h>
	#define isnan _isnan
#else
	#define isnan std::isnan
#endif

using namespace shawn;

namespace localization
{

   bool
   est_pos_min_max( const NeighborInfoList& neighbors, Vec& pos )
      throw()
   {
      if ( neighbors.empty() ) return false;

      std::pair<Vec, Vec> intersection;
      ConstNeighborInfoListIterator it = neighbors.begin();

      intersection.first = Vec(
         (*it)->pos().x() - (*it)->distance(),
         (*it)->pos().y() - (*it)->distance() );
      intersection.second = Vec(
         (*it)->pos().x() + (*it)->distance(),
         (*it)->pos().y() + (*it)->distance() );

      for ( ++it; it != neighbors.end(); ++it )
      {
         intersection.first = Vec(
            std::max( intersection.first.x(),
                  (*it)->pos().x() - (*it)->distance() ),
            std::max( intersection.first.y(),
                  (*it)->pos().y() - (*it)->distance() ) );
         intersection.second = Vec(
            std::min( intersection.second.x(),
                  (*it)->pos().x() + (*it)->distance() ),
            std::min( intersection.second.y(),
                  (*it)->pos().y() + (*it)->distance() ) );
      }

      pos = Vec(
         ( intersection.first.x() + intersection.second.x() ) / 2,
         ( intersection.first.y() + intersection.second.y() ) / 2 );

      return true;
   }
   // ----------------------------------------------------------------------
   bool
   est_pos_lateration(
         const NeighborInfoList& neighbors,
         Vec& pos,
         const LaterationType& lat_type,
         bool use_pos )
      throw()
   {
      int nbr_size = neighbors.size();
      if ( nbr_size < 3 ) return false;

      SimpleMatrix<double> m_a;
      SimpleMatrix<double> m_b;
      SimpleMatrix<double> m_x;

      ConstNeighborInfoListIterator it = neighbors.begin();

      double x_1, y_1, d_1;
      if ( use_pos )
      {
         m_a = SimpleMatrix<double>( nbr_size, 2 );
         m_b = SimpleMatrix<double>( nbr_size, 1 );
         x_1 = pos.x();
         y_1 = pos.y();
         d_1 = 0;
      }
      else
      {
         m_a = SimpleMatrix<double>( nbr_size - 1, 2 );
         m_b = SimpleMatrix<double>( nbr_size - 1, 1 );
         x_1 = (*it)->pos().x();
         y_1 = (*it)->pos().y();
         d_1 = (*it)->distance();
         ++it;
      }

      int row = 0;
      for ( ; it != neighbors.end(); ++it )
      {
         double confidence = (*it)->confidence();
         if ( lat_type == lat_anchors ) confidence = 1;

         m_a(row,0) = 2 * ( (*it)->pos().x() - x_1 ) * confidence;
         m_a(row,1) = 2 * ( (*it)->pos().y() - y_1 ) * confidence;

       m_b(row,0) =
            ( SQR( (*it)->pos().x() ) - SQR( x_1 )
               + SQR( (*it)->pos().y() ) - SQR( y_1 )
               + SQR( d_1 )
               - SQR( (*it)->distance() ) )
            * confidence;
//	   std::cout << "distanz von Knoten"<<  <<"geschätzt: " << (*
/*		 m_b(row,0) = 
			 ( SQR( (*it)->pos().x() -  x_1 )
               + SQR( (*it)->pos().y() - y_1 )
               + SQR( d_1 )
               - SQR( (*it)->distance() ) ) *  0.5;
*/         row++;
      }

      if ( ( m_a.transposed() * m_a ).det() == 0 )
         return false;
      // solve Ax = b
      m_x = ( m_a.transposed() * m_a ).inverse()
         * ( m_a.transposed() * m_b );

      pos = Vec( m_x(0,0), m_x(1,0));

      return true;
   }
   // ----------------------------------------------------------------------
   bool
   check_residue(
         const NeighborInfoList& neighbors,
         const Vec& est_pos,
         const LaterationType& lat_type,
         double comm_range )
      throw()
   {
      int nbr_size = neighbors.size();
      if ( nbr_size == 0 ) return false;

      double residue = 0;
      double conf_sum = 0;

      for ( ConstNeighborInfoListIterator
               it = neighbors.begin();
               it != neighbors.end();
               ++it )
      {
         double confidence = (*it)->confidence();
         if ( lat_type == lat_anchors ) confidence = 1;

         conf_sum += confidence;
         residue +=
            ( sqrt(
                  SQR( est_pos.x() - (*it)->pos().x() ) +
                  SQR( est_pos.y() - (*it)->pos().y() ) )
               - (*it)->distance() )
            * confidence;
      }

      residue /= conf_sum;

      if ( fabs( residue ) > comm_range )
         return false;
      else
         return true;
   }
   // ----------------------------------------------------------------------
   DistancePair
   trilateration_distance(
         double s_n1,
         double s_n2,
         double n1_n2,
         double n1_anchor,
         double n2_anchor )
      throw()
   {
      // trilaterate triangle self-n1-n2
      double self_x = ( SQR(s_n1) - SQR(s_n2) + SQR(n1_n2) ) / ( 2*n1_n2 );
      double tmp = SQR(s_n1) - SQR(self_x);
      if ( tmp < -0.1 )
         return DistancePair( -1, -1 );

      double self_y = sqrt( fabs(tmp) );

      // trilaterate triangle anchor-n1-n2
      double anchor_x = ( SQR(n1_anchor) - SQR(n2_anchor) + SQR(n1_n2) ) / ( 2*n1_n2 );
      tmp = SQR(n1_anchor) - SQR(anchor_x);
      if ( tmp < -0.1 )
         return DistancePair( -1, -1 );
      double anchor_y = sqrt( fabs(tmp) );

      // distance between self and anchor
      double d_x = self_x - anchor_x;
      double d_y1 = self_y - anchor_y;
      double d_y2 = self_y + anchor_y;

      double dist1 = sqrt( SQR(d_x) + SQR(d_y1) );
      double dist2 = sqrt( SQR(d_x) + SQR(d_y2) );

      return DistancePair( dist1, dist2 );
   }
   // ----------------------------------------------------------------------
   void
   collect_neighbors(
         const LocalizationNeighborhood& nbrhood,
         const LaterationType& lat_type,
         NeighborInfoList& nbrs )
      throw()
   {
      for ( ConstNeighborhoodIterator
               it = nbrhood.begin_neighborhood();
               it != nbrhood.end_neighborhood();
               ++it )
         if ( ( lat_type == lat_anchors && it->second->is_anchor() && it->second->is_valid() ) ||
               ( lat_type == lat_confident && it->second->is_confident() ) )
            nbrs.push_back( it->second );
   }
   // ----------------------------------------------------------------------
   bool
   compute_basis_coords(
         double src_p, double src_q, double p_q,
         shawn::Vec& pos_p, shawn::Vec& pos_q )
      throw()
   {
      if ( p_q == UNKNOWN_DISTANCE || p_q == 0 ||
            src_p == UNKNOWN_DISTANCE || src_p == 0 ||
            src_q == UNKNOWN_DISTANCE || src_q == 0 )
         return false;

      double gamma = acos( ( pow(src_q,2) + pow(src_p,2) - pow(p_q,2) ) / ( 2 * src_p * src_q ) );

      double pos_x_q = src_q*cos(gamma);
      double pos_y_q = src_q*sin(gamma);

      if ( isnan( pos_x_q ) || isnan( pos_y_q ) )
         return false;

      pos_p = Vec( src_p, 0.0, 0.0 );
      pos_q = Vec( pos_x_q, pos_y_q, 0.0 );

      return true;
   }
   // ----------------------------------------------------------------------
   bool
   compute_rel_coord_triangulisation(
         double src_p, double src_q, double src_j, double p_j, double q_j, double p_q,
         shawn::Vec& pos_j, double epsilon )
      throw()
   {
      if ( src_j == UNKNOWN_DISTANCE || src_j == 0 ||
            p_j == UNKNOWN_DISTANCE || p_j == 0 ||
            q_j == UNKNOWN_DISTANCE || q_j == 0 )
         return false;

      double alpha_j =
         acos( ( pow(src_j,2) + pow(src_p,2) - pow(p_j,2) ) / ( 2 * src_j * src_p )  );
      double beta_j =
         acos( ( pow(src_j,2) + pow(src_q,2) - pow(q_j,2) ) / ( 2 * src_j * src_q )  );

      double gamma = acos( ( pow(src_q,2) + pow(src_p,2) - pow(p_q,2) ) / ( 2 * src_p * src_q ) );

      double pos_x_j = src_j * cos( alpha_j );
      double pos_y_j = 0.0;
      if ( fabs( fabs( alpha_j - gamma ) - beta_j ) <= epsilon )
         pos_y_j = src_j * sin( alpha_j );
      else
         pos_y_j = -src_j * sin( alpha_j );

      if ( isnan( pos_x_j ) || isnan( pos_y_j ) )
         return false;

      pos_j = Vec( pos_x_j, pos_y_j, 0.0 );

      return true;
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/math/localization_triangulation.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_triangulation.cpp,v $
 *-----------------------------------------------------------------------*/
