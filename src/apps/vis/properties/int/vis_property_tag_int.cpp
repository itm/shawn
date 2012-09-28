/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/properties/int/vis_property_tag_int.h"
#include "apps/vis/elements/vis_drawable_node.h"
#include "sys/taggings/basic_tags.h"

using namespace shawn;

namespace vis
{

   PropertyTagIntTask::PropertyTagInt::
   PropertyTagInt( const shawn::Node* node, std::string tag  )
      : node_(node),
        tag_(tag)
   {}
   // ----------------------------------------------------------------------
   PropertyTagIntTask::PropertyTagInt::
   ~PropertyTagInt()
   {}
   // ----------------------------------------------------------------------
   int
   PropertyTagIntTask::PropertyTagInt::
   value( double,
          const PropertyStack<int>&,
          const Element& e) 
      const throw()
   {       
      return read_tag(e);
   }
   // ----------------------------------------------------------------------
   int 
   PropertyTagIntTask::PropertyTagInt::
   read_tag(const Element& e) const throw()
   {
      const DrawableNode* dn = dynamic_cast<const DrawableNode*>(&e);

      const shawn::IntegerTag *dtag = NULL;

      if(dn != NULL)
      {
         ConstTagHandle taghandle = dn->node().find_tag(tag_);
         if(taghandle != NULL)
         {
            dtag = dynamic_cast<const IntegerTag*>(taghandle.get());
         }
      }

      if(dtag!=NULL)
         return dtag->value();
      else
         return -1;
   }
   // ----------------------------------------------------------------------
   PropertyTagIntTask::
   PropertyTagIntTask()
   {}
   // ----------------------------------------------------------------------
   PropertyTagIntTask::
   ~PropertyTagIntTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   PropertyTagIntTask::
   name( void )
      const throw()
   {
      return "vis_tag_int";
   }
   // ----------------------------------------------------------------------
   std::string
   PropertyTagIntTask::
   description( void )
      const throw()
   {
      return "create a tag based int property";
   }
   // ----------------------------------------------------------------------
   ConstPropertyHandle
   PropertyTagIntTask::
   create_property( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      std::string tagname = sc.environment().required_string_param("dynamictag");

      PropertyTagInt* tag = new PropertyTagInt(NULL, tagname);
      try {
         fill_default_params(*tag,sc);
      }
      catch( std::runtime_error& ) {
         delete tag;
         throw;
      }
      return tag;
   }


}
#endif
