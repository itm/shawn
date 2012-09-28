/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/messages/refinement/localization_iter_dls_messages.h"


namespace localization
{

   LocalizationIterDLSMessage::
   LocalizationIterDLSMessage( double confidence )
      : confidence_( confidence )
   {}
   // ----------------------------------------------------------------------
   LocalizationIterDLSMessage::
   LocalizationIterDLSMessage( const LocalizationIterDLSMessage& lilm )
      : confidence_( lilm.confidence() )
   {}
   // ----------------------------------------------------------------------
   LocalizationIterDLSMessage::
   ~LocalizationIterDLSMessage()
   {}
   // ----------------------------------------------------------------------
   double
   LocalizationIterDLSMessage::
   confidence( void )
      const throw()
   {
      return confidence_;
   }

   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------

   LocalizationIterDLSSoundMessage::
   LocalizationIterDLSSoundMessage()
   {}
   // ----------------------------------------------------------------------
   LocalizationIterDLSSoundMessage::
   LocalizationIterDLSSoundMessage( const LocalizationIterDLSSoundMessage&  )
   {}
   // ----------------------------------------------------------------------
   LocalizationIterDLSSoundMessage::
   ~LocalizationIterDLSSoundMessage()
   {}

}// namespace localization
#endif
