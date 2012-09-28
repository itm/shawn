/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENTS_PLAYBACK_MOVEMENT_INFO
#define __SHAWN_SYS_NODE_MOVEMENTS_PLAYBACK_MOVEMENT_INFO

#include "sys/util/refcnt_pointable.h"
#include "sys/event_scheduler.h"
#include "sys/vec.h"
#include "sys/node.h"
#include <fstream>
#include <string>
#include <sstream>

namespace shawn
{

   //DECLARE_HANDLES(MovementInfo);


   /// Encapsulation for the movement order parameters created by the NodeMovementCreator
   /** This data package contains all data necessary for creating a movement order
    *  configuration file and take some initial settings.
    */
   class MovementInfo
   // :public RefcntPointable
   {

   public:

      enum Urgency
      {
         Immediately,
         Delayed
      };

       ///@ name construction/destruction
       ///@ {
      MovementInfo();
      ///
      virtual ~MovementInfo();
      ///@ }

      virtual void set_urgency(Urgency);

      virtual void set_dispatch_time(double);

      virtual void set_node(Node*);

      //virtual void set_node_movement(NodeMovement*);
	  virtual void set_node_movement(const NodeMovementHandle&);

      virtual MovementInfo::Urgency urgency();

      virtual double dispatch_time();

      virtual Node* node();

      virtual NodeMovement* node_movement();

   private:

      Urgency urgency_;
      Node * node_;
      //NodeMovement * node_movement_;
	  NodeMovementHandle node_movement_;
      double dispatch_time_;
      Vec vector_;
      double velocity_;

   };
}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/playback/movement_info.h,v $
 * Version $Revision: 248 $
 * Date    $Date: 2008-09-23 16:28:10 +0200 (Tue, 23 Sep 2008) $
 *-----------------------------------------------------------------------
 * $Log: movement_info.h,v $
 *-----------------------------------------------------------------------*/
