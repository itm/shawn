/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MESSAGES_DISTANCE_GPSFREE_LCS_MESSAGES_H
#define __SHAWN_APPS_LOCALIZATION_MESSAGES_DISTANCE_GPSFREE_LCS_MESSAGES_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "sys/node.h"
#include "sys/message.h"
#include "apps/localization/util/localization_defutils.h"


namespace localization
{

   class LocalizationGPSfreeLCSInitMessage
      : public shawn::Message
   {

   public:
      LocalizationGPSfreeLCSInitMessage();
      LocalizationGPSfreeLCSInitMessage( const LocalizationGPSfreeLCSInitMessage& );
      virtual ~LocalizationGPSfreeLCSInitMessage();

   };

   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------

   class LocalizationGPSfreeLCSNeighborMessage
      : public shawn::Message
   {

   public:

      LocalizationGPSfreeLCSNeighborMessage( const localization::DistanceMap& );
      LocalizationGPSfreeLCSNeighborMessage( const LocalizationGPSfreeLCSNeighborMessage& );
      virtual ~LocalizationGPSfreeLCSNeighborMessage();

      ///
      const localization::DistanceMap& neighbors( void )
         const throw();


   private:

      localization::DistanceMap neighbors_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/distance/localization_gpsfree_lcs_messages.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_gpsfree_lcs_messages.h,v $
 *-----------------------------------------------------------------------*/
