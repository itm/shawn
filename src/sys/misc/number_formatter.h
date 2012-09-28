/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_NUMBER_FORMATTER_H
#define __SHAWN_SYS_MISC_NUMBER_FORMATTER_H

#include <string>
#include <ostream>

namespace shawn
{
    
   class DoubleFormatter
   {
       int digits_;
       int width_;
       double value_;
   public:
        DoubleFormatter(double val);
        DoubleFormatter(double val, int digits);
        DoubleFormatter(double val , int digits , int width);
   
        friend std::ostream& operator<<(std::ostream&, const DoubleFormatter&);
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/number_formatter.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: number_formatter.h,v $
 *-----------------------------------------------------------------------*/
