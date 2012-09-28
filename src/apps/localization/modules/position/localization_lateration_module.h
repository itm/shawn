/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MODULES_POSITION_LATERATION_MODULE_H
#define __SHAWN_APPS_LOCALIZATION_MODULES_POSITION_LATERATION_MODULE_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/localization_module.h"


namespace localization
{

   ///
   /**
    */
   class LocalizationLaterationModule
      : public LocalizationModule
   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      LocalizationLaterationModule();
      ///
      virtual ~LocalizationLaterationModule();
      ///@}

      ///@name standard methods startup/simulation steps
      ///@{
      /**
       */
      virtual void work( void ) throw();
      ///@}

      ///@name module status info
      ///@{
      /** \return \c true
       */
      virtual bool finished( void ) throw();
      ///@}
		virtual void rollback(void ) throw();


   private:

      enum LaterationState
      {
         lat_wait,
         lat_work,
         lat_finished
      };

      LaterationState state_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/position/localization_lateration_module.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_lateration_module.h,v $
 *-----------------------------------------------------------------------*/
