/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MESSAGES_DISTANCE_DV_HOP_MESSAGES_H
#define __SHAWN_APPS_LOCALIZATION_MESSAGES_DISTANCE_DV_HOP_MESSAGES_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "sys/message.h"
#include "sys/node.h"


namespace localization
{

   class LocalizationDVhopMessage
      : public shawn::Message
   {

   public:

      LocalizationDVhopMessage( const shawn::Node&, int = 0 );
      LocalizationDVhopMessage( const LocalizationDVhopMessage& ldvhm );
      virtual ~LocalizationDVhopMessage();

      ///
      virtual const shawn::Node& anchor( void )
         const throw();
      ///
      virtual int hop_count( void )
         const throw();


   private:

      const shawn::Node& anchor_;
      int hop_cnt_;

   };

   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------


   class LocalizationDVcalMessage
      : public shawn::Message
   {

   public:

      LocalizationDVcalMessage( double );
      LocalizationDVcalMessage( const LocalizationDVcalMessage& );
      virtual ~LocalizationDVcalMessage();

      ///
      virtual double avg_hop_dist( void )
         const throw();


   private:
      double avg_hop_dist_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/distance/localization_dv_hop_messages.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_dv_hop_messages.h,v $
 *-----------------------------------------------------------------------*/
