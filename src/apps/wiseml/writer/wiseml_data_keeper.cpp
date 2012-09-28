/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/wiseml_data_keeper.h"
#ifdef ENABLE_WISEML

namespace wiseml
{
   WisemlDataKeeper::WisemlDataKeeper(shawn::SimulationController &sc)
      :  HandleKeeper<WisemlDataCollector>("wiseml_data_keeper", "wiseml"),
         sc_(sc)
   {
      setup_ = new WisemlSetupCollector(sc);
      add(setup_);
   }
   WisemlDataKeeper::~WisemlDataKeeper()
   {
   }
   // ----------------------------------------------------------------------
   // Creation methods
   // ----------------------------------------------------------------------
   WisemlTraceCollector* WisemlDataKeeper::add_trace(std::string trace_id)
   {
      if(!has(trace_id))
      {
         WisemlTraceCollector *trace = new WisemlTraceCollector(
            sc_, trace_id);
         add(trace);
         return trace;
      }
      return dynamic_cast<WisemlTraceCollector*> (find_w(trace_id).get());
   }
   // ----------------------------------------------------------------------
   WisemlScenarioCollector*
      WisemlDataKeeper::add_scenario(std::string scenario_id)
   {
      if(!has(scenario_id))
      {
         WisemlScenarioCollector *scen = new WisemlScenarioCollector(
            sc_, scenario_id);
         add(scen);
         return scen;
      }
      return dynamic_cast<WisemlScenarioCollector*> (
         find_w(scenario_id).get());
   }
   // ----------------------------------------------------------------------
   // Access methods
   // ----------------------------------------------------------------------
   WisemlSetupCollector& WisemlDataKeeper::setup()
   {
      return *(dynamic_cast<WisemlSetupCollector*>(
         find_w("wiseml_setup").get()));
   }
   // ----------------------------------------------------------------------
   WisemlTraceCollector& WisemlDataKeeper::trace(std::string id)
      throw(std::runtime_error)
   {
      if(has(id))
      {
         WisemlTraceCollector* trace =
            dynamic_cast<WisemlTraceCollector*>(find_w(id).get());
         if(trace != NULL)
         {
            return *trace;
         }
         else
         {
            std::string msg = "ID ";
            msg += id + " is not a trace!";
            std::cout << msg << std::endl;
            throw(std::runtime_error(msg.c_str()));
         }
      }
      else
      {
         std::string msg = "ID ";
         msg += id + " is unkown!";
         std::cout << msg << std::endl;
         throw(std::runtime_error(msg.c_str()));
      }
   }
   // ----------------------------------------------------------------------
   WisemlScenarioCollector& WisemlDataKeeper::scenario(std::string id)
      throw(std::runtime_error)
   {
      if(has(id))
      {
         WisemlScenarioCollector* trace =
            dynamic_cast<WisemlScenarioCollector*>(find_w(id).get());
         if(trace != NULL)
         {
            return *trace;
         }
         else
         {
            std::string msg = "ID ";
            msg += id + " is not a scenario!";
            std::cout << msg << std::endl;
            throw(std::runtime_error(msg.c_str()));
         }
      }
      else
      {
         std::string msg = "ID ";
         msg += id + " is unkown!";
         std::cout << msg << std::endl;
         throw(std::runtime_error(msg.c_str()));
      }
   }
   // ----------------------------------------------------------------------
   std::list<WisemlTraceCollector*> WisemlDataKeeper::traces()
   {
      std::list<WisemlTraceCollector*> traceslist;

      for(std::map<std::string,RefcntPointer<WisemlDataCollector> >::iterator
         it = begin_handles_w();
         it != end_handles_w();
         ++it)
      {
         WisemlTraceCollector* trace =
            dynamic_cast<WisemlTraceCollector*>(it->second.get());
         if(trace!=NULL)
         {
            traceslist.push_back(trace);
         }
      }

      return traceslist;
   }
   // ----------------------------------------------------------------------
   std::list<WisemlScenarioCollector*> WisemlDataKeeper::scenarios()
   {
      std::list<WisemlScenarioCollector*> scenariolist;
      for(std::map<std::string,RefcntPointer<WisemlDataCollector> >::iterator
         it = begin_handles_w();
         it != end_handles_w();
         ++it)
      {
         WisemlScenarioCollector* scenario =
            dynamic_cast<WisemlScenarioCollector*>(it->second.get());
         if(scenario!=NULL)
         {
            scenariolist.push_back(scenario);
         }
      }

      return scenariolist;
   }
   // ----------------------------------------------------------------------
   double WisemlDataKeeper::next_timestamp_after(double time)
   {
      double next_ts = -1.0;
      for(std::map<std::string,RefcntPointer<WisemlDataCollector> >::iterator
         it = begin_handles_w();
         it != end_handles_w();
         ++it)
      {
         double tmp = it->second.get()->next_timestamp_after(time);
         if( tmp > 0.0 && (tmp < next_ts || next_ts < 0.0))
         {
            next_ts = tmp;
         }
      }

      return next_ts;
   }
   // ----------------------------------------------------------------------
   std::string WisemlDataKeeper::generate_xml()
   {
      std::stringstream wml;
      wml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
      wml << "<wiseml xmlns=\"http://wisebed.eu/ns/wiseml/1.0\" version=\"1.0\">" << std::endl;
      wml << setup_->generate_xml();

      std::list<WisemlScenarioCollector*> scens = scenarios();
      for(list<WisemlScenarioCollector*>::iterator sit =
         scens.begin(); sit != scens.end(); ++sit)
      {
         wml << (*sit)->generate_xml() << std::endl;
      }

      std::list<WisemlTraceCollector*> tracs = traces();
      for(list<WisemlTraceCollector*>::iterator tit =
         tracs.begin(); tit != tracs.end(); ++tit)
      {
         wml << (*tit)->generate_xml() << std::endl;
      }

      wml << "</wiseml>" << std::endl;

      return wml.str();
   }
}

#endif
