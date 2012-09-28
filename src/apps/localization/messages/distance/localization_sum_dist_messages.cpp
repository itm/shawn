/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/messages/distance/localization_sum_dist_messages.h"


namespace localization
{

   LocalizationSumDistMessage::
   LocalizationSumDistMessage( const shawn::Node& anchor, double path_length )
      : anchor_( anchor ),
         path_length_( path_length )
   {}
   // ----------------------------------------------------------------------
   LocalizationSumDistMessage::
   LocalizationSumDistMessage( const LocalizationSumDistMessage& lsdm )
      : anchor_( lsdm.anchor() ),
         path_length_( lsdm.path_length() )
   {}
   // ----------------------------------------------------------------------
   LocalizationSumDistMessage::
   ~LocalizationSumDistMessage()
   {}
   // ----------------------------------------------------------------------
   const shawn::Node&
   LocalizationSumDistMessage::
   anchor( void )
      const throw()
   {
      return anchor_;
   }
   // ----------------------------------------------------------------------
   double
   LocalizationSumDistMessage::
   path_length( void )
      const throw()
   {
      return path_length_;
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/distance/localization_sum_dist_messages.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_sum_dist_messages.cpp,v $
 *-----------------------------------------------------------------------*/
