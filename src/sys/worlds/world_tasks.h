/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_WORLDS_WORLD_TASKS_H
#define __SHAWN_SYS_WORLDS_WORLD_TASKS_H

#include "shawn_config.h"
#include "sys/simulation/simulation_task.h"

namespace shawn
{
   class ProcessorWorldFactory;
   class SimulationController;

   void init_world_tasks( SimulationController& ) throw();


   class SimulationTaskProcessorWorldFactory
      : public SimulationTask
   {
   public:
      SimulationTaskProcessorWorldFactory();
      virtual ~SimulationTaskProcessorWorldFactory();
      virtual void run( SimulationController& ) throw( std::runtime_error );

   protected:
      virtual ProcessorWorldFactory* create_factory( SimulationController& )
         throw( std::runtime_error ) = 0;
      virtual void set_node_count( SimulationController&,
                                   ProcessorWorldFactory& ) throw( std::runtime_error );
      virtual void add_processor_factories( SimulationController&,
                                   ProcessorWorldFactory& ) throw( std::runtime_error );
   };






   class SimulationTaskRectWorldFactory
      : public SimulationTaskProcessorWorldFactory
   {
   public:
      SimulationTaskRectWorldFactory();
      virtual ~SimulationTaskRectWorldFactory();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();


   protected:
      virtual ProcessorWorldFactory* create_factory( SimulationController& )
         throw( std::runtime_error );
   };






   class SimulationTaskCuboidWorldFactory
      : public SimulationTaskProcessorWorldFactory
   {
   public:
      SimulationTaskCuboidWorldFactory();
      virtual ~SimulationTaskCuboidWorldFactory();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();


   protected:
      virtual ProcessorWorldFactory* create_factory( SimulationController& )
         throw( std::runtime_error );
   };

   
   
   
   
   class SimulationTaskFlegsensWorldFactory
         : public SimulationTaskProcessorWorldFactory
      {
      public:
    	  SimulationTaskFlegsensWorldFactory();
         virtual ~SimulationTaskFlegsensWorldFactory();

         virtual std::string name( void ) const throw();
         virtual std::string description( void ) const throw();


      protected:
         virtual ProcessorWorldFactory* create_factory( SimulationController& )
            throw( std::runtime_error );
         virtual void set_node_count( SimulationController&,
                                            ProcessorWorldFactory& ) throw( std::runtime_error );
      };





class SimulationTaskLoadWorldFactory
      : public SimulationTaskProcessorWorldFactory
   {
   public:
      SimulationTaskLoadWorldFactory();
      virtual ~SimulationTaskLoadWorldFactory();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   protected:
      virtual ProcessorWorldFactory* create_factory( SimulationController& )
         throw( std::runtime_error );
      virtual void set_node_count( SimulationController&,
                                   ProcessorWorldFactory& ) throw( std::runtime_error );
   };

   
}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/worlds/world_tasks.h,v $
 * Version $Revision: 195 $
 * Date    $Date: 2008-04-28 17:51:03 +0200 (Mon, 28 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: world_tasks.h,v $
 *-----------------------------------------------------------------------*/
