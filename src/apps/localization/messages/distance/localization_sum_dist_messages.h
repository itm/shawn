/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MESSAGES_DISTANCE_SUM_DIST_MESSAGES_H
#define __SHAWN_APPS_LOCALIZATION_MESSAGES_DISTANCE_SUM_DIST_MESSAGES_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "sys/message.h"
#include "sys/node.h"


namespace localization
{

   class LocalizationSumDistMessage
      : public shawn::Message
   {

   public:

      LocalizationSumDistMessage( const shawn::Node&, double = 0 );
      LocalizationSumDistMessage( const LocalizationSumDistMessage& );
      virtual ~LocalizationSumDistMessage();

      ///
      virtual const shawn::Node& anchor( void )
         const throw();
      ///
      virtual double path_length( void )
         const throw();


   private:

      const shawn::Node& anchor_;
      double path_length_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/distance/localization_sum_dist_messages.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_sum_dist_messages.h,v $
 *-----------------------------------------------------------------------*/
