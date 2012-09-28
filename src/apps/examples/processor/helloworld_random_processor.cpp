/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/examples/processor/helloworld_random_processor.h"
#ifdef ENABLE_EXAMPLES

#include "apps/examples/processor/helloworld_random_processor.h"
#include "apps/examples/processor/helloworld_message.h"
#include "sys/node.h"
#include "sys/misc/random/basic_random.h"
#include <iostream>

using namespace std;
using namespace shawn;

namespace helloworld
{
   HelloworldRandomProcessor::
	HelloworldRandomProcessor()
      : last_time_of_receive_( 0 )
   {
	sending_time_ = random(0,5);
		}
	HelloworldRandomProcessor::
	HelloworldRandomProcessor(double lowerBound,double upperBound): last_time_of_receive_( 0 )
	{
		sending_time_ = random(lowerBound,upperBound);
		}
   // ----------------------------------------------------------------------
   HelloworldRandomProcessor::
   ~HelloworldRandomProcessor()
   {}
   // ----------------------------------------------------------------------
   void
   HelloworldRandomProcessor::
   boot( void )
      throw()
   {
      last_time_of_receive_ = simulation_round();
		//Creates an event to the event_scheduler
		event_handle_ = owner_w().world_w().scheduler_w().new_event(*this,sending_time_ ,NULL);
   }
   // ----------------------------------------------------------------------
   bool
   HelloworldRandomProcessor::
   process_message( const ConstMessageHandle& mh )
      throw()
   {
      const HelloworldMessage* hmsg =
         dynamic_cast<const HelloworldMessage*>
         ( mh.get() );
      if( hmsg != NULL )
         {
            last_time_of_receive_=simulation_round();
            neighbours_.insert( &hmsg->source() );
            cout << "ID '" << owner().label() << "' GOT HELLO FROM '"
                 << hmsg->source().label() << "'" << endl;
            return true;
         }
      else
      {
         cout << "ID '" << owner().label() << "' GOT MESSAGE FROM '" << mh.get()->source().label() << "'" << endl;
      }
      return Processor::process_message( mh );
   }
   // ----------------------------------------------------------------------
   void
   HelloworldRandomProcessor::
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
            //set_state( Inactive );
         }
   }

	void HelloworldRandomProcessor::timeout(shawn::EventScheduler & event_scheduler, shawn::EventScheduler::EventHandle event_handle, double time, shawn::EventScheduler::EventTagHandle & event_tag_handle) throw()
	{
		send( new HelloworldMessage((int)random(1,10)) );
	}

	double
		HelloworldRandomProcessor::random(double lowerBound, double upperBound) throw(){
		return (lowerBound + shawn::uniform_random_0i_1i()*(upperBound - lowerBound));
    }

}
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_processor.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: helloworld_processor.cpp,v $
 *-----------------------------------------------------------------------*/
