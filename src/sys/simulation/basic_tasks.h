/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_SIMULATION_BASIC_TASKS_H
#define __SHAWN_SYS_SIMULATION_BASIC_TASKS_H

#include "sys/simulation/simulation_task.h"

namespace shawn
{
   class World;
   class SimulationController;

   void init_basic_tasks( SimulationController& ) throw();


   class SimulationTaskSimulation
      : public SimulationTask
   {
   public:
      SimulationTaskSimulation();
      virtual ~SimulationTaskSimulation();
      virtual void run( SimulationController& ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   };



   class SimulationTaskAddWorldTask
      : public SimulationTask
   {
   public:
      SimulationTaskAddWorldTask( bool pre );
      virtual ~SimulationTaskAddWorldTask();
      virtual void run( SimulationController& ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   private:
      bool pre_;
   };

   class SimulationTaskWorld
      : public SimulationTask
   {
   public:
      SimulationTaskWorld();
      virtual ~SimulationTaskWorld();
      virtual void run( SimulationController& ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   protected:
	  virtual void load_environment_tags(SimulationController&)
			const  throw( std::runtime_error );
      virtual void set_edge_model( SimulationController&,
                                   World& ) const  throw( std::runtime_error );
      virtual void set_comm_model( SimulationController&,
                                   World& ) const throw( std::runtime_error );
      virtual void set_transmission_model( SimulationController& sc,
                                   World& ) const throw( std::runtime_error );
      virtual void set_movement_controller( SimulationController&,
                                   World& ) const throw( std::runtime_error );

   };



   class SimulationTaskDegreeHistogram
      : public SimulationTask
   {
   public:
      SimulationTaskDegreeHistogram();
      virtual ~SimulationTaskDegreeHistogram();
      virtual void run( SimulationController& ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   };


   template<typename KeeperClass>
   class SimulationTaskShowHandleKeeper
      : public SimulationTask
   {
   public:
      SimulationTaskShowHandleKeeper();
      virtual ~SimulationTaskShowHandleKeeper();
      virtual void run( SimulationController& ) throw( std::runtime_error );
    
   protected:
      virtual const KeeperClass& keeper( const SimulationController& ) const throw() = 0;
   };

   class SimulationTaskKeeper;
   class SimulationTaskShowTasks
      : public SimulationTaskShowHandleKeeper<SimulationTaskKeeper>
   {
   public:
      SimulationTaskShowTasks();
      virtual ~SimulationTaskShowTasks();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   protected:
      virtual const SimulationTaskKeeper& keeper( const SimulationController& ) const throw();
   };



   class ProcessorKeeper;
   class SimulationTaskShowProcessors
      : public SimulationTaskShowHandleKeeper<ProcessorKeeper>
   {
   public:
      SimulationTaskShowProcessors();
      virtual ~SimulationTaskShowProcessors();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   protected:
      virtual const ProcessorKeeper& keeper( const SimulationController& ) const throw();
   };

   class RandomVariableKeeper;
   class SimulationTaskShowRandomVariables
      : public SimulationTaskShowHandleKeeper<RandomVariableKeeper>
   {
   public:
      SimulationTaskShowRandomVariables();
      virtual ~SimulationTaskShowRandomVariables();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   protected:
      virtual const RandomVariableKeeper& keeper( const SimulationController& ) const throw();
   };

   class EdgeModelKeeper;
   class SimulationTaskShowEdgeModels
      : public SimulationTaskShowHandleKeeper<EdgeModelKeeper>
   {
   public:
      SimulationTaskShowEdgeModels();
      virtual ~SimulationTaskShowEdgeModels();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   protected:
      virtual const EdgeModelKeeper& keeper( const SimulationController& ) const throw();
   };

   class DistanceEstimateKeeper;
   class SimulationTaskShowDistanceEstimates
      : public SimulationTaskShowHandleKeeper<DistanceEstimateKeeper>
   {
   public:
      SimulationTaskShowDistanceEstimates();
      virtual ~SimulationTaskShowDistanceEstimates();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   protected:
      virtual const DistanceEstimateKeeper& keeper( const SimulationController& ) const throw();
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/simulation/basic_tasks.h,v $
 * Version $Revision: 649 $
 * Date    $Date: 2012-07-09 17:30:43 +0200 (Mon, 09 Jul 2012) $
 *-----------------------------------------------------------------------
 * $Log: basic_tasks.h,v $
 *-----------------------------------------------------------------------*/
