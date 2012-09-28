/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#ifdef ENABLE_EXAMPLES

#include "apps/vis/examples/processors/vis_energy_processor.h"
#include "apps/examples/processor/helloworld_message.h"
#include "sys/node.h"
#include "sys/misc/random/basic_random.h"
#include "sys/taggings/basic_tags.h"
#include "sys/tag.h"
#include <iostream>

using namespace std;
using namespace shawn;

namespace vis
{
   VisEnergyProcessor::
	VisEnergyProcessor()
      : last_time_of_receive_( 0 )
   {
	sending_time_ = random(0,15);
		}
	VisEnergyProcessor::
	VisEnergyProcessor(double lowerBound,double upperBound): last_time_of_receive_( 0 )
	{
		sending_time_ = random(lowerBound,upperBound);
		}
   // ----------------------------------------------------------------------
   VisEnergyProcessor::
   ~VisEnergyProcessor()
   {}
   // ----------------------------------------------------------------------
   void
   VisEnergyProcessor::
   boot( void )
      throw()
   {
      last_time_of_receive_ = simulation_round();
	  //Creates an event to the event_scheduler
	  event_handle_ = owner_w().world_w().scheduler_w().new_event(*this,sending_time_ ,NULL);
	  this->owner_w().add_tag(new shawn::DoubleTag("VisBattery", 1.0));

      std::string labelvalue = "LabelTest";
      this->owner_w().add_tag(new shawn::StringTag("VisLabel", labelvalue));
   }
   // ----------------------------------------------------------------------
   bool
   VisEnergyProcessor::
   process_message( const ConstMessageHandle& mh )
      throw()
   {
	   const helloworld::HelloworldMessage* hmsg =
         dynamic_cast<const helloworld::HelloworldMessage*>
         ( mh.get() );
      if( hmsg != NULL )
         {
            last_time_of_receive_=simulation_round();
            neighbours_.insert( &hmsg->source() );
            cout << "ID '" << owner().label() << "' GOT HELLO FROM '"
                 << hmsg->source().label() << "'" << endl;
			   this->owner_w().remove_tag_by_name("VisBattery");
			   this->owner_w().add_tag(new shawn::DoubleTag("VisBattery", 0.0));
            return true;
         }
      this->owner_w().remove_tag_by_name("VisBattery");
	   this->owner_w().add_tag(new shawn::DoubleTag("VisBattery", 0.0));
      return Processor::process_message( mh );
   }
   // ----------------------------------------------------------------------
   void
   VisEnergyProcessor::
   work( void )
      throw()
   {
      if( simulation_round()-last_time_of_receive_ > 30 )
         {
            cout << "ID " << id()
                 << " DONE: "
                 << (unsigned int)neighbours_.size()
                 << " neighbours: ";

            for( std::set<const Node*>::const_iterator
                    it    = neighbours_.begin(),
                    first = it,
                    endit = neighbours_.end();
                 it != endit; ++it )
               {
                  if( it != first )
                     cout << ", ";
                  cout << "'"<<(**it).label()<<"'";
               }
            cout << endl;
            set_state( Inactive );
         }
   }

	void VisEnergyProcessor::timeout(shawn::EventScheduler & event_scheduler, shawn::EventScheduler::EventHandle event_handle, double time, shawn::EventScheduler::EventTagHandle & event_tag_handle) throw()
	{
		send( new helloworld::HelloworldMessage((int)random(1,10)) );
	}

	double
		VisEnergyProcessor::random(double lowerBound, double upperBound) throw(){
		return (lowerBound + shawn::uniform_random_0i_1i()*(upperBound - lowerBound));
    }

}
#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_processor.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: helloworld_processor.cpp,v $
 *-----------------------------------------------------------------------*/
