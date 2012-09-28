/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MODULES_NOP_MODULE_H
#define __SHAWN_APPS_LOCALIZATION_MODULES_NOP_MODULE_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/localization_module.h"


namespace localization
{

   /// Module that does nothing
   /** This module is just a place holder, that has no special task. It just
    *  returns, that it is finished, and is used, e.g., if there is no
    *  refinement algorithm selected.
    */
   class LocalizationNopModule
      : public LocalizationModule
   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      LocalizationNopModule();
      ///
      virtual ~LocalizationNopModule();
      ///@}


      ///@name module status info
      ///@{
      /** \return \c true
       */
      virtual bool finished( void ) throw();
      ///@}

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/localization_nop_module.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_nop_module.h,v $
 *-----------------------------------------------------------------------*/
