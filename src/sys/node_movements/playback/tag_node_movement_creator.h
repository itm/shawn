/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENTS_PLAYBACK_TAG_NODE_MOVEMENT_CREATOR
#define __SHAWN_SYS_NODE_MOVEMENTS_PLAYBACK_TAG_NODE_MOVEMENT_CREATOR

#include "sys/event_scheduler.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/node_movements/playback/node_movement_creator.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/node_movement.h"
#include "sys/taggings/group_tag.h"
#include "sys/node.h"
#include <fstream>
#include <string>
#include "sys/simulation/simulation_environment.h"


namespace shawn
{
	/// This class provides an implementation of the NodeMovementCreator
   /** This NodeMovementCreator reads input from a xml file containing movement of different nodes adhering from 
     * xml standard. The following syntax can be parsed:
	  * @verbatim
<tag type="group" name="movement">
  <tag type="double" name="time0" value="double value" > <-- These "time tags" need unique names within a node -->
    <tag type="group" name="jump">
      <tag type="double" name="x-pos" value="double value"/>
      <tag type="double" name="y-pos" value="double value"/>
      <tag type="double" name="z-pos" value="double value"/>
    </tag>
  </tag>
  ...
  <tag type="double" name="time" value="double value" >
    <tag type="group" name="linear">
      <tag type="double" name="x-pos" value="double value"/>
      <tag type="double" name="y-pos" value="double value"/>
      <tag type="double" name="z-pos" value="double value"/>
      <!-- You can choose from one of the following opportunities -->
      <!-- 1. -->
      <tag type="double" name="arrival time" value="double value" />
      <!-- 2. -->
      <tag type="double" name="velocity" value="double value" />
      <!-- 3. -->
      <tag type="double" name="duration" value="double value" />
    </tag>
  </tag>
</tag>
@endverbatim
     */
	class TagNodeMovementCreator:
		public NodeMovementCreator
	{
		public:
      ///@name construction / destruction
      ///@{
      ///
      TagNodeMovementCreator(SimulationController& sc);

		~TagNodeMovementCreator();
		      ///@}


      ///@name methods inherited from NodeMovementCreator
      ///@{
      /** This method scans all Nodes regarding the next Movement from the xml tags belonging 
		 *  to distinct nodes.
		 *
       */

		virtual shawn::MovementInfo *next_movement(void) throw( std::runtime_error );
	
		/** This method causes the NodeMovementCreator to start movements
       *  from the beginning.
       *
       */
		virtual void reset();

	private:
		/**
		* This method reveives the destination position from xml-tags
		* for node movement.
		*/
		virtual void getDestinationPosition(const Tag* movement_type_tag);
		SimulationController * sc_;
		MovementInfo * movement_info_;
		Vec * destination;
	};
}
#endif
