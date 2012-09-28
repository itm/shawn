/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/xml/xml_obj.h"


namespace shawn
{
	namespace xml
	{

		XMLObj::XMLObj():xml_data_(0) {
			current_line_read_ = -1;
		}

		// ----------------------------------------------------------------------
		XMLObj::~XMLObj() {
			// de-allocate memory
			xml_data_.clear();
		}


		bool XMLObj::eof() {
			// cast current_line_read_ to avoid implicit integer conversion
			if ((current_line_read_<0) || (static_cast<unsigned long>(current_line_read_) < xml_data_.size())) {
				return false;
			} else {
				return true;
			}
		}


		void XMLObj::read(char * buf, int len ) {
			current_line_read_ = current_line_read_+1;
			std::string temp = *(xml_data_.end()-(xml_data_.size()-current_line_read_));
			for (unsigned int i=0; i<temp.size(); i++) {
				buf[i] = temp[i];
			}
		}

		int XMLObj::gcount() {
			std::string temp = *(xml_data_.end()-(xml_data_.size()-current_line_read_));
			int gcount = temp.size();
			return gcount;
		}

		void XMLObj::appendLine(std::string line) {
			xml_data_.push_back(line);
			// std::cerr << "appendLine " << *(xml_data_.end()-1) << std::endl;
		}

	}
}

