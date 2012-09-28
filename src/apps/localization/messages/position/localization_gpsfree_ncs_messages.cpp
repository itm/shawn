/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/messages/position/localization_gpsfree_ncs_messages.h"


namespace localization
{

   LocalizationGPSfreeNCSLCSMessage::
   LocalizationGPSfreeNCSLCSMessage( const LocalizationLocalCoordinateSystem& local_coord_sys )
      : local_coord_sys_( &local_coord_sys )
   {}
   // ----------------------------------------------------------------------
   LocalizationGPSfreeNCSLCSMessage::
   LocalizationGPSfreeNCSLCSMessage( const LocalizationGPSfreeNCSLCSMessage& lcs_message )
      : local_coord_sys_( &lcs_message.local_coord_sys() )
   {}
   // ----------------------------------------------------------------------
   LocalizationGPSfreeNCSLCSMessage::
   ~LocalizationGPSfreeNCSLCSMessage()
   {}
   // ----------------------------------------------------------------------
   const LocalizationLocalCoordinateSystem&
   LocalizationGPSfreeNCSLCSMessage::
   local_coord_sys( void )
      const throw()
   {
      return *local_coord_sys_;
   }   

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/position/localization_gpsfree_ncs_messages.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_gpsfree_ncs_messages.cpp,v $
 *-----------------------------------------------------------------------*/
