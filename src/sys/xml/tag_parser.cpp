/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/xml/tag_parser.h"

#include <iostream>
#include <exception>

using namespace std;
using namespace shawn::xml;

namespace shawn
{
	namespace xml 
	{

		// ----------------------------------------------------------------------
		TagParser::
			~TagParser()
		{}

		// ----------------------------------------------------------------------
		void
			TagParser::
			handle_open_tag_tag(AttList& atts, TagContainer& tc) 
			throw()
		{
			string name  = attribute("name", atts);
			string type  = attribute("type", atts);        
			string value = attribute("value", atts);
			TagFactoryHandle tfh = find_tag_factory(type);

			if( tfh.is_not_null() ) 
			{
				//Create a new tag
				TagHandle new_tag = tfh->create(type, name, value);
				if( new_tag.is_not_null() )
                {
					new_tag->set_persistency( true );
    				//Add it to the current tag container
    				if( has_current_container() )
	    				current_container(tc)->add_tag(new_tag);

    				//Make the created tag the current tag (for entries, subtags, ...)
    				current_tags_.push( new_tag.get() );
                }
				else
                {
					cerr << "LoadWorldFactory: Unable to create tag (name: " << 
						name<<", type: " << type << ", value: " << value << ")" << endl;
    				current_tags_.push( NULL );
                }
			} 
			else
			{
				cerr << "LoadWorldFactory: Unable to find factory for tag type: " << type << endl; 
				current_tags_.push(NULL);
			}
		}

		// ----------------------------------------------------------------------
		void
			TagParser::
			handle_close_tag_tag(AttList& atts, TagContainer& tc) 
			throw() 
		{
			current_tags_.pop();
		}

		// ----------------------------------------------------------------------
		void 
			TagParser::
			handle_tag_entry(AttList& atts)
			throw()
		{
			string index = attribute("index", atts);
			string value = attribute("value", atts);
			assert(value != "");

			//No factory for the current tag found -> Ignoring entries
			if(current_tags_.top() == NULL)
				return;

			//Index -> Map like, else Set like behavior
			if( index != "")
				current_tags_.top()->add_indexed_entry(index, value);
			else 
				current_tags_.top()->add_value_entry(value);
		}    

		// ----------------------------------------------------------------------
		bool 
			TagParser::
			has_current_container() 
			const throw()
		{
			if(current_tags_.empty() )
				return true;

			return current_tags_.top() != NULL;
		}

		// ----------------------------------------------------------------------
		shawn::TagContainer* 
			TagParser::
			current_container(shawn::TagContainer& tc) 
			const throw()
		{
			if( current_tags_.empty() )
				return &tc;

			return current_tags_.top();
		}

		// ----------------------------------------------------------------------
		TagFactoryHandle
			TagParser::
			find_tag_factory( const string& type )
			const throw( )
		{
			try 
			{
				assert(tfk_ != NULL);
				return tfk_->find_w(type);
			}
			catch( std::runtime_error ) 
			{
				cerr << "WARNING: ignoring tag(s) of unknown type '" << type << "'" << endl;
			}

			return NULL;
		}    

		// ----------------------------------------------------------------------
		void 
			TagParser::
			set_tag_factory_keeper(TagFactoryKeeper& tfk)
		{
			tfk_ = &tfk;
		}

	}
}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/xml/tag_parser.cpp,v $
* Version $Revision: 189 $
* Date    $Date: 2008-04-24 14:14:34 +0200 (Thu, 24 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: tag_parser.cpp,v $
 *-----------------------------------------------------------------------*/
