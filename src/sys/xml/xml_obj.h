/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_XML_OBJ_H
#define __SHAWN_SYS_XML_OBJ_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

namespace shawn
{

    namespace xml 
    {

		class XMLObj {
		public:
			///
			XMLObj();
			
			virtual ~XMLObj();

			bool eof();

			void read(char * buf, int len );

			int gcount();
						
			void appendLine(std::string line);

		private:
			std::vector<std::string> xml_data_;
			int current_line_read_;			
		};

    }

}

#endif
