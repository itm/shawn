/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/distance/localization_sum_dist_module.h"
#include "apps/localization/util/localization_defutils.h"
#include "sys/node.h"
#include "sys/world.h"


using namespace shawn;

namespace localization
{

   LocalizationSumDistModule::
   LocalizationSumDistModule()
      : state_( sd_init )
   {}
   // ----------------------------------------------------------------------
   LocalizationSumDistModule::
   ~LocalizationSumDistModule()
   {}
   // ----------------------------------------------------------------------
   void
   LocalizationSumDistModule::
   boot( void )
      throw()
   {
      last_useful_msg_ = simulation_round();
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationSumDistModule::
   process_message( const ConstMessageHandle& mh ) 
      throw()
   {
      if ( dynamic_cast<const LocalizationSumDistMessage*>( mh.get() ) != NULL )
      {
         return process_sum_dist_message(
            *dynamic_cast<const LocalizationSumDistMessage*>( mh.get() ) );
      }

      return LocalizationModule::process_message( mh );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationSumDistModule::
   work( void )
      throw()
   {
      // do initial stuff;
      //    if anchor, send 'init-message' and set state to 'finished';
      //    if unknown, start working
      if ( state_ == sd_init )
      {
         if ( owner().is_anchor() )
         {
            send( new LocalizationSumDistMessage( node() ) );
            state_ = sd_finished;
         }
         else
            state_ = sd_work;
      }

      // if given idle-time is passed ( no more useful messages came in ),
      // set state to 'finished'
      if ( simulation_round() - last_useful_msg_ > observer().idle_time() )
      {
//          std::cout << "SumDIst Finished with anchor=" << owner().is_anchor()
//                << ", simround=" << simulation_round()
//                << ", lastuseful=" << last_useful_msg_
//                << ", idletime=" << observer().idle_time() << std::endl;
         state_ = sd_finished;
      }
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationSumDistModule::
   process_sum_dist_message( const LocalizationSumDistMessage& lsdm )
      throw()
   {
      if ( state_ == sd_finished || owner().is_anchor() )
         return true;

      const Node& source = lsdm.source();
      const Node& anchor = lsdm.anchor();
      Vec anchor_pos = (lsdm.anchor().has_est_position())?(lsdm.anchor().est_position()):(lsdm.anchor().real_position());
      double rcvd_path = lsdm.path_length();

      double distance = estimate_distance(lsdm.source(), node());
      if ( distance ==UNKNOWN_DISTANCE )
         return false;
      rcvd_path += distance; 

      // if anchor is new and floodlimit not reached, insert new anchor;
      // if anchor is known and new distance is smaller than old one,
      //    update info;
      // otherwise return
      localization::NeighborhoodIterator it = neighborhood_w().find_w( anchor );
      if ( it == neighborhood_w().end_neighborhood_w() )
      {
         if ( neighborhood().anchor_cnt() >= (int)observer().floodlimit() )
            return true;

         neighborhood_w().update_anchor( anchor, anchor_pos );
         // set iterator to the new inserted anchor
         it = neighborhood_w().find_w( anchor );
      }
      else
      {
         if ( it->second->distance() <= rcvd_path )
            return true;
      }
      it->second->set_distance( rcvd_path );

      neighborhood_w().set_ref_node( anchor, source );

      // set time of last useful message to actual and broadcast new info
      last_useful_msg_ = simulation_round();
      send( new LocalizationSumDistMessage( lsdm.anchor(), rcvd_path ) );

      DEBUG( owner().logger(),
         node().label() << " : new or smaller path to "
            << anchor.label() << " is " << rcvd_path );

      return true;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationSumDistModule::
   finished( void )
      throw()
   {
      return state_ == sd_finished;
   }
   // ----------------------------------------------------------------------
   void LocalizationSumDistModule::
   rollback( void )
      throw()
   {
      state_ = sd_init;
      last_useful_msg_ = simulation_round();
   }
}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/distance/localization_sum_dist_module.cpp,v $
 * Version $Revision: 511 $
 * Date    $Date: 2011-03-09 16:54:37 +0100 (Wed, 09 Mar 2011) $
 *-----------------------------------------------------------------------
 * $Log: localization_sum_dist_module.cpp,v $
 *-----------------------------------------------------------------------*/
