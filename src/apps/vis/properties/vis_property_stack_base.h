/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_PROPERTY_STACK_BASE_H
#define __SHAWN_TUBSAPPS_VIS_PROPERTY_STACK_BASE_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "sys/util/refcnt_pointer.h"
#include "sys/util/refcnt_pointable.h"
#include "apps/vis/properties/vis_property_base.h"

#include <stdexcept>

namespace vis
{
   class PropertyStackBase;
   typedef shawn::RefcntPointer<PropertyStackBase> PropertyStackHandle;
   typedef shawn::RefcntPointer<const PropertyStackBase> ConstPropertyStackHandle;

   class PropertyStackBase
      : public shawn::RefcntPointable
   {
   public:

      PropertyStackBase();
      virtual ~PropertyStackBase();

      virtual void add( const ConstPropertyHandle& ) throw( std::runtime_error ) = 0;
      virtual void set_father( const ConstPropertyStackHandle& ) throw( std::runtime_error ) = 0;
      virtual ConstPropertyStackHandle father( void ) const throw() = 0;

   protected:
      static std::string error_no_property( double ) throw();
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vis_property_stack_base.h,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/01/29 21:02:01 $
 *-----------------------------------------------------------------------
 * $Log: vis_property_stack_base.h,v $
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/

