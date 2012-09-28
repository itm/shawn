/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MESSAGES_REFINEMENT_ITER_DLS_MESSAGES_H
#define __SHAWN_APPS_LOCALIZATION_MESSAGES_REFINEMENT_ITER_DLS_MESSAGES_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "sys/message.h"


namespace localization
{

   class LocalizationIterDLSMessage
      : public shawn::Message
   {

   public:

      LocalizationIterDLSMessage( double = 0 );
      LocalizationIterDLSMessage( const LocalizationIterDLSMessage& );
      virtual ~LocalizationIterDLSMessage();

      ///
      virtual double confidence( void )
         const throw();


   private:

      double confidence_;

   };

   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------

   class LocalizationIterDLSSoundMessage
      : public shawn::Message
   {

   public:

      LocalizationIterDLSSoundMessage();
      LocalizationIterDLSSoundMessage( const LocalizationIterDLSSoundMessage& );
      virtual ~LocalizationIterDLSSoundMessage();

   };

}// namespace localization
#endif
#endif
