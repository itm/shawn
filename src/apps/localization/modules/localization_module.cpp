/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/localization_module.h"


using namespace shawn;

namespace localization
{

   LocalizationModule::
   LocalizationModule()
   {
      observer_ = new LocalizationObserver();
   }
   // ----------------------------------------------------------------------
   LocalizationModule::
   ~LocalizationModule()
   {
      if ( observer_ ) delete observer_;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationModule::
   boot( void )
      throw()
   {}
   // ----------------------------------------------------------------------
   bool
   LocalizationModule::
   process_message( const ConstMessageHandle& )
      throw()
   {
      return false;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationModule::
   work( void )
      throw()
   {}
   // ----------------------------------------------------------------------
   void
   LocalizationModule::
   rollback( void )
      throw()
   {
	}
   // ----------------------------------------------------------------------
   void
   LocalizationModule::
   set_owner( LocalizationProcessor& owner )
      throw()
   {
      owner_ = &owner;
      observer_->set_owner( owner );
      neighborhood_ = &owner.neighborhood_;
      local_coord_sys_ = &owner.local_coord_sys_;
   }
   // ----------------------------------------------------------------------
   const LocalizationProcessor&
   LocalizationModule::
   owner( void )
      const throw()
   {
      return *owner_;
   }
   // ----------------------------------------------------------------------
   LocalizationProcessor&
   LocalizationModule::
   owner_w( void )
      throw()
   {
      return *owner_;
   }
   // ----------------------------------------------------------------------
   const LocalizationObserver&
   LocalizationModule::
   observer( void )
      const throw()
   {
      return *observer_;
   }
   // ----------------------------------------------------------------------
   const LocalizationNeighborhood&
   LocalizationModule::
   neighborhood( void )
      const throw()
   {
      return *neighborhood_;
   }
   // ----------------------------------------------------------------------
   LocalizationNeighborhood&
   LocalizationModule::
   neighborhood_w( void )
      throw()
   {
      return *neighborhood_;
   }
   // ----------------------------------------------------------------------
   const LocalizationLocalCoordinateSystem&
   LocalizationModule::
   local_coord_sys( void )
      const throw()
   {
      return *local_coord_sys_;
   }
   // ----------------------------------------------------------------------
   LocalizationLocalCoordinateSystem&
   LocalizationModule::
   local_coord_sys_w( void )
      throw()
   {
      return *local_coord_sys_;
   }
   // ----------------------------------------------------------------------
   const Node&
   LocalizationModule::
   node( void )
      const throw()
   {
      return owner_->owner();
   }
   // ----------------------------------------------------------------------
   Node&
   LocalizationModule::
   node_w( void )
      throw()
   {
      return owner_->owner_w();
   }
   // ----------------------------------------------------------------------
   int
   LocalizationModule::
   simulation_round( void )
      const throw()
   {
      return owner_->simulation_round();
   }
   // ----------------------------------------------------------------------
   void
   LocalizationModule::
   set_confidence( double confidence )
      throw()
   {
      owner_->confidence_ = confidence;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationModule::
   send( const MessageHandle& mh )
      throw()
   {
      owner_->send( mh );
   }
   // ----------------------------------------------------------------------
   double
   LocalizationModule::
   estimate_distance( const Node& src, const Node& dest )
      const throw()
   {
      return owner_->estimate_distance( src, dest );
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/localization_module.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_module.cpp,v $
 *-----------------------------------------------------------------------*/
