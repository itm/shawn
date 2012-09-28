/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include <limits>
#include "apps/localization/modules/distance/localization_dv_hop_module.h"
#include "apps/localization/util/localization_defutils.h"
#include "sys/node.h"
#include "sys/world.h"


using namespace shawn;

namespace localization
{

   LocalizationDVhopModule::
   LocalizationDVhopModule()
      : state_            ( dvh_init ),
         last_useful_msg_ ( 0 ),
         avg_hop_dist_    ( UNKNOWN_AVG_HOP_DIST ),
         hop_sum_         ( 0 ),
         hop_cnt_         ( 0 )
   {}
   // ----------------------------------------------------------------------
   LocalizationDVhopModule::
   ~LocalizationDVhopModule()
   {}
   // ----------------------------------------------------------------------
   void
   LocalizationDVhopModule::
   boot( void )
      throw()
   {
      last_useful_msg_ = simulation_round();
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationDVhopModule::
   process_message( const ConstMessageHandle& mh )
      throw()
   {
      if ( dynamic_cast<const LocalizationDVhopMessage*>
               ( mh.get() ) != NULL )
      {
         if ( owner().is_anchor() )
            return process_dv_hop_message_anchor(
               *dynamic_cast<const LocalizationDVhopMessage*>( mh.get() ));
         else
            return process_dv_hop_message_unknown(
               *dynamic_cast<const LocalizationDVhopMessage*>( mh.get() ));
      }
      else if ( dynamic_cast<const LocalizationDVcalMessage*>
               ( mh.get() ) != NULL )
      {
         if ( !owner().is_anchor() )
            return process_dv_cal_message(
               *dynamic_cast<const LocalizationDVcalMessage*>( mh.get() ));
         else
            return true;
      }

      return LocalizationModule::process_message( mh );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationDVhopModule::
   work( void )
      throw()
   {
      // do initial stuff and set state to 'working'
      if ( state_ == dvh_init )
      {
         if ( owner().is_anchor() )
            send( new LocalizationDVhopMessage( node() ) );

         state_ = dvh_work;
      }

      // check, whether state can set to be finished or not
      // - unknown node is set finished, if given idle-time is passed ( no
      //   more useful messages came in ) and hops were converted into distances
      // - anchor is finished, if given idle-time is passed and there are
      //   at least one more anchors known, so that the average hop distance
      //   could estimated
      if ( state_ != dvh_finished && simulation_round() - last_useful_msg_ > observer().idle_time() )
      {
         if ( !owner().is_anchor() && state_ == dvh_calibrated )
         {
            state_ = dvh_finished;
         }
         else if ( owner().is_anchor() && state_ == dvh_calibrated )
         {
            send( new LocalizationDVcalMessage( hop_sum_ / hop_cnt_ ) );
            state_ = dvh_finished;
         }
      }
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationDVhopModule::
   process_dv_hop_message_anchor( const LocalizationDVhopMessage& ldvhm )
      throw()
   {
      if ( state_ == dvh_finished ) return true;

      const Node& anchor = ldvhm.anchor();

      // if the anchor is already known, 'floodlimit' is reached or
      // the message is from the anchor itself
      if ( ( known_anchors_.find( &anchor ) != known_anchors_.end() ) ||
           ( known_anchors_.size() >= observer().floodlimit() ) ||
           ( node() == anchor ) )
         return true;

      // add anchor to known ones and set time of last useful message to
      // actual
      known_anchors_.insert( &anchor );
      last_useful_msg_ = simulation_round();

      // calculate distance and build average hop distance to anchor

	  /*
	  *	Edited. If anchor has position error 
	  */
	double distance= euclidean_distance(
		  ((anchor.has_est_position())?(anchor.est_position()):(anchor.real_position())),
		  ((node().has_est_position())?(node().est_position()):(node().real_position()))
		  );
      double avg_hop_dist = distance / (ldvhm.hop_count() + 1);

      // build average hop distance of all known anchors
      ++hop_cnt_;
      hop_sum_ += avg_hop_dist;

      DEBUG( owner().logger(),
         node().label() << ": "
            << hop_sum_ << " / " << hop_cnt_ << " = " << hop_sum_ / hop_cnt_ );

      state_ = dvh_calibrated;
      return true;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationDVhopModule::
   process_dv_hop_message_unknown( const LocalizationDVhopMessage& ldvhm )
      throw()
   {
      if ( state_ == dvh_finished ) return true;

      const Node& source = ldvhm.source();
      const Node& anchor = ldvhm.anchor();
      Vec anchor_pos = ldvhm.anchor().real_position();
      int rcvd_hops = ldvhm.hop_count() + 1;

      // if anchor is new and floodlimit not reached, insert new anchor;
      // if anchor is known and new hop-distance is smaller than old one,
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
         if ( it->second->hops() < rcvd_hops )
            return true;
         else if ( it->second->hops() == rcvd_hops )
         {
            neighborhood_w().update_ref_node( anchor, source );
            return true;
         }
      }
      it->second->set_hops( rcvd_hops );
      if ( avg_hop_dist_ != UNKNOWN_AVG_HOP_DIST )
         it->second->convert_hops( avg_hop_dist_ );

      neighborhood_w().set_ref_node( anchor, source );

      // set time of last useful message to actual and broadcast new info
      last_useful_msg_ = simulation_round();
      send( new LocalizationDVhopMessage( ldvhm.anchor(), rcvd_hops ) );

      DEBUG( owner().logger(),
         node().label() << " : new or less hop-count to "
            << anchor.label() << " is " << rcvd_hops );

      return true;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationDVhopModule::
   process_dv_cal_message( const LocalizationDVcalMessage& ldvcm )
      throw()
   {
      if ( state_ == dvh_finished ) return true;

      // if there is already an average hop distance known, return
      if ( avg_hop_dist_ != UNKNOWN_AVG_HOP_DIST )
         return true;

      // set received info and convert hop-count into real distances
      avg_hop_dist_ = ldvcm.avg_hop_dist();
      for ( localization::NeighborhoodIterator
               it = neighborhood_w().begin_neighborhood_w();
               it != neighborhood_w().end_neighborhood_w();
               ++it )
         it->second->convert_hops( avg_hop_dist_ );

      // set state to calibrated and broadcast info
      state_ = dvh_calibrated;
      send( new LocalizationDVcalMessage( ldvcm ) );

      return true;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationDVhopModule::
   finished( void )
      throw()
   {
      return state_ == dvh_finished;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationDVhopModule::
   rollback( void )
      throw()
   {
      state_  = dvh_init;
      last_useful_msg_ = simulation_round();;
      avg_hop_dist_ = UNKNOWN_AVG_HOP_DIST;
      hop_sum_ = 0;
      hop_cnt_ = 0;
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/distance/localization_dv_hop_module.cpp,v $
 * Version $Revision: 511 $
 * Date    $Date: 2011-03-09 16:54:37 +0100 (Wed, 09 Mar 2011) $
 *-----------------------------------------------------------------------
 * $Log: localization_dv_hop_module.cpp,v $
 *-----------------------------------------------------------------------*/
