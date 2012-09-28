/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/messages/refinement/localization_iter_lateration_messages.h"


namespace localization
{

   LocalizationIterLaterationMessage::
   LocalizationIterLaterationMessage( double confidence )
      : confidence_( confidence )
   {}
   // ----------------------------------------------------------------------
   LocalizationIterLaterationMessage::
   LocalizationIterLaterationMessage( const LocalizationIterLaterationMessage& lilm )
      : confidence_( lilm.confidence() )
   {}
   // ----------------------------------------------------------------------
   LocalizationIterLaterationMessage::
   ~LocalizationIterLaterationMessage()
   {}
   // ----------------------------------------------------------------------
   double
   LocalizationIterLaterationMessage::
   confidence( void )
      const throw()
   {
      return confidence_;
   }

   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------

   LocalizationIterLaterationSoundMessage::
   LocalizationIterLaterationSoundMessage()
   {}
   // ----------------------------------------------------------------------
   LocalizationIterLaterationSoundMessage::
   LocalizationIterLaterationSoundMessage( const LocalizationIterLaterationSoundMessage&  )
   {}
   // ----------------------------------------------------------------------
   LocalizationIterLaterationSoundMessage::
   ~LocalizationIterLaterationSoundMessage()
   {}

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/refinement/localization_iter_lateration_messages.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_iter_lateration_messages.cpp,v $
 *-----------------------------------------------------------------------*/
