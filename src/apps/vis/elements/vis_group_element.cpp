/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_group_element.h"

namespace vis
{

   GroupElement::
   GroupElement( const std::string& n )
      : Element( n ),
        in_adding_ ( false )
   {}
   // ----------------------------------------------------------------------
   GroupElement::
   ~GroupElement()
   {}
   // ----------------------------------------------------------------------
   void
   GroupElement::
   init( void )
      throw()
   {
      Element::init();
   }
   // ----------------------------------------------------------------------
   void
   GroupElement::
   add_property( const std::string& n,
                 const ConstPropertyHandle& cph )
      throw( std::runtime_error )
   {
      if( in_adding_ )
         throw std::runtime_error("Loop in GroupElement setup -- aborting");

      in_adding_ = true;
      for( ElementList::iterator
              it    = elements_.begin(),
              endit = elements_.end();
           it != endit; ++it )
         (**it).add_property(n,cph);
      in_adding_ = false;
   }
   // ----------------------------------------------------------------------
   void
   GroupElement::
   add_element( Element& e )
      throw()
   {
      elements_.push_back(&e);
   }
   // ----------------------------------------------------------------------
   void
   GroupElement::
   remove_element( Element& e )
      throw()
   {
      elements_.remove(&e);
   }
   // ----------------------------------------------------------------------
   const PropertySet&
   GroupElement::
   properties( void )
      const throw()
   { return props_; }
   // ----------------------------------------------------------------------
   PropertySet&
   GroupElement::
   properties_w( void )
      throw()
   { return props_; }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/elements/vis_group_element.cpp,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/02/04 09:10:07 $
 *-----------------------------------------------------------------------
 * $Log: vis_group_element.cpp,v $
 * Revision 1.1  2006/02/04 09:10:07  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/
