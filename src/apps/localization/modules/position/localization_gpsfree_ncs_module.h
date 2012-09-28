/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MODULES_DISTANCE_GPSFREE_NCS_MODULE_H
#define __SHAWN_APPS_LOCALIZATION_MODULES_DISTANCE_GPSFREE_NCS_MODULE_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "sys/node.h"
#include "apps/localization/modules/localization_module.h"
#include "apps/localization/messages/position/localization_gpsfree_ncs_messages.h"
#include "apps/localization/util/localization_defutils.h"


namespace localization
{

   /// Module implementing GPS-free Network Coordinate System building
   /** ...
    */
   class LocalizationGPSfreeNCSModule
      : public LocalizationModule
   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      LocalizationGPSfreeNCSModule();
      ///
      virtual ~LocalizationGPSfreeNCSModule();
      ///@}


      ///@name standard methods startup/simulation steps
      ///@{
      /** Read given parameters.
       *
       *  \sa init_parameters(), LocalizationModule::boot()
       */
      virtual void boot( void ) throw();
      /** Handling of GPS-free-NCS-Messages.
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


      ///@name module status info
      ///@{
      /** \return \c true, if module is finished. \c false otherwise
       *  \sa LocalizationModule::finished()
       */
      virtual bool finished( void ) throw();
      ///@}

		virtual void rollback( void ) throw();


   protected:

      enum GPSfreeNCSState
      {
         gfncs_init,
         gfncs_wait_lcs,
         gfncs_build_ncs,
         gfncs_finished
      };

      ///@name processing gpsfree ncs messages
      ///@{
      /**
       */
      virtual bool process_gpsfree_ncs_lcs_message(
            const LocalizationGPSfreeNCSLCSMessage& )
         throw();
      ///@}

      ///@name initial methods
      ///@{
      /** Read the given parameters, which have been set via simulator
       *  commands or configuration file.
       */
      virtual void init_parameters( void ) throw();
      ///@}

      ///@name module protected access to private attributes
      ///@{
      /** \return actual state of module
       */
      virtual const GPSfreeNCSState& state( void ) const throw();
      /** \return actual state of module
       */
      virtual void set_state( const GPSfreeNCSState& ) throw();
      /** \return if actual node a root_node
       */
      virtual bool root_node( void ) const throw();
      /** \return if actual node a root_node
       */
      virtual void set_root_node( bool ) throw();
      /** \return if ncs is already computed
       */
      virtual bool computed_ncs( void ) const throw();
      /** \return if ncs is already computed
       */
      virtual void set_computed_ncs( bool ) throw();
      /** \return the time of arrival of last useful message
       */
      virtual int last_useful_msg( void ) const throw();
      /** \return the time of arrival of last useful message
       */
      virtual void set_last_useful_msg( int ) throw();
      ///@}


   private:

      GPSfreeNCSState state_;

      bool root_node_;
      bool computed_ncs_;
      int last_useful_msg_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/position/localization_gpsfree_ncs_module.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_gpsfree_ncs_module.h,v $
 *-----------------------------------------------------------------------*/
