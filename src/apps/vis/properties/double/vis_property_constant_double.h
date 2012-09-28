/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_PROPERTY_CONSTANT_DOUBLE_H
#define __SHAWN_TUBSAPPS_VIS_PROPERTY_CONSTANT_DOUBLE_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_property_task.h"
#include "apps/vis/properties/vis_property.h"
#include "apps/vis/properties/vis_property_stack.h"

namespace vis
{

   /** \brief Task for creating constant double-type properties.
    * This task is used to create constant double properties. 
    */
   class PropertyConstantDoubleTask
      : public PropertyTask
   {
   public:
      /** \brief A double property
       * This class is used to define properties of type 'double'.
       */
      class PropertyConstantDouble
         : public Property<double>
      {
      public:
         ///@name Constructor/Destructor
         ///@{
         PropertyConstantDouble( double );
         virtual ~PropertyConstantDouble();
         ///@}
         /**
          * Returns the value which is kept by this property.
          * \return Property value
          */
         virtual double value( double, const PropertyStack<double>&, 
            const Element& ) const throw();
      private:
         double val_;
      };
      ///@name Constructor/Destructor
      ///@{
      PropertyConstantDoubleTask();
      virtual ~PropertyConstantDoubleTask();
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
       * Creates a new constant double property
       *
       * \param sc SimulationController instance
       * \return Handle of the new property
       */
      virtual ConstPropertyHandle create_property( 
         shawn::SimulationController& sc ) throw( std::runtime_error );
   };

}

#endif
#endif
