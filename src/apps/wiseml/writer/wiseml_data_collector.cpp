/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/wiseml_data_collector.h"
#ifdef ENABLE_WISEML
namespace wiseml
{
   WisemlDataCollector::
      WisemlDataCollector(shawn::SimulationController &sc, std::string id) 
      :  sc_(sc),
         id_(id)
   {}
   WisemlDataCollector::
      ~WisemlDataCollector()
   {}
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   double WisemlDataCollector::current_time()
   {
      return sc_.world().current_time();
   }
   // ----------------------------------------------------------------------
   std::list<std::string> WisemlDataCollector::make_list(std::string str)
      const
   {
      std::list<std::string> out;
      int next_seperator = str.find_first_of(',', 0);
      int last_seperator = 0;

      while(last_seperator != std::string::npos)
      {
         std::string cut = "";

         if(next_seperator != std::string::npos)
         {
            cut = str.substr(last_seperator, next_seperator-last_seperator);
         }
         else
         {
            cut = str.substr(last_seperator, str.length() - 1);
         }
         out.push_back(cut);
         last_seperator = next_seperator + 1;
         if(next_seperator == std::string::npos)
            break;
         next_seperator = str.find_first_of(',', last_seperator);
      }

      return out;
   }
   // ----------------------------------------------------------------------
}
#endif
