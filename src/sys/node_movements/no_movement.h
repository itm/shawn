/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENTS_NO_MOVEMENT
#define __SHAWN_SYS_NODE_MOVEMENTS_NO_MOVEMENT

#include "sys/node_movement.h"

namespace shawn
{

   /// NodeMovement for completely immobile nodes
   /**
    *  The NodeMovement does not allow a node to move.
    *  Calls to set_position() are rejected once the position
    *  is initialized (via set_initial_zone()).
    */
   class NoMovement
      : public NodeMovement
   {
   public:
      ///@name construction / destruction
      ///@{
      ///
      NoMovement();
      ///
      virtual ~NoMovement();
      ///
      virtual void init( Node& ) throw();
      ///
      virtual void init_from( Node&, NodeMovement& ) throw();
      ///@}

      ///@name NodeMovement interface
      ///@{
      /// only accepted *BEFORE* initialized...
      virtual bool set_position( const Vec& ) throw();
      ///
      virtual Vec position( void ) const throw();
      ///
      virtual Vec velocity( void ) const throw();
      ///
      virtual std::string name( void ) const throw();
      ///@}
   protected:
	   ///
	   virtual void boxes_changed() throw();

   private:
      ///
      Vec pos_;
      ///
      bool initialized_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/no_movement.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: no_movement.h,v $
 *-----------------------------------------------------------------------*/
