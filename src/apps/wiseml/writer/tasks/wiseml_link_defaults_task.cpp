/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/tasks/wiseml_link_defaults_task.h"
#ifdef ENABLE_WISEML
#include "apps/wiseml/writer/wiseml_data_keeper.h"
namespace wiseml
{
   WisemlLinkDefaultsTask::WisemlLinkDefaultsTask()
   {
   }
   // ----------------------------------------------------------------------
   WisemlLinkDefaultsTask::~WisemlLinkDefaultsTask()
   {
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   std::string WisemlLinkDefaultsTask::name() const throw()
   {
      return "wiseml_link_defaults";
   }
   // ----------------------------------------------------------------------
   std::string WisemlLinkDefaultsTask::description() const throw()
   {
      return "Gathers data for the setup section of a WiseML file.";
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   void WisemlLinkDefaultsTask::run(SimulationController &sc) throw()
   {
      WisemlDataKeeper *keeper = 
         sc.keeper_by_name_w<WisemlDataKeeper>("wiseml_data_keeper");
      WisemlSetupCollector &setup = keeper->setup();

      LinkInfo linkdef;
      linkdef.is_encrypted = sc.environment().optional_bool_param(
         "is_encrypted", false);
      linkdef.is_virtual = sc.environment().optional_bool_param(
         "is_virtual", false);

      bool set_type=false, set_unit=false, 
         set_value=false;

      linkdef.rssi_datatype = sc.environment().optional_string_param(
         "rssi_datatype", "", &set_type);
      linkdef.rssi_unit = sc.environment().optional_string_param(
         "rssi_unit", "", &set_unit);
      linkdef.rssi = sc.environment().optional_string_param(
         "rssi_default", "0", &set_value);
      if(!(set_type && set_unit && set_value))
      {
         linkdef.rssi_datatype = "";
         linkdef.rssi_unit = "";
         linkdef.rssi = "";
      }

      setup.set_link_defaults(linkdef);
   }

   std::string 
      WisemlLinkDefaultsTask::
      replace_underline(std::string src)
   {
      std::string out = src;
      if(!src.empty())
      {
         int first_ul = out.find_first_of('_');
         while(first_ul != std::string::npos)
         {
            out = out.replace(first_ul, 1, " ");
            first_ul = out.find_first_of('_');
         }
      }
      return out;
   }
}
#endif
