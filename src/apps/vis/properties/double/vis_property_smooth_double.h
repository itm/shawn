/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_PROPERTY_SMOOTH_DOUBLE_H
#define __SHAWN_TUBSAPPS_VIS_PROPERTY_SMOOTH_DOUBLE_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_property_task.h"
#include "apps/vis/properties/vis_property.h"
#include "apps/vis/properties/vis_property_stack.h"
#include "apps/vis/properties/vis_transition.h"

namespace vis
{

   /** \brief Task for creating smooth transition double properties.
    * This task is used to create properties which are able to create smooth
    * transitions (intermediate values) between two given double values.
    */
   class PropertySmoothDoubleTask
      : public PropertyTask
   {
   public:
      /** \brief A vector property
       * This class is used to define smooth transitions between values of type
       * 'double'.
       */
      class PropertySmoothDouble
         : public Property<double>
      {
      public:
         ///@name Constructor/Destructor
         ///@{
         PropertySmoothDouble( double v1, double v2, double tmid, Transition );
         virtual ~PropertySmoothDouble();
         ///@}
         /**
          * Returns the value which is kept by this property.
          * \param t current simulator timestamp
          * \return Property value
          */
         virtual double value( double, const PropertyStack<double>&, const Element& ) const throw();
      private:
         double v1_;
         double v2_;
         double tmid_;
         Transition trans_;
      };
      ///@name Constructor/Destructor
      ///@{
      PropertySmoothDoubleTask();
      virtual ~PropertySmoothDoubleTask();
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
       * Creates a new vis::PropertySmoothVec property
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
