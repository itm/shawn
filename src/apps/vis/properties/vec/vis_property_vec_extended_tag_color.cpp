/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/properties/vec/vis_property_vec_extended_tag_color.h"
#include "apps/vis/elements/vis_drawable_node.h"
#include "sys/taggings/basic_tags.h"

using namespace shawn;

namespace vis
{

   PropertyExtendedTagColorVecTask::PropertyExtendedTagColorVec::
      PropertyExtendedTagColorVec( const shawn::Node* n, std::string dynamictag, double tag_max )
      :  node_(n),
         tag_(dynamictag),
         tag_max_(tag_max)
   {}
   // ----------------------------------------------------------------------
   PropertyExtendedTagColorVecTask::PropertyExtendedTagColorVec::
   ~PropertyExtendedTagColorVec()
   {}
   // ----------------------------------------------------------------------
   shawn::Vec
   PropertyExtendedTagColorVecTask::PropertyExtendedTagColorVec::
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
	// Tag available, compute color
	
	 double r=0.0, g=0.0, b=0.0;
	 if(tagval>tag_max_)
	 {
	    tagval = tag_max_;
	 }
	 
	 double percent = tagval / tag_max_;
	 
	 if(percent <= 0.2)
	 {
	    // Black to Red
	    r = percent / 0.2;
	 }
	 else if(percent > 0.2 && percent <= 0.4)
	 {
	    // Red to Yellow
	    r = 1.0;
	    g = (percent - 0.2) / 0.2;
	 }
	 else if(percent > 0.4 && percent <= 0.6)
	 {
	    // Yellow to Green
	    r = (percent - 0.4) / 0.2;
	    g = 1.0;
	 }
	 else if(percent > 0.6 && percent <= 0.8)
	 {
	    // Green to Cyan
	    g = 1.0;
	    b = (percent - 0.6) / 0.2;
	 }
	 else if(percent > 0.8 && percent <= 1.0)
	 {
	    // Cyan to Blue
	    g = (percent - 0.8) / 0.2;
	    b = 1.0;
	 }
	 
         
         out = shawn::Vec(r, g, b);
      }
      return out;
   }
   // ----------------------------------------------------------------------
   double PropertyExtendedTagColorVecTask::PropertyExtendedTagColorVec::
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
   PropertyExtendedTagColorVecTask::
   PropertyExtendedTagColorVecTask()
   {}
   // ----------------------------------------------------------------------
   PropertyExtendedTagColorVecTask::
   ~PropertyExtendedTagColorVecTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   PropertyExtendedTagColorVecTask::
   name( void )
      const throw()
   {
      return "vis_extended_tag_color_vec";
   }
   // ----------------------------------------------------------------------
   std::string
   PropertyExtendedTagColorVecTask::
   description( void )
      const throw()
   {
      return "Creates a tag-dependend color-vector.";
   }
   // ----------------------------------------------------------------------
   ConstPropertyHandle
   PropertyExtendedTagColorVecTask::
   create_property( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      //if( !sc.environment().optional_string_param("node","").empty() )
      //   { ABORT_NOT_IMPLEMENTED; }
      
      std::string tagname = sc.environment().required_string_param("dynamictag");
      double tagmax = sc.environment().required_double_param("max_tag_value");

      PropertyExtendedTagColorVec* rtc = new PropertyExtendedTagColorVec(NULL, tagname, tagmax);
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
