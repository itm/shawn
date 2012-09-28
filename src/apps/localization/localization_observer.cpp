/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/localization_observer.h"
#include "apps/localization/util/localization_defutils.h"
#include "apps/localization/neighborhood/localization_neighbor_info.h"

#include <climits>
#include <cmath>


namespace localization
{

   LocalizationObserver::
   LocalizationObserver()
   {}
   // ----------------------------------------------------------------------
   LocalizationObserver::
   ~LocalizationObserver()
   {}
   // ----------------------------------------------------------------------
   void
   LocalizationObserver::
   set_owner( LocalizationProcessor& owner )
      throw()
   {
      owner_ = &owner;
   }
   // ----------------------------------------------------------------------
   const LocalizationProcessor&
   LocalizationObserver::
   owner( void )
      const throw()
   {
      return *owner_;
   }
   // ----------------------------------------------------------------------
   LocalizationProcessor&
   LocalizationObserver::
   owner_w( void )
      throw()
   {
      return *owner_;
   }
   // ----------------------------------------------------------------------
   double
   LocalizationObserver::
   startup_anchor_frac( void )
      const throw()
   {
      return owner().startup_anchor_frac_;
   }
   // ----------------------------------------------------------------------
   const std::string&
   LocalizationObserver::
   distance_algorithm( void )
      const throw()
   {
      return localization::DIST_ALGOS[owner().dist_algo_];
   }
   // ----------------------------------------------------------------------
   const std::string&
   LocalizationObserver::
   position_algorithm( void )
      const throw()
   {
      return localization::POS_ALGOS[owner().pos_algo_];
   }
   // ----------------------------------------------------------------------
   const std::string&
   LocalizationObserver::
   refinement_algorithm( void )
      const throw()
   {
      return localization::REF_ALGOS[owner().ref_algo_];
   }
   // ----------------------------------------------------------------------
   unsigned int
   LocalizationObserver::
   floodlimit( void )
      const throw()
   {
      return owner().floodlimit_;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationObserver::
   check_residue( void )
      const throw()
   {
      return owner().check_residue_;
   }
   // ----------------------------------------------------------------------
   int
   LocalizationObserver::
   idle_time( void )
      const throw()
   {
      return owner().idle_time_;
   }
   // ----------------------------------------------------------------------
   int
   LocalizationObserver::
   idle_shutdown_time( void )
      const throw()
   {
      if ( owner().idle_shutdown_time_ == 0 )
         return std::numeric_limits<int>::max();
      else
         return owner().idle_shutdown_time_;
   }
   // ----------------------------------------------------------------------
   double
   LocalizationObserver::
   comm_range( void )
      const throw()
   {
      return owner().comm_range_;
   }
   // ----------------------------------------------------------------------
   double
   LocalizationObserver::
   confidence( void )
      const throw()
   {
      return owner().confidence_;
   }
   // ----------------------------------------------------------------------
   int
   LocalizationObserver::
   neighbor_cnt( void )
      const throw()
   {
      return owner().owner().world().edge_model().nof_adjacent_nodes( owner().owner() ) - 1;
   }
   // ----------------------------------------------------------------------
   int
   LocalizationObserver::
   anchor_cnt( void )
      const throw()
   {
      return owner().neighborhood_.anchor_cnt();
   }
   // ----------------------------------------------------------------------
   int
   LocalizationObserver::
   valid_anchor_cnt( void )
      const throw()
   {
      return owner().neighborhood_.valid_anchor_cnt();
   }
   // ----------------------------------------------------------------------
   void
   LocalizationObserver::
   fill_stat_rel_anchor_distance(
         LocalizationStatistic& stat_real_err,
         LocalizationStatistic& stat_comm_err )
      const throw()
   {
      for ( localization::ConstNeighborhoodIterator
               it = owner().neighborhood_.begin_neighborhood();
               it != owner().neighborhood_.end_neighborhood();
               ++it )
         if ( it->second->is_anchor() && it->second->has_distance() )
         {
            double real_dist = euclidean_distance(
               owner().owner().real_position(),
               it->second->node().real_position() );

//TODO: Remove this unfortunate solution, because it is just an actually
//       work-around to compare the results with the paper of K. Langendoen.
//
//       Comment by the author:
//       The simulations have been done (and saved) without the following
//       check, too. They are just a little bit worse than the ones of
//       K. Langendoen, but the tendency is almost the same.
            if ( real_dist >= 1 )
               stat_real_err += ( ( it->second->distance() - real_dist ) / real_dist );
            stat_comm_err += ( ( it->second->distance() - real_dist ) / comm_range() );
         }
   }
   // ----------------------------------------------------------------------
 void
   LocalizationObserver::
   fill_stat_abs_anchor_distance(
         LocalizationStatistic& stat_abs_err)
      const throw()
   {
      for ( localization::ConstNeighborhoodIterator
               it = owner().neighborhood_.begin_neighborhood();
               it != owner().neighborhood_.end_neighborhood();
               ++it )
         if ( it->second->is_anchor() && it->second->has_distance() )
         {
            double real_dist = euclidean_distance(
               owner().owner().real_position(),
               it->second->node().real_position() );

//TODO: Remove this unfortunate solution, because it is just an actually
//       work-around to compare the results with the paper of K. Langendoen.
//
//       Comment by the author:
//       The simulations have been done (and saved) without the following
//       check, too. They are just a little bit worse than the ones of
//       K. Langendoen, but the tendency is almost the same.
            if ( real_dist >= 1 )
               stat_abs_err += std::abs( ( it->second->distance() - real_dist ));
         }
   }
   // ----------------------------------------------------------------------
   void
   LocalizationObserver::
   fill_stat_rel_neighbor_distance(
         LocalizationStatistic& stat_real_err,
         LocalizationStatistic& stat_comm_err )
      const throw()
   {
      for ( localization::ConstNeighborhoodIterator
               it = owner().neighborhood_.begin_neighborhood();
               it != owner().neighborhood_.end_neighborhood();
               ++it )
         if ( !it->second->is_anchor() && it->second->has_distance() )
         {
            double real_dist = euclidean_distance(
               owner().owner().real_position(),
               it->second->node().real_position() );

            stat_real_err += ( ( it->second->distance() - real_dist ) / real_dist );
            stat_comm_err += ( ( it->second->distance() - real_dist ) / comm_range() );
         }
   }
   // ----------------------------------------------------------------------
   void
   LocalizationObserver::
   fill_stat_abs_neighbor_distance(
         LocalizationStatistic& stat_abs_err)
      const throw()
   {
      for ( localization::ConstNeighborhoodIterator
               it = owner().neighborhood_.begin_neighborhood();
               it != owner().neighborhood_.end_neighborhood();
               ++it )
         if ( !it->second->is_anchor() && it->second->has_distance() )
         {
            double real_dist = euclidean_distance(
               owner().owner().real_position(),
               it->second->node().real_position() );

            stat_abs_err += ( ( it->second->distance() - real_dist ) );
		 }
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/localization_observer.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_observer.cpp,v $
 *-----------------------------------------------------------------------*/

