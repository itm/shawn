/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_GROUP_ELEMENT_H
#define __SHAWN_TUBSAPPS_VIS_GROUP_ELEMENT_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_element.h"

#include <list>

namespace vis
{
   typedef std::list<Element*> ElementList;

   /** \brief An element group.
    * This class represents a group of elements. It allows to group elements as
    * needed.
    */
   class GroupElement
      : public Element
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      GroupElement( const std::string& );
      virtual ~GroupElement();
      ///@}

      ///@name Getters
      ///@{
      /**
       * Returns the property set (constant).
       */
      virtual const PropertySet& properties( void ) const throw();
      /**
       * Returns the property set.
       */
      virtual PropertySet& properties_w( void ) throw();
      ///@}

      /**
       * Adds a property value to the corresponding property.
       */
      virtual void add_property( const std::string&, const ConstPropertyHandle& )
         throw( std::runtime_error );

      /**
       * Base initialization of this element.
       */
      virtual void init( void ) throw();

      /**
       * Adds a given element to this group.
       */
      virtual void add_element( Element& ) throw();

      virtual void remove_element( Element& ) throw();
   private:
      /// Name of this group.
      std::string name_;
      /// List of elements added to this group.
      ElementList elements_;
      /// Security variable to detect loops in adding mechanism.
      bool in_adding_;
      /// Property set.
      PropertySet props_;
   };


}

#endif
#endif
