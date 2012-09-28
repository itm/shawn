/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "apps/wiseml/templates.h"
#ifdef ENABLE_WISEML

namespace wiseml
{
   NodeTemplate::
   NodeTemplate()
   {
      label="";
      posx = 0.0;
      posy=0.0;
      posz=0.0;
      gateway = false;
      image = "";
      nodetype = "";
      description = "";
      capabilities = CapList();
   }

   NodeTemplate::
   NodeTemplate(const NodeTemplate &cpy)
   {
      label = cpy.label;
      posx = cpy.posx;
      posy = cpy.posy;
      posz = cpy.posz;
      gateway = cpy.gateway;
      image = cpy.image;
      nodetype = cpy.nodetype;
      description = cpy.description;
      capabilities = CapList(cpy.capabilities);
   }

   NodeTemplate::
   NodeTemplate(shawn::Node &src)
   {

      label = src.label();
      shawn::Vec pos = src.real_position();
      posx = pos.x();
      posy = pos.y();
      posz = pos.z();

      // TODO: Reload informations from node:
      gateway = false;
      image = "";
      nodetype = "";
      description = "";
      capabilities = CapList();
   }
   // ----------------------------------------------------------------------
   std::list<std::string> NodeTemplate::make_list(std::string str)
   {
      std::list<std::string> out;
      int next_seperator = str.find_first_of(',', 0);
      int last_seperator = 0;

      while(last_seperator != std::string::npos)
      {
         std::string cut = "";

         if(next_seperator != std::string::npos)
         {
            cut = str.substr(last_seperator+1, next_seperator - 1);
         }
         else
         {
            cut = str.substr(last_seperator+1, str.length() - 1);
         }
         out.push_back(cut);
         last_seperator = next_seperator;
         next_seperator = str.find_first_of(',', last_seperator + 1);
      }

      return out;
   }

}

#endif
