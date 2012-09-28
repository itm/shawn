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
#include "apps/localization/modules/position/localization_gpsfree_ncs_module.h"
#include "apps/localization/util/localization_defutils.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/taggings/basic_tags.h"


using namespace shawn;

namespace localization
{

   LocalizationGPSfreeNCSModule::
   LocalizationGPSfreeNCSModule()
      : state_            ( gfncs_init ),
         root_node_       ( false ),
         computed_ncs_    ( false ),
         last_useful_msg_ ( 0 )
   {}
   // ----------------------------------------------------------------------
   LocalizationGPSfreeNCSModule::
   ~LocalizationGPSfreeNCSModule()
   {}
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeNCSModule::
   boot( void )
      throw()
   {
      init_parameters();

      ConstTagHandle tag = node().find_tag( SEL_NODE_TAG_NAME );
      if ( tag.is_not_null() )
      {
         const BoolTag* boolt = dynamic_cast<const BoolTag*>( tag.get() );
         if ( boolt != NULL && boolt->value() )
         {
            root_node_ = true;
            owner_w().set_proc_type( LocalizationProcessorType::anchor );
         }
      }
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationGPSfreeNCSModule::
   process_message( const ConstMessageHandle& mh )
      throw()
   {
      ///
      if ( dynamic_cast<const LocalizationGPSfreeNCSLCSMessage*>( mh.get() ) != NULL )
         return process_gpsfree_ncs_lcs_message(
            *dynamic_cast<const LocalizationGPSfreeNCSLCSMessage*>( mh.get() ) );

      return LocalizationModule::process_message( mh );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeNCSModule::
   work( void )
      throw()
   {
      if ( state_ == gfncs_finished )
         return;

      // send initial message (only root_node)
      if ( root_node_ && state_ == gfncs_init )
      {
         state_ = gfncs_build_ncs;

         LocalizationLocalCoordinateSystem::CorrectionData cd;

         local_coord_sys().correct_lcs_to_real_ncs( cd );
         local_coord_sys_w().perform_correction( cd );
         node_w().set_est_position( cd.pos );

         send( new LocalizationGPSfreeNCSLCSMessage( local_coord_sys() ) );
         computed_ncs_ = true;
         state_ = gfncs_finished;
      }
      else if ( state_ == gfncs_init )
      {
         state_ = gfncs_wait_lcs;
      }

      // maybe shutdown processor
      if ( simulation_round() - last_useful_msg_ > observer().idle_shutdown_time() )
      {
         state_ = gfncs_finished;
      }
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationGPSfreeNCSModule::
   finished( void )
      throw()
   {
      return state_ == gfncs_finished;
   }
	 
	void
	LocalizationGPSfreeNCSModule::
	rollback( void )
		throw()
	{
		state_ = gfncs_init;
		root_node_ = false;
		computed_ncs_ = false;
		last_useful_msg_ = 0;
	}


   // ----------------------------------------------------------------------
   bool
   LocalizationGPSfreeNCSModule::
   process_gpsfree_ncs_lcs_message( const LocalizationGPSfreeNCSLCSMessage& lcs_mess )
      throw()
   {
      if ( computed_ncs_ )
         return true;

      last_useful_msg_ = simulation_round();

      LocalizationLocalCoordinateSystem::CorrectionData cd;
      // compute NCS
      if ( local_coord_sys().correct_lcs( lcs_mess.local_coord_sys(), cd ) )
      {
         local_coord_sys_w().perform_correction( cd );
         node_w().set_est_position( cd.pos );

         send( new LocalizationGPSfreeNCSLCSMessage( local_coord_sys() ) );
      }
      else
      {
         shawn::Vec pos = lcs_mess.local_coord_sys().node_position( node() );
         if ( pos != UNKNOWN_POSITION )
            node_w().set_est_position( pos );
      }

      computed_ncs_ = true;
      state_ = gfncs_finished;

      return true;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeNCSModule::
   init_parameters( void )
      throw()
   {
      // just a little bit empty, but maybe it'll be filled later
   }
   // ----------------------------------------------------------------------
   const LocalizationGPSfreeNCSModule::GPSfreeNCSState&
   LocalizationGPSfreeNCSModule::
   state( void )
      const throw()
   {
      return state_;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeNCSModule::
   set_state( const LocalizationGPSfreeNCSModule::GPSfreeNCSState& state )
      throw()
   {
      state_ = state;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationGPSfreeNCSModule::
   root_node( void )
      const throw()
   {
      return root_node_;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeNCSModule::
   set_root_node( bool root_node )
      throw()
   {
      root_node_ = root_node;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationGPSfreeNCSModule::
   computed_ncs( void )
      const throw()
   {
      return computed_ncs_;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeNCSModule::
   set_computed_ncs( bool computed_ncs )
      throw()
   {
      computed_ncs_ = computed_ncs;
   }
   // ----------------------------------------------------------------------
   int
   LocalizationGPSfreeNCSModule::
   last_useful_msg( void )
      const throw()
   {
      return last_useful_msg_;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeNCSModule::
   set_last_useful_msg( int last_useful_msg )
      throw()
   {
      last_useful_msg_ = last_useful_msg;
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/position/localization_gpsfree_ncs_module.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_gpsfree_ncs_module.cpp,v $
 *-----------------------------------------------------------------------*/
