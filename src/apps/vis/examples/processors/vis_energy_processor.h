/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __VIS_ENERGY_PROCESSOR_H__
#define __VIS_ENERGY_PROCESSOR_H__
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#ifdef ENABLE_EXAMPLES

#include "apps/examples/processor/helloworld_processor.h"
#include "sys/event_scheduler.h"
#include "sys/processor.h"
#include <set>

namespace vis
{
	/** This is a Helloworld processor which sends a Hello message on a randomly chosen
	*  time to it's neighbors and a randomly chosen message size.
	*  Can be used by: processors=helloworld_random
	*  The Processor will send the message within six seconds as default.
	*  One can also specify the sending time by the intervals lower and upper bound.
	*  These can be set in Shawn with the parameters:
	*  "lowerBound = double_value" and "upperBound = double_value"
	*/
   class VisEnergyProcessor
	   : public helloworld::HelloworldProcessor,shawn::EventScheduler::EventHandler
   {
   public:
      ///@name construction / destruction
      ///@{
      ///
		VisEnergyProcessor();
		VisEnergyProcessor(double lowerBound, double upperBound);
      virtual ~VisEnergyProcessor();
		///@}
		///@name basic method inherited from shawn::HelloworldProcessor
		/**
		* Boots the processor.
		* Here an event is added to the shawn::EventScheduler which indicates
		* when message will be send
		*/
      virtual void boot( void ) throw();
		///@name basic method inherited from shawn::HelloworldProcessor
		/**
		* Is called when Processor receives a Message.
		*@sa shawn::HelloworldProcessor
		*/
      virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
		///@name basic method inherited from shawn::HelloworldProcessor
		/**
		* Sets the Processor to inactive if it has not received a message for a  specified interval
		*@sa shawn::HelloworldProcessor
		*/
		virtual void work( void ) throw();

		void timeout(shawn::EventScheduler & event_scheduler, shawn::EventScheduler::EventHandle event_handle, double time, shawn::EventScheduler::EventTagHandle & event_tag_handle) throw();
		///@name Method which calculates a uniform random double value within the given range.
		/**
		*@
		* Returns a double value within the given Bounds.
		* \Return random double variable
		*/
		virtual double random(double lowerBound, double upperBound) throw();


   private:
      int last_time_of_receive_;
		double sending_time_;
      std::set<const shawn::Node*> neighbours_;
		shawn::EventScheduler::EventHandle event_handle_;
   };


}

#endif
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_processor.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: helloworld_processor.h,v $
 *-----------------------------------------------------------------------*/

