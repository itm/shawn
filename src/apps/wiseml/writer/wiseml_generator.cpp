/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/wiseml_generator.h"

#ifdef ENABLE_WISEML
#include <sstream>
#include <time.h>
#include "sys/taggings/node_reference_tag.h"
#include "sys/taggings/group_tag.h"
#include "sys/taggings/basic_tags.h"
namespace wiseml
{
   WisemlGenerator::WisemlGenerator(SimulationController &ctrl)
      : ctrl_(ctrl),
        tabs_(""),
        use_tabs_(true)
   {
   }
   // ----------------------------------------------------------------------
   WisemlGenerator::~WisemlGenerator()
   {
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   std::string WisemlGenerator::generate_wiseml( bool use_tabs )
   {
      use_tabs_ = use_tabs;
      std::stringstream wml;
      wml << "<wiseml version=\"1.0\" xmlns=\"http://wisebed.eu/ns/wiseml"
         << "/1.0\">" << std::endl;
      add_tab();

      wml << generate_setup() << std::endl;

      remove_tab();
      wml << "</wiseml>" << std::endl;
      return wml.str();
   }
   // ----------------------------------------------------------------------
   std::string WisemlGenerator::generate_setup()
   {  
      std::stringstream wml;
      wml << tabs_ << "<setup>" << std::endl;
      add_tab();

      wml << generate_origin() << std::endl;
      wml << generate_timeinfo() << std::endl;
      wml << generate_defaults();
      
      wml << tabs_ << "<interpolation/>" << std::endl;
      wml << tabs_ << "<description/>" << std::endl;
      wml << std::endl;

      wml << generate_node_inst() << std::endl;
      wml << generate_link_inst();

      remove_tab();
      wml << tabs_ << "</setup>" << std::endl;

      return wml.str();
   }
   // ----------------------------------------------------------------------
   std::string WisemlGenerator::generate_origin()
   {
      std::stringstream wml;
      wml << tabs_ << "<origin>" << std::endl;
      add_tab();

      wml << tabs_ << "<x>0.0</x>" << std::endl;
      wml << tabs_ << "<y>0.0</y>" << std::endl;
      wml << tabs_ << "<z>0.0</z>" << std::endl;
      wml << tabs_ << "<phi>0.0</phi>" << std::endl;
      wml << tabs_ << "<theta>0.0</theta>" << std::endl;

      remove_tab();
      wml << tabs_ << "</origin>" << std::endl;

      return wml.str();
   }
   // ----------------------------------------------------------------------
   std::string WisemlGenerator::generate_timeinfo()
   {
      std::stringstream wml;
      wml << tabs_ << "<timeinfo>" << std::endl;
      add_tab();
      
      wml << tabs_ << "<start>" << generate_timestring();
      wml << "</start>" << std::endl;

      wml << tabs_ << "<duration>0</duration>" << std::endl;
      wml << tabs_ << "<unit>seconds</unit>" << std::endl;

      remove_tab();
      wml << tabs_ << "</timeinfo>" << std::endl;

      return wml.str();
   }
   // ----------------------------------------------------------------------
   std::string WisemlGenerator::generate_timestring()
   {
      std::stringstream wml;

      //Current time
      time_t t = time(NULL);
      tm *ts = localtime(&t);

      
      wml << ts->tm_year + 1900 << "-" ;  //year
      if(ts->tm_mon + 1 < 10)
         wml << "0";
      wml << ts->tm_mon + 1 << "-";       //month
      if(ts->tm_mday < 10)
         wml << "0";
      wml << ts->tm_mday << "T";          //day
      if(ts->tm_hour < 10)
         wml << "0";
      wml << ts->tm_hour << ":";          //hour
      if(ts->tm_min < 10)
         wml << "0";
      wml << ts->tm_min << ":";           //minute
      if(ts->tm_sec < 10)
         wml << "0";
      wml << ts->tm_sec << "Z";           //second

      return wml.str();
   }
   // ----------------------------------------------------------------------
   std::string WisemlGenerator::generate_defaults()
   {
      return "";
   }
   // ----------------------------------------------------------------------
   std::string WisemlGenerator::generate_node_inst()
   {
      std::stringstream wml;

      for(World::const_node_iterator it = world().begin_nodes(); 
         it != world().end_nodes(); it++)
      {
         wml << tabs_ << "<node id=\"" << it->label() << "\">" << std::endl;
         add_tab();

         // Position
         Vec npos = it->real_position();
         wml << tabs_ << "<position>" << std::endl;
         add_tab();
         wml << tabs_ << "<x>" << npos.x() << "</x>" << std::endl;
         wml << tabs_ << "<y>" << npos.y() << "</y>" << std::endl;
         wml << tabs_ << "<z>" << npos.z() << "</z>" << std::endl;
         remove_tab();
         wml << tabs_ << "</position>" << std::endl;

         remove_tab();
         wml << tabs_ << "</node>" << std::endl << std::endl;
      }

      return wml.str();
   }
   // ----------------------------------------------------------------------
   std::string WisemlGenerator::generate_link_inst()
   {
      std::stringstream wml;

      for(World::const_node_iterator it = world().begin_nodes(); 
         it != world().end_nodes(); it++)
      {
         for(World::const_adjacency_iterator ait = 
            world().begin_adjacent_nodes(*it, EdgeModel::CD_ANY);
            ait !=  world().end_adjacent_nodes(*it); ++ait)
         {
            wml << tabs_ << "<link source=\"" << it->label()
               << "\" target=\"" << ait->label()
               << "\">" << std::endl;
            add_tab();

            wml << tabs_ << "<encrypted>false</encrypted>" << std::endl;
            wml << tabs_ << "<virtual>false</virtual>" << std::endl;
            // ...

            remove_tab();
            wml << tabs_ << "</link>" << std::endl << std::endl;
         }


         /*const GroupTag *links = dynamic_cast<const GroupTag*>
            (it->find_tag("WISEML_LINKS").get());

         for(TagContainer::tag_iterator t_it = links->begin_tags();
            t_it != links->end_tags(); t_it++)
         {
            NodeReferenceTag *target = 
               dynamic_cast<NodeReferenceTag*>
               (t_it->second.get());
            
            wml << tabs_ << "<link source=\"" << it->label()
               << "\" target=\"" << target->value()->label()
               << "\">" << std::endl;
            add_tab();

            wml << tabs_ << "<encrypted>false</encrypted>" << std::endl;
            wml << tabs_ << "<virtual>false</virtual>" << std::endl;
            // ...

            remove_tab();
            wml << tabs_ << "</link>" << std::endl << std::endl;
            
         }*/
      }

      return wml.str();
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   void WisemlGenerator::add_tab()
   {
      if(use_tabs_)
         tabs_ += "   ";
   }
   // ----------------------------------------------------------------------
   void WisemlGenerator::remove_tab()
   {
      if(use_tabs_ && tabs_.length() > 0)
         tabs_ = tabs_.erase(tabs_.length() - 3, 3);
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   const World& WisemlGenerator::world() const
   {
      return ctrl_.world();
   }
   // ----------------------------------------------------------------------
   World& WisemlGenerator::world_w()
   {
      return ctrl_.world_w();
   }
}
#endif
