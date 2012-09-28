/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_PROPERTY_BREATHING_DOUBLE_H
#define __SHAWN_TUBSAPPS_VIS_PROPERTY_BREATHING_DOUBLE_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_property_task.h"
#include "apps/vis/properties/vis_property.h"
#include "apps/vis/properties/vis_property_stack.h"
#include "apps/vis/properties/vis_transition.h"

namespace vis
{

   /** \brief Task for creating breathing double-type properties.
    * This task is used to create breathing double properties.
    */
   class PropertyBreathingDoubleTask
      : public PropertyTask
   {
   public:
      /** \brief A breathing double-type property.
       * This property represents a breathing double value. Breathing double
       * properties are changing their value over time, defined by an amplitude,
       * a speed value and time definitions.
       */
      class PropertyBreathingDouble
         : public Property<double>
      {
      public:
         ///@name Constructor/Destructor
         ///@{
         PropertyBreathingDouble( double t_start, double t_length,
                                  double amplitude, double speed,
                                  double v_start, double v_breath, double v_end );
         virtual ~PropertyBreathingDouble();
         ///@}
         /**
          * Returns the value which is computed by this property.
          * \return Property value
          */
         virtual double value( double, const PropertyStack<double>&, const Element& ) const throw();
      private:
         double t0_, l_, s_, a_, D1_, D2_, v0_, v2_;
         int k_;
      };

      ///@name Constructor/Destructor
      ///@{
      PropertyBreathingDoubleTask();
      virtual ~PropertyBreathingDoubleTask();
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
       * Creates a new breathing double property instance.
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
