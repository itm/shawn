/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MODULES_DISTANCE_DV_HOP_MODULE_H
#define __SHAWN_APPS_LOCALIZATION_MODULES_DISTANCE_DV_HOP_MODULE_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/localization_module.h"
#include "apps/localization/messages/distance/localization_dv_hop_messages.h"
#include "apps/localization/util/localization_defutils.h"


namespace localization
{

   /// Module implementing DV-hop
   /** This module implements DV-hop. On the one hand, unknown nodes store the
    *  minimal hop count to at most 'floodlimit' anchors, where the floodlimit
    *  is taken from the LocalizationObserver. On the other hand, anchors
    *  compute an average hop distance between each other and send this
    *  information out. The unknowns again convert their minimal hop counts
    *  via the average hop distance into distances to anchors.
    */
   class LocalizationDVhopModule
      : public LocalizationModule
   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      LocalizationDVhopModule();
      ///
      virtual ~LocalizationDVhopModule();
      ///@}


      ///@name standard methods startup/simulation steps
      ///@{
      /** This case, do nothing.
       *
       *  \sa LocalizationModule::boot()
       */
      virtual void boot( void ) throw();
      /** Handling of DV-Hop-Messages.
       *
       *  \sa LocalizationModule::process_message()
       */
      virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
      /** Check, whether state can be set to finished or not. Moreover, if
       *  owner is an anchor, initial and calibrating message are send.
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
      /** Message handling by anchors. If anchor receives a dv-hop message, it
       *  computes the real distance between them and devides it by the
       *  hop count to get an average hop distance.
       *
       *  This is done for each received message (as long as floodlimit is not
       *  reached), so that the mean of average hop distances can be builded
       *  to send out a calibration message.
       *
       *  \sa LocalizationDVhopMessage
       */
      virtual bool process_dv_hop_message_anchor(
            const LocalizationDVhopMessage& )
         throw();
      /** Message handling by unknowns. If unknown receives a dv-hop message,
       *  it decides, whether anchor and hop count should be stored or not.
       *
       *  If a message about a new anchor arrived and floodlimit is not
       *  reached, or the received hop count is lower than known one,
       *  information is stored and a new message containing this is sent out.
       *
       *  \sa LocalizationDVhopMessage
       */
      virtual bool process_dv_hop_message_unknown(
            const LocalizationDVhopMessage& )
         throw();
      /** If an unknown receives dv-hop calibration message containing an
       *  average hop distance, the hops to known anchors are converted into
       *  distances by multiplying them with each other.
       *
       *  \sa LocalizationDVcalMessage
       */
      virtual bool process_dv_cal_message( const LocalizationDVcalMessage& )
         throw();
      ///@}


   private:

      enum DvHopState
      {
         dvh_init,
         dvh_work,
         dvh_calibrated,
         dvh_finished
      };

      DvHopState state_;
      int last_useful_msg_;
      double avg_hop_dist_;
      double hop_sum_;
      int hop_cnt_;

      localization::NodeSet known_anchors_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/distance/localization_dv_hop_module.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_dv_hop_module.h,v $
 *-----------------------------------------------------------------------*/
