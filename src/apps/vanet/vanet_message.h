/**
 * @file vanet_message.h
 * Defines several message structures.
 *
 * @author Walter Bamberger
 *
 * $Revision: 535 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/vanet_message.h $
 * $Id: vanet_message.h 535 2011-04-12 00:18:18Z andylz $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_MESSAGE_H
#define __SHAWN_APPS_VANET_MESSAGE_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "environment/information_description.h"
#include "opinion.h"
#include "sys/vec.h"
#include "sys/message.h"

namespace vanet
{
  class VanetMessage : public shawn::Message
  {
  public:
    virtual
    ~VanetMessage()
    {
    }

    // Information from the observation
    shawn::Vec location;
    InformationDescription::Type type;
    int time;
  };

  class VanetIntMessage : public VanetMessage
  {
  public:
    virtual
    ~VanetIntMessage()
    {
    }

    IntOpinion opinion;
    int value;
  };

  class VanetTrustMessage : public VanetMessage
  {
    public:
      virtual
      ~VanetTrustMessage()
      {
      }
      std::string name;
      double trustValue;
  };
}

#endif
#endif
