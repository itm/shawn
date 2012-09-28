/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TAGGINGS_TAG_FACTORY_H
#define __SHAWN_SYS_TAGGINGS_TAG_FACTORY_H

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"

#include <string>
#include <stdexcept>

namespace shawn
{
   class Tag;
   class World;

   DECLARE_HANDLES( TagFactory );
   class TagFactory
      : public KeeperManaged
   {
   private:
	   World* world_;

   public:
	   TagFactory();
      virtual ~TagFactory();

	  virtual void set_world(World& w);
	  virtual bool has_world() const;
	  virtual World& world_w();
	  virtual const World& world() const;

      virtual Tag* create( const std::string& type,
                           const std::string& name,
                           const std::string& content
						  ) throw( std::runtime_error ) = 0;
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/taggings/tag_factory.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: tag_factory.h,v $
 *-----------------------------------------------------------------------*/
