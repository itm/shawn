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
#include "apps/localization/modules/distance/localization_gpsfree_lcs_module.h"
#include "apps/localization/neighborhood/localization_local_coordinate_system.h"
#include "apps/localization/math/localization_triangulation.h"
#include "apps/localization/math/localization_general_math.h"
#include "apps/localization/math/localization_statistic.h"
#include "apps/localization/util/localization_defutils.h"
#include "apps/localization/util/localization_utils.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

using namespace shawn;

namespace localization
{

   LocalizationGPSfreeLCSModule::
   LocalizationGPSfreeLCSModule()
      : state_            ( gflcs_init ),
         last_useful_msg_ ( 0 )
   {}
   // ----------------------------------------------------------------------
   LocalizationGPSfreeLCSModule::
   ~LocalizationGPSfreeLCSModule()
   {}
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeLCSModule::
   boot( void )
      throw()
   {
      init_parameters();
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationGPSfreeLCSModule::
   process_message( const ConstMessageHandle& mh )
      throw()
   {
      ///
      if ( dynamic_cast<const LocalizationGPSfreeLCSInitMessage*>( mh.get() ) != NULL )
         return process_gpsfree_lcs_init_message(
            *dynamic_cast<const LocalizationGPSfreeLCSInitMessage*>( mh.get() ) );
      ///
      else if ( dynamic_cast<const LocalizationGPSfreeLCSNeighborMessage*>( mh.get() ) != NULL )
         return process_gpsfree_lcs_neighbor_message(
            *dynamic_cast<const LocalizationGPSfreeLCSNeighborMessage*>( mh.get() ) );

      return LocalizationModule::process_message( mh );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeLCSModule::
   work( void )
      throw()
   {
      // send initial messages
      if ( state_ == gflcs_init )
      {
         send( new LocalizationGPSfreeLCSInitMessage() );
         state_ = gflcs_wait_bc;
      }

      // after idle-time passed, initial messages of neighbors had already been
      // received and state is set to 'broadcast'.
      if ( state_ == gflcs_wait_bc && simulation_round() - last_useful_msg_ > observer().idle_time() )
      {
         state_ = gflcs_broadcast;
         last_useful_msg_ = simulation_round();
      }

      // broadcast collected information
      if ( state_ == gflcs_broadcast )
      {
         broadcast_neighborhood();
         state_ = gflcs_wait_w;
      }

      // after idle-time passed, neighborhood messages of neighbors had already been
      // received and state is set to 'work'.
      if ( state_ == gflcs_wait_w && simulation_round() - last_useful_msg_ > observer().idle_time() )
         state_ = gflcs_work;

      if ( state_ == gflcs_work )
      {
         build_lcs();
         state_ = gflcs_finished;
      }

      // maybe shutdown processor
      if ( simulation_round() - last_useful_msg_ > observer().idle_shutdown_time() )
      {
         state_ = gflcs_finished;
      }
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationGPSfreeLCSModule::
   finished( void )
      throw()
   {
      return state_ == gflcs_finished;
   }
   // ----------------------------------------------------------------------
	void
	LocalizationGPSfreeLCSModule::
	rollback( void )
		throw()
	{
		state_  = gflcs_init;
      last_useful_msg_ = 0;
	}

   // ----------------------------------------------------------------------
   bool
   LocalizationGPSfreeLCSModule::
   process_gpsfree_lcs_init_message( const LocalizationGPSfreeLCSInitMessage& lgflcsim )
      throw()
   {
      const Node& source = lgflcsim.source();
      Vec source_pos = lgflcsim.source().real_position();
      double distance = estimate_distance( source, node() );

      last_useful_msg_ = simulation_round();

      neighborhood_w().update_neighbor( source, distance );

      return true;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeLCSModule::
   broadcast_neighborhood( void )
      throw()
   {
      // send info about own neighborhood
      send( new LocalizationGPSfreeLCSNeighborMessage(
         neighborhood().neighbor_distance_map() ) );
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationGPSfreeLCSModule::
   process_gpsfree_lcs_neighbor_message( const LocalizationGPSfreeLCSNeighborMessage& lgflcsnm )
      throw()
   {
      // set neighborhood of received node
      neighborhood_w().update_nneighbors( lgflcsnm.source(), lgflcsnm.neighbors() );

      return true;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeLCSModule::
   build_lcs( void )
      throw()
   {
      for ( ConstNeighborhoodIterator
               it1 = neighborhood().begin_neighborhood();
               it1 != neighborhood().end_neighborhood();
               ++it1 )
         for ( ConstNeighborhoodIterator
                  it2 = it1;
                  it2 != neighborhood().end_neighborhood();
                  ++it2 )
         {
            if ( it1 == it2 )
               continue;

            LocalizationLocalCoordinateSystem lcs;
            lcs.update_basic_nodes( *it1->first, *it2->first, neighborhood() );

            for ( ConstNeighborhoodIterator
                     it3 = neighborhood().begin_neighborhood();
                     it3 != neighborhood().end_neighborhood();
                     ++it3 )
            {
               if ( it3 == it1 || it3 == it2 )
                  continue;

               lcs.update_node( *it3->first );
            }

            if (  lcs.is_valid() &&
                  ( !local_coord_sys().is_valid() ||
                      local_coord_sys().size() < lcs.size() ) )
               local_coord_sys_w() = lcs;
         }

      local_coord_sys_w().set_src_node( node() );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeLCSModule::
   init_parameters( void )
      throw()
   {
      const SimulationEnvironment& se = node().world().simulation_controller().environment();

      local_coord_sys_w().set_epsilon( se.optional_double_param( "loc_gpsfree_epsilon", local_coord_sys().epsilon() ) );
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/distance/localization_gpsfree_lcs_module.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_gpsfree_lcs_module.cpp,v $
 *-----------------------------------------------------------------------*/
