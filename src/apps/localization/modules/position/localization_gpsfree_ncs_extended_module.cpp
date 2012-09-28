/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/position/localization_gpsfree_ncs_extended_module.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

using namespace shawn;

namespace localization
{

   LocalizationGPSfreeNCSExtendedModule::
   LocalizationGPSfreeNCSExtendedModule()
      : wait_for_lcs_    ( 5 ),
         minimal_hop_cnt_( UNKNOWN_HOP_CNT )
   {}
   // ----------------------------------------------------------------------
   LocalizationGPSfreeNCSExtendedModule::
   ~LocalizationGPSfreeNCSExtendedModule()
   {}
   // ----------------------------------------------------------------------
   bool
   LocalizationGPSfreeNCSExtendedModule::
   process_message( const ConstMessageHandle& mh )
      throw()
   {
      ///
      if ( dynamic_cast<const LocalizationGPSfreeNCSLCSExtendedMessage*>( mh.get() ) != NULL )
         return process_gpsfree_ncs_lcs_extended_message(
            *dynamic_cast<const LocalizationGPSfreeNCSLCSExtendedMessage*>( mh.get() ) );

      return LocalizationModule::process_message( mh );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeNCSExtendedModule::
   work( void )
      throw()
   {
      if ( state() == gfncs_finished )
         return;

      // send initial message (only root_node)
      if ( root_node() && state() == gfncs_init )
      {
         set_state( gfncs_build_ncs );

         LocalizationLocalCoordinateSystem::CorrectionData cd;

         local_coord_sys_w().correct_lcs_to_real_ncs( cd );
         local_coord_sys_w().perform_correction( cd );
         node_w().set_est_position( cd.pos );

         send( new LocalizationGPSfreeNCSLCSExtendedMessage( local_coord_sys(), 1 ) );
         set_computed_ncs( true );
         set_state( gfncs_finished );
      }
      else if ( state() == gfncs_init )
      {
         set_state( gfncs_wait_lcs );
      }
      else if ( state() == gfncs_build_ncs &&
                  simulation_round() - last_useful_msg() > wait_for_lcs_ )
      {
         compute_ncs();
      }

      // maybe shutdown processor
      if ( simulation_round() - last_useful_msg() > observer().idle_shutdown_time() )
      {
         set_state( gfncs_finished );
      }
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationGPSfreeNCSExtendedModule::
   process_gpsfree_ncs_lcs_extended_message( const LocalizationGPSfreeNCSLCSExtendedMessage& lcs_mess )
      throw()
   {
      if ( computed_ncs() )
         return true;

      set_state( gfncs_build_ncs );
      set_last_useful_msg( simulation_round() );

      ConstLCShopMapIterator it = lcs_hop_map_.find( &lcs_mess.local_coord_sys() );
      if ( it == lcs_hop_map_.end() ||
            ( it != lcs_hop_map_.end() && it->second > lcs_mess.hop_cnt() ) )
      {
         lcs_hop_map_[ &lcs_mess.local_coord_sys() ] = lcs_mess.hop_cnt();

         if ( lcs_mess.hop_cnt() < minimal_hop_cnt_ )
            minimal_hop_cnt_ = lcs_mess.hop_cnt();
      }

      return true;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeNCSExtendedModule::
   compute_ncs( void )
      throw()
   {
      if ( computed_ncs() )
         return;

      LocalizationLocalCoordinateSystem::CorrectionData cd_mean, cd_temp;
      int cnt = 0;
      int cnt_pos = 0;

      cd_mean.correction_angle = UNKNOWN_ANGLE;
      cd_mean.pos = shawn::Vec( 0.0, 0.0, 0.0 );

      for ( ConstLCShopMapIterator
               it = lcs_hop_map_.begin();
               it != lcs_hop_map_.end();
               it++ )
      {
         if ( local_coord_sys().correct_lcs( *it->first, cd_temp ) )
         {
            if ( cd_mean.correction_angle == UNKNOWN_ANGLE )
            {
               cd_mean.correction_angle = cd_temp.correction_angle;
               cd_mean.mirror           = cd_temp.mirror;
               cd_mean.pos              = cd_temp.pos;

               DEBUG( owner().logger(),
                        "FIRST     : CorrAngle: " << cd_temp.correction_angle <<
                        ", Mirror: " << cd_temp.mirror <<
                        ", Pos: " << cd_temp.pos );
            }
            else
            {
               cd_mean.correction_angle += cd_temp.correction_angle;
               cd_mean.pos              += cd_temp.pos;

               DEBUG( owner().logger(),
                        "OK_FURTHER: CorrAngle: " << cd_temp.correction_angle <<
                        ", Mirror: " << cd_temp.mirror <<
                        ", Pos: " << cd_temp.pos );
            }

            cnt++;
         }
         else if ( cnt == 0 )
         {
            shawn::Vec pos = it->first->node_position( node() );
            if ( pos != UNKNOWN_POSITION )
            {
               cd_mean.pos += pos;
               cnt_pos++;

               DEBUG( owner().logger(), "ONLY_POS  : " << " Pos: " << pos );
            }
         }
      }

      if ( cnt > 0 )
      {
         cd_mean.correction_angle /= (double)cnt;
         cd_mean.pos /= (double)cnt;

         local_coord_sys_w().perform_correction( cd_mean );
         node_w().set_est_position( cd_mean.pos );

         send( new LocalizationGPSfreeNCSLCSExtendedMessage(
                     local_coord_sys(),
                     minimal_hop_cnt_ + 1 ) );
      }
      else if ( cnt_pos > 0 )
      {
         cd_mean.pos /= (double)cnt_pos;

         node_w().set_est_position( cd_mean.pos );
      }
      else
      {
         set_computed_ncs( true );
         set_state( gfncs_finished );
         return;
      }

      //DEBUG( owner().logger(),
      //         "----------------" << std::endl <<
      //         "CorrAngle: " << cd_mean.correction_angle <<
      //            ", Mirror: " << cd_mean.mirror <<
      //            ", Pos: " << cd_mean.pos << std::endl <<
      //         "EST: " << node().est_position() <<
      //            ", REAL: " << node().real_position() << std::endl <<
      //         "----------------------------------------------------------" );

      set_computed_ncs( true );
      set_state( gfncs_finished );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeNCSExtendedModule::
   init_parameters( void )
      throw()
   {
      const SimulationEnvironment& se = node().world().simulation_controller().environment();

      wait_for_lcs_ = se.optional_int_param( "loc_gpsfree_wait_for_lcs", wait_for_lcs_ );
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/position/localization_gpsfree_ncs_extended_module.cpp,v $
 * Version $Revision: 485 $
 * Date    $Date: 2011-01-12 13:57:34 +0100 (Wed, 12 Jan 2011) $
 *-----------------------------------------------------------------------
 * $Log: localization_gpsfree_ncs_extended_module.cpp,v $
 *-----------------------------------------------------------------------*/
