/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/tasks/wiseml_trace_task.h"
#ifdef ENABLE_WISEML
#include "apps/wiseml/writer/wiseml_data_keeper.h"
namespace wiseml
{
   WisemlTraceTask::WisemlTraceTask()
   {
   }
   // ----------------------------------------------------------------------
   WisemlTraceTask::~WisemlTraceTask()
   {
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   std::string WisemlTraceTask::name() const throw()
   {
      return "wiseml_trace";
   }
   // ----------------------------------------------------------------------
   std::string WisemlTraceTask::description() const throw()
   {
      return "Creates a new trace.";
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   void WisemlTraceTask::run(SimulationController &sc) throw()
   {
      WisemlDataKeeper *keeper = 
         sc.keeper_by_name_w<WisemlDataKeeper>("wiseml_data_keeper");
      if(keeper == NULL)
      {
         cout << "WisemlTraceTask:"
            << " Unable to create trace because WisemlDataKeeper was not"
            << " found. Make shure to run wiseml_setup before this task, or"
            << " to create a WisemlDataKeeper by code." << endl;
         return;
      }

      keeper->add_trace(sc.environment().required_string_param("id"));
   }
}
#endif
