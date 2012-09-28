/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/timestamps/wiseml_timestamps.h"
#ifdef ENABLE_WISEML
#include "sys/taggings/node_reference_tag.h"
#include "sys/taggings/group_tag.h"
#include "sys/taggings/basic_tags.h"
using namespace shawn;

namespace wiseml
{
   WiseMlTimestamps::WiseMlTimestamps(string filename, string scenario_id,
      string trace_id, shawn::World &w)
      :time_(0.0),
       next_timestamp_(0.0),
       timefactor_(1.0),
       scenario_finished_(false),
       trace_finished_(false),
       scenario_id_(scenario_id),
       trace_id_(trace_id),
       state_(TSL_NONE),
       world_(w),
       elem_count_(0)
   {
      set_document_uri(filename);
      register_updater(w.simulation_controller_w());
   }
   // *********************************************************************
   void WiseMlTimestamps::set_timefactor(double t)
   {
      timefactor_ = t;
   }
   // *********************************************************************
   WiseMlTimestamps::~WiseMlTimestamps()
   {}   
   // *********************************************************************
   void WiseMlTimestamps::timeout(shawn::EventScheduler &scheduler,
      shawn::EventScheduler::EventHandle handle, double time,
      shawn::EventScheduler::EventTagHandle &taghandle)
      throw()
   {
      time_ = time / timefactor_;
      next_timestamp_ = -1.0;

      // parse trace of current timestamp
      if(trace_id_ != "" && !trace_finished_)
      {
         state_ = TSL_TID;
         set_skip_target("trace");
         set_skip_target_add_attr("id", trace_id_);
         parse();
      }

      // parse scenario of current timestamp
      if(scenario_id_ != "" &&!scenario_finished_)
      {
         state_ = TSL_SID;
         set_skip_target("scenario");
         set_skip_target_add_attr("id", scenario_id_);
         parse();
      }

      cout << "Updating node positions" << endl;
      update_node_positions();

      cout << "Next timestamp at " << next_timestamp_ << endl;

      nodes_.clear();
      // move event to next timestamp available
      if(next_timestamp_ >= 0.0)
         scheduler.move_event(handle, next_timestamp_*timefactor_);
   }

   // *********************************************************************
   void WiseMlTimestamps::
   skip_target_reached(std::string name, AttList& atts)
   {
      if(state_ == TSL_TID)
      {
         //set_skip_target("timestamp");
         state_ = TSL_TTIMESTAMP;

         cout << "Found trace " << attribute("id", atts) << endl;
      }
      else if(state_ == TSL_SID)
      {
         //set_skip_target("timestamp");
         state_ = TSL_STIMESTAMP;

         cout << "Found scenario " << attribute("id", atts) << endl;
      }
   }

   // *********************************************************************
   void WiseMlTimestamps::
   handle_start_element(std::string name, AttList& atts)
         throw(std::runtime_error)
   {
      SAXSimpleSkipReader::handle_start_element(name, atts);
      if(skipping() || state_ == TSL_INTERRUPT)
      {
          return;
      }

      WmlElement el;
      el.name = name;
      el.atts = AttList(atts);
      elements_.push_front(el);
      elem_count_++;

      if(state_ == TSL_TRACE)
      {
         handle_trace_start(name, atts);
      }
      if(state_ == TSL_SCENARIO)
      {
         handle_scenario_start(name, atts);
      }
   }
   // *********************************************************************
   void WiseMlTimestamps::
   handle_end_element(std::string name)
            throw(std::runtime_error)
   {
      SAXSimpleSkipReader::handle_end_element(name);
      if(state_ == TSL_INTERRUPT)
      {
         return;
      }

      if(skipping())
      {
         if((state_ == TSL_TTIMESTAMP && name=="trace")
            || (state_ == TSL_STIMESTAMP && name=="scenario"))
         {
            state_ = TSL_INTERRUPT;
            interrupt();
         }
      }

      if(!skipping())
      {
         if((state_ == TSL_TTIMESTAMP || state_ == TSL_STIMESTAMP)
            && name=="timestamp")
         {
            set_skip_target("timestamp");
         }
         // timestamp not found
         if(state_ == TSL_TTIMESTAMP && name=="trace")
         {
            cout << "No more timestamps in trace" << endl;
            trace_finished_ = true;
            state_ = TSL_INTERRUPT;
            interrupt();
         }
         // trace end found
         else if(state_ == TSL_TRACE && name=="trace")
         {
            cout << "Timestamp finished. No more timestamps in trace" << endl;
            trace_finished_ = true;
            state_ = TSL_INTERRUPT;
            interrupt();
         }
         else if(state_ == TSL_STIMESTAMP && name=="scenario")
         {
            cout << "No more timestamps in scenario" << endl;
            scenario_finished_ = true;
            state_ = TSL_INTERRUPT;
            interrupt();
         }
         // scenario end found
         else if(state_ == TSL_SCENARIO && name=="scenario")
         {
            cout << "Timestamp finished. No more timestamps in scenario" << endl;
            scenario_finished_ = true;
            state_ = TSL_INTERRUPT;
            interrupt();
         }

         if(elem_count_<=0)
         {
            state_ = TSL_INTERRUPT;
            interrupt();
         }
         else if(elements_.front().name == name)
         {
            elements_.pop_front();
            elem_count_--;
         }
         else
         {
            cout << "HandleEndElement: Element misfit" << endl;
         }
      }


   }
   // *********************************************************************
   void WiseMlTimestamps::
   handle_text_element(std::string name)
            throw(std::runtime_error)
   {
      SAXSkipReader::handle_text_element(name);
      if(skipping() || state_ == TSL_INTERRUPT)
      {
          return;
      }

      switch (state_)
      {
      case TSL_TTIMESTAMP:
         handle_ttimestamp_text(name);
         break;
      case TSL_TRACE:
         handle_trace_text(name);
         break;
      case TSL_STIMESTAMP:
         handle_stimestamp_text(name);
         break;
      case TSL_SCENARIO:
         handle_scenario_text(name);
         break;
      }
   }
   // *********************************************************************
   void WiseMlTimestamps::handle_ttimestamp_text(string content)
   {
      WmlList::const_iterator it = elements_.begin();
      if(elem_count_ < 1)
         return;
      WmlElement parent1 = *(it);

      if(parent1.name == "timestamp")
      {
         double tstamp = atof(content.c_str());
         if(tstamp > time_)
         {
            next_timestamp_ = tstamp;
            state_ = TSL_INTERRUPT;
            interrupt();
         }
         else if(tstamp == time_)
         {
            state_ = TSL_TRACE;
            cout << "Timestamp " << time_ << " found, reading..." << endl;
         }
         else
         {
            //set_skip_target("timestamp");
            //cout << "Trace: Skipping timestamp " << tstamp << endl;
         }
      }
   }
   // *********************************************************************
   void WiseMlTimestamps::handle_trace_text(string content)
   {
      WmlList::const_iterator it = elements_.begin();
      if(elem_count_ < 1)
         return;
      WmlElement parent1 = *(it);

      if(parent1.name == "timestamp")
      {
         double tstamp = atof(content.c_str());
         next_timestamp_ = tstamp;
         cout << "Next Timestamp at " << tstamp << " reached" << endl;
         state_ = TSL_INTERRUPT;
         interrupt();
      }
      else
      {
         if(elem_count_ < 3)
            return;
         WmlElement parent2 = *(++it);
         WmlElement parent3 = *(++it);
         // Node positions (mobility):
         if(parent1.name == "x")
         {
            nodes_[attribute("id", parent3.atts)]->posx = atof(content.c_str());
         }
         else if(parent1.name == "y")
         {
            nodes_[attribute("id", parent3.atts)]->posy = atof(content.c_str());
         }
         else if(parent1.name == "z")
         {
            nodes_[attribute("id", parent3.atts)]->posz = atof(content.c_str());
         }
         // Sensor data (capabilities):
         else if(parent2.name == "node" && parent1.name == "data")
         {
            // sensor_id = <node id>:<capability key>
            string sensor_id = attribute("id", parent2.atts);
            sensor_id += ":" + attribute("key", parent1.atts);
            update_sensor(sensor_id, content);
         }

      }
   }
   // *********************************************************************
   void WiseMlTimestamps::
   handle_trace_start(std::string name, AttList& atts)
   {
      // Change nodes [readings and/or] position
      if(name == "node")
      {
         //cout << "Trace: Node " << attribute("id",atts) << " wird ge�ndert" << endl;
         Node *tmp = world_.find_node_by_label_w(attribute("id",atts));
         NodeTemplate *tmpl = new NodeTemplate(*tmp);
         nodes_.insert(pair<string,NodeTemplate *>(tmpl->label,tmpl));
      }
   }
   // *********************************************************************
   void WiseMlTimestamps::handle_stimestamp_text(string content)
   {
      WmlList::const_iterator it = elements_.begin();
      if(elem_count_ < 1)
         return;
      WmlElement parent1 = *(it);

      if(parent1.name == "timestamp")
      {
         double tstamp = atof(content.c_str());
         if(tstamp > time_)
         {
            if(tstamp < next_timestamp_)
               next_timestamp_ = tstamp;
            state_ = TSL_INTERRUPT;
            interrupt();
         }
         else if(tstamp == time_)
         {
            state_ = TSL_SCENARIO;
            cout << "Scenario: Timestamp " << time_ << " found. Reading..." << endl;
         }
         else
         {
            //set_skip_target("timestamp");
            //cout << "Scenario: Skipping timestamp " << tstamp << endl;
         }
      }
   }
   // *********************************************************************
   void WiseMlTimestamps::handle_scenario_text(string content)
   {
      WmlList::const_iterator it = elements_.begin();
      if(elem_count_ < 1)
         return;
      WmlElement parent1 = *(it);

      if(parent1.name == "timestamp")
      {
         double tstamp = atof(content.c_str());
         next_timestamp_ = tstamp;
         cout << "Scenario: Next timestamp at " << tstamp << " reached" << endl;
         state_ = TSL_INTERRUPT;
         interrupt();
      }
      else
      {
         if(elem_count_ < 3)
            return;
         WmlElement parent2 = *(++it);
         WmlElement parent3 = *(++it);
         if(parent1.name == "x")
         {
            nodes_[attribute("id", parent3.atts)]->posx = atof(content.c_str());
         }
         else if(parent1.name == "y")
         {
            nodes_[attribute("id", parent3.atts)]->posy = atof(content.c_str());
         }
         else if(parent1.name == "z")
         {
            nodes_[attribute("id", parent3.atts)]->posz = atof(content.c_str());
         }
         // Sensor data (capabilities):
         else if(parent2.name == "node" && parent1.name == "data")
         {
            // sensor_id = <node id>:<capability key>
            string sensor_id = attribute("id", parent2.atts);
            sensor_id += ":" + attribute("key", parent1.atts);
            update_sensor(sensor_id, content);
         }
      }
   }
   // *********************************************************************
   void WiseMlTimestamps::
   handle_scenario_start(std::string name, AttList& atts)
   {
      // Change nodes [readings and/or] position
      if(name == "node")
      {
         if(!(nodes_.count(attribute("id",atts))>0))
         {
            Node *tmp = world_.find_node_by_label_w(attribute("id",atts));
            NodeTemplate *tmpl = new NodeTemplate(*tmp);
            nodes_.insert(pair<string,NodeTemplate *>(tmpl->label,tmpl));
         }
         else
         {
         }
      }
      // Enabling and disabling of links
      else if(name=="enablelink")
      {
         //TODO: Check if links actually existed before
         string source_node_id = attribute("source", atts);
         string target_node_id = attribute("target", atts);

         Node* source_node = world_.find_node_by_label_w(source_node_id);
         Node* target_node = world_.find_node_by_label_w(target_node_id);

         GroupTag *links = dynamic_cast<GroupTag*>(
            source_node->find_tag_w("WISEML_LINKS").get());
         NodeReferenceTag *n_tag =
            new NodeReferenceTag(target_node_id, target_node);
         links->add_tag(n_tag);
      }
      else if(name=="disablelink")
      {
         string source_node_id = attribute("source", atts);
         string target_node_id = attribute("target", atts);

         Node* source_node = world_.find_node_by_label_w(source_node_id);
         GroupTag *links = dynamic_cast<GroupTag*>(
            source_node->find_tag_w("WISEML_LINKS").get());
         links->remove_tag_by_name(target_node_id);
      }
      // Enabling and disabling of nodes
      else if(name=="enablenode")
      {
         string node_id = attribute("id", atts);

         Node* node = world_.find_node_by_label_w(node_id);

         BoolTag *enabled = dynamic_cast<BoolTag*>(
            node->find_tag_w("WISEML_ENABLED").get());
         enabled->set_value(true);
      }
      else if(name=="disablenode")
      {
         string node_id = attribute("id", atts);

         Node* node = world_.find_node_by_label_w(node_id);

         BoolTag *enabled = dynamic_cast<BoolTag*>(
            node->find_tag_w("WISEML_ENABLED").get());
         enabled->set_value(false);
      }
   }
   // *********************************************************************
   void WiseMlTimestamps::
   update_node_positions()
   {
      cout << "Update " << nodes_.size() << " node positions" << endl;
      for(map<string, NodeTemplate*>::const_iterator it = nodes_.begin();
         it != nodes_.end(); it++)
      {
         NodeTemplate *tmpl = (*it).second;
         Node *tmp = world_.find_node_by_label_w((*it).first);
         tmp->set_real_position(shawn::Vec(tmpl->posx, tmpl->posy, tmpl->posz));
      }
   }
   // *********************************************************************
   void WiseMlTimestamps::reset() throw()
   {
      shawn::xml::SAXSkipReader::reset();
      elements_.clear();
      elem_count_ = 0;
   }
}

#endif
