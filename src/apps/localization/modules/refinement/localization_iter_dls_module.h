/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MODULES_REFINEMENT_ITER_DLS_MODULE_H
#define __SHAWN_APPS_LOCALIZATION_MODULES_REFINEMENT_ITER_DLS_MODULE_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/localization_module.h"
//#include "apps/localization/messages/refinement/localization_iter_dls_messages.h"
#include "apps/localization/messages/position/localization_dls_messages.h"
#include "apps/localization/math/localization_simple_matrix.h"
#include "apps/localization/modules/position/localization_dls_module.h"

namespace localization
{

   class LocalizationiDLSModule
      : public LocalizationModule
   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      LocalizationiDLSModule();
      ///
      virtual ~LocalizationiDLSModule();
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

	  virtual void initialize(void ) throw();

   protected:

      ///@name processing iterative lateration messages
      ///@{
      /** This method processes sound messages. Message source is added to
       *  sound nodes and, if necessary, the sound check is started again.
       *
       *  \sa LocalizationIterLaterationSoundMessage
       */
 /*     virtual bool process_iter_dls_sound_message(
            const LocalizationIterDLSSoundMessage& )
         throw();
   */   /** This method processes normal messages. The received information is
       *  updated in own neighborhood.
       *
       *  \sa LocalizationIterLaterationMessage
       */
      virtual bool process_iter_dls_message(
            const LocalizationDLSIterMessage& )
         throw();
      ///@}


      ///@name main refinement step
      ///@{
      /** This method executes the iterative lateration step. There are passed
       *  all checks mentioned in the \em Detailed \em Description above.
       */
      virtual void iter_step( void ) throw();
      ///@}
	  //virtual void se


   private:

      enum IterLaterationState
      {
         il_init,
		 il_wait,
         il_work,
         il_finished
      };

      IterLaterationState state_;
      int last_useful_msg_;

	 SimpleMatrix<double>* matrix_at_;
	SimpleMatrix<double>* vector_r_;
	SimpleMatrix<double>* covariance_a_;
	shawn::Node* linearization_tool_;
	std::vector<shawn::Node*>* beacons_;
	std::size_t neighbor_cnt_;

	std::vector<const shawn::Vec*>* iter_positions_;

   };

}// namespace localization
#endif
#endif
