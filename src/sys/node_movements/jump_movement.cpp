/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/node_movements/jump_movement.h"


namespace shawn
{
    JumpMovement::
        JumpMovement()
    {
      update_mask_[JumpMovement::X] = false;
      update_mask_[JumpMovement::Y] = false;
      update_mask_[JumpMovement::Z] = false;
    }
    // ----------------------------------------------------------------------
    JumpMovement::
        ~JumpMovement()
    {}
    // ----------------------------------------------------------------------
    void
        JumpMovement::
        init( Node& v )
        throw()
    {
        set_node(v);
        movement_initial_boxes();
    }
    // ----------------------------------------------------------------------
    void
        JumpMovement::
        init_from( Node& v,
        NodeMovement& nm )
        throw()
    {
        set_node(v);
        set_position(nm.position());

        if ( update_mask_[JumpMovement::X] )
            pos_ = Vec( update_pos_[JumpMovement::X], pos_.y(), pos_.z() );
        if ( update_mask_[JumpMovement::Y] )
            pos_ = Vec( pos_.x(), update_pos_[JumpMovement::Y], pos_.z() );
        if ( update_mask_[JumpMovement::Z] )
            pos_ = Vec( pos_.x(), pos_.y(), update_pos_[JumpMovement::Z] );
        
        movement_initial_boxes();
    }
    // ----------------------------------------------------------------------
    bool
        JumpMovement::
        set_position( const Vec& p )
        throw()
    {
        pos_ = p;
		  boxes_changed();
        return true;
    }
    // ----------------------------------------------------------------------
    Vec
        JumpMovement::
        position( void )
        const throw()
    {
        return pos_;
    }
    // ----------------------------------------------------------------------
    Vec
        JumpMovement::
        velocity( void )
        const throw()
    {
        return Vec(0,0,0);
    }
    // ----------------------------------------------------------------------
    std::string
        JumpMovement::
        name( void )
        const throw()
    {
        return std::string("JumpMovement");
    }
    // ----------------------------------------------------------------------
    void 
		JumpMovement::
		set_dimension(Dimension dimension, double coordinate) 
	{
        Vec newpos = pos_;

        switch (dimension)
        {
        case (JumpMovement::X):
            newpos = Vec(coordinate, pos_.y(), pos_.z());
            update_mask_[JumpMovement::X] = true;
            update_pos_[JumpMovement::X] = coordinate;
            break;
        case (JumpMovement::Y):
            newpos = Vec(pos_.x(), coordinate, pos_.z());
            update_mask_[JumpMovement::Y] = true;
            update_pos_[JumpMovement::Y] = coordinate;
            break;
        case (JumpMovement::Z):
            newpos = Vec(pos_.x(), pos_.y(), coordinate);
            update_mask_[JumpMovement::Z] = true;
            update_pos_[JumpMovement::Z] = coordinate;
            break;
        }

        set_position(newpos);
    }

    // ----------------------------------------------------------------------
	
	 void 
		JumpMovement::
		set_dimensions(const Vec* newpos)
	{
		set_dimension(JumpMovement::X,newpos->x());
		set_dimension(JumpMovement::Y,newpos->y());
		set_dimension(JumpMovement::Z,newpos->z());
	    
    }

 // ----------------------------------------------------------------------
	
	 void 
		JumpMovement::
		boxes_changed() 
		throw()
	{
		if(movement_boxes_violated(pos_))
			movement_update_boxes();
	}

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/node_movements/jump_movement.cpp,v $
* Version $Revision: 600 $
* Date    $Date: 2011-05-26 21:53:32 +0200 (Thu, 26 May 2011) $
*-----------------------------------------------------------------------
* $Log: jump_movement.cpp,v $
 *-----------------------------------------------------------------------*/
