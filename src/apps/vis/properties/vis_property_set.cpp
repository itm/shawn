/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_property_set.h"
#include <string>

namespace vis
{
   PropertySet::
   PropertySet()
      : owner_ ( NULL )
   {}
   // ----------------------------------------------------------------------
   PropertySet::
   ~PropertySet()
   {}
   // ----------------------------------------------------------------------
   void
   PropertySet::
   init( const Element& e )
      throw( std::runtime_error )
   {
      owner_ = &e;
   }
   // ----------------------------------------------------------------------
   void
   PropertySet::
   set_father( const ConstPropertySetHandle& cpsh )
      throw( std::runtime_error )
   {
      father_=cpsh;
      if( cpsh.is_null() )
         for( PropertyMap::iterator 
                 it    = props_.begin(),
                 endit = props_.end();
              it != endit; ++it )
            it->second->set_father(NULL);
      else
         for( PropertyMap::iterator 
                 it    = props_.begin(),
                 endit = props_.end();
              it != endit; ++it )
            it->second->set_father(father_->property_stack(it->first));
   }
   // ----------------------------------------------------------------------
   ConstPropertyStackHandle
   PropertySet::
   property_stack( const std::string& s )
      const throw()
   {
      PropertyMap::const_iterator it = props_.find(s);
      return it==props_.end()
         ? NULL
         : it->second;
   }
   // ----------------------------------------------------------------------
   PropertyStackHandle
   PropertySet::
   property_stack_w( const std::string& s )
      const throw()
   {
      PropertyMap::const_iterator it = props_.find(s);
      return it==props_.end()
         ? NULL
         : it->second;
   }
   // ----------------------------------------------------------------------
   void
   PropertySet::
   add_property( const std::string& s,
                 const ConstPropertyHandle& p )
      throw( std::runtime_error )
   {
      PropertyStackHandle ps(property_stack_w(s));
      if( ps.is_null() )
         throw std::runtime_error( std::string("no such property stack: '") +
                                   s +
                                   std::string("'") );
      ps->add(p);
   }
   // ----------------------------------------------------------------------
   void
   PropertySet::
   add_property_stack( const std::string& s,
                       const PropertyStackHandle& psh )
      throw( std::runtime_error )
   {
      if( props_.find(s)!=props_.end() )
         throw std::runtime_error(std::string("adding property stack '") +
                                  s +
                                  std::string("' twice"));
      if( father_.is_not_null() )
         psh->set_father(father_->property_stack(s));

      props_[s] = psh;
   }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vis_property_set.cpp,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2006/03/15 14:14:02 $
 *-----------------------------------------------------------------------
 * $Log: vis_property_set.cpp,v $
 * Revision 1.3  2006/03/15 14:14:02  pfister
 * *** empty log message ***
 *
 * Revision 1.2  2006/01/31 12:44:00  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/
