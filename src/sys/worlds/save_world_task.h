/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SNS_SYS_XML_SAX_TAGSAVE_H
#define __SNS_SYS_XML_SAX_TAGSAVE_H

#include "shawn_config.h"

#include "sys/world.h"
#include "sys/xml/sax_reader.h"
#include "sys/simulation/simulation_task.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/tag.h"

#include <iostream>
#include <fstream>
#include <string>

namespace shawn 
{

	//-------------------------------------------------------------------------
    /** Saves all tags of the world and all nodes into a XML File. 
     * The tags can either be inserted in an existing file or written to a new file. This task creates a new
     * &lt;snapshot time=""&gt; ... &lt;/snapshot&gt; entry in the file. A minor 
     * thing is that in an existing file all lt;tagname /&gt; tags will be rewritten to &lt;tagname&gt;&lt;tagname/&gt;
     */
    class SimulationTaskSaveWorld 
    	: public shawn::xml::SAXSkipReader, 
    	  public SimulationTask 
        {
        public:
			SimulationTaskSaveWorld();
			virtual void run( shawn::SimulationController& ) throw( std::runtime_error );
			virtual std::string name( void ) const throw();
			virtual std::string description( void ) const throw();

        protected:
            virtual bool check_skip_target_reached(std::string name, shawn::xml::AttList& atts, bool opening_tag);
            virtual void skip_target_reached(std::string name, shawn::xml::AttList& atts);
            virtual void clear_skip_target();
            virtual bool skipping();

            virtual void handle_start_element(std::string name, shawn::xml::AttList& atts) throw(std::runtime_error);
			virtual void handle_end_element(std::string name) throw(std::runtime_error);
			virtual void construct_snapshot_id( shawn::SimulationController& ) throw( std::runtime_error );

        private:
            void prepare_file() throw( std::runtime_error );
            void finalize_file() throw();
            
            void start_saving() throw();

            void save_world() throw();
            void save_nodes() throw();
            void save_tags(shawn::TagContainer::tag_iterator begin, shawn::TagContainer::tag_iterator end ) throw();
            void save_tag(shawn::TagHandle) throw(std::runtime_error);

			std::string filename_;
			std::string tmpoutname_;
			std::string snapshot_id_;
			bool snapshot_id_is_default_;
			std::ofstream *outf;
			bool insert_done_;
			bool append_;
			shawn::SimulationController* sc;
			int run_count_;
			int tag_depth_;
			std::string uuid_;
        };

}

#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/worlds/save_world_task.h,v $
* Version $Revision: 189 $
* Date    $Date: 2008-04-24 14:14:34 +0200 (Thu, 24 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: save_world_task.h,v $
 *-----------------------------------------------------------------------*/
