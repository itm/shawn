/**
 * @file sumo_net.h
 * Defines the class sumo::SumoNet.
 *
 * @author Josef Schlittenlacher
 * @author Walter Bamberger
 *
 * $Revision: 545 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/sumo/sumo_net.h $
 * $Id: sumo_net.h 545 2011-04-15 10:39:01Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010 Josef Schlittenlacher<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_SUMO_SUMO_NET_H
#define __SHAWN_APPS_SUMO_SUMO_NET_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_SUMO

#include "sys/vec.h"
#include "sys/xml/sax_reader.h"
#include <vector>
#include <map>
#include <string>

namespace sumo
{

/**
 * Loads and represents a SUMO network.
 *
 * This class loads a SUMO network file and exposes the network to the
 * simulation. It can also transform coordinates from the triplet
 * (edge, lane, position) to vectors (x, y, z=0).
 */
class SumoNet
{
public:

   /**
    * Position expressed with SUMO parameters (edge_id, lane_id, lane_position)
    */
   struct SumoPosition
   {
      std::string edge_id;
      std::string lane_id;
      double lane_position;
   };

   /**
    * Initialises the SumoNet.
    *
    * Parses the net file and creates the internal data structures from it.
    *
    * @param path_net_file the path and name of the SUMO network file (.net.xml)
    */
   SumoNet( const std::string& path_net_file );

   /**
    * Compute the x and y coordinates out of a SUMO position
    * (edge, lane, lane position).
    *
    * @param sumo_position the position to transform
    * @return the same position but in (x, y, z=0) coordinates
    */
   shawn::Vec
   sumo_pos_to_vec( const SumoPosition& sumo_position ) const;

private:
   /**
    * This helper class parses the SUMO net file with the SAX parser.
    *
    * At present, it can only parse the lane tags and extract their ids and
    * shapes.
    */
   class SUMONetReader : public shawn::xml::SAXSimpleSkipReader
   {
   public:
      /**
       * Initialises the SAX parser.
       *
       * @param file_name the file name or URI of the SUMO net file.
       */
      SUMONetReader( const std::string& file_name ) :
         SAXSimpleSkipReader()
      {
         SAXSimpleSkipReader::set_document_uri(file_name);
      }

      /**
       * Extracts the ids and shapes of the lanes in the net file.
       *
       * This method parses the net file for lane tags. For each lane tag,
       * it collects the id and shape in the resulting map. This map must be
       * pre-allocated by the user.
       *
       * @param lanes_ptr A pointer to the map where the lanes should be stored in
       * @return The same pointer as given by lanes_ptr
       */
      std::map<std::string, std::vector<shawn::Vec> >*
      extract_lanes(
               std::map<std::string, std::vector<shawn::Vec> > * lanes_ptr )
               throw ( std::runtime_error );

   protected:
      /**
       * Processes the lane tags.
       *
       * A callback function of the SAXSimpleSkipReader. This method evaluates
       * the lane tag and stores it in the map.
       *
       * @param name the first parameter is the name of the tag found (always "lane")
       * @param atts a list of the attributes of the lane tag
       */
      virtual void
      skip_target_reached( std::string name, shawn::xml::AttList& atts )
               throw ( std::runtime_error );

   private:
      /**
       * A pointer to the map all lanes are stored in.
       *
       * This map has been pre-allocated by the user.
       */
      std::map<std::string, std::vector<shawn::Vec> >* lanes_ptr_;
   };

   /**
    * List of lane shapes indexed by lane ids.
    *
    * A lane shape is a vector of cartesian positions.
    */
   std::map<std::string, std::vector<shawn::Vec> > map_lanes_;

};

}

#endif /* ENABLE_SUMO */
#endif /* __SHAWN_APPS_SUMO_SUMO_NET_H */
