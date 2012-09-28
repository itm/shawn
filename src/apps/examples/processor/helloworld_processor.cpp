/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/examples/processor/helloworld_processor.h"
#ifdef ENABLE_EXAMPLES

#include "apps/examples/processor/helloworld_processor.h"
#include "apps/examples/processor/helloworld_message.h"
#include "sys/node_distance_estimate.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/node.h"
#include <iostream>


using namespace std;
using namespace shawn;

namespace helloworld
{
   HelloworldProcessor::
   HelloworldProcessor()
      : last_time_of_receive_( 0 )
   {}
   // ----------------------------------------------------------------------
   HelloworldProcessor::
   ~HelloworldProcessor()
   {}
   // ----------------------------------------------------------------------
   void
   HelloworldProcessor::
   boot( void )
      throw()
   {
      last_time_of_receive_ = simulation_round();
      send( new HelloworldMessage );
   }
   // ----------------------------------------------------------------------
   bool
   HelloworldProcessor::
   process_message( const ConstMessageHandle& mh ) 
      throw()
   {
      const HelloworldMessage* hmsg = dynamic_cast<const HelloworldMessage*> ( mh.get() );

      if( hmsg != NULL )
		{
			last_time_of_receive_=simulation_round();
			neighbours_.insert( &hmsg->source() );
			if( owner() != hmsg->source() )
				cout << "ID '" << owner().label() << "' GOT HELLO FROM '" << hmsg->source().label() << "'" << endl;
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
   HelloworldProcessor::
   work( void )
      throw()
   {
      if( simulation_round()-last_time_of_receive_ > 4 )
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






}
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_processor.cpp,v $
 * Version $Revision: 370 $
 * Date    $Date: 2009-11-12 14:12:56 +0100 (Thu, 12 Nov 2009) $
 *-----------------------------------------------------------------------
 * $Log: helloworld_processor.cpp,v $
 *-----------------------------------------------------------------------*/
