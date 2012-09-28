/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TAG_SYNCABLE_H
#define __SHAWN_SYS_TAG_SYNCABLE_H

#include "shawn_config.h"
#include "sys/tag_container.h"
#include <stdexcept>

namespace shawn
{

   
   /** Dont complain about "Syncable". After all,
    *  Kraft claims "Lunchable" is a meaningful word. */
   class TagSyncable
   {
   public:
      ///@name Ctor, Dtor
      ///@{
      ///
      TagSyncable();
      ///
      virtual ~TagSyncable();
      ///@}


      ///@name TagSyncable abstract interface
      ///@{
      ///
      virtual void sync_to_tags( TagContainer& ) throw() = 0;
      ///
      virtual void sync_from_tags( const TagContainer& ) throw( std::runtime_error ) = 0;
      ///@}

   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/tag_syncable.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: tag_syncable.h,v $
 *-----------------------------------------------------------------------*/
