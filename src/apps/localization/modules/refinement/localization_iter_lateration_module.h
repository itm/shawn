/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MODULES_REFINEMENT_ITER_LATERATION_MODULE_H
#define __SHAWN_APPS_LOCALIZATION_MODULES_REFINEMENT_ITER_LATERATION_MODULE_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/localization_module.h"
#include "apps/localization/messages/refinement/localization_iter_lateration_messages.h"


namespace localization
{

   /// Module implementing refinement by iterative lateration
   /** This module implements refinement by iterative lateration. Idea is to
    *  take into account all neighbors, respectively their distances and
    *  positions, and start lateration with this information. After that the
    *  new position is broadcasted, so that neighbors are able to start
    *  lateration again with this new data, and so on.
    *
    *  The process finished, either after a given number of steps, or, if the
    *  position update becomes very small.
    *
    *  Main problem of this procedere are nodes with inaccurate information.
    *  To filter out most of these cases, there are different methods.
    *
    *  At first, there is the use of confidences. This means, that every node
    *  is associated with a special confidence level between 0 and 1. Unknowns
    *  start with a low confidence like 0.1, anchors with a high one like 1.
    *  These values are used in the
    *  \ref localization::est_pos_lateration "lateration" by a weighted least
    *  squares approach. Instead of solving \f$ Ax = b\f$,
    *  \f$ \omega Ax = \omega b\f$ is solved, where \f$\omega\f$ is the vector
    *  of confidences. After a successful lateration a node sets its
    *  confidence to the average of its neighbors confidences. This will, in
    *  general, increase the confidence level.
    *
    *  Second issue is filtering out ill-connected nodes.\n
    *  To take part in refinement phase, a node has to be \em sound. This
    *  means, that there are \em independent references to at least three/four
    *  anchors. That is, information about anchors has to be get from
    *  different neighbors. Moreover, if a node becomes sound, it sends this
    *  out. The neighbors again receiving this information, add the sound node
    *  to an own list, so that an unsound node is able to become sound, if
    *  the size of intersection between sound and references becomes greater
    *  or equal three/four.
    *
    *  Third, the twin neighbors should be filtered out. This means, that one
    *  neighbor, that is very close to another, is set to be a twin and is
    *  ignored in further computation. The twin check has to be done in every
    *  iteration, because in most cases their position change.
    *
    *  At least, there is the
    *  \ref localization::check_residue "residuen check" to check the validity
    *  of the new position. To avoid being trapped in a local minimum, there
    *  is a given chance to accept a failed check anyway. If this happens, the
    *  confidence of affected node is reduced by 50 percent.
    */
   class LocalizationIterLaterationModule
      : public LocalizationModule
   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      LocalizationIterLaterationModule();
      ///
      virtual ~LocalizationIterLaterationModule();
      ///@}


      ///@name standard methods startup/simulation steps
      ///@{
      /** Read given parameters.
       *
       *  \sa LocalizationModule::boot()
       */
      virtual void boot( void ) throw();
      /** Handling of Iter-Lateration-Messages.
       *
       *  \sa LocalizationModule::process_message()
       */
      virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
      /** Check, whether state can be set to finished or not. Moreover, send
       *  initial messages and check on first pass, whether node is sound or
       *  not.
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

      ///@name processing iterative lateration messages
      ///@{
      /** This method processes sound messages. Message source is added to
       *  sound nodes and, if necessary, the sound check is started again.
       *
       *  \sa LocalizationIterLaterationSoundMessage
       */
      virtual bool process_iter_lateration_sound_message(
            const LocalizationIterLaterationSoundMessage& )
         throw();
      /** This method processes normal messages. The received information is
       *  updated in own neighborhood.
       *
       *  \sa LocalizationIterLaterationMessage
       */
      virtual bool process_iter_lateration_message(
            const LocalizationIterLaterationMessage& )
         throw();
      ///@}


      ///@name main refinement step
      ///@{
      /** This method executes the iterative lateration step. There are passed
       *  all checks mentioned in the \em Detailed \em Description above.
       */
      virtual void iter_lateration_step( void ) throw();
      ///@}


   private:

      enum IterLaterationState
      {
         il_init,
         il_work,
         il_finished
      };

      IterLaterationState state_;
      int last_useful_msg_;
      int iteration_limit_;
      int iteration_cnt_;
      int min_confident_nbrs_;
      double twin_measure_;
      double abort_pos_update_;
      double res_acceptance_;
      bool sound_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/refinement/localization_iter_lateration_module.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_iter_lateration_module.h,v $
 *-----------------------------------------------------------------------*/
