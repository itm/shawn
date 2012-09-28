/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/properties/vec/vis_property_tag_color_vec.h"
#include "apps/vis/elements/vis_drawable_node.h"
#include "sys/taggings/basic_tags.h"

using namespace shawn;

namespace vis
{

   PropertyTagColorVecTask::PropertyTagColorVec::
      PropertyTagColorVec( const shawn::Node* n, std::string dynamictag )
      :  node_(n),
         tag_(dynamictag)
   {}
   // ----------------------------------------------------------------------
   PropertyTagColorVecTask::PropertyTagColorVec::
   ~PropertyTagColorVec()
   {}
   // ----------------------------------------------------------------------
   shawn::Vec
   PropertyTagColorVecTask::PropertyTagColorVec::
   value( double,
          const PropertyStack<shawn::Vec>&,
          const Element& e) 
      const throw()
   { 
      shawn::Vec out;

      //Looking for the provided (double) tag:
      double tagval = read_tag(e);
      
      if(tagval < 0.0)
      {
         // Tag not available or not compatible, so the node gets a gray color:
         out = shawn::Vec(0.5, 0.5, 0.5);
      }
      else
      {
         // Tag available, red for a tag-value of 0.0 and blue for a value of 1.0:
         out = shawn::Vec(1.0-tagval, 0.0, tagval);
      }
      return out;
   }
   // ----------------------------------------------------------------------
   double PropertyTagColorVecTask::PropertyTagColorVec::
      read_tag(const Element& e) const throw()
   {
      const DrawableNode* dn = dynamic_cast<const DrawableNode*>(&e);

      const shawn::DoubleTag *dtag = NULL;

      if(dn != NULL)
      {
         ConstTagHandle taghandle = dn->node().find_tag(tag_);
         if(taghandle != NULL)
         {
            dtag = dynamic_cast<const DoubleTag*>(taghandle.get());
         }
      }

      if(dtag!=NULL)
         return dtag->value();
      else
         return -1.0;
   }
   // ----------------------------------------------------------------------
   PropertyTagColorVecTask::
   PropertyTagColorVecTask()
   {}
   // ----------------------------------------------------------------------
   PropertyTagColorVecTask::
   ~PropertyTagColorVecTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   PropertyTagColorVecTask::
   name( void )
      const throw()
   {
      return "vis_tag_color_vec";
   }
   // ----------------------------------------------------------------------
   std::string
   PropertyTagColorVecTask::
   description( void )
      const throw()
   {
      return "create a tag-dependend vector";
   }
   // ----------------------------------------------------------------------
   ConstPropertyHandle
   PropertyTagColorVecTask::
   create_property( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      //if( !sc.environment().optional_string_param("node","").empty() )
      //   { ABORT_NOT_IMPLEMENTED; }
      
      std::string tagname = sc.environment().required_string_param("dynamictag");

      PropertyTagColorVec* rtc = new PropertyTagColorVec(NULL, tagname);
      try {
         fill_default_params(*rtc,sc);
      }
      catch( std::runtime_error& ) {
         delete rtc;
         throw;
      }
      return rtc;
   }


}
#endif
