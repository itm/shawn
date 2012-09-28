/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENTS_MOVEMENT_OBSERVER
#define __SHAWN_SYS_NODE_MOVEMENTS_MOVEMENT_OBSERVER

#include "sys/misc/box.h"

namespace shawn
{
    class Node;
    class Vec;

    /** Used to trace movements of nodes, this abstract callback interface is used for 
	  * notification and information retrieval.
	  * 
      * A class implementing this interface has to provide the methods below.
	  *
      * An initial box is the first box that a MovementObservable object must
      * only leave if observer_update_zone() is called immediately after that. 
      * This will serve two purposes: Notification, that the box has been left and
      * retrieval of a new box.
	  *
	  * Currently, NodeMovement and Node inherit this interface. If you want to trace
	  * a nodes movement, simply add a MovementOverserver reference using the nodes'
	  * Node::observer_add method. 
	  * You will then need to supply an initial box and later an updated box which a node 
	  * must not leave whithout notification via a callback to your observer_update_zone()
	  * method.
      */
    class MovementObserver
    {
    public:
       /** destructor */
       virtual ~MovementObserver();

        /** Returns an initial bounding box. If a node leaves this box, observer_update_zone() 
          * must be called - by convention - by the observed movement to retrieve a new box.
          */
        virtual Box observer_initial_zone (Node&, const Vec& newpos, const Vec& velo) throw () = 0;
        
        /** Informs this Observer that the old box has been left by the NodeMovement / Node. 
		  * A new box must be provided by this method.
          */
        virtual Box observer_update_zone (Node&, const Vec& newpos, const Vec& velo) throw () = 0;
    };


}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/node_movements/movement_observer.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: movement_observer.h,v $
 *-----------------------------------------------------------------------*/
