/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/properties/vec/vis_property_real_node_position.h"
#include "apps/vis/elements/vis_drawable_node.h"

using namespace shawn;

namespace vis
{

   PropertyRealNodePositionTask::PropertyRealNodePosition::
   PropertyRealNodePosition( const shawn::Node* v)
      : node_ ( v )
   {}
   // ----------------------------------------------------------------------
   PropertyRealNodePositionTask::PropertyRealNodePosition::
   ~PropertyRealNodePosition()
   {}
   // ----------------------------------------------------------------------
   shawn::Vec
   PropertyRealNodePositionTask::PropertyRealNodePosition::
   value( double t,
          const PropertyStack<shawn::Vec>&,
          const Element& e )
      const throw()
   { 
      if( node_ != NULL )
         return node_->real_position();

      const DrawableNode* dn = dynamic_cast<const DrawableNode*>(&e);
      return dn ? dn->node().real_position() : Vec(0,0,0);
   }
   // ----------------------------------------------------------------------
   PropertyRealNodePositionTask::
   PropertyRealNodePositionTask()
   {}
   // ----------------------------------------------------------------------
   PropertyRealNodePositionTask::
   ~PropertyRealNodePositionTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   PropertyRealNodePositionTask::
   name( void )
      const throw()
   {
      return "vis_real_node_position";
   }
   // ----------------------------------------------------------------------
   std::string
   PropertyRealNodePositionTask::
   description( void )
      const throw()
   {
      return "create a property that returns nodes' real positions. Note 1: Only works for node elements. Note 2: Returns erraneous values if visualization time and simulation time are different. If $node is specified, ...";
   }
   // ----------------------------------------------------------------------
   ConstPropertyHandle
   PropertyRealNodePositionTask::
   create_property( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      if( !sc.environment().optional_string_param("node","").empty() )
         { ABORT_NOT_IMPLEMENTED; }

      PropertyRealNodePosition* rnp = new PropertyRealNodePosition(NULL);
      try {
         fill_default_params(*rnp,sc);
      }
      catch( std::runtime_error& ) {
         delete rnp;
         throw;
      }
      return rnp;
   }
   // ----------------------------------------------------------------------
   bool
   PropertyRealNodePositionTask::
   keep_property( shawn::SimulationController& sc )
      throw( std::runtime_error )
   { return false; }


}
#endif
