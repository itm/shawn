/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/node_movements/simple_movement.h"

namespace shawn
{

    SimpleMovement::
        SimpleMovement()
    {}
    // ----------------------------------------------------------------------
    SimpleMovement::
        ~SimpleMovement()
    {}
    // ----------------------------------------------------------------------
    void
        SimpleMovement::
        init( Node& v )
        throw()
    {
        set_node(v);
        movement_initial_boxes();
    }
    // ----------------------------------------------------------------------
    void
        SimpleMovement::
        init_from( Node& v,
        NodeMovement& nm )
        throw()
    {
        set_node(v);
        movement_update_boxes();
    }
    // ----------------------------------------------------------------------
    bool
        SimpleMovement::
        set_position( const Vec& p )
        throw()
    {
        pos_ = p;

		if( movement_boxes_violated(p) )
			movement_update_boxes();

        return true;
    }
    // ----------------------------------------------------------------------
    Vec
        SimpleMovement::
        position( void )
        const throw()
    {
        return pos_;
    }
    // ----------------------------------------------------------------------
    Vec
        SimpleMovement::
        velocity( void )
        const throw()
    {
        return Vec(0,0,0);
    }
    // ----------------------------------------------------------------------
    std::string
        SimpleMovement::
        name( void )
        const throw()
    {
        return std::string("SimpleMovement");
    }


}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/node_movements/simple_movement.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: simple_movement.cpp,v $
 *-----------------------------------------------------------------------*/
