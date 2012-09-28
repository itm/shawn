/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/simulation/basic_tasks.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include "sys/misc/degree/degree_histogram.h"
#include "sys/processors/processor_keeper.h"
#include "sys/misc/random/random_variable_keeper.h"
#include "sys/misc/random/random_variable.h"
#include "sys/simulation/environment_config_loader.h"

#include "sys/edge_models/lazy_edge_model.h"
#include "sys/edge_models/list_edge_model.h"
#include "sys/edge_models/edge_model_keeper.h"
#include "sys/transm_models/transmission_model_keeper.h"
#include "sys/comm_models/communication_model_keeper.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/node_distance_estimate.h"
#include "sys/node_movements/playback/movement_controller.h"
#include "sys/tag.h"

#include <limits>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;

namespace shawn
{

	// ----------------------------------------------------------------------
	SimulationTaskSimulation::
		SimulationTaskSimulation()
	{}

	// ----------------------------------------------------------------------
	SimulationTaskSimulation::
		~SimulationTaskSimulation()
   {}

   // ----------------------------------------------------------------------
	void
		SimulationTaskSimulation::
		run( SimulationController& sc )
    	throw( std::runtime_error )
   {
      int max_it = sc.environment().optional_int_param( "max_iterations", std::numeric_limits<int>::max() );
      bool run_without_nodes = sc.environment().optional_bool_param("run_without_nodes", false);

      if( !sc.has_world() )
         throw std::runtime_error( std::string("Task '") + name() + std::string("' needs a world.") );

      World& w = sc.world_w();

      w.scheduler_w().set_rate_adaptation(sc.environment().optional_bool_param("enable_realtime_rate", false));

      while( (run_without_nodes || !w.is_done()) && (max_it>0) )
      {
    	  w.step();
    	  --max_it;
      }
   }

   // ----------------------------------------------------------------------
	std::string
		SimulationTaskSimulation::
		name( void )
		const throw()
   {
      return "simulation";
   }

	// ----------------------------------------------------------------------
	std::string
		SimulationTaskSimulation::
		description( void )
		const throw()
   {
      return "runs the real simulation";
   }






	// ----------------------------------------------------------------------
	SimulationTaskAddWorldTask::
		SimulationTaskAddWorldTask( bool pre )
		: pre_ ( pre )
	{}

	// ----------------------------------------------------------------------
	SimulationTaskAddWorldTask::
		~SimulationTaskAddWorldTask()
	{}

	// ----------------------------------------------------------------------
	void
		SimulationTaskAddWorldTask::
		run( SimulationController& sc )
		throw( std::runtime_error )
	{
		require_world(sc);
		std::string task = sc.environment().required_string_param( "task" );
		SimulationTaskHandle sth = sc.simulation_task_keeper_w().find_w(task);

		if( pre_ )
			sc.world_w().add_pre_step_task( sth );
		else
			sc.world_w().add_post_step_task( sth );
	}

	// ----------------------------------------------------------------------
	std::string
		SimulationTaskAddWorldTask::
		name( void )
		const throw()
	{
		if( pre_ )
			return std::string("add_prestep");
		else
			return std::string("add_poststep");
   }

	// ----------------------------------------------------------------------
	std::string
		SimulationTaskAddWorldTask::
		description( void )
		const throw()
	{
		return std::string("makes $task being run ") + std::string( pre_ ? "before" : "after") + std::string("each simulation round");
	}





	SimulationTaskWorld::
		SimulationTaskWorld()
	{}

	// ----------------------------------------------------------------------
	SimulationTaskWorld::
		~SimulationTaskWorld()
	{}

	// ----------------------------------------------------------------------
	void
		SimulationTaskWorld::
		run( SimulationController& sc )
		throw( std::runtime_error )
   {
      World* nw = new World;
      nw->set_simulation_controller(sc);

      try {
	     load_environment_tags(sc);
         set_comm_model( sc, *nw );
         set_edge_model( sc, *nw );
         set_transmission_model( sc, *nw );
	     set_movement_controller( sc, *nw );

		 //Set the world size hint
         double size_hint = sc.environment().optional_double_param("size_hint", -1.0 );
         if( size_hint >= 0.0 )
            nw->set_size_hint( size_hint );

      }
      catch( std::runtime_error& )
	  {
         delete nw;
         throw;
      }

      nw->init();
	  sc.set_world(nw);
   }

   // ----------------------------------------------------------------------
   void
	   SimulationTaskWorld::
	   load_environment_tags(SimulationController& sc)
	   const  throw( std::runtime_error )
   {
	   std::string config_file = sc.environment().optional_string_param("env_config", "");
	   if( config_file != "" )
	   {
		   EnvironmentConfigLoader cl(sc);
		   cl.set_document_uri(config_file);
		   cl.parse();
	   }
   }

   // ----------------------------------------------------------------------
   void
	   SimulationTaskWorld::
	   set_edge_model( SimulationController& sc, World& w )
	   const throw( std::runtime_error )
   {
	   std::string em = sc.environment().required_string_param("edge_model");
	   EdgeModelFactoryHandle emfh = sc.edge_model_keeper_w().find_w(em);
	   w.set_edge_model( *(emfh->create(sc)) );
   }

   // ----------------------------------------------------------------------
   void
	   SimulationTaskWorld::
	   set_transmission_model( SimulationController& sc, World& w )
	   const throw( std::runtime_error )
   {
	   std::string tm_name = sc.environment().optional_string_param("transm_model", "reliable");
	   TransmissionModelFactoryHandle tmfh = sc.transmission_model_keeper_w().find_w(tm_name);
	   w.set_transmission_model( *(tmfh->create(sc)) );
   }

   // ----------------------------------------------------------------------
   void
	   SimulationTaskWorld::
	   set_comm_model( SimulationController& sc, World& w)
	   const throw( std::runtime_error )
   {
	   std::string name = sc.environment().optional_string_param("comm_model", "disk_graph");
	   CommunicationModelFactoryHandle fh = sc.communication_model_keeper_w().find_w(name);
	   w.set_communication_model(*(fh->create(sc)));
   }

   // ----------------------------------------------------------------------
   void
   SimulationTaskWorld::
   set_movement_controller( SimulationController& sc, World& w )
      const throw( std::runtime_error )
   {
      w.set_movement_controller( new MovementController(sc) );
   }

   // ----------------------------------------------------------------------
   std::string
   SimulationTaskWorld::
   name( void )
      const throw()
   {
      return "prepare_world";
   }

   // ----------------------------------------------------------------------
   std::string
   SimulationTaskWorld::
   description( void )
      const throw()
   {
      return "constructs an empty world with given models. $transm_model defaults to 'reliable'";
   }







#ifndef WIN32
	#warning MOVE TASK TO MISC/DEGFREE
#endif

   SimulationTaskDegreeHistogram::
   	SimulationTaskDegreeHistogram()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskDegreeHistogram::
   ~SimulationTaskDegreeHistogram()
   {}
   // ----------------------------------------------------------------------
   void
   SimulationTaskDegreeHistogram::
   run( SimulationController& sc )
      throw( std::runtime_error )
   {
      if( !sc.has_world() )
         throw std::runtime_error( std::string("Task '") + name() + std::string("' needs a world.") );
      std::string ofn = sc.environment().required_string_param("dhist_file");

      ofstream out(ofn.c_str());
      if( !out )
         throw std::runtime_error( std::string("Cannot write to '") + ofn + std::string("'!") );

      DegreeHistogram dhist;
      dhist.set_world(sc.world());
      dhist.init();
      out << dhist << endl;
      out << "PEAK: " << dhist.peak_degree() << endl;

      int sum=0;
      int cnt=0;
      for( int i=0; i<=dhist.max_degree(); ++i )
         {
            sum+=dhist.value(i)*i;
            cnt+=dhist.value(i);
         }
      out << "AVG: " << double(sum)/double(cnt) << endl;
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskDegreeHistogram::
   name( void )
      const throw()
   {
      return "dhist";
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskDegreeHistogram::
   description( void )
      const throw()
   {
      return "writes a degree histogram to $dhist_file";
   }








   template<typename KeeperClass>
   SimulationTaskShowHandleKeeper<KeeperClass>::
   SimulationTaskShowHandleKeeper()
   {}
   // ----------------------------------------------------------------------
   template<typename KeeperClass>
   SimulationTaskShowHandleKeeper<KeeperClass>::
   ~SimulationTaskShowHandleKeeper()
   {}
   // ----------------------------------------------------------------------
   template<typename KeeperClass>
   void
   SimulationTaskShowHandleKeeper<KeeperClass>::
   run( SimulationController& sc )
      throw( std::runtime_error )
   {
      bool v = sc.environment().optional_bool_param("verbose",0);
      keeper(sc).print_contents(std::cout,v);
   }


   SimulationTaskShowTasks::
   SimulationTaskShowTasks()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskShowTasks::
   ~SimulationTaskShowTasks()
   {}
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskShowTasks::
   name( void )
      const throw()
   {
      return std::string("show_tasks");
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskShowTasks::
   description( void )
      const throw()
   {
      return std::string("prints all known simulation tasks");
   }
   // ----------------------------------------------------------------------
   const SimulationTaskKeeper&
   SimulationTaskShowTasks::
   keeper( const SimulationController& sc )
      const throw()
   {
      return sc.simulation_task_keeper();
   }







   SimulationTaskShowProcessors::
   SimulationTaskShowProcessors()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskShowProcessors::
   ~SimulationTaskShowProcessors()
   {}
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskShowProcessors::
   name( void )
      const throw()
   {
      return std::string("show_processors");
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskShowProcessors::
   description( void )
      const throw()
   {
      return std::string("prints all known processors (resp. factories)");
   }
   // ----------------------------------------------------------------------
   const ProcessorKeeper&
   SimulationTaskShowProcessors::
   keeper( const SimulationController& sc )
      const throw()
   {
      return sc.processor_keeper();
   }







   SimulationTaskShowRandomVariables::
   SimulationTaskShowRandomVariables()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskShowRandomVariables::
   ~SimulationTaskShowRandomVariables()
   {}
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskShowRandomVariables::
   name( void )
      const throw()
   {
      return std::string("show_random_variables");
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskShowRandomVariables::
   description( void )
      const throw()
   {
      return std::string("prints all registered random variables");
   }
   // ----------------------------------------------------------------------
   const RandomVariableKeeper&
   SimulationTaskShowRandomVariables::
   keeper( const SimulationController& sc )
      const throw()
   {
      return sc.random_variable_keeper();
   }





   SimulationTaskShowEdgeModels::
   SimulationTaskShowEdgeModels()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskShowEdgeModels::
   ~SimulationTaskShowEdgeModels()
   {}
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskShowEdgeModels::
   name( void )
      const throw()
   {
      return std::string("show_edge_models");
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskShowEdgeModels::
   description( void )
      const throw()
   {
      return std::string("prints all registered edge models (resp. factories)");
   }
   // ----------------------------------------------------------------------
   const EdgeModelKeeper&
   SimulationTaskShowEdgeModels::
   keeper( const SimulationController& sc )
      const throw()
   {
      return sc.edge_model_keeper();
   }






   SimulationTaskShowDistanceEstimates::
   SimulationTaskShowDistanceEstimates()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskShowDistanceEstimates::
   ~SimulationTaskShowDistanceEstimates()
   {}
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskShowDistanceEstimates::
   name( void )
      const throw()
   {
      return std::string("show_distance_estimates");
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskShowDistanceEstimates::
   description( void )
      const throw()
   {
      return std::string("prints all registered NodeDistanceEstimate instances");
   }
   // ----------------------------------------------------------------------
   const DistanceEstimateKeeper&
   SimulationTaskShowDistanceEstimates::
   keeper( const SimulationController& sc )
      const throw()
   {
      return sc.distance_estimate_keeper();
   }


   void init_basic_tasks( SimulationController& sc ) throw()
   {
      sc.simulation_task_keeper_w().add( new SimulationTaskSimulation );
      sc.simulation_task_keeper_w().add( new SimulationTaskWorld );
      sc.simulation_task_keeper_w().add( new SimulationTaskDegreeHistogram );
      sc.simulation_task_keeper_w().add( new SimulationTaskShowTasks );
      sc.simulation_task_keeper_w().add( new SimulationTaskShowProcessors );
      sc.simulation_task_keeper_w().add( new SimulationTaskShowRandomVariables );
      sc.simulation_task_keeper_w().add( new SimulationTaskShowEdgeModels );
      sc.simulation_task_keeper_w().add( new SimulationTaskShowDistanceEstimates );
      sc.simulation_task_keeper_w().add( new SimulationTaskAddWorldTask(true) );
      sc.simulation_task_keeper_w().add( new SimulationTaskAddWorldTask(false) );
   }



}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/simulation/basic_tasks.cpp,v $
 * Version $Revision: 517 $
 * Date    $Date: 2011-03-23 16:02:59 +0100 (Wed, 23 Mar 2011) $
 *-----------------------------------------------------------------------
 * $Log: basic_tasks.cpp,v $
 *-----------------------------------------------------------------------*/
