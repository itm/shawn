/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/properties/vec/vis_property_indexed_tag_color_vec.h"
#include "apps/vis/elements/vis_drawable_node.h"
#include "sys/taggings/basic_tags.h"

using namespace shawn;

namespace vis
{

   PropertyIndexedTagColorVecTask::PropertyIndexedTagColorVec::
      PropertyIndexedTagColorVec( const shawn::Node* n, std::string dynamictag )
      :  node_(n),
         tag_(dynamictag)
   {}
   // ----------------------------------------------------------------------
   PropertyIndexedTagColorVecTask::PropertyIndexedTagColorVec::
   ~PropertyIndexedTagColorVec()
   {}
   // ----------------------------------------------------------------------
   shawn::Vec
   PropertyIndexedTagColorVecTask::PropertyIndexedTagColorVec::
   value( double,
          const PropertyStack<shawn::Vec>&,
          const Element& e) 
      const throw()
   { 
      shawn::Vec out;

      //Looking for the provided (double) tag:
      int tagval = read_tag(e);
      
      switch(tagval)
	  {
		  case 0:
			  out = shawn::Vec(0,0,0);
			  break;
		  case 1:
			  out = shawn::Vec(0.5,0.5,0.5);
			  break;
		  case 2:
			  out = shawn::Vec(0.5,0,0);
			  break;
		  case 3:
			  out = shawn::Vec(0.5,0.5,0.0);
			  break;
		  case 4:
			  out = shawn::Vec(1.0,1.0,1.0);
			  break;
		  case 5:
			  out = shawn::Vec(0.75,0.75,0.75);
			  break;
		  case 6:
			  out = shawn::Vec(1.0,0.0,0.0);
			  break;
		  case 7:
			  out = shawn::Vec(1.0,1.0,0.0);
			  break;
		  case 8:
			  out = shawn::Vec(0.0,0.5,0.0);
			  break;
		  case 9:
			  out = shawn::Vec(0.0,0.5,0.5);
			  break;
		  case 10:
			  out = shawn::Vec(0.0,0.0,0.5);
			  break;
		  case 11:
			  out = shawn::Vec(0.5,0.0,0.5);
			  break;
		  case 12:
			  out = shawn::Vec(0.0,1.0,0.0);
			  break;
		  case 13:
			  out = shawn::Vec(0.0,1.0,1.0);
			  break;
		  case 14:
			  out = shawn::Vec(0.0,0.0,1.0);
			  break;
		  case 15:
			  out = shawn::Vec(1.0,0.0,1.0);
			  break;
		  default:
			  out = shawn::Vec(0.33,0.33,0.33);
			  break;
	  }
	  
      return out;
   }
   // ----------------------------------------------------------------------
   int PropertyIndexedTagColorVecTask::PropertyIndexedTagColorVec::
      read_tag(const Element& e) const throw()
   {
      const DrawableNode* dn = dynamic_cast<const DrawableNode*>(&e);

      const shawn::IntegerTag *itag = NULL;

      if(dn != NULL)
      {
         ConstTagHandle taghandle = dn->node().find_tag(tag_);
         if(taghandle != NULL)
         {
            itag = dynamic_cast<const IntegerTag*>(taghandle.get());
         }
      }

      if(itag!=NULL)
         return itag->value();
      else
         return -1;
   }
   // ----------------------------------------------------------------------
   PropertyIndexedTagColorVecTask::
   PropertyIndexedTagColorVecTask()
   {}
   // ----------------------------------------------------------------------
   PropertyIndexedTagColorVecTask::
   ~PropertyIndexedTagColorVecTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   PropertyIndexedTagColorVecTask::
   name( void )
      const throw()
   {
      return "vis_indexed_tag_color_vec";
   }
   // ----------------------------------------------------------------------
   std::string
   PropertyIndexedTagColorVecTask::
   description( void )
      const throw()
   {
      return "create a tag-dependend vector";
   }
   // ----------------------------------------------------------------------
   ConstPropertyHandle
   PropertyIndexedTagColorVecTask::
   create_property( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      //if( !sc.environment().optional_string_param("node","").empty() )
      //   { ABORT_NOT_IMPLEMENTED; }
      
      std::string tagname = sc.environment().required_string_param("dynamictag");

      PropertyIndexedTagColorVec* rtc = new PropertyIndexedTagColorVec(NULL, tagname);
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
