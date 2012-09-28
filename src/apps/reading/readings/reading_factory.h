/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __READING_FACTORY_H
#define __READING_FACTORY_H
#include "../buildfiles/_apps_enable_cmake.h"

#ifdef ENABLE_READING
#include <string>
#include <map>
#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/reading/readings/reading.h"

namespace reading
{
   class ReadingFactory
         : public shawn::KeeperManaged
   {
   public:
      typedef std::map<std::string,Reading*> ReadingMap;
      ReadingFactory( void );
      virtual ~ReadingFactory();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual Reading* create( std::string name ) = 0;
      virtual void set_world(shawn::World *world);

   protected:
      Reading* find_reading( std::string name );
      ReadingMap reading_map_;
      shawn::World *world_;
   };
   DECLARE_HANDLES(ReadingFactory);
}
#endif
#endif
