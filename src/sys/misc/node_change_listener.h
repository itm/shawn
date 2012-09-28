/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_SYS_MISC_NODE_CHANGE_LISTENER_H__
#define __SHAWN_SYS_MISC_NODE_CHANGE_LISTENER_H__

namespace shawn
{

   class Node;

    /** Listener to receive events when nodes are added to the world
      *
      */
   class NodeChangeListener
   {
   public:
      virtual ~NodeChangeListener();
      /// Is called whenever a node gets added to the world
      virtual void node_added( Node& ) throw() = 0;
      /// Is called whenever a node gets removed from the world
      virtual void node_removed( Node& ) throw() = 0;
      /** Is called whenever the id of a node has changed. This occurs, for
         *  example, if the method reorganize_nodes() of the world is called,
         *  because the internal NodeVector is optimized (compressed) there
         *  and ids may change.
         *
         *  \param old_id the id of the node before the id change
         *  \param new_id the id of the same node after the id change
         */
      virtual void id_changed( int old_id, int new_id) throw() = 0;
      /// Is called when the world gets deleted.
      /** \return true, if the world should delete the listener */
      virtual bool invalidate( void ) throw() = 0;
   };
    
}
    
#endif
    
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/node_change_listener.h,v $
 * Version $Revision: 504 $
 * Date    $Date: 2011-01-24 20:00:04 +0100 (Mon, 24 Jan 2011) $
 *-----------------------------------------------------------------------
 * $Log: node_change_listener.h,v $
 *-----------------------------------------------------------------------*/

    
