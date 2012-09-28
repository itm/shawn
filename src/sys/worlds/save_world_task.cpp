/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "shawn_config.h"
#include "sys/worlds/save_world_task.h"
#include "sys/vec.h"
#include "sys/misc/uuid.h"
#include <cstdio>
#include <sstream>

using namespace std;
using namespace shawn::xml;

namespace shawn
{
    // ----------------------------------------------------------------------
    /// 
    /**
    *
    */
    SimulationTaskSaveWorld::
    	SimulationTaskSaveWorld() 
        : outf(NULL),
          insert_done_(false),
          append_(false),
          sc(NULL),
          run_count_(0),
		  tag_depth_(0),
	      uuid_(UUIDGenerator::uuid())
    {}

    // ----------------------------------------------------------------------
    /** Returns the name of the tag to be used in the config file
    *
    */
    std::string 
    	SimulationTaskSaveWorld::
    	name( void ) 
    	const throw() 
    {
        return "save_world";
    }

    // ----------------------------------------------------------------------
    /** Description
    *
    */
    std::string 
    	SimulationTaskSaveWorld::description( void ) 
    	const throw() 
    {
        return "Saves the state of the world and the nodes to a xml file.";
    }


    // ----------------------------------------------------------------------
    /** Determines snapshot ID to use in saving
    *  Environment string "snapshot" determines the snapshot id that is used
    *  in saving.
    *
    *  A few magics can be used:
    *  - %r is replaced by the current simulation round
    *  - %n is replaced by a counter that increases with every save
    *  - %u is replaced by a uuid that is unique for the current shawn run
    *  - %% is %
    *
    *  Default is %r
    *
    *  If %r is used, an attempt will be made to keep the snapshots in the file
    *  sorted. Otherwise, the new snapshot will be append at the end of the file.
    */
	void
		SimulationTaskSaveWorld::
		construct_snapshot_id( shawn::SimulationController& sc )
		throw( runtime_error )
   {
      string fmt = sc.environment().optional_string_param("snapshot","%r");
      snapshot_id_.clear();

      snapshot_id_is_default_ = ( fmt == string("%r") );

      string::size_type pos = 0;
      while( 1 )
         {
            string::size_type fmtsym = fmt.find('%',pos);
            if( fmtsym == string::npos )
               {
                  snapshot_id_.append(fmt,pos,string::npos);
                  return;
               }
            else if( fmtsym==fmt.size()-1 )
               throw runtime_error(string("trailing % in format spec '")+fmt+string("'"));
            else
               {
                  snapshot_id_.append(fmt,pos,fmtsym-pos);
                  ostringstream oss;
                  switch(fmt[fmtsym+1])
                     {
                     case 'r':
                        oss << sc.world().simulation_round();
                        snapshot_id_.append(oss.str());
                        break;
                     case 'n':
                        oss << run_count_;
                        snapshot_id_.append(oss.str());
                        break;
                     case 'u':
                        snapshot_id_.append(uuid_);
                        break;
                     case '%':
                        snapshot_id_.append("%");
                        break;
                     default:
                        throw runtime_error(string("unknown magic in format spec '")+fmt+string("'"));
                     }
                  if( fmtsym==fmt.size()-2 )
                     return;
                  else
                     pos=fmtsym+2;
               }
         }
   }

    // ----------------------------------------------------------------------
    /// Triggered by the simulation controller, saves all tags
    /**
    *
    */
    void 
    	SimulationTaskSaveWorld::
    	run( shawn::SimulationController& sc) 
    	throw( runtime_error )
    {
        this->sc = &sc;
        insert_done_ = false;
		tag_depth_ = 0;
        append_ = sc.environment().optional_bool_param("append", false);
        require_world(sc);
        construct_snapshot_id(sc);

        prepare_file();

        if( append_ )
            parse();
        else
            start_saving();

        finalize_file();
        ++run_count_;
    }

    // ----------------------------------------------------------------------
    /// Call back method when the skip target has been reached, triggers the saving of tags
    /** The saving of tags is initiated when we are at a position in the xml file that
    * is matching the current simulation time. This is necessary, since we want to parse the file
    * in chronological order when parsing it with a SAX stream parser.
    */
    void 
    	SimulationTaskSaveWorld::
    	skip_target_reached(string name, AttList& atts) 
    {
        //Check if the next snapshot time is greater than our desired insert time
        if( name == "snapshot" )
        {
           if( snapshot_id_is_default_ )
              {
                 int cur_time = attribute("id", atts) != "" ? atoi(attribute("id", atts).c_str()) : 0;
                 
                 if( cur_time > sc->world().simulation_round() )
                    start_saving();
              }
        }
        else if( name == "scenario" )
        {
            start_saving();
        }
    }

    // ----------------------------------------------------------------------
    /// Checks if we are at a potential insert position in the XML file.
    /** Returns true if we have not inserted tags yet and the parsing is
    * either at an opening snapshot tag or a closing scenarion tag.
    *
    */
    bool 
    	SimulationTaskSaveWorld::
    	check_skip_target_reached(string name, AttList&, bool opening_tag) 
    {
        //Check if we are past the insertion point
        if(insert_done_)
            return false;

        //All opening of snapshot are welcome
        if( name == "snapshot" && opening_tag) 
            return true;

        //All closing tags of scenario are welcome
        if( name == "scenario" && !opening_tag ) 
            return true;

        return false;
    }

    // ----------------------------------------------------------------------
    /// Dummy.
    /**
    *
    */
    void 
    	SimulationTaskSaveWorld::
    	clear_skip_target()
    {
    }

    // ----------------------------------------------------------------------
    /// Return whether we are still skipping to a certain position or not. 
    /** After the insert is done, true is returned.
    *
    */
    bool 
    	SimulationTaskSaveWorld::
    	skipping() 
    {
        return insert_done_;
    }

    // ----------------------------------------------------------------------
    /// Callback from the SAX Parser: Handles the copy process from the old to the new file.
    /** All encountered tags are copied to the outfile. 
    * @todo Delay the writing until the closing tag is encoutered to avoid the problem described above.
    */
    void 
    	SimulationTaskSaveWorld::
    	handle_start_element(string name, AttList& atts) 
        throw(runtime_error)
    {
        SAXSkipReader::handle_start_element(name, atts);

        *outf << "<" << name << " ";
        
    	for(AttList::iterator it = atts.begin(), end = atts.end(); it!=end; ++it)
            *outf << it->first << "=\"" << it->second << "\" ";
        
        *outf << ">" << endl;

    }

    // ----------------------------------------------------------------------
    /// Callback from the SAX Parser: Handles the copy process from the old to the new file.
    /** All encountered tags are copied to the outfile. 
    */
    void 
    	SimulationTaskSaveWorld::
    	handle_end_element(string name) 
        throw(runtime_error)
    {
        SAXSkipReader::handle_end_element(name);
        *outf << "</" << name << ">" << endl;
    }


    // ----------------------------------------------------------------------
    /// Prepares the in and outfile. 
    /** Depending on the append or truncate mode it creates a temporary file or a new one.
    */
    void 
    	SimulationTaskSaveWorld::
    	prepare_file() 
    	throw( runtime_error )
    {
        filename_ = sc->environment().required_string_param("file");
        tmpoutname_ = filename_ + ".tmp";

        if(!append_) //Write preamble to the file since we do not append
        {
            outf = new ofstream(filename_.c_str(), ofstream::out | ofstream::trunc);
            *outf << "<?xml version=\"1.0\" ?>" << endl;
            *outf << "<scenario>" << endl;
        }
        else //Append to an existing file: Seek to the insert position first
        {
            set_document_uri(filename_);
            outf = new ofstream(tmpoutname_.c_str(), ofstream::out | ofstream::trunc);
        }

    }

    // ----------------------------------------------------------------------
    /// Prepares the in and outfile for closing. 
    /** Depending on the append or truncate mode overwrites the old xml file with the new one 
    * or it just closes the newly created file.
    */
    void 
    	SimulationTaskSaveWorld::
    	finalize_file() 
    	throw()
    {

        if(!append_) 
        {
            *outf <<  "</scenario>" << endl; 
            outf->flush();
            outf->close();
            delete outf;
        } 
        else 
        {
            remove(filename_.c_str());
            rename(tmpoutname_.c_str(), filename_.c_str());
        }

    }

    // ----------------------------------------------------------------------
    /// Starts the saving of world and node tags (only if insert_done_ == false).
    /** Writes a new snapshot tag to the current location with the current simulation
    * round as timestamp and saves all tags.
    * @see save_world_tags 
    * @see save_node_tags
    */
    void 
    	SimulationTaskSaveWorld::
    	start_saving() 
    	throw()
    {
        if(insert_done_)
            return;

        *outf << "<snapshot id=\""<< snapshot_id_ <<"\">" << endl;
        save_world();
        save_nodes();
        *outf <<  "</snapshot>" << endl; 

        insert_done_ = true;
    }


    // ----------------------------------------------------------------------
    /// Saves all world tags below a new world tag
    /**
    *
    */
    void 
    	SimulationTaskSaveWorld::
    	save_world() 
    	throw ()
    {
        if( sc->world().count_tags() == 0 )
            return;

        *outf << "\t<world>" << endl;

        if( sc->world().has_size_hint() )
            *outf << "\t<sizehint value=\""<< sc->world().size_hint() <<"\" />" << endl;

        save_tags(sc->world().begin_tags(), sc->world().end_tags() );

        *outf <<  "\t</world>" << endl;             
    }

    // ----------------------------------------------------------------------
    /// Iterates through all nodes and writes the tags of each node below a node tag
    /**
    *
    */
    void 
    	SimulationTaskSaveWorld::
    	save_nodes() 
    	throw ()
    {  
        for( shawn::World::const_node_iterator it = sc->world().begin_nodes(); it != sc->world().end_nodes(); ++it) 
        {
            const shawn::Node* node = &(*it);

            Vec pos = node->real_position();

            *outf << "\t<node id=\""<< node->label() <<"\">" << endl;

            *outf << "\t\t<location x=\""<< pos.x() <<
                "\" y=\""<< pos.y() <<
                "\" z=\""<< pos.z() <<"\" />" << endl;

            save_tags(node->begin_tags(), node->end_tags() );
            *outf << "\t</node>" << endl;
        }
    }

    // ----------------------------------------------------------------------
    /// Writes a list of tags to the current position of outf_
    /**
    *
    */
    void SimulationTaskSaveWorld::
        save_tags(shawn::TagContainer::tag_iterator begin, shawn::TagContainer::tag_iterator end ) 
        throw()
    {

        for( shawn::TagContainer::tag_iterator tagit = begin; tagit != end; tagit++) 
        {
            shawn::TagHandle th = tagit->second;

            if( th.is_not_null() )
                if( th->is_persistent() )
                {
                    try 
                    {
                        save_tag(th);
                    }
                    catch( std::runtime_error& re ) 
                    {
                        std::cerr << "Cannot save tag '" << th->name() << "': " << re.what() << std::endl;
                    }
                }
        }
    }

    // ----------------------------------------------------------------------
    /// Writes one tag to the current position of outf_
    /**
    *
    */
    void SimulationTaskSaveWorld::
        save_tag(shawn::TagHandle th) 
        throw(std::runtime_error)
    {
        bool first_entry = true;
		string padding = "\t\t";

		for(int i = 0; i < tag_depth_; ++i)
			padding += "\t";

        //Iterate over all entries of this tag.
        for( shawn::Tag::const_entry_iterator it = th->begin_entries(); it != th->end_entries(); ++it) 
        {
            if( first_entry ) 
            {
                *outf << padding << "<tag type=\""<< th->type_identifier()
                    << "\" name=\""<< th->name() << "\" >" << endl;
                first_entry = false;
            }

            *outf << padding << "\t<entry ";
            if( it.has_index() )
                *outf << "index=\"" << it.index() << "\" ";
            *outf << "value=\""<< it.value() <<"\" />" << endl;

        }

		if( th->count_tags() > 0 )
		{
			if( first_entry )
			{
				*outf << padding << "<tag type=\""<< th->type_identifier()
					<< "\" name=\""<< th->name() << "\" >" << endl;
				first_entry = false;
			}

			++tag_depth_;
			save_tags(th->begin_tags(), th->end_tags());
			--tag_depth_;
		}

        //If first entry is still true, the current tag is a simple tag
        if( first_entry ) 
            *outf << padding << "<tag type=\""<< th->type_identifier()
            << "\" name=\""<< th->name()
            << "\" value=\""<< th->encoded_content() <<"\" />"
            << endl;
        else 
            *outf << padding << "</tag>" << endl;

    }

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/worlds/save_world_task.cpp,v $
* Version $Revision: 380 $
* Date    $Date: 2009-12-07 17:53:32 +0100 (Mon, 07 Dec 2009) $
*-----------------------------------------------------------------------
* $Log: save_world_task.cpp,v $
 *-----------------------------------------------------------------------*/
