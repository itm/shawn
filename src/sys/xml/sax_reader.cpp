/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sax_reader.h"
#include <cassert>
#include <fstream>
#include <string>
#include <cstring>
#include <cerrno>
#include <sstream>

using namespace std;

namespace shawn
{
	namespace xml
	{

		// ----------------------------------------------------------------------
	    /// Expat C-style callback funcion
		void XMLCALL saxreader_start(void *userdata, const char *name, const char **atts)
		{
			SAXReader* ptr = (SAXReader*) userdata;

			AttList a;
			for(; *atts; atts += 2)
				a.insert( pair<string, string>( atts[0], atts[1] ) );

			ptr->handle_start_element(name, a);
		}

		// ----------------------------------------------------------------------
	    /// Expat C-style callback funcion
		void XMLCALL saxreader_end(void *userdata, const char *name)
		{
			SAXReader* ptr = (SAXReader*) userdata;
			ptr->handle_end_element(name);
		}

		void XMLCALL saxreader_text(void *userdata, const char* text, int length)
		{
			SAXReader* ptr = (SAXReader*) userdata;
			string content(text, length);
			ptr->handle_text_element(content);
		}

	    // ----------------------------------------------------------------------

	    string
	        attribute(string name, AttList& atts, string default_val /* = "" */)
	    {
	    	for(AttList::iterator it = atts.begin(), end = atts.end(); it!=end; ++it)
	    		if( it->first == name )
	    			return it->second;

	        return default_val;
	    }


		// ----------------------------------------------------------------------
        ///
        /**
          */
        SAXReader::
            SAXReader()
            : stop_flag_(false),
              document_uri_(""),
              is_(NULL)
        {
#ifdef ENABLE_LOGGING
           set_logger_name( "SAXReader" );
#endif
        }

		// ----------------------------------------------------------------------
        SAXReader::
            ~SAXReader()
        {
            reset();
        }

		// ----------------------------------------------------------------------
        void
        	SAXReader::
            set_document_uri( const std::string& s)
            throw()
        {
            document_uri_ = s;
        }

		// ----------------------------------------------------------------------
        void
        	SAXReader::
           interrupt()
           throw()
        {
            stop_flag_ = true;
        }

		// ----------------------------------------------------------------------
        void
        	SAXReader::
            reset()
            throw()
        {
            //Destroy the parser if existing
            if( is_ )
            {
            	delete is_;
                is_ = NULL;
            }

            stop_flag_ = false;
        }

		// ----------------------------------------------------------------------
        void
        	SAXReader::
            open_file()
            throw(std::runtime_error)
        {
        	assert( document_uri_.c_str() != NULL );

			is_ = new std::ifstream();
			is_->open(document_uri_.c_str(), std::ios::in);

			if( ! (*is_) )
			{
				ERROR(logger(), "Unable to open file (" << document_uri_.c_str() <<
					") for reading: " << strerror(errno));
				throw std::runtime_error("Unable to open XML file");
			}
        }

		// ----------------------------------------------------------------------
        void
        	SAXReader::
            parse()
            throw(std::runtime_error)
        {
            char buf[16384];
            int len;

            //Open the file
            open_file();

            //Create the SAX parser and register the C-Style handlers which will call back on this object instace
			parser = XML_ParserCreate(NULL);
			XML_SetUserData(parser, (void*)this);
			XML_SetElementHandler(parser, saxreader_start, saxreader_end);
			XML_SetCharacterDataHandler(parser, saxreader_text);

            //Read the file until the end of file
            while( !is_->eof() && !stop_flag_ )
            {
            	is_->read( buf, sizeof(buf) );
				len = is_->gcount();

				if (XML_Parse(parser, buf, len, is_->eof()) == XML_STATUS_ERROR)
				{
					 ERROR(logger(), XML_ErrorString(XML_GetErrorCode(parser)) <<
					         "at line " << XML_GetCurrentLineNumber(parser));
				    reset();
				    throw std::runtime_error("Error in parsing XML input");
				}
            }

            //Done -> Close the file and free all associated memory
			parsing_done();
            reset();
			XML_ParserFree(parser);
        }


        void SAXReader::parse_Object(XMLObj * flegsens_xml_obj) throw(std::runtime_error) {
            char buf[16384];
            int len;
            //Create the SAX parser and register the C-Style handlers which will call back on this object instace
			parser = XML_ParserCreate(NULL);
			XML_SetUserData(parser, (void*)this);
			XML_SetElementHandler(parser, saxreader_start, saxreader_end);

            while( !flegsens_xml_obj->eof() && !stop_flag_ ) {
            	flegsens_xml_obj->read( buf, sizeof(buf) );
				len = flegsens_xml_obj->gcount();

				if (XML_Parse(parser, buf, len, flegsens_xml_obj->eof()) == XML_STATUS_ERROR)
				{
					 ERROR(logger(), XML_ErrorString(XML_GetErrorCode(parser)) <<
					         "at line " << XML_GetCurrentLineNumber(parser));
				    reset();
				    throw std::runtime_error("Error in parsing XML input");
				}
            }
            //Done -> Close the file and free all associated memory
			parsing_done();
            reset();
			XML_ParserFree(parser);
        }

		// ----------------------------------------------------------------------
		void
			SAXReader::
			parsing_done()
			throw()
		{}

		// ----------------------------------------------------------------------
        void
        	SAXReader::
        	handle_start_element(std::string name, AttList& atts)
            throw(std::runtime_error)
        {}

		// ----------------------------------------------------------------------
        void
        	SAXReader::
        	handle_end_element(std::string name)
        	throw(std::runtime_error)
        {}

		// ----------------------------------------------------------------------
        void
        	SAXReader::
        	handle_text_element(std::string content)
        	throw(std::runtime_error)
        {}

        // *********************************************************************
        // *********************************************************************
        // *********************************************************************

		// ----------------------------------------------------------------------
        void
        	SAXSkipReader::
            reset()
            throw()
        {
            SAXReader::reset();
            clear_skip_target();
        }

		// ----------------------------------------------------------------------
        ///
        void
        	SAXSkipReader::
        	handle_start_element(std::string name, AttList& atts)
            throw(std::runtime_error)
        {
            SAXReader::handle_start_element(name, atts);
            if( check_skip_target_reached(name, atts, true) )
                skip_target_reached(name, atts);
        }

		// ----------------------------------------------------------------------
        void SAXSkipReader::
        	handle_end_element(std::string name)
            throw(std::runtime_error)
        {
            SAXReader::handle_end_element(name);
            AttList atts;
            if( check_skip_target_reached(name, atts, false) )
                skip_target_reached(name, atts);
        }



        // *********************************************************************
        // *********************************************************************
        // *********************************************************************


	    // ----------------------------------------------------------------------
        SAXSimpleSkipReader::
            SAXSimpleSkipReader()
           : skip_to_tag_( "" )
        {
            clear_skip_target();
        }

	    // ----------------------------------------------------------------------
        void
        	SAXSimpleSkipReader::
            set_skip_target(string name, bool opening_tag /* = true */)
        {
            assert(name != "");
            clear_skip_target();
            skip_to_tag_ = name;
            skip_to_opening_tag = opening_tag;
        }

		// ----------------------------------------------------------------------
        void
        	SAXSimpleSkipReader::
            clear_skip_target()
        {
            skip_to_tag_ = "";
            skip_to_tag_atts_.clear();
            skip_to_opening_tag = true;
            return;
        }

		// ----------------------------------------------------------------------
        /// Returns true if we are in skipping mode
        /**
          *
          */
        bool
        	SAXSimpleSkipReader::
            skipping()
        {
            return skip_to_tag_ != "";
        }

		// ----------------------------------------------------------------------
        void
        	SAXSimpleSkipReader::
            set_skip_target_add_attr(string name, string value)
        {
            skip_to_tag_atts_[name] = value;
        }

		// ----------------------------------------------------------------------
        bool
        	SAXSimpleSkipReader::
            check_skip_target_reached(string name, AttList& atts, bool opening_tag)
        {
            string tmp;

			//Check if the current tag name equals the needed skip target
			if( this->skip_to_tag_ != name )
				return false;

			//Check if we need to skip at all or the tag is of the same type
            if( !skipping() || opening_tag != skip_to_opening_tag)
                return false;

            //Check all attributes if we have reached an opening tag
            if( opening_tag )
            {
                for( str_str_map::iterator it = skip_to_tag_atts_.begin(); it != skip_to_tag_atts_.end(); it++)
                {
                    tmp = attribute( (*it).first, atts );

                    //Attribute not found -> No match
                    if(tmp == "")
                        return false;

                    //Attribute has wrong value -> No match
                    if( (*it).second != tmp )
                        return false;

                }
            }

            //Skip target reached
            clear_skip_target();
            return true;
        }

    }
}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/xml/sax_reader.cpp,v $
* Version $Revision: 550 $
* Date    $Date: 2011-04-18 10:21:12 +0200 (Mon, 18 Apr 2011) $
*-----------------------------------------------------------------------
* $Log: sax_reader.cpp,v $
 *-----------------------------------------------------------------------*/
