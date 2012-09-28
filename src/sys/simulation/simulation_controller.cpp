/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include "sys/simulation/simulation_task.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/processors/processor_keeper.h"
#include "sys/node_distance_estimate.h"
#include "sys/misc/random/random_variable_keeper.h"
#include "sys/edge_models/edge_model_keeper.h"
#include "sys/transm_models/transmission_model_keeper.h"
#include "sys/comm_models/communication_model_keeper.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/taggings/tag_factory_keeper.h"

using namespace std;

namespace shawn
{
   //const std::string SimulationController::WHITE_SPACE = " \t\n\r";
   // ----------------------------------------------------------------------
   SimulationController::
   SimulationController()
      : env_   ( new SimulationEnvironment ),
        world_ ( NULL )
   {
      init_keepers();
#ifdef ENABLE_LOGGING
      set_logger_name( "simulation_controller" );
#endif
   }
   // ----------------------------------------------------------------------
   void
   SimulationController::
   init_keepers( void )
      throw()
   {
      add_keeper( simulation_task_keeper_ = new SimulationTaskKeeper );
      add_keeper( processor_keeper_ = new ProcessorKeeper );
      add_keeper( random_variable_keeper_ = new RandomVariableKeeper );
      add_keeper( edge_model_keeper_ = new EdgeModelKeeper );
      add_keeper( transmission_model_keeper_ = new TransmissionModelKeeper );
      add_keeper( distance_estimate_keeper_ = new DistanceEstimateKeeper );
      add_keeper( tag_factory_keeper_ = new TagFactoryKeeper );
	   add_keeper( communication_model_keeper_ = new CommunicationModelKeeper );
   }
   // ----------------------------------------------------------------------
   void
   SimulationController::
   add_keeper( HandleKeeperBase* hkb )
      throw()
   {
      assert( hkb != NULL );
      assert( keeper_by_name_.find(hkb->name()) ==
              keeper_by_name_.end() );
      keeper_by_name_[hkb->name()]=hkb;
   }
   // ----------------------------------------------------------------------
   SimulationController::
   ~SimulationController()
   {
      if( env_ != NULL )   delete env_;
      if( world_ != NULL ) delete world_;

      for( NamedKeeperMap::iterator
              it = keeper_by_name_.begin();
           it != keeper_by_name_.end();
           ++it )
         delete it->second;
   }
   // ----------------------------------------------------------------------
   const SimulationEnvironment&
   SimulationController::
   environment( void )
      const throw()
   {
      assert( env_ != NULL );
      return *env_;
   }
   // ----------------------------------------------------------------------
   SimulationEnvironment&
   SimulationController::
   environment_w( void )
      throw()
   {
      assert( env_ != NULL );
      return *env_;
   }
   // ----------------------------------------------------------------------
   bool
   SimulationController::
   has_world( void )
      const throw()
   {
      return world_ != NULL;
   }
   // ----------------------------------------------------------------------
   World&
   SimulationController::
   world_w( void )
      throw()
   {
      assert( world_ != NULL );
      return *world_;
   }
   // ----------------------------------------------------------------------
   const World&
   SimulationController::
   world( void )
      const throw()
   {
      assert( world_ != NULL );
      return *world_;
   }
   // ----------------------------------------------------------------------
   void
   SimulationController::
   set_world( World* w )
      throw()
   {
      if( world_ != NULL )
         delete world_;
      world_=w;
      world_->set_simulation_controller(*this);

	  //Set the world on the tag factories
	  TagFactoryKeeper& tfk = tag_factory_keeper_w();
	  for(TagFactoryKeeper::HandleMapType::iterator it =  tfk.begin_handles_w(),
		  end = tfk.end_handles_w(); it!=end; ++it)
		  it->second->set_world(*w);
   }
   // ----------------------------------------------------------------------
   const SimulationTaskKeeper&
   SimulationController::
   simulation_task_keeper( void )
      const throw()
   {
      assert( simulation_task_keeper_ != NULL );
      return *simulation_task_keeper_;
   }
   // ----------------------------------------------------------------------
   SimulationTaskKeeper&
   SimulationController::
   simulation_task_keeper_w( void )
      throw()
   {
      assert( simulation_task_keeper_ != NULL );
      return *simulation_task_keeper_;
   }
   // ----------------------------------------------------------------------
   const ProcessorKeeper&
   SimulationController::
   processor_keeper( void )
      const throw()
   {
      assert( processor_keeper_ != NULL );
      return *processor_keeper_;
   }
   // ----------------------------------------------------------------------
   ProcessorKeeper&
   SimulationController::
   processor_keeper_w( void )
      throw()
   {
      assert( processor_keeper_ != NULL );
      return *processor_keeper_;
   }
   // ----------------------------------------------------------------------
   const EdgeModelKeeper&
   SimulationController::
   edge_model_keeper( void )
      const throw()
   {
      assert( edge_model_keeper_ != NULL );
      return *edge_model_keeper_;
   }
   // ----------------------------------------------------------------------
   EdgeModelKeeper&
   SimulationController::
   edge_model_keeper_w( void )
      throw()
   {
      assert( edge_model_keeper_ != NULL );
      return *edge_model_keeper_;
   }
   // ----------------------------------------------------------------------
   const TransmissionModelKeeper&
   SimulationController::
   transmission_model_keeper( void )
      const throw()
   {
      assert( transmission_model_keeper_ != NULL );
      return *transmission_model_keeper_;
   }
   // ----------------------------------------------------------------------
   TransmissionModelKeeper&
   SimulationController::
   transmission_model_keeper_w( void )
      throw()
   {
      assert( transmission_model_keeper_ != NULL );
      return *transmission_model_keeper_;
   }
   // ----------------------------------------------------------------------
   const CommunicationModelKeeper&
   SimulationController::
   communication_model_keeper( void )
      const throw()
   {
      assert( communication_model_keeper_ != NULL );
      return *communication_model_keeper_;
   }
   // ----------------------------------------------------------------------
   CommunicationModelKeeper&
   SimulationController::
   communication_model_keeper_w( void )
      throw()
   {
      assert( communication_model_keeper_ != NULL );
      return *communication_model_keeper_;
   }
   // ----------------------------------------------------------------------
   const RandomVariableKeeper&
   SimulationController::
   random_variable_keeper( void )
      const throw()
   {
      assert( random_variable_keeper_ != NULL );
      return *random_variable_keeper_;
   }
   // ----------------------------------------------------------------------
   RandomVariableKeeper&
   SimulationController::
   random_variable_keeper_w( void )
      throw()
   {
      assert( random_variable_keeper_ != NULL );
      return *random_variable_keeper_;
   }
   // ----------------------------------------------------------------------
   const DistanceEstimateKeeper&
   SimulationController::
   distance_estimate_keeper( void )
      const throw()
   {
      assert( distance_estimate_keeper_ != NULL );
      return *distance_estimate_keeper_;
   }
   // ----------------------------------------------------------------------
   DistanceEstimateKeeper&
   SimulationController::
   distance_estimate_keeper_w( void )
      throw()
   {
      assert( distance_estimate_keeper_ != NULL );
      return *distance_estimate_keeper_;
   }
   // ----------------------------------------------------------------------
   const TagFactoryKeeper&
   SimulationController::
   tag_factory_keeper( void )
      const throw()
   {
      assert( tag_factory_keeper_ != NULL );
      return *tag_factory_keeper_;
   }
   // ----------------------------------------------------------------------
   TagFactoryKeeper&
   SimulationController::
   tag_factory_keeper_w( void )
      throw()
   {
      assert( tag_factory_keeper_ != NULL );
      return *tag_factory_keeper_;
   }
}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/simulation/simulation_controller.cpp,v $
 * Version $Revision: 353 $
 * Date    $Date: 2009-05-28 12:14:08 +0200 (Thu, 28 May 2009) $
 *-----------------------------------------------------------------------
 * $Log: simulation_controller.cpp,v $
 *-----------------------------------------------------------------------*/
