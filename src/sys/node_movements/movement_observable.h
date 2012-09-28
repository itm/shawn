/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_SYS_NODE_MOVEMENTS_MOVEMENT_OBSERVABLE
#define __SHAWN_SYS_NODE_MOVEMENTS_MOVEMENT_OBSERVABLE

#include "sys/misc/observable.h"
#include "sys/node_movements/movement_observer.h"

namespace shawn
{
    class Node;
    class Vec;

    /** A movement that needs to inform Observers that certains areas have been left has
      * to implement this interface (e.g. NodeMovement extends this class). Observers can
      * be added by using the Observable<MovementObserver> interface. 
      *
      * Two helper methods implement this notification mechanism: obtain_initial_boxes
      * and update_boxes. 
      *
      * By using boxes_begin and boxes_end, all currently active boxes can be iterated.
      */
    class MovementObservable 
        : public Observable<MovementObserver>
    {
    private:
        ///
        BoxContainer bo;
    protected:

		/// Begin iterator over the currenty active boxes
		virtual BoxContainer::BoxIterator movement_boxes_begin() throw() 
        {
            return bo.boxes_begin();
        }

        /// End iterator over the currenty active boxes
		virtual BoxContainer::BoxIterator movement_boxes_end() throw() 
        {
            return bo.boxes_end();
        }

        ///
        virtual bool movement_boxes_violated(const Vec& pos)
        {
            return bo.boxes_violated(pos);
        }

        ///
        virtual void movement_boxes_from(MovementObservable& mo) throw() 
        {
            bo.boxes_clear();

            for( BoxContainer::BoxIterator it = mo.movement_boxes_begin(), end = mo.movement_boxes_end(); it!=end; ++it)
                bo.boxes_add( *it );
        }

        /// Removes all active boxes and retrieves a list of initial boxes from all registered Observers.
        void movement_initial_boxes(Node& n, const Vec& newpos, const Vec& velo) throw()
        {
            bo.boxes_clear();
            for(ObserverIterator it = observers_begin(), endit = observers_end(); it!=endit; ++it)
                bo.boxes_add( (*it)->observer_initial_zone(n, newpos, velo) );
        }

        /// Removes all active boxes and updates a list of new boxes from all registered Observers.
        void movement_update_boxes(Node& n, const Vec& newpos, const Vec& velo) throw()
        {
            bo.boxes_clear();
            for(ObserverIterator it = observers_begin(), endit = observers_end(); it!=endit; ++it)
                bo.boxes_add( (*it)->observer_update_zone(n, newpos, velo) );
        }

    };

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/node_movements/movement_observable.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: movement_observable.h,v $
 *-----------------------------------------------------------------------*/
