/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/position/localization_minmax_module.h"
#include "apps/localization/math/localization_triangulation.h"


namespace localization
{

   LocalizationMinMaxModule::
   LocalizationMinMaxModule()
      : state_( minmax_wait )
   {}
   // ----------------------------------------------------------------------
   LocalizationMinMaxModule::
   ~LocalizationMinMaxModule()
   {}
   // ----------------------------------------------------------------------
   void
   LocalizationMinMaxModule::
   work( void )
      throw()
   {
      if ( owner().is_anchor() )
         state_ = minmax_finished;

      if ( state_ == minmax_finished )
         return;

      if ( state_ == minmax_wait )
         state_ = minmax_work;

      shawn::Vec est_pos;
      NeighborInfoList neighbors;
      collect_neighbors( neighborhood(), lat_anchors, neighbors );

      if ( est_pos_min_max( neighbors, est_pos ) )
      {
         if ( !observer().check_residue() || check_residue( neighbors, est_pos, lat_anchors, observer().comm_range() ) )
            node_w().set_est_position( est_pos );
      }

      state_ = minmax_finished;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationMinMaxModule::
   finished( void )
      throw()
   {
      return state_ == minmax_finished;
   }
	void
	LocalizationMinMaxModule::
	rollback( void )
		throw()
	{
   state_ = minmax_wait;
	}
}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/position/localization_minmax_module.cpp,v $
 * Version $Revision: 510 $
 * Date    $Date: 2011-03-01 20:37:06 +0100 (Tue, 01 Mar 2011) $
 *-----------------------------------------------------------------------
 * $Log: localization_minmax_module.cpp,v $
 *-----------------------------------------------------------------------*/
