/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_PROPERTY_STACK_H
#define __SHAWN_TUBSAPPS_VIS_PROPERTY_STACK_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_property.h"
#include "apps/vis/properties/vis_property_stack_base.h"
#include <set>
#include <iterator>

namespace vis
{
   class Element;

   template<typename T>
   class PropertyStack
      : public PropertyStackBase
   {
   public:
      typedef T value_t;
      typedef PropertyStack<T> self_t;
      typedef Property<T> property_t;
      typedef shawn::RefcntPointer<const Property<T> > ConstPropertyTHandle;
      typedef shawn::RefcntPointer<const PropertyStack<T> > ConstPropertyStackTHandle;
      struct PropertyTSort
      {
         bool operator() ( const ConstPropertyTHandle& a,
                           const ConstPropertyTHandle& b )
         { return a->priority() > b->priority(); }
      };
      typedef std::set<ConstPropertyTHandle,PropertyTSort> PropertyTSet;



      PropertyStack() 
      {}
      virtual ~PropertyStack() 
      {}

      virtual void add( const ConstPropertyHandle& cph )
         throw( std::runtime_error )
      {
         const property_t* p = 
            dynamic_cast<const Property<T>*>(cph.get());
         if( p==NULL )
            throw std::runtime_error("bad property type for stack");
         add_t(p);
      }
      inline void add_t( const ConstPropertyTHandle& cph )
      {
         props_.insert( cph );
      }
      virtual void set_father( const ConstPropertyStackHandle& cpsh )
         throw( std::runtime_error )
      {
         if( cpsh.is_null() )
            set_father_t(NULL);
         else {
            const PropertyStack<T>* ps =
               dynamic_cast<const PropertyStack<T>*>(cpsh.get());
            if( ps==NULL )
               throw std::runtime_error("bad stack type, cannot be father");
            set_father_t(ps);
         }
      }
      inline void set_father_t( const ConstPropertyStackTHandle& cpsh )
         throw()
      {
         father_ = cpsh;
      }
      virtual ConstPropertyStackHandle father( void ) const throw()
      { return father_.get(); }
      inline const ConstPropertyStackTHandle& father_t( void ) const throw()
      { return father_; }


      const property_t* find_property_self( double t, const self_t&, double below=std::numeric_limits<double>::max() ) const throw()
      {
         if( below == std::numeric_limits<double>::max() )
            {
               for( typename PropertyTSet::const_iterator
                       it    = props_.begin(),
                       endit = props_.end();
                    it != endit; ++it )
                  if( (**it).start_time() <= t &&
                      (**it).end_time() >= t )
                     return it->get();
            }
         else
            {
               for( typename PropertyTSet::const_iterator
                       it    = props_.begin(),
                       endit = props_.end();
                    it != endit; ++it )
                  if( (**it).start_time() <= t &&
                      (**it).end_time() >= t &&
                      (**it).priority() < below )
                     return it->get();
            }
         return NULL;
      }
      const property_t* find_property( double t, const self_t& s, double below=std::numeric_limits<double>::max() ) const throw()
      {
         const property_t* my = find_property_self(t,s,below);
         if( father_.is_null() )
            return my;
         else if( my==NULL )
            return father_->find_property(t,s,below);
         else {
            const property_t* his = father_->find_property(t,s,below);
            if( his==NULL )
               return my;
            else
               return
                  my->priority()>=his->priority()
                  ? my
                  : his;
         }
      }

      inline T value( double t,
                      const Element& e,
                      double below=std::numeric_limits<double>::max() )
         const throw( std::runtime_error )
      {
         const property_t* prop = find_property(t,*this,below);
         if( prop==NULL )
            throw std::runtime_error( error_no_property(t) );
         return prop->value(t,*this,e);
      }
      

   private:
      PropertyTSet props_;
      ConstPropertyStackTHandle father_;
   };


}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vis_property_stack.h,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2006/02/01 17:07:29 $
 *-----------------------------------------------------------------------
 * $Log: vis_property_stack.h,v $
 * Revision 1.3  2006/02/01 17:07:29  ali
 * *** empty log message ***
 *
 * Revision 1.2  2006/01/31 12:44:00  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/

