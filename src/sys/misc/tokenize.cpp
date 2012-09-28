/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/misc/tokenize.h"

using namespace std;

namespace shawn
{

    // ----------------------------------------------------------------------
    StrTok::
        StrTok(const std::string& str, const std::string& delimiters)
    {
        if( str.empty() )
            return;

        string::size_type lastPos = str.find_first_not_of(delimiters, 0);
        string::size_type pos = str.find_first_of(delimiters, lastPos);

        while (string::npos != pos || string::npos != lastPos)
        {
            res_.push_back(str.substr(lastPos, pos - lastPos));
            lastPos = str.find_first_not_of(delimiters, pos);
            pos = str.find_first_of(delimiters, lastPos);
        }
    }

    // ----------------------------------------------------------------------
    StrTok::iterator
        StrTok::
        begin()
    {
        return res_.begin();
    }

    // ----------------------------------------------------------------------
    StrTok::const_iterator
        StrTok::
        begin() const
    {
        return res_.begin();
    }

    // ----------------------------------------------------------------------
    StrTok::iterator
        StrTok::
        end()
    {
        return res_.end();
    }

    // ----------------------------------------------------------------------
    StrTok::const_iterator
        StrTok::
        end() const
    {
        return res_.end();
    }

    // ----------------------------------------------------------------------
    StrTok::Result::size_type
        StrTok::
        size()
            const
    {
        return res_.size();
    }

    // ----------------------------------------------------------------------
    const StrTok::Result
        StrTok::
        tokens()
            const
    {
        return res_;
    }


}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/misc/tokenize.cpp,v $
* Version $Revision: 475 $
* Date    $Date: 2010-11-26 08:56:02 +0100 (Fri, 26 Nov 2010) $
*-----------------------------------------------------------------------
* $Log: tokenize.cpp,v $
 *-----------------------------------------------------------------------*/
