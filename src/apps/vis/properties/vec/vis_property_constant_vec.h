/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_PROPERTY_CONSTANT_VEC_H
#define __SHAWN_TUBSAPPS_VIS_PROPERTY_CONSTANT_VEC_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_property_task.h"
#include "apps/vis/properties/vis_property.h"
#include "apps/vis/properties/vis_property_stack.h"

namespace vis
{

   /** \brief Task for creating constant vector properties.
    * This task is used to create constant 3D vector properties. 
    */
   class PropertyConstantVecTask
      : public PropertyTask
   {
   public:
      /** \brief A vector property
       * This class is used to define properties of type 'shawn::vec'.
       */
      class PropertyConstantVec
         : public Property<shawn::Vec>
      {
      public:
         ///@name Constructor/Destructor
         ///@{
         PropertyConstantVec( shawn::Vec );
         virtual ~PropertyConstantVec();
         ///@}
         /**
          * Returns the value which is kept by this property.
          * \return Property value
          */
         virtual shawn::Vec value( double, const PropertyStack<shawn::Vec>&, 
            const Element& ) const throw();
      private:
         shawn::Vec val_;
      };
      ///@name Constructor/Destructor
      ///@{
      PropertyConstantVecTask();
      virtual ~PropertyConstantVecTask();
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
