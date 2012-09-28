/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/localization_nop_module.h"


namespace localization
{

   LocalizationNopModule::
   LocalizationNopModule()
   {}
   // ----------------------------------------------------------------------
   LocalizationNopModule::
   ~LocalizationNopModule()
   {}
   // ----------------------------------------------------------------------
   bool
   LocalizationNopModule::
   finished( void )
      throw()
   {
      return true;
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/localization_nop_module.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_nop_module.cpp,v $
 *-----------------------------------------------------------------------*/
