/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_PROPERTY_REAL_NODE_POSITION_H
#define __SHAWN_TUBSAPPS_VIS_PROPERTY_REAL_NODE_POSITION_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_property_task.h"
#include "apps/vis/properties/vis_property.h"
#include "apps/vis/properties/vis_property_stack.h"

namespace shawn
{ class Node; }

namespace vis
{

   /** \brief Task for creating real node position properties.
    * This task is used to create properties representing the real position of
    * nodes.
    */
   class PropertyRealNodePositionTask
      : public PropertyTask
   {
   public:
      /** \brief A real node position property
       * This class is used to define properties representing a real (current)
       * position of a network node.
       */
      class PropertyRealNodePosition
         : public Property<shawn::Vec>
      {
      public:
         ///@name Constructor/Destructor
         ///@{
         PropertyRealNodePosition( const shawn::Node* );
         virtual ~PropertyRealNodePosition();
         ///@}
         /**
          * Returns the value which is kept by this property.
          * \return Property value
          */
         virtual shawn::Vec value( double, const PropertyStack<shawn::Vec>&, const Element& ) const throw();
      private:
         const shawn::Node* node_;
      };
      ///@name Constructor/Destructor
      ///@{
      PropertyRealNodePositionTask();
      virtual ~PropertyRealNodePositionTask();

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

      virtual bool keep_property( shawn::SimulationController& sc ) 
         throw( std::runtime_error );
      /**
       * Creates a new constant double property
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
