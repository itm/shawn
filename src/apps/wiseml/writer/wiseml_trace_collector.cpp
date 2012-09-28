/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/wiseml_trace_collector.h"
#ifdef ENABLE_WISEML
namespace wiseml
{
   WisemlTraceCollector::
      WisemlTraceCollector(shawn::SimulationController &sc,
      std::string id) 
      :  WisemlDataCollector(sc, id)
   {}
   // ----------------------------------------------------------------------
   WisemlTraceCollector::~WisemlTraceCollector(){}
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   std::string WisemlTraceCollector::name()
      const throw()
   {
      return id_;
   }
   // ----------------------------------------------------------------------
   std::string WisemlTraceCollector::description()
      const throw()
   {
      return "Trace section data";
   }
   // ----------------------------------------------------------------------
   double WisemlTraceCollector::next_timestamp_after(double time)
   {
      double next = -1.0;
      if(items_.upper_bound(time) != items_.end())
         next = items_.upper_bound(time)->first;
      return next;
   }
   // ----------------------------------------------------------------------
   void WisemlTraceCollector::node_movement(std::string node)
   {
      double timestamp = current_time();
      shawn::Vec pos = 
         sc_.world().find_node_by_label(node)->real_position();
      std::stringstream item;
      item << "np," << node << "," << pos.x() << 
         "," << pos.y() << "," << pos.z();
      items_.insert(pair<double, string>(timestamp, item.str()));
   }
   // ----------------------------------------------------------------------
   void WisemlTraceCollector::capability_value(std::string node, 
         std::string capability,std::string value)
   {
      double timestamp = current_time();
      std::stringstream item;
      item << "nc," << node << "," << capability << "," << value;
      items_.insert(pair<double, string>(timestamp, item.str()));
   }
   // ----------------------------------------------------------------------
   void WisemlTraceCollector::rssi(std::string src, 
         std::string dst,std::string value)
   {
      double timestamp = current_time();
      std::stringstream item;
      item << "lrssi," << src << "," << dst << "," << value;
      items_.insert(pair<double, string>(timestamp, item.str()));
   }
   // ----------------------------------------------------------------------
   std::string WisemlTraceCollector::generate_xml() const
   {
      std::stringstream wml;
      wml << "\t" << "<trace id=\"" << id_ << "\">" << std::endl;
      double next_timestamp = items_.lower_bound(0.0)->first;
      while(next_timestamp >= 0.0)
      {
         wml << "\t\t" << "<timestamp>" << next_timestamp << 
            "</timestamp>" << std::endl;

         wml << generate_timestamp_xml(next_timestamp);

         if(items_.upper_bound(next_timestamp)!= items_.end())
            next_timestamp = items_.upper_bound(next_timestamp)->first;
         else
            next_timestamp = -1.0;
      }
      wml << "\t</trace>" << std::endl;

      return wml.str();
   }
   // ----------------------------------------------------------------------
   std::string 
      WisemlTraceCollector::generate_timestamp_xml(double timestamp) 
      const
   {
      std::stringstream wml;
      std::map<string, NodeTemplate*> nodes;
      std::map<string, pair<LinkInfo*, bool> > links;
      std::multimap<double, std::string>::const_iterator ts_begin = 
         items_.lower_bound(timestamp);
      std::multimap<double, std::string>::const_iterator ts_end = 
         items_.upper_bound(timestamp);

      for(std::multimap<double, std::string>::const_iterator it = ts_begin;
         it != ts_end; ++it)
      {
         std::list<std::string> data(make_list(it->second));
         list<string>::iterator dit = data.begin();
         std::string datatype = dit->c_str();

         /// Item syntax: np;node_label;x_position;y_position;z_position
         if(datatype == "np")
         {
            ++dit;
            std::string label = dit->c_str();

            NodeTemplate *tmp;
            if(nodes.find(label) != nodes.end())
            {
               tmp = nodes.find(label)->second;
            }
            else
            {
               tmp = new NodeTemplate();
               nodes.insert(pair<string, NodeTemplate*>(label, tmp));
            }
            tmp->label = label;
            ++dit;
            tmp->posx = atof(dit->c_str());
            ++dit;
            tmp->posy= atof(dit->c_str());
            ++dit;
            tmp->posz = atof(dit->c_str());
            Capability dummy;
            dummy.name = "positioned";
            tmp->capabilities.push_front(dummy);
         }
         /// Item syntax: nc;node_label;capability_name;capability_value
         else if(datatype == "nc")
         {
            ++dit;
            std::string label = dit->c_str();

            NodeTemplate *tmp;
            if(nodes.find(label) != nodes.end())
            {
               tmp = nodes.find(label)->second;
            }
            else
            {
               tmp = new NodeTemplate();
               nodes.insert(pair<string, NodeTemplate*>(label, tmp));
            }
            tmp->label = label;
            Capability cap;
            ++dit;
            cap.name = dit->c_str();
            ++dit;
            cap.def_value = dit->c_str();
            tmp->capabilities.push_back(cap);
         }
         /// Item syntax: lrssi;source_node;target_node;rssi_value
         else if(datatype == "lrssi")
         {
            ++dit;
            std::string src = dit->c_str();
            ++dit;
            std::string dst = dit->c_str();
            ++dit;
            std::string value = dit->c_str();
            LinkInfo *info;
            if(links.find(src + dst) != links.end())
            {
               info = links.find(src + dst)->second.first;
               links.find(src + dst)->second.second = true;
            }
            else
            {
               info = new LinkInfo();
               links.insert(pair<string, pair<LinkInfo*, bool> >(src+dst,
                  pair<LinkInfo*, bool>(info, true)));
            }
            info->source = src;
            info->target = dst;
            info->rssi = value;

         }
         /// Item syntax: 
         /// lc;source_node;target_node;capability_name;capability_value
         else if(datatype == "lc")
         {
            ++dit;
            std::string src = dit->c_str();
            ++dit;
            std::string dst = dit->c_str();
            LinkInfo *info;
            if(links.find(src + dst) != links.end())
            {
               info = links.find(src + dst)->second.first;
            }
            else
            {
               info = new LinkInfo();
               links.insert(pair<string, pair<LinkInfo*, bool> >(src+dst,
                  pair<LinkInfo*, bool>(info, false)));
            }
            info->source = src;
            info->target = dst;
            Capability cap;
            ++dit;
            cap.name = dit->c_str();;
            ++dit;
            cap.def_value = dit->c_str();
            info->capabilities.push_back(cap);
         }

         
      }

      for(map<string, NodeTemplate*>::iterator it = nodes.begin();
         it != nodes.end(); ++it)
      {
         wml << "\t\t<node id=\"" << it->second->label << "\">" << std::endl;
         if(it->second->capabilities.front().name == "positioned")
         {
            wml << "\t\t\t<position>" << std::endl;
            wml << "\t\t\t\t<x>" << it->second->posx << "</x>" << std::endl;
            wml << "\t\t\t\t<y>" << it->second->posy << "</y>" << std::endl;
            wml << "\t\t\t\t<z>" << it->second->posz << "</z>" << std::endl;
            wml << "\t\t\t</position>" << std::endl;
         }

         for(CapList::iterator cit = it->second->capabilities.begin();
            cit != it->second->capabilities.end(); ++cit)
         {
            if(cit->name != "positioned")
               wml << "\t\t\t<data key=\"" << cit->name << "\">" << 
                  cit->def_value << "</data>" << std::endl;
         }
         wml << "\t\t</node>" << std::endl;
      }

      for(map<string, pair<LinkInfo*,bool> >::iterator it = links.begin();
         it != links.end(); ++it)
      {
         wml << "\t\t<link source=\"" << it->second.first->source << "\" "
            << "target=\"" << it->second.first->target << "\">"
            << std::endl;
         if(it->second.second)
         {
            wml << "\t\t\t<rssi>" << it->second.first->rssi << "</rssi>"
               << std::endl;
         }

         for(CapList::iterator cit = it->second.first->capabilities.begin();
            cit != it->second.first->capabilities.end(); ++cit)
         {
            wml << "\t\t\t<data key=\"" << cit->name << "\">" << 
               cit->def_value << "</data>" << std::endl;
         }
         wml << "\t\t</link>" << std::endl;
      }
      
      return wml.str();
   }
   // ----------------------------------------------------------------------
   void WisemlTraceCollector::clear()
   {
      items_.clear();
   }
}
#endif
