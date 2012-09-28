/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_WORLDS_CUBOID_WORLD_FACTORY
#define __SHAWN_SYS_WORLDS_CUBOID_WORLD_FACTORY

#include "sys/worlds/processor_world_factory.h"


namespace shawn
{

   class CuboidWorldFactory
      : public ProcessorWorldFactory
   {
   public:
      CuboidWorldFactory();
      virtual ~CuboidWorldFactory();

      virtual void set_size( double, double, double ) throw();
      virtual double width( void ) const throw();
      virtual double height( void ) const throw();
      virtual double length( void ) const throw();

   protected:
      virtual Vec new_position( void ) const throw();

   private:
      double length_;
      double width_;
      double height_;
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/worlds/cuboid_world_factory.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: cuboid_world_factory.h,v $
 *-----------------------------------------------------------------------*/
