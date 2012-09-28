/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MESSAGES_REFINEMENT_ITER_LATERATION_MESSAGES_H
#define __SHAWN_APPS_LOCALIZATION_MESSAGES_REFINEMENT_ITER_LATERATION_MESSAGES_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "sys/message.h"


namespace localization
{

   class LocalizationIterLaterationMessage
      : public shawn::Message
   {

   public:

      LocalizationIterLaterationMessage( double = 0 );
      LocalizationIterLaterationMessage( const LocalizationIterLaterationMessage& );
      virtual ~LocalizationIterLaterationMessage();

      ///
      virtual double confidence( void )
         const throw();


   private:

      double confidence_;

   };

   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------

   class LocalizationIterLaterationSoundMessage
      : public shawn::Message
   {

   public:

      LocalizationIterLaterationSoundMessage();
      LocalizationIterLaterationSoundMessage( const LocalizationIterLaterationSoundMessage& );
      virtual ~LocalizationIterLaterationSoundMessage();

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/refinement/localization_iter_lateration_messages.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_iter_lateration_messages.h,v $
 *-----------------------------------------------------------------------*/
