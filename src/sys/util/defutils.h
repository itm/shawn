/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_DEFUTILS_H
#define __SHAWN_SYS_DEFUTILS_H

#include <math.h>


#define DECLARE_HANDLES(CLASS)                            \
  class CLASS;                                            \
  typedef shawn::RefcntPointer<CLASS>       CLASS##Handle;       \
  typedef shawn::RefcntPointer<const CLASS> Const##CLASS##Handle

#define ABORT_NOT_IMPLEMENTED \
  {std::cerr << "SORRY, NOT IMPLEMENTED YET -- " << __FILE__ << ":" << __LINE__ <<std::endl; abort();}

#define ABORT_INCONSISTENT_CONFIGURATION(reason) \
  {std::cerr << "SORRY, THIS CONFIGURATION IS INCONSISTENT -- " << __FILE__ << ":" << __LINE__ << " -- " << reason << std::endl; abort();}


//#define EPSILON .00000001
#define EPSILON .00000000001

#define EQDOUBLE(a,b) (fabs((a)-(b))<EPSILON)

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/util/defutils.h,v $
 * Version $Revision: 374 $
 * Date    $Date: 2009-11-24 10:15:19 +0100 (Tue, 24 Nov 2009) $
 *-----------------------------------------------------------------------
 * $Log: defutils.h,v $
 *-----------------------------------------------------------------------*/
