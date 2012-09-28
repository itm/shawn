/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_PROPERTY_H
#define __SHAWN_TUBSAPPS_VIS_PROPERTY_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_property_base.h"
#include <limits>

namespace vis
{
   template<typename T> class PropertyStack;
   class Element;

   /** \brief A property template class.
    * This class is the template base for all property classes.
    */
   template<typename T>
   class Property
      : public PropertyBase
   {
   public:
      typedef T value_t;

      Property() {}
      virtual ~Property() {}

      virtual T value( double, const PropertyStack<T>&, const Element& ) const throw() = 0;
   };


   template<typename T>
   inline Property<T>*
   auto_init_property( Property<T>* p,
                       double prio = 0.0,
                       double start = -std::numeric_limits<double>::max(),
                       double end = std::numeric_limits<double>::max() )
      throw()
   {
      p->set_priority( prio );
      p->set_start( start );
      p->set_end( end );
      return p;
   }

}

#endif
#endif
