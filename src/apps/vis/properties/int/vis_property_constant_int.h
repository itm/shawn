/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_PROPERTY_CONSTANT_INT_H
#define __SHAWN_TUBSAPPS_VIS_PROPERTY_CONSTANT_INT_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_property_task.h"
#include "apps/vis/properties/vis_property.h"
#include "apps/vis/properties/vis_property_stack.h"

namespace vis
{

   /** \brief Task for creating constant int-type properties.
    * This task is used to create constant integer properties. 
    */
   class PropertyConstantIntTask
      : public PropertyTask
   {
   public:
      /** \brief An integer property
       * This class is used to define properties of type 'integer'.
       */
      class PropertyConstantInt
         : public Property<int>
      {
      public:
         ///@name Constructor/Destructor
         ///@{
         PropertyConstantInt( int );
         virtual ~PropertyConstantInt();
         ///@}
         /**
          * Returns the value which is kept by this property.
          * \return Property value
          */
         virtual int value( double, const PropertyStack<int>&, const Element& ) const throw();
      private:
         int val_;
      };
      ///@name Constructor/Destructor
      ///@{
      PropertyConstantIntTask();
      virtual ~PropertyConstantIntTask();
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
