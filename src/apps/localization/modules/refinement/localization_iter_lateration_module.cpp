/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/refinement/localization_iter_lateration_module.h"
#include "apps/localization/math/localization_triangulation.h"
#include "apps/localization/util/localization_defutils.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/misc/random/basic_random.h"


using namespace shawn;

namespace localization
{

   LocalizationIterLaterationModule::
   LocalizationIterLaterationModule()
      : state_               ( il_init ),
         last_useful_msg_    ( 0 ),
         iteration_limit_    ( 5 ),
         iteration_cnt_      ( 0 ),
         min_confident_nbrs_ ( 5 ),
         twin_measure_       ( 0 ),
         abort_pos_update_   ( 0.001 ),
         res_acceptance_     ( 0.1 ),
         sound_              ( false )
   {}
   // ----------------------------------------------------------------------
   LocalizationIterLaterationModule::
   ~LocalizationIterLaterationModule()
   {}
   // ----------------------------------------------------------------------
   void
   LocalizationIterLaterationModule::
   boot( void )
      throw()
   {
      const SimulationEnvironment& se = node().world().simulation_controller().environment();

      iteration_limit_ = se.optional_int_param( "loc_ref_iteration_limit", iteration_limit_ );
      min_confident_nbrs_ = se.optional_int_param( "loc_ref_min_confident_nbrs", min_confident_nbrs_ );
      twin_measure_ = se.optional_double_param( "loc_ref_twin_measure", twin_measure_ );
      abort_pos_update_ = se.optional_double_param( "loc_ref_abort_pos_update", abort_pos_update_ );
      res_acceptance_ = se.optional_double_param( "loc_ref_res_acceptance", res_acceptance_ );
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationIterLaterationModule::
   process_message( const ConstMessageHandle& mh )
      throw()
   {
      if ( dynamic_cast<const LocalizationIterLaterationMessage*>( mh.get() ) != NULL )
      {
         return process_iter_lateration_message(
            *dynamic_cast<const LocalizationIterLaterationMessage*>( mh.get() ));
      }
      else if ( dynamic_cast<const LocalizationIterLaterationSoundMessage*>( mh.get() ) != NULL )
      {
         return process_iter_lateration_sound_message(
            *dynamic_cast<const LocalizationIterLaterationSoundMessage*>( mh.get() ));
      }

      return LocalizationModule::process_message( mh );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationIterLaterationModule::
   work( void )
      throw()
   {
      if ( state_ == il_finished )
         return;

      // do initial stuff;
      //    if anchor, send 'init-message' and set state to 'finished';
      //    if unknown, check whether the node is sound or not. if sound,
      //       send messages, if not, clear estimated position.
      if ( state_ == il_init )
      {
         if ( owner().is_anchor() )
         {
            send( new LocalizationIterLaterationMessage( observer().confidence() ) );
            state_ = il_finished;
         }
         else
         {
            sound_ = neighborhood().is_sound();

            if ( sound_ && node().has_est_position() )
            {
               send( new LocalizationIterLaterationMessage( observer().confidence() ) );
               send( new LocalizationIterLaterationSoundMessage() );
            }
            else
            {
               set_confidence( 0 );
               node_w().clear_est_position();
            }

            state_ = il_work;
         }
      }

      if ( state_ == il_work )
         iter_lateration_step();
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationIterLaterationModule::
   process_iter_lateration_sound_message( const LocalizationIterLaterationSoundMessage& lilsm )
      throw()
   {
      if ( owner().is_anchor() || state_ == il_finished )
         return true;

      neighborhood_w().add_sound( lilsm.source() );

      if ( !sound_ )
      {
         if ( (sound_ = neighborhood().is_sound()) )
            send( new LocalizationIterLaterationSoundMessage() );
      }

      return true;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationIterLaterationModule::
   process_iter_lateration_message( const LocalizationIterLaterationMessage& lilm )
      throw()
   {
      if ( owner().is_anchor() || state_ == il_finished )
         return true;

      const Node& source = lilm.source();
      Vec source_real_pos = lilm.source().real_position();
      Vec source_est_pos = lilm.source().est_position();
      double distance = estimate_distance( source, node() );

      neighborhood_w().update_neighbor( source, distance );
      localization::NeighborhoodIterator it = neighborhood_w().find_w( source );
      it->second->set_pos( source_est_pos );
      it->second->set_confidence( lilm.confidence() );

      return true;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationIterLaterationModule::
   iter_lateration_step( void )
      throw()
   { 
      if ( state_ == il_finished || !sound_ )
         return;

      if ( iteration_cnt_ >= iteration_limit_ )
         state_ = il_finished;
      ++iteration_cnt_;

      neighborhood_w().reassign_twins( twin_measure_ * observer().comm_range() );

	  if ( neighborhood().confident_neighbor_cnt() < min_confident_nbrs_ )
         return;

      Vec est_pos;
      NeighborInfoList neighbors;
      collect_neighbors( neighborhood(), lat_confident, neighbors );

	  // try to update position. if lateration fails, confidence is set to 0,
      // else position is updated and confidence is set to average of all
      // neighbor confidences
      if ( est_pos_lateration( neighbors, est_pos, lat_confident, false ) &&
            est_pos_lateration( neighbors, est_pos, lat_confident, true ) )
      {
         set_confidence( neighborhood().avg_neighbor_confidence() );

         // check validity of new position. if check fails, there is a chance
         // of 'res_acceptance_', which is normally set to 0.1, to accept
         // the position anyway. this is done to avoid being trapped in a
         // local minimum. moreover, if the bad position is accepted, the
         // confidence is reduced by 50%.
         if ( !check_residue( neighbors, est_pos, lat_confident, observer().comm_range() ) )
         {
            if ( res_acceptance_ > uniform_random_0i_1i() )
               set_confidence( observer().confidence() / 2 );
            else
            {
               if ( observer().confidence() == 0 ) return;
               set_confidence( 0 );
               node_w().clear_est_position();
               send( new LocalizationIterLaterationMessage( observer().confidence() ) );
               return;
            }
         }

         // check, whether the new position is very close to the old one or
         // not. if so, refinement is finished.
         if ( node().has_est_position() )
         {
            double pos_diff = euclidean_distance( est_pos, node().est_position() );

            if ( pos_diff < abort_pos_update_ * observer().comm_range() )
               state_ = il_finished;
         }

         node_w().set_est_position( est_pos );
	  }
      else
      {
	     if ( observer().confidence() == 0 ) return;
         set_confidence( 0 );
         node_w().clear_est_position();
      }

      send( new LocalizationIterLaterationMessage( observer().confidence() ) );
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationIterLaterationModule::
   finished( void )
      throw() 
   {
      return state_ == il_finished;
   }
    // ----------------------------------------------------------------------
	void
	LocalizationIterLaterationModule::
	rollback( void )
		throw()
	{
		state_ = il_init;
         last_useful_msg_ = 0;
         iteration_cnt_ = 0;
         sound_ = false;
	}

}// namespace localization

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/refinement/localization_iter_lateration_module.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_iter_lateration_module.cpp,v $
 *-----------------------------------------------------------------------*/
