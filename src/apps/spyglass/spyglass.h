/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_SPYGLASS_H
#define __SHAWN_APPS_SPYGLASS_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_SPYGLASS

#include <string>
#include "sys/simulation/simulation_task.h"

#define NEIGHBORHOODTYPE 1;
#define TEMPERATURETYPE 2;
#define POSITIONTYPE 3;
#define COLORTYPE 4;
#define POLYGONTOPOLOGYTYPE 5;

#define STRINGTYPE 7;


namespace shawn {
	class SimulationController;
	class SimulationTask;
}

namespace spyglass
{
	class SpyglassPacket;

	class Spyglass
		: public shawn::RefcntPointable
	{
	public:
		static Spyglass* set_spyglass(std::string);
		static Spyglass* get_spyglass( void );
		void sendSpyglassPacket( spyglass::SpyglassPacket* ) throw( std::runtime_error );
		
	protected:
		Spyglass(std::string);
		virtual ~Spyglass();

	private:
		static Spyglass* spyglass_;
		std::string spyglass_outfile_;
	};

   class SpyglassTask
	   : public shawn::SimulationTask
   {
   public:
      SpyglassTask();
      virtual ~SpyglassTask();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );

   };

}

#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/spyglass/spyglass.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: spyglass.h,v $
 *-----------------------------------------------------------------------*/
