/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MODULES_DISTANCE_TEST_MODULE_
#define __SHAWN_APPS_LOCALIZATION_MODULES_DISTANCE_TEST_MODULE_

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/localization_module.h"
#include "apps/localization/util/localization_defutils.h"
#include "apps/localization/math/localization_statistic.h"

namespace localization
{

   class LocalizationTestModule
      : public LocalizationModule
   {

   public:

      LocalizationTestModule();
      virtual ~LocalizationTestModule();
      
      virtual void boot( void ) throw();
      virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
      virtual void work( void ) throw();
      virtual bool finished( void ) throw();
      virtual void rollback( void ) throw();

   private:
     virtual void estimate( void ) throw();
     virtual void dumpValue( void ) throw();
	 virtual int getIndex(double value) throw();
   
    bool finished_;
    LocalizationStatistic* values_;
	int* val_;
   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/distance/localization_dv_hop_module.h,v $
 * Version $Revision: 56 $
 * Date    $Date: 2007-07-12 11:04:27 +0200 (Do, 12 Jul 2007) $
 *-----------------------------------------------------------------------
 * $Log: localization_dv_hop_module.h,v $
 *-----------------------------------------------------------------------*/
