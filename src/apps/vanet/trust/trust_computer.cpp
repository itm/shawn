/**
 * @file trust_computer.cpp
 * Default implementation for some methods of the interface vanet::TrustComputer.
 *
 * @author Walter Bamberger
 *
 * $Revision: 590 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/trust/trust_computer.cpp $
 * $Id: trust_computer.cpp 590 2011-05-11 11:56:36Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "trust_computer.h"
#ifdef ENABLE_VANET

#include <ostream>

using namespace std;

namespace vanet
{

ostream&
TrustComputer::computeCsvRecord( ostream& os, const Report* report )
{
   pair<float, float> trustValue = computeTrustValue(report);
   return os << trustValue.first << "," << trustValue.second;
}

string
TrustComputer::getCsvHeader() const
{
   return "\"trust value\",\"evidence\"";
}

}

#endif /* ENABLE_VANET */
