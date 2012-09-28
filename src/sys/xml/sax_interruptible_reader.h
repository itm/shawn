/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_SYS_XML_SAX_INTERRUPTIBLE_READER_H
#define __SHAWN_SYS_XML_SAX_INTERRUPTIBLE_READER_H

#include <stdexcept>
#include <string>
#include <list>
#include <map>

#include "sys/xml/sax_reader.h"

namespace shawn
{
    namespace xml
    {

        // -------------------------------------------------------------------
        /**
        *
        */
		class SAXInterruptibleReader
			: public SAXReader
        {
		private:

		   enum TagType
		   {
		      SIR_OPENTAG,
		      SIR_TEXTTAG,
		      SIR_CLOSETAG
		   };
	        // -------------------------------------------------------------------
	        /**
	        *
	        */
			struct CacheData
			{
				std::string name;
				AttList atts;
				TagType tag_type;
			};

			typedef std::list<struct CacheData*> Cache;

        public:
            ///@name Construction / Destruction
            ///@{

            SAXInterruptibleReader();
            virtual ~SAXInterruptibleReader();
            ///@}

            ///@name Configuration and control
            ///@{

            ///
            void parse() throw(std::runtime_error);
            ///@}

        protected:

			virtual void start_element(std::string name, AttList& atts) throw(std::runtime_error) = 0;
			virtual void text_element(std::string content) throw(std::runtime_error) = 0;
			virtual void end_element(std::string name) throw(std::runtime_error) = 0;

            virtual void handle_start_element(std::string name, AttList& atts) throw(std::runtime_error);
            virtual void handle_end_element(std::string name) throw(std::runtime_error);
            virtual void handle_text_element(std::string name) throw(std::runtime_error);

		private:
			Cache cache_;
			bool initialized_;
        };
	}
}
#endif
