#ifndef __SHAWN_SYS_TAGGINGS_TAGGINGS_TASKS_H
#define __SHAWN_SYS_TAGGINGS_TAGGINGS_TASKS_H

#include "sys/simulation/simulation_task.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"

namespace shawn
{
	class World;
	class SimulationController;
   
	class SimulationTaskSetGlobalTagInteger
		: public SimulationTask
	{
	public:
		SimulationTaskSetGlobalTagInteger();
		virtual ~SimulationTaskSetGlobalTagInteger();
		virtual void run( SimulationController& ) throw( std::runtime_error );
		virtual std::string name( void ) const throw();
		virtual std::string description( void ) const throw();
	};
	
	class SimulationTaskSetGlobalTagDouble
		: public SimulationTask
	{
	public:
		SimulationTaskSetGlobalTagDouble();
		virtual ~SimulationTaskSetGlobalTagDouble();
		virtual void run( SimulationController& ) throw( std::runtime_error );
		virtual std::string name( void ) const throw();
		virtual std::string description( void ) const throw();
	};
	
	class SimulationTaskSetGlobalTagString
		: public SimulationTask
	{
	public:
		SimulationTaskSetGlobalTagString();
		virtual ~SimulationTaskSetGlobalTagString();
		virtual void run( SimulationController& ) throw( std::runtime_error );
		virtual std::string name( void ) const throw();
		virtual std::string description( void ) const throw();
	};
	
	class SimulationTaskSetGlobalTagBool
		: public SimulationTask
	{
	public:
		SimulationTaskSetGlobalTagBool();
		virtual ~SimulationTaskSetGlobalTagBool();
		virtual void run( SimulationController& ) throw( std::runtime_error );
		virtual std::string name( void ) const throw();
		virtual std::string description( void ) const throw();
	};
}


#endif
