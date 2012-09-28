/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_ELEMENT_H
#define __SHAWN_TUBSAPPS_VIS_ELEMENT_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "sys/util/refcnt_pointer.h"
#include "sys/util/refcnt_pointable.h"
#include "sys/util/defutils.h"

#include "apps/vis/properties/vis_property_set.h"

namespace vis
{
   class Visualization;
   DECLARE_HANDLES( Element );

   /** \brief A base class for all Vis elements.
    * This is the base class for all Vis elements, like the camera and drawables.
    * To create all-new elements, derive them from this class.
    */
   class Element
      : public shawn::RefcntPointable
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      Element( const std::string& );
      virtual ~Element();
      ///@}

      ///@name Getter/Setter
      ///@{
      /**
       * Returns the name of this element.
       */
      inline std::string name( void ) const throw()
      { return name_; }
      /**
       * Returns the property set (constant and abstract).
       */
      virtual const PropertySet& properties( void ) const throw() = 0;
      /**
       * Returns the property set (abstract).
       */
      virtual PropertySet& properties_w( void ) throw() = 0;
      /**
       * Sets the visualization object.
       */
      virtual void set_visualization( Visualization& ) throw();
      /**
       * Gets the visualization object.
       */
      inline Visualization& visualization_w( void ) throw()
      { assert( vis_ != NULL ); return *vis_; }
      /**
       * Gets the visualization object (constant).
       */
      inline const Visualization& visualization( void ) const throw()
      { assert( vis_ != NULL ); return *vis_; }
      ///@}
      /**
       * Adds a property value to the corresponding property.
       */
      virtual void add_property( const std::string&, const ConstPropertyHandle& ) throw( std::runtime_error );
      /**
       * Base initialization of this element.
       */
      virtual void init( void ) throw();
   private:
      /// Name of the element.
      std::string name_;
      /// Visualization object.
      Visualization* vis_;
   };
}

#endif
#endif
