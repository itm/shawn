/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_SYS_WORLDS_TOPOLOGY_POLYGON_XML_POLYGON_TOPOLOGY_H
#define __SHAWN_SYS_WORLDS_TOPOLOGY_POLYGON_XML_POLYGON_TOPOLOGY_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY
#include "shawn_config.h"

#include "apps/topology/polygon/polygon.h"
#include "apps/topology/topology/polygon_topology.h"
#include "sys/xml/sax_reader.h"
#include "sys/tag_container.h"
#include "sys/xml/tag_parser.h"
#include "apps/topology/polygon/segment_2d.h"
#include "sys/vec.h"

#include <string>
#include <stdexcept>

namespace topology
{

    /** Reads a XML polygon input file. The format of the file looks like:
    * <pre>
    * <topology>
    * <polygon type="outer">
	*	  <!-- Here, all valid tags from shawn::Tag may be present -->
    *     <vertex x="2117.41163264478" y="8044.34768255157"/>
    *     <vertex x="2117.48762643993" y="8454.22570670335"/>
    *     <vertex x="3268.4534325899" y="8573.10638444706"/> 
    *     ...
    * </polygon>
    * ...
    * <polygon type="hole">
	*	  <!-- Here, all valid tags from shawn::Tag may be present -->
    *     <vertex x="3287.62118775287" y="13646.0022118151"/>
    *     <vertex x="4862.11689576203" y="12958.9956104826"/>
    *     <vertex x="4293.52515754043" y="11767.3139256284"/>
    *     <vertex x="3095.27629153908" y="11847.9885837907"/>
    *     <vertex x="2725.59266336733" y="11966.2864374151"/>
    * </polygon>
    *
    * </pre>
    *
    * If a polygon is not simple (i.e. if edges don’t intersect, except consecutive 
    * edges, which intersect in their common vertex), a convex hull is created 
    * around the vertices of the polygon.    
    */
    class XMLPolygonTopology
        : public PolygonTopology, 
    	  public shawn::xml::TagParser,
		  public shawn::xml::SAXReader
    {
    public:
        XMLPolygonTopology();
        virtual ~XMLPolygonTopology();

        /** Parses the given xml file */
        virtual void read(shawn::SimulationController& sc, const std::string& filename, 
					bool create_outer_polygon = false, bool fix_non_simple_polygons = false) 
					throw( std::runtime_error );
    
    protected:
        enum ParsingState 
        { 
            UnknownState,    ///< No expected tag has been encoutered yet
            TopologyState,   ///< Inside a topology tag
            PolygonState,    ///< Inside a polygon tag
            TagState,		 ///< Inside a shawn::Tag element
            DoneState        ///< Parsing has stopped
        };   

        enum PolygonType 
        { 
            HoleType,   ///< Hole polygon
            OuterType   ///< Outer polygon
        };            
        
		/// The current parsing state in the XML file
        ParsingState parsing_state_;        

		/// The type of the currently read polygon
        PolygonType polygon_type_;          

		/// The currently read polygon
        Polygon* polygon_; 

		/// The outer polygon
        Polygon* outer_;   

        int polygon_count_;
        int tag_count_;
        bool create_outer_polygon_;
		bool fix_non_simple_polygons_;

        /** Returns the polygon type based on the XML attributes
          * @return Polygon type
          * @throws std::runtime_error If the polygon type is invalid
          */
        PolygonType polygon_type(shawn::xml::AttList& atts)  throw(std::runtime_error);

        /** Callback handler for opening xml tags
          */
        virtual void handle_start_element(std::string name, shawn::xml::AttList& atts) throw(std::runtime_error);

        /** Callback handler for closing xml tags
          */
        virtual void handle_end_element(std::string name) throw(std::runtime_error);
        
        /** */
       Vec to_point(shawn::xml::AttList& atts) const throw(std::runtime_error);
    };

}

#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/topology/topology/xml_polygon_topology.h,v $
* Version $Revision: 197 $
* Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: xml_polygon_topology.h,v $
*-----------------------------------------------------------------------*/
