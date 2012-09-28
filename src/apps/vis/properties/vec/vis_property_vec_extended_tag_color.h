/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __VIS_EXTENDED_TAG_COLOR_VEC_H
#define __VIS_EXTENDED_TAG_COLOR_VEC_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_property_task.h"
#include "apps/vis/properties/vis_property.h"
#include "apps/vis/properties/vis_property_stack.h"

#include <string>

namespace shawn
{ class Node; }

namespace vis
{

   /** \brief Extended Version of PropertyTagColorVec
    * The PropertyExtendedTagColorVecTask is an extended version of
    * the PropertyTagColorVecTask, being more configurable and creating
    * bigger color ranges.
    */
   class PropertyExtendedTagColorVecTask
      : public PropertyTask
   {
   public:
      /** \brief A vector property
       * This class is used to define properties of type 'shawn::vec'.
       */
      class PropertyExtendedTagColorVec
         : public Property<shawn::Vec>
      {
      public:
         ///@name Constructor/Destructor
         ///@{
         PropertyExtendedTagColorVec( const shawn::Node*, std::string tag, double tag_max );
         virtual ~PropertyExtendedTagColorVec();
         ///@}
         /**
          * Returns the value which is kept by this property.
          * \return Property value
          */
         virtual shawn::Vec value( double, const PropertyStack<shawn::Vec>&, 
            const Element& ) const throw();
      private:
         const shawn::Node* node_;
         std::string tag_;
	 double tag_max_;

         /**
          * Returns the value of a given tag and returns it's value,
          * or -1.0, if the tag is not available or of a wrong type.
          */
         double read_tag(const Element& e) const throw();
      };
      ///@name Constructor/Destructor
      ///@{
      PropertyExtendedTagColorVecTask();
      virtual ~PropertyExtendedTagColorVecTask();
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
       * Creates a new constant vis::vec property
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
