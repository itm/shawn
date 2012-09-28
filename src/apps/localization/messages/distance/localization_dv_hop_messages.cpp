/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/messages/distance/localization_dv_hop_messages.h"

namespace localization
{

   LocalizationDVhopMessage::
   LocalizationDVhopMessage( const shawn::Node& anchor, int hop_cnt )
      : anchor_( anchor ),
         hop_cnt_( hop_cnt )
   {}
   // ----------------------------------------------------------------------
   LocalizationDVhopMessage::
   LocalizationDVhopMessage( const LocalizationDVhopMessage& ldvhm )
      : anchor_( ldvhm.anchor() ),
         hop_cnt_( ldvhm.hop_count() )
   {}
   // ----------------------------------------------------------------------
   LocalizationDVhopMessage::
   ~LocalizationDVhopMessage()
   {}
   // ----------------------------------------------------------------------
   const shawn::Node&
   LocalizationDVhopMessage::
   anchor( void )
      const throw()
   {
      return anchor_;
   }
   // ----------------------------------------------------------------------
   int
   LocalizationDVhopMessage::
   hop_count( void )
      const throw()
   {
      return hop_cnt_;
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   LocalizationDVcalMessage::
   LocalizationDVcalMessage( double avg_hop_dist )
      : avg_hop_dist_( avg_hop_dist )
   {}
   // ----------------------------------------------------------------------
   LocalizationDVcalMessage::
   LocalizationDVcalMessage( const LocalizationDVcalMessage& ldvcm )
      : avg_hop_dist_( ldvcm.avg_hop_dist() )
   {}
   // ----------------------------------------------------------------------
   LocalizationDVcalMessage::
   ~LocalizationDVcalMessage()
   {}
   // ----------------------------------------------------------------------
   double
   LocalizationDVcalMessage::
   avg_hop_dist( void )
      const throw()
   {
      return avg_hop_dist_;
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/distance/localization_dv_hop_messages.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_dv_hop_messages.cpp,v $
 *-----------------------------------------------------------------------*/
