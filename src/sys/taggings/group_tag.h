/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TAGGINGS_GROUP_TAG_H
#define __SHAWN_SYS_TAGGINGS_GROUP_TAG_H

#include "sys/tag.h"

namespace shawn
{

	class GroupTag
		: public Tag
	{
	private:
		static const std::string TAG_TYPE;

	public:
		///@name Ctor, Dtor
		///@{

		///
		GroupTag( const std::string& n, bool lock = false );
		///
		virtual ~GroupTag();
		///@}

		///
		virtual const std::string & type_identifier (void) const throw ();

		///
		virtual std::string encoded_content (void) const throw ( std::runtime_error );

		friend class GroupTagFactory;
	};


}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/taggings/group_tag.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: group_tag.h,v $
 *-----------------------------------------------------------------------*/
