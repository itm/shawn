/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/messages/position/localization_gpsfree_ncs_extended_messages.h"


namespace localization
{

   LocalizationGPSfreeNCSLCSExtendedMessage::
   LocalizationGPSfreeNCSLCSExtendedMessage( const LocalizationLocalCoordinateSystem& local_coord_sys, int hop_cnt )
      : LocalizationGPSfreeNCSLCSMessage( local_coord_sys ),
         hop_cnt_( hop_cnt )
   {}
   // ----------------------------------------------------------------------
   LocalizationGPSfreeNCSLCSExtendedMessage::
   LocalizationGPSfreeNCSLCSExtendedMessage( const LocalizationGPSfreeNCSLCSExtendedMessage& lcs_message )
      : LocalizationGPSfreeNCSLCSMessage( lcs_message ),
         hop_cnt_( lcs_message.hop_cnt() )
   {}
   // ----------------------------------------------------------------------
   LocalizationGPSfreeNCSLCSExtendedMessage::
   ~LocalizationGPSfreeNCSLCSExtendedMessage()
   {}
   // ----------------------------------------------------------------------
   int
   LocalizationGPSfreeNCSLCSExtendedMessage::
   hop_cnt( void )
      const throw()
   {
      return hop_cnt_;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationGPSfreeNCSLCSExtendedMessage::
   set_hop_cnt( int hop_cnt )
      throw()
   {
      hop_cnt_ = hop_cnt;
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/position/localization_gpsfree_ncs_extended_messages.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_gpsfree_ncs_extended_messages.cpp,v $
 *-----------------------------------------------------------------------*/
