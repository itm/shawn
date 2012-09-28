/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENT_H
#define __SHAWN_SYS_NODE_MOVEMENT_H

#include "sys/util/refcnt_pointable.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"
#include "sys/misc/box.h"
#include "sys/node_movements/movement_observable.h"

namespace shawn
{
    class World;
    class Node;
    class Vec;
    class Box;


    DECLARE_HANDLES(NodeMovement);

    /// Abstract interface for node mobility implementations
    /** A NodeMovement that supports only a single Node at once
    *  should probably derive from SingleNodeMovement , which
    *  provides a few utility methods for easier implementation.
    */
    class NodeMovement
        : public RefcntPointable,
	      public MovementObservable
    {
    public:
        ///@name construction / destruction
        ///@{
        ///
        NodeMovement();
        ///
        virtual ~NodeMovement();
        /** Called from Node when added there!
        *
        *  Should call set_node() and obtain_initial_boxes() */
        virtual void init( Node& ) throw() = 0;
        /* Called from Node when added there!
        *
        *  Should call set_node() and update_boxes() */
        virtual void init_from( Node&, NodeMovement& ) throw() = 0;
        ///@}


        ///@name node positioning
        ///@{
        /** sets the current position of the given node.
        *  There is no need for a NodeMovement to actually
        *  place the Node there, as it may be the single
        *  authority about node positions.
        *
        *  \return whether the NodeMovement accepted the call, ie, 
        *  a subsequent call to position() will return the set position. */
        virtual bool set_position( const Vec& ) throw() = 0;
        /** returns the current position of the node.
        */
        virtual Vec position( void ) const throw() = 0;
        /** returns the current velocity vector of the node, ie, the
        *  derivate of the position function.
        *
        *  Therefore,
        *  \code
        *  velocity().euclidean_norm()
        *  \endcode
        *  gives the current speed, and
        *  \code
        *  velocity() / velocity().euclidean_norm()
        *  \endcode
        *  gives the direction, if velocity() is not (0,0,0).
        */
        virtual Vec velocity( void ) const throw() = 0;
        ///@}

        ///@name misc access
        ///@{
        /** \return an identifying string */
        virtual std::string name( void ) const throw() = 0;
        /** \return the current simulation round */
        virtual int simulation_round( void ) const throw();
        /** \return the current clock */
        virtual double current_time( void ) const throw();
        /** \return the World */
        virtual const World& world( void ) const throw();
        /** \return the World, writable */
        virtual World& world_w( void ) throw();
        ///
        virtual Node& node_w( void ) throw();
        ///
        virtual const Node& node( void ) throw();
        ///@}

    protected:
        ///
        virtual void set_node( Node& ) throw();

		///
		virtual void movement_initial_boxes() throw();

		///
		virtual void movement_update_boxes() throw();

		///
		virtual void observers_added(MovementObserver& obs) throw();

		///
		virtual void boxes_changed() throw() = 0;

        ///
        World* world_;
        ///
        Node*  node_;
    };

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/node_movement.h,v $
* Version $Revision: 233 $
* Date    $Date: 2008-08-18 13:54:18 +0200 (Mon, 18 Aug 2008) $
*-----------------------------------------------------------------------
* $Log: node_movement.h,v $
 *-----------------------------------------------------------------------*/
