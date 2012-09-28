/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENTS_SIMPLE_MOVEMENT
#define __SHAWN_SYS_NODE_MOVEMENTS_SIMPLE_MOVEMENT

#include "sys/node_movement.h"

namespace shawn
{

   class SimpleMovement
      : public NodeMovement
   {
   public:
      ///@name construction / destruction
      ///@{
      ///
      SimpleMovement();
      ///
      virtual ~SimpleMovement();
      ///
      virtual void init( Node& ) throw();
      ///
      virtual void init_from( Node&, NodeMovement& ) throw();
      ///@}

      ///@name NodeMovement interface
      ///@{
      ///
      virtual bool set_position( const Vec& ) throw();
      ///
      virtual Vec position( void ) const throw();
      ///
      virtual Vec velocity( void ) const throw();
      ///
      virtual std::string name( void ) const throw();
      ///@}

   private:
      Vec pos_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/simple_movement.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: simple_movement.h,v $
 *-----------------------------------------------------------------------*/
