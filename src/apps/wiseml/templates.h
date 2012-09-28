/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_WORLD_TEMPLATES_H
#define __SHAWN_APPS_WISEML_WORLD_TEMPLATES_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISEML

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <set>
#include <exception>
#include <string>
#include <map>
#include <list>
#include "sys/node.h"
#include "sys/xml/sax_reader.h"

using namespace std;
using namespace shawn;
using namespace shawn::xml;

namespace wiseml
{
   /**
    * Contains all informations of a node/link capability.
    */
   struct Capability
   {
      Capability()
      {
         changed=false;
      }
      string name;
      string datatype;
      string unit;
      string def_value;
      bool changed;
   };

   typedef std::list<Capability> CapList;

   /**
    * A class which is used to gather node properties
    * while parsing the wiseml file. It contains all
    * informations which are needed to finally create the
    * node.
    */
   class NodeTemplate
   {
   public:
      string label; // ID label
      double posx, posy, posz; // Position
      bool gateway;
      string image;
      string nodetype;
      string description;
      CapList capabilities;

      NodeTemplate();

      NodeTemplate(const NodeTemplate &cpy);
      
      /// Creates a template based on an existing node
      NodeTemplate(shawn::Node &src);
   protected:
      std::list<std::string> make_list(std::string str); 
   };

   struct LinkInfo
   {
      LinkInfo()
      {
         is_encrypted = false;
         is_virtual = false;
      }
      string source;
      string target;
      bool is_encrypted;
      bool is_virtual;
      string rssi;
      string rssi_unit;
      string rssi_datatype;
      CapList capabilities;
   };

   struct LinkConn
   {
      shawn::Node *source;
      shawn::Node *target;
   };

   typedef pair<LinkConn, LinkInfo> Link;
   typedef map<LinkConn, LinkInfo*> Linkmap;

   /**
    * XML parsing information
    */
   struct WmlElement
   {
      std::string name;
      AttList atts;
   };

   typedef list<WmlElement> WmlList;
}

#endif
#endif

