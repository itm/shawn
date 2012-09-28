/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MESSAGES_DISTANCE_EUCLIDEAN_MESSAGES_H
#define __SHAWN_APPS_LOCALIZATION_MESSAGES_DISTANCE_EUCLIDEAN_MESSAGES_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "sys/node.h"
#include "sys/message.h"
#include "apps/localization/localization_processor.h"
#include "apps/localization/util/localization_defutils.h"


namespace localization
{

   class LocalizationEuclideanInitMessage
      : public shawn::Message
   {

   public:

      typedef LocalizationProcessor::ProcessorType ProcessorType;

      LocalizationEuclideanInitMessage( const ProcessorType& );
      LocalizationEuclideanInitMessage( const LocalizationEuclideanInitMessage& );
      virtual ~LocalizationEuclideanInitMessage();

      ///
      ProcessorType proc_type( void )
         const throw();


   private:

      ProcessorType proc_type_;

   };

   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------

   class LocalizationEuclideanAnchorMessage
      : public shawn::Message
   {

   public:

      LocalizationEuclideanAnchorMessage( const shawn::Node&, double );
      LocalizationEuclideanAnchorMessage( const LocalizationEuclideanAnchorMessage& );
      virtual ~LocalizationEuclideanAnchorMessage();

      ///
      const shawn::Node& anchor( void )
         const throw();
      ///
      virtual double distance( void )
         const throw();

   private:

      const shawn::Node& anchor_;
      double distance_;

   };

   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------

   class LocalizationEuclideanNeighborMessage
      : public shawn::Message
   {

   public:

      LocalizationEuclideanNeighborMessage( const localization::DistanceMap& );
      LocalizationEuclideanNeighborMessage( const LocalizationEuclideanNeighborMessage& );
      virtual ~LocalizationEuclideanNeighborMessage();

      ///
      const localization::DistanceMap& neighbors( void )
         const throw();


   private:

      localization::DistanceMap neighbors_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/distance/localization_euclidean_messages.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_euclidean_messages.h,v $
 *-----------------------------------------------------------------------*/
