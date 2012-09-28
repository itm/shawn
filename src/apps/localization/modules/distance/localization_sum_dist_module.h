/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MODULES_DISTANCE_SUM_DIST_MODULE_H
#define __SHAWN_APPS_LOCALIZATION_MODULES_DISTANCE_SUM_DIST_MODULE_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/localization_module.h"
#include "apps/localization/messages/distance/localization_sum_dist_messages.h"


namespace localization
{

   /// Module implementing Sum-dist
   /** This module implements Sum-dist. Distance to anchors is get by simply
    *  flooding the network beginning at the anchors. Then the unknowns adding
    *  the distance estimated each hop and broadcast this information again.
    *
    *  Each unknown takes the minimal distance to at most 'floodlimit'
    *  anchors, where the floodlimit is taken from the LocalizationObserver.
    */
   class LocalizationSumDistModule
      : public LocalizationModule
   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      LocalizationSumDistModule();
      virtual ~LocalizationSumDistModule();
      ///@}


      ///@name standard methods startup/simulation steps
      ///@{
      /** This case, do nothing.
       *
       *  \sa LocalizationModule::boot()
       */
      virtual void boot( void ) throw();
      /** Handling of Sum-dist-Messages.
       *
       *  \sa LocalizationModule::process_message()
       */
      virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
      /** Check, whether state can be set to finished or not. Moreover, if
       *  owner is an anchor, initial message is send.
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

      ///@name message handling methods
      ///@{
      /** Message handling by unknowns such as described above.
       *
       *  \sa LocalizationSumDistMessage
       */
      bool process_sum_dist_message( const LocalizationSumDistMessage& ) throw();
      ///@}


   private:

      enum SumDistState
      {
         sd_init,
         sd_work,
         sd_finished
      };

      SumDistState state_;
      int last_useful_msg_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/distance/localization_sum_dist_module.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_sum_dist_module.h,v $
 *-----------------------------------------------------------------------*/
