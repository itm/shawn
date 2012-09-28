/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/misc/number_formatter.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

namespace shawn
{
 
    // ----------------------------------------------------------------------
    DoubleFormatter::
        DoubleFormatter(double val)
        : digits_(10),
          width_(10+5),
          value_(val)
    {}
        
    // ----------------------------------------------------------------------
    DoubleFormatter::
        DoubleFormatter(double val, int digits)
        : digits_(digits),
          width_(digits+5),
          value_(val)
    {}
        
    // ----------------------------------------------------------------------
    DoubleFormatter::
        DoubleFormatter(double val, int digits, int width)
        : digits_(digits),
          width_(width),
          value_(val)
    {}
   
    // ----------------------------------------------------------------------
    ostream& operator<<(ostream& os, const DoubleFormatter& df)
    {
        ostringstream oss;
        oss << setprecision(df.digits_) << setw(df.width_) << df.value_;

        string s = oss.str();
        
        size_t i = s.find(".", 0);
        if(i != string::npos)
            s.replace(i, 1, ",");        
        
        return os << s;
    }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/number_formatter.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: number_formatter.cpp,v $
 *-----------------------------------------------------------------------*/
