/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_PROPERTY_TAG_SHAPE_INT_H
#define __SHAWN_TUBSAPPS_VIS_PROPERTY_TAG_SHAPE_INT_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_property_task.h"
#include "apps/vis/properties/vis_property.h"
#include "apps/vis/properties/vis_property_stack.h"

namespace shawn
{ class Node; }

namespace vis
{

   /** \brief A tag based integer property.
    * A tag based integer property.
    */
   class PropertyTagIntTask
      : public PropertyTask
   {
   public:
       
      /** \brief An integer property
       * This class is used to define properties of type 'integer'.
       */
      class PropertyTagInt
         : public Property<int>
      {
      public:
         ///@name Constructor/Destructor
         ///@{
         PropertyTagInt( const shawn::Node*, std::string tag );
         virtual ~PropertyTagInt();
         ///@}
         /**
          * Returns the value which is kept by this property.
          * \return Property value
          */
         virtual int value( double, const PropertyStack<int>&, const Element& ) const throw();
	 
      protected:
	 const shawn::Node* node_;
         std::string tag_;
	 
	 /**
          * Returns the value of a given tag and returns it's value,
          * or -1, if the tag is not available or of a wrong type.
          */
         int read_tag(const Element& e) const throw();
      };
      
      ///@name Constructor/Destructor
      ///@{
      PropertyTagIntTask();
      virtual ~PropertyTagIntTask();
      ///@}

      ///@name Identification
      ///@{
      /**
       * Returns the name of this property task.
       * \return Task name
       */
      virtual std::string name( void ) const throw();
      /**
       * Returns a short description of this property task.
       * \return Task description
       */
      virtual std::string description( void ) const throw();
      ///@}

   protected:

      /**
       * Creates a new constant integer property
       *
       * \param sc SimulationController instance
       * \return Handle of the new property
       */
      virtual ConstPropertyHandle create_property( shawn::SimulationController& sc )
         throw( std::runtime_error );
   };

}

#endif
#endif
