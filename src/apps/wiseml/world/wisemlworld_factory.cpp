/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/world/wisemlworld_factory.h"
#ifdef ENABLE_WISEML
#include "sys/taggings/node_reference_tag.h"
#include "sys/taggings/group_tag.h"
#include "sys/taggings/basic_tags.h"
#include "apps/wiseml/timestamps/wiseml_timestamps.h"
#include "apps/wiseml/sensors/wiseml_string_sensor_factory.h"
#include "apps/wiseml/sensors/wiseml_double_sensor_factory.h"
#include "apps/wiseml/sensors/wiseml_int_sensor_factory.h"
using namespace shawn;

namespace wiseml
{
   WiseMLWorldFactory::WiseMLWorldFactory()
      : scenario_id_(""),
        trace_id_("")
   {
      state_ = WML_NONE;
      def_node_ = NodeTemplate();
      elem_count_ = 0;
   }
   // *********************************************************************
   WiseMLWorldFactory::~WiseMLWorldFactory()
   {

   }
   // *********************************************************************
   void WiseMLWorldFactory::fill_world(World &w) throw()
   {
      shawn::SimulationController &sc = w.simulation_controller_w();
      // Add Wiseml<type>SensorFactories here:
      try
      {
         sc.keeper_by_name_w<reading::SensorKeeper>("SensorKeeper")->find("wiseml_string_sensor");
      }
      catch(std::runtime_error e)
      {
         sc.keeper_by_name_w<reading::SensorKeeper>("SensorKeeper")->add( new WisemlStringSensorFactory );
         sc.keeper_by_name_w<reading::SensorKeeper>("SensorKeeper")->add( new WisemlDoubleSensorFactory );
         sc.keeper_by_name_w<reading::SensorKeeper>("SensorKeeper")->add( new WisemlIntSensorFactory );
      }

      world_ = &w;
      // Load and apply world setup data:
      set_state(WML_DEFAULTSNODE);
      parse();
      set_state(WML_TOPOLOGY);
      parse();
      // Load dynamic parser:
      WiseMlTimestamps *tracer = new WiseMlTimestamps(filename_,
         scenario_id_, trace_id_, *world_);
      tracer->set_timefactor(timefactor_);
      world_->scheduler_w().new_event(*tracer,0.0,NULL);
   }
   // *********************************************************************
   void WiseMLWorldFactory::set_timefactor(double t)
   {
       timefactor_ = t;
   }
   // *********************************************************************
   void WiseMLWorldFactory::set_filename(string filename)
   {
       filename_ = filename;
       set_document_uri(filename);
   }
   // *********************************************************************
   void WiseMLWorldFactory::set_scenario_id(string id)
   {
       scenario_id_ = id;
   }
   // *********************************************************************
   void WiseMLWorldFactory::set_trace_id(string id)
   {
       trace_id_ = id;
   }

   // *********************************************************************
   void WiseMLWorldFactory::set_state(LoaderState state)
   {
      state_ = state;
      switch(state_)
      {
      case WML_DEFAULTSNODE:
         set_skip_target("defaults");
         set_skip_target_add_attr("for", "node");
         break;
      case WML_DEFAULTSLINK:
         set_skip_target("defaults");
         set_skip_target_add_attr("for", "link");
         break;
      case WML_TOPOLOGY:
         break;
      case WML_INTERRUPT:
         set_skip_target("wiseml", false);
         interrupt();
         break;
      }
   }


   // *********************************************************************
   void WiseMLWorldFactory::
   skip_target_reached(std::string, AttList& atts)
   {
   }
   // *********************************************************************
   void WiseMLWorldFactory::
   handle_start_element(std::string name, AttList& atts)
         throw(std::runtime_error)
   {
      SAXSimpleSkipReader::handle_start_element(name, atts);
      WmlElement el;
      el.name = name;
      el.atts = AttList(atts);
      elements_.push_front(el);
      elem_count_++;
      if(skipping())
      {
          return;
      }



      if(state_ == WML_DEFAULTSNODE)
      {
      }

      if(state_ == WML_DEFAULTSLINK)
      {
      }

      if(state_ == WML_TOPOLOGY)
      {
         if(name=="node")
         {
            handle_start_node(name,atts);
         }
         else if(name=="link")
         {
            handle_start_link(name,atts);
         }
      }
   }
   // *********************************************************************
   void WiseMLWorldFactory::
   handle_start_node(std::string name, AttList& atts)
         throw(std::runtime_error)
   {
      // A new node is going to be created
      new_node_ = NodeTemplate(def_node_);
      new_node_.label = attribute("id", atts, "");
   }
   // *********************************************************************
   void WiseMLWorldFactory::
   handle_start_link(std::string name, AttList& atts)
         throw(std::runtime_error)
   {
      // A new link is going to be created
   }
   // *********************************************************************
   void WiseMLWorldFactory::
   handle_end_element(std::string name)
            throw(std::runtime_error)
   {
      SAXSimpleSkipReader::handle_end_element(name);
      if(skipping())
      {
         return;
      }

      if(name == "setup")
      {
         set_state(WML_INTERRUPT);
         return;
      }

      if(state_ == WML_DEFAULTSNODE)
      {
         if(name=="defaults")
         {
         }
      }

      if(state_ == WML_DEFAULTSLINK)
      {
         if(name=="defaults")
         {
            set_state(WML_INTERRUPT);
         }
      }

      if(state_ == WML_TOPOLOGY)
      {
         if(name=="node")
         {
            handle_end_node(name);
         }
         else if(name=="link")
         {
            handle_end_link(name);
         }
      }

      if(elements_.front().name == name)
      {
         elements_.pop_front();
         elem_count_--;
      }
      else
      {
         cout << "HandleEndElement: Element misfit" << endl;
      }
   }
   // *********************************************************************
   void WiseMLWorldFactory::
   handle_end_node(std::string name)
            throw(std::runtime_error)
   {
      Node *node = new_node(Vec(new_node_.posx, new_node_.posy,
         new_node_.posz), new_node_.label);
      GroupTag *link_group = new GroupTag("WISEML_LINKS");
      node->add_tag(link_group);
      BoolTag *enabled = new BoolTag("WISEML_ENABLED", true);
      node->add_tag(enabled);
      world_->add_node(*node);
      //cout << "Added node " << new_node_.label << endl;
   }
   // *********************************************************************
   void WiseMLWorldFactory::
   handle_end_link(std::string name)
            throw(std::runtime_error)
   {
      if(elements_.front().name != "link")
      {
         //Error
         return;
      }

      Node* src = world_->find_node_by_label_w(attribute("source", elements_.front().atts));
      Node* tgt = world_->find_node_by_label_w(attribute("target", elements_.front().atts));
      if(src == NULL || tgt == NULL)
      {
         //Error
         return;
      }

      NodeReferenceTag *n_tag = new NodeReferenceTag(tgt->label(), tgt);
      GroupTag *links = dynamic_cast<GroupTag*>(src->find_tag_w("WISEML_LINKS").get());
      links->add_tag(n_tag);

      //std::cout << "Added Link from " << src->label() << " to " << tgt->label() << std::endl;

   }
   // *********************************************************************
   void WiseMLWorldFactory::
   handle_text_element(std::string name)
            throw(std::runtime_error)
   {
      SAXSkipReader::handle_text_element(name);
      if(skipping())
      {
          return;
      }

      WmlList::const_iterator it = elements_.begin();
      if(elem_count_ < 3)
         return;
      WmlElement parent1 = *(it);
      WmlElement parent2 = *(++it);
      WmlElement parent3 = *(++it);

      // Set positions
      if(parent2.name == "position")
      {
         // for default node template
         if(parent3.name == "defaults")
         {
            if(parent1.name == "x")
            {
               def_node_.posx = atof(name.c_str());
            }
            else if(parent1.name == "y")
            {
               def_node_.posy = atof(name.c_str());
            }
            else if(parent1.name == "z")
            {
               def_node_.posz = atof(name.c_str());
            }
         }
         // for current node template
         else if(parent3.name == "node")
         {
            if(parent1.name == "x")
            {
               new_node_.posx = atof(name.c_str());
            }
            else if(parent1.name == "y")
            {
               new_node_.posy = atof(name.c_str());
            }
            else if(parent1.name == "z")
            {
               new_node_.posz = atof(name.c_str());
            }
         }
      }

   }
   // *********************************************************************
}
#endif
