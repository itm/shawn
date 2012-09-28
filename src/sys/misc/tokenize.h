/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_TOKENIZE_H
#define __SHAWN_SYS_MISC_TOKENIZE_H

#include <string>
#include <vector>

namespace shawn
{
    ///String tokenizer helper class
    class StrTok
    {
    public:
        typedef std::vector<std::string> Result;
        typedef std::vector<std::string>::iterator iterator;
        typedef std::vector<std::string>::const_iterator const_iterator;

        StrTok(const std::string& str, const std::string& delimiters);
        StrTok::iterator begin();
        StrTok::const_iterator begin() const;
        StrTok::iterator end();
        StrTok::const_iterator end() const;
        StrTok::Result::size_type size() const;
        const StrTok::Result tokens() const;

    protected:
        Result res_;
    };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/tokenize.h,v $
 * Version $Revision: 475 $
 * Date    $Date: 2010-11-26 08:56:02 +0100 (Fri, 26 Nov 2010) $
 *-----------------------------------------------------------------------
 * $Log: tokenize.h,v $
 *-----------------------------------------------------------------------*/
