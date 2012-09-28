/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_PROPERTY_BASE_H
#define __SHAWN_TUBSAPPS_VIS_PROPERTY_BASE_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "sys/util/refcnt_pointer.h"
#include "sys/util/refcnt_pointable.h"

namespace vis
{
   class PropertyBase;
   typedef shawn::RefcntPointer<PropertyBase> PropertyHandle;
   typedef shawn::RefcntPointer<const PropertyBase> ConstPropertyHandle;

   /**
    * Base class for visualization properties. 
    */
   class PropertyBase
      : public shawn::RefcntPointable
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      PropertyBase();
      virtual ~PropertyBase();
      ///@}

      ///@name Get and set methods
      ///@{
      virtual double start_time( void ) const throw();
      virtual double end_time( void ) const throw();
      virtual double priority( void ) const throw();

      virtual void set_start( double ) throw();
      virtual void set_end( double ) throw();
      virtual void set_priority( double ) throw();
      ///@}

   private:
      double start_; 
      double end_;
      double prio_;
   };

}

#endif
#endif
