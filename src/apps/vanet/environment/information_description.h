/**
 * @file information_description.h
 * Defines data structures that describe the kinds of information
 * in the environment.
 *
 * @author Walter Bamberger
 *
 * $Revision: 527 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/environment/information_description.h $
 * $Id: information_description.h 527 2011-04-11 18:29:43Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_ENVIRONMENT_INFORMATION_DESCRIPTION_H
#define __SHAWN_APPS_VANET_ENVIRONMENT_INFORMATION_DESCRIPTION_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include <string>

namespace vanet
{

/**
 * Types and data that describe the information exchanged in the VANET.
 */
class InformationDescription
{
public:
   /**
    * Kinds of information that can be perceived by the sensors. These are also
    * the content types of the VANET messages.
    */
   enum Type
   {
      OwnMovement, IntSituational, IntShort, IntMedium, IntLong, IntNone
   };

   /**
    * For every type, the maximum value if the type is an integer type.
    *
    * For example, maxIntValues[IntShort] gives you the maximum value,
    * such an reading can give. At present, the minimum value is assumed to
    * always be zero.
    */
   static const int maxIntValues[6];

   /**
    * The name of every type of information. This array maps the value of #Type
    * to a string.
    */
   static const std::string typeNames[6];
};

}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_ENVIRONMENT_INFORMATION_DESCRIPTION_H */
