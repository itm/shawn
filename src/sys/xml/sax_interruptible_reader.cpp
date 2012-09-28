/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "shawn_config.h"
#include "sys/xml/sax_interruptible_reader.h"

#include <assert.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

namespace shawn
{
	namespace xml
	{
		// ----------------------------------------------------------------------
		SAXInterruptibleReader::
			SAXInterruptibleReader()
			: initialized_(false)
		{}

		// ----------------------------------------------------------------------
		SAXInterruptibleReader::
			~SAXInterruptibleReader()
		{}

		// ----------------------------------------------------------------------
		void SAXInterruptibleReader::
			parse()
			throw(runtime_error)
		{
			char buf[16384];
			int len;

			stop_flag_ = false;

			if(!initialized_)
				open_file();

			while(!cache_.empty() && !stop_flag_)
			{
				struct CacheData* cd = cache_.front();

				switch(cd->tag_type)
				{
				case SIR_OPENTAG:
				   start_element(cd->name, cd->atts);
				   break;
				case SIR_TEXTTAG:
				   text_element(cd->name);
				   break;
				case SIR_CLOSETAG:
				   end_element(cd->name);
				   break;
				}

				cache_.pop_front();
				delete cd;
			}

			//Read the file until the end of file
            while( !is_->eof() && !stop_flag_ )
            {
            	is_->read( buf, sizeof(buf) );
				len = is_->gcount();

				if (XML_Parse(parser, buf, len, is_->eof()) == XML_STATUS_ERROR)
				{
					std::cerr << XML_ErrorString(XML_GetErrorCode(parser)) << "at line " << XML_GetCurrentLineNumber(parser) << std::endl;
				    reset();
				    throw std::runtime_error("Error in parsing XML input");
				}
            }

			//Done -> Close the file and free all associated memory
			if( !stop_flag_ )
			{
				parsing_done();
				reset();
			}
		}

		// ----------------------------------------------------------------------
		void SAXInterruptibleReader::
			handle_start_element(string name, AttList& atts)
			throw(runtime_error)
		{
			string _name = string(name);

			if( stop_flag_ )
			{
				CacheData* cd = new CacheData;
				cd->tag_type = SIR_OPENTAG;
				cd->name = _name ;
				cd->atts = atts;
				cache_.push_back(cd);
			}
			else
			{
				start_element(_name, atts);
			}
		}

		// ----------------------------------------------------------------------
       void SAXInterruptibleReader::
           handle_text_element(string content)
           throw(runtime_error)
       {
           string n = string(content);
           if( stop_flag_ )
           {
               CacheData* cd = new CacheData;
               cd->tag_type = SIR_TEXTTAG;
               cd->name = n;
               cache_.push_back(cd);
           }
           else
               text_element(n);
       }


		// ----------------------------------------------------------------------
		void SAXInterruptibleReader::
			handle_end_element(string name)
			throw(runtime_error)
		{
			string n = string(name);
			if( stop_flag_ )
			{
				CacheData* cd = new CacheData;
				cd->tag_type = SIR_CLOSETAG;
				cd->name = n;
				cache_.push_back(cd);
			}
			else
				end_element(n);
		}

	}
}


/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/xml/sax_interruptible_reader.cpp,v $
* Version $Revision: 395 $
* Date    $Date: 2010-04-02 20:21:59 +0200 (Fri, 02 Apr 2010) $
*-----------------------------------------------------------------------
* $Log: sax_interruptible_reader.cpp,v $
 *-----------------------------------------------------------------------*/
