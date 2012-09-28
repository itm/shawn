/**
 * @file information_description.cpp
 * Implements data structures that describe the kinds of information
 * in the environment.
 *
 * @author Walter Bamberger
 *
 * $Revision: 533 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/environment/information_description.cpp $
 * $Id: information_description.cpp 533 2011-04-11 18:50:13Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "information_description.h"
#ifdef ENABLE_VANET

using namespace std;

namespace vanet
{

const int InformationDescription::maxIntValues[6] = { 0, 4, 4, 4, 4, 0 };

const string InformationDescription::typeNames[6] = { string("ownMovement"),
         string("IntSituational"), string("IntShort"), string("IntMedium"),
         string("IntLong"), string("IntNone") };

}

#endif /* ENABLE_VANET */
