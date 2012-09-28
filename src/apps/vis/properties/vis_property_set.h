/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_PROPERTY_SET_H
#define __SHAWN_TUBSAPPS_VIS_PROPERTY_SET_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "sys/util/refcnt_pointer.h"
#include "sys/util/refcnt_pointable.h"
#include "sys/util/defutils.h"

#include "apps/vis/properties/vis_property_stack_base.h"

#include <map>

namespace vis
{
   class Element;
   DECLARE_HANDLES( PropertySet );

#define DECL_PROPERTY( name, type ) \
   public:\
   inline const vis::PropertyStack<type>& stack_##name( void ) const throw() \
   { assert( stack_##name##_ != NULL ); return *stack_##name##_; }\
   \
   inline vis::PropertyStack<type>& stack_##name##_w( void ) throw()\
   { assert( stack_##name##_ != NULL ); return *stack_##name##_; }\
   \
   inline type name( double t ) const throw( std::runtime_error )\
   { return stack_##name().value(t,owner()); }\
   \
   protected:\
   vis::PropertyStack<type>* stack_##name##_

#define IMPL_PROPERTY( name, type ) \
   add_property_stack( #name , stack_##name##_ = new vis::PropertyStack<type> )



  /**
   * Base class for all vis property sets.
	*
	* Property sets are a collection of all properties of a specific vis object, like color
	* and size of a node or the position of the virtual camera. See this specific property sets
	* to learn about the corresponding object's properties.
	*
	* To add a property to a given set, call the DECL_PROPERTY(name, type) makro in the header file
	* of the property set and add the IMPL_PROPERTY(name, type) makro to it's init method, where 
	* name is the name of the new property and type is one of the supported property types.
	*/
   class PropertySet
      : public shawn::RefcntPointable
   {
   public:
      typedef std::map<std::string,PropertyStackHandle> PropertyMap;

      PropertySet();
      virtual ~PropertySet();

      virtual void init( const Element& ) throw( std::runtime_error );
      virtual void set_father( const ConstPropertySetHandle& ) throw( std::runtime_error );
      virtual ConstPropertyStackHandle property_stack( const std::string& ) const throw();
      virtual PropertyStackHandle property_stack_w( const std::string& ) const throw();
      
      virtual void add_property( const std::string&,
                                 const ConstPropertyHandle& ) throw( std::runtime_error );
      inline const Element& owner( void ) const throw()
      { assert( owner_ != NULL ); return *owner_; }

   protected:
      void add_property_stack( const std::string&,
                               const PropertyStackHandle& ) throw( std::runtime_error );
   private:
      PropertyMap props_;
      ConstPropertySetHandle father_;
      const Element* owner_;
   };


}

#endif
#endif
