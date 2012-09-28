/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENTS_JUMP_MOVEMENT
#define __SHAWN_SYS_NODE_MOVEMENTS_JUMP_MOVEMENT

#include "sys/node_movement.h"
#include "sys/world.h"
#include <map>

namespace shawn
{

    /// JumpMovement
    /**
    * In short, the JumpMovement changes a single Node Coordinate, i.e. x,y or z. If the Node
    * was moving before, it stops. The velocity of a JumpMovement is always zero.
    *
    * To ensure compatibility with ns-2 movement files a method set_dimension() is provided,
    * allowing the setting of only a single dimension. The remaining dimensions are taking from
    * the previous Node position.
    * 
    * set_position will always use all dimensions of the provided Vec
    */

    class JumpMovement
        : public NodeMovement
    {
    public:

        enum Dimension 
        {
            X, Y, Z
        };

        ///@name construction / destruction
        ///@{
        ///
        JumpMovement();
        ///
        virtual ~JumpMovement();
        ///
        virtual void init( Node& ) throw();
        ///
        virtual void init_from( Node&, NodeMovement& ) throw();
        ///@}

        ///@name inherited from NodeMovement
        ///@{
        ///
        /** Sets the new position to be identical to the given position Vector.
        */
        virtual bool set_position( const Vec& ) throw();
        ///
        /** Returns the current position of the corresponding Node
        */
        virtual Vec position( void ) const throw();
        ///
        /** Always returns a zero velocity (0,0,0).
        */
        virtual Vec velocity( void ) const throw();
        ///
        /** Returns the type of Movement, in this case JumpMovement.
        */      
        virtual std::string name( void ) const throw();
        ///@}

        /** This method sets one Dimension (X,Y or Z) that is to be replaced
        */
        virtual void set_dimension(Dimension, double);
		 
		  /** This method alters whole position vector (X,Y or Z) 
        */

		  virtual void set_dimensions(const Vec* newpos);

	protected:
		virtual void boxes_changed() throw();

    private:    
        Vec pos_;

        std::map<int, bool> update_mask_;
        std::map<int, double> update_pos_;
        
    };
}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/node_movements/jump_movement.h,v $
* Version $Revision: 600 $
* Date    $Date: 2011-05-26 21:53:32 +0200 (Thu, 26 May 2011) $
*-----------------------------------------------------------------------
* $Log: jump_movement.h,v $
 *-----------------------------------------------------------------------*/
