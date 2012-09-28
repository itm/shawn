/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MESSAGES_DISTANCE_GPSFREE_NCS_EXTENDED_MESSAGES_H
#define __SHAWN_APPS_LOCALIZATION_MESSAGES_DISTANCE_GPSFREE_NCS_EXTENDED_MESSAGES_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/messages/position/localization_gpsfree_ncs_messages.h"


namespace localization
{

   class LocalizationGPSfreeNCSLCSExtendedMessage
      : public LocalizationGPSfreeNCSLCSMessage
   {

   public:

      LocalizationGPSfreeNCSLCSExtendedMessage( const LocalizationLocalCoordinateSystem&, int );
      LocalizationGPSfreeNCSLCSExtendedMessage( const LocalizationGPSfreeNCSLCSExtendedMessage& );
      virtual ~LocalizationGPSfreeNCSLCSExtendedMessage();

      ///
      int hop_cnt( void ) const throw();
      ///
      void set_hop_cnt( int ) throw();


   private:

      int hop_cnt_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/position/localization_gpsfree_ncs_extended_messages.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_gpsfree_ncs_extended_messages.h,v $
 *-----------------------------------------------------------------------*/
