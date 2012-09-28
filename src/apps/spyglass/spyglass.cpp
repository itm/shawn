/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "spyglass.h"
#ifdef ENABLE_SPYGLASS

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "apps/spyglass/spyglass.h"
#include "apps/spyglass/spyglass_packet.h"
#include "apps/spyglass/temperature_packet.h"
#include "apps/spyglass/color_packet.h"
#include "apps/spyglass/polygon_topology_packet.h"
#include "apps/spyglass/neighborhood_packet.h"
#include "apps/spyglass/string_packet.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

namespace spyglass
{
	Spyglass::
	Spyglass(std::string filen)
	{
		spyglass_outfile_ = filen;
		std::ofstream sgout(spyglass_outfile_.c_str(),std::ios_base::out | std::ios_base::binary );
	}
    // ----------------------------------------------------------------------
	Spyglass::
	~Spyglass()
	{}
	// ----------------------------------------------------------------------
	Spyglass*
	Spyglass::
		set_spyglass(std::string filen)
	{
		if ( spyglass_ == NULL )
			spyglass_ = new Spyglass(filen);
		else {
			spyglass_->spyglass_outfile_ = filen;
			std::ofstream sgout(spyglass_->spyglass_outfile_.c_str(),std::ios_base::out | std::ios_base::binary );
		}
		return spyglass_;
	}
	// ----------------------------------------------------------------------
	Spyglass*
	Spyglass::
		get_spyglass( void )
	{
		return spyglass_;
	}
	// ----------------------------------------------------------------------
   void
   Spyglass::
   sendSpyglassPacket( spyglass::SpyglassPacket* sp )
   throw( std::runtime_error )
   {
	   std::ofstream sgout(spyglass_outfile_.c_str(),std::ios_base::out | std::ios_base::binary | std::ios_base::app );
	   if( !sgout )
		   throw std::runtime_error( std::string("Cannot write to '") +
									spyglass_outfile_ +
									std::string("'!") );

	   std::vector<char> *byteVector;

	   spyglass::NeighborhoodPacket *nhood;
	   spyglass::TemperaturePacket *temp;
	   spyglass::ColorPacket *color;
	   spyglass::PolygonTopologyPacket *top;
	   spyglass::StringPacket *strp;
	   switch(sp->get_type()) 
	   {
	   case 1: // Neighbourhood
		   nhood = static_cast<spyglass::NeighborhoodPacket*>(sp);
		   byteVector = nhood->serialize();
		   break;
	   case 2: // Temperature
		   temp = static_cast<spyglass::TemperaturePacket*>(sp);
		   byteVector = temp->serialize();
		   break;
	   case 3: // Position
		   byteVector = sp->serialize();
		   break;
	   case 4: // Color
		   color = static_cast<spyglass::ColorPacket*>(sp);
		   byteVector = color->serialize();
		   break;
	   case 5: // PolygonTopology
		   top = static_cast<spyglass::PolygonTopologyPacket*>(sp);
		   byteVector = top->serialize();
		   break;
	   case 6: // (SignalAlarm)
		   byteVector = sp->serialize();
		   break;
	   case 7: // String
		   strp = static_cast<spyglass::StringPacket*>(sp);
		   byteVector = strp->serialize();
		   break;
	   case 8: // Knowlegde (a Person crossed the corridor)
		   byteVector = sp->serialize();
		   break;
	   }
	   
		for (int i=0; i<int(byteVector->size()); ++i)
		{
			sgout << (*byteVector)[i];
		}
		delete byteVector;
   }
   // ----------------------------------------------------------------------
	Spyglass* Spyglass::spyglass_ = NULL;

   // ----------------------------------------------------------------------
   SpyglassTask::
   SpyglassTask()
   {}
   // ----------------------------------------------------------------------
   SpyglassTask::
   ~SpyglassTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   SpyglassTask::
   name( void )
      const throw()
   {
      return "spyglass";
   }
   // ----------------------------------------------------------------------
   std::string
   SpyglassTask::
   description( void )
      const throw()
   {
      return "task for the visualisation in spyglass";
   }
   // ----------------------------------------------------------------------
   void
   SpyglassTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
	   std::string spyglass_outfile = sc.environment().required_string_param("spyglass_file");
	   Spyglass::set_spyglass(spyglass_outfile);
   }

}
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/spyglass/spyglass.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: spyglass.cpp,v $
 *-----------------------------------------------------------------------*/
