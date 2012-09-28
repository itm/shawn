/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MODULES_DISTANCE_GPSFREE_NCS_EXTENDED_MODULE_H
#define __SHAWN_APPS_LOCALIZATION_MODULES_DISTANCE_GPSFREE_NCS_EXTENDED_MODULE_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include <map>
#include "apps/localization/modules/position/localization_gpsfree_ncs_module.h"
#include "apps/localization/messages/position/localization_gpsfree_ncs_extended_messages.h"


namespace localization
{

   /// Module implementing the extention of GPS-free Network Coordinate System
   /// building
   /** This module implements the ideas of Jens Boehme, the author of the
    *  student research project <em>Algorithmus mit verbesserter
    *  Skalierbarkeit zur Bestimmung eines Koordinatensystems aus rein lokalen
    *  Informationen innerhalb eines mobilen Ad-Hoc-Netzwerkes</em> at the
    *  TU Braunschweig.
    *
    *  The idea is, that a node does not only takes one LCS of a neighbor, but
    *  also takes the mean value of the correction angle and the given
    *  position of different neighbors.
    */
   class LocalizationGPSfreeNCSExtendedModule
      : public LocalizationGPSfreeNCSModule
   {

   public:

      typedef std::map<const LocalizationLocalCoordinateSystem*, int> LCShopMap;
      typedef std::map<const LocalizationLocalCoordinateSystem*, int>::const_iterator ConstLCShopMapIterator;

      ///@name construction / destruction
      ///@{
      ///
      LocalizationGPSfreeNCSExtendedModule();
      ///
      virtual ~LocalizationGPSfreeNCSExtendedModule();
      ///@}


      ///@name standard methods startup/simulation steps
      ///@{
      /** Handling of GPS-free-NCS-Extended-Messages.
       *
       *  \sa LocalizationModule::process_message()
       */
      virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
      /** Check, whether state can be set to finished or not. Moreover, send
       *  local coordinate system, compute correction angle and build network
       *  coordinate system.
       *
       *  \sa LocalizationModule::work()
       */
      virtual void work( void ) throw();
      ///@}


   protected:

      ///@name processing gpsfree ncs messages
      ///@{
      /**
       */
      virtual bool process_gpsfree_ncs_lcs_extended_message(
            const LocalizationGPSfreeNCSLCSExtendedMessage& )
         throw();
      /**
       */
      virtual void compute_ncs( void ) throw();
      ///@}

      ///@name initial methods
      ///@{
      /** Read the given parameters, which have been set via simulator
       *  commands or configuration file.
       */
      virtual void init_parameters( void ) throw();
      ///@}


   private:

      int wait_for_lcs_;
      int minimal_hop_cnt_;
      LCShopMap lcs_hop_map_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/position/localization_gpsfree_ncs_extended_module.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_gpsfree_ncs_extended_module.h,v $
 *-----------------------------------------------------------------------*/
