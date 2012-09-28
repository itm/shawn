/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/messages/distance/localization_euclidean_messages.h"


namespace localization
{

   LocalizationEuclideanInitMessage::
   LocalizationEuclideanInitMessage( const ProcessorType& proc_type )
      : proc_type_( proc_type )
   {}
   // ----------------------------------------------------------------------
   LocalizationEuclideanInitMessage::
   LocalizationEuclideanInitMessage( const LocalizationEuclideanInitMessage& leim )
      : proc_type_( leim.proc_type() )
   {}
   // ----------------------------------------------------------------------
   LocalizationEuclideanInitMessage::
   ~LocalizationEuclideanInitMessage()
   {}
   // ----------------------------------------------------------------------
   LocalizationEuclideanInitMessage::ProcessorType
   LocalizationEuclideanInitMessage::
   proc_type( void )
      const throw()
   {
      return proc_type_;
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   LocalizationEuclideanAnchorMessage::
   LocalizationEuclideanAnchorMessage( const shawn::Node& anchor, double distance )
      : anchor_( anchor ),
         distance_( distance )
   {}
   // ----------------------------------------------------------------------
   LocalizationEuclideanAnchorMessage::
   LocalizationEuclideanAnchorMessage( const LocalizationEuclideanAnchorMessage& leam )
      : anchor_( leam.anchor() ),
         distance_( leam.distance() )
   {}
   // ----------------------------------------------------------------------
   LocalizationEuclideanAnchorMessage::
   ~LocalizationEuclideanAnchorMessage()
   {}
   // ----------------------------------------------------------------------
   const shawn::Node&
   LocalizationEuclideanAnchorMessage::
   anchor( void )
      const throw()
   {
      return anchor_;
   }
   // ----------------------------------------------------------------------
   double
   LocalizationEuclideanAnchorMessage::
   distance( void )
      const throw()
   {
      return distance_;
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   LocalizationEuclideanNeighborMessage::
   LocalizationEuclideanNeighborMessage( const localization::DistanceMap& neighbors )
      : neighbors_( neighbors )
   {}
   // ----------------------------------------------------------------------
   LocalizationEuclideanNeighborMessage::
   LocalizationEuclideanNeighborMessage( const LocalizationEuclideanNeighborMessage& lenm )
      : neighbors_( lenm.neighbors() )
   {}
   // ----------------------------------------------------------------------
   LocalizationEuclideanNeighborMessage::
   ~LocalizationEuclideanNeighborMessage()
   {}
   // ----------------------------------------------------------------------
   const localization::DistanceMap&
   LocalizationEuclideanNeighborMessage::
   neighbors( void )
      const throw()
   {
      return neighbors_;
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/distance/localization_euclidean_messages.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_euclidean_messages.cpp,v $
 *-----------------------------------------------------------------------*/
