/**
 * @file value_type_int_reading.h
 * Defines the reading vanet::ValueTypeIntReading and
 * the data structure vanet::ReadingEvent.
 *
 * @author Walter Bamberger
 *
 * $Revision: 532 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/environment/value_type_int_reading.h $
 * $Id: value_type_int_reading.h 532 2011-04-11 18:46:54Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_ENVIRONMENT_VALUE_TYPE_INT_READING_H
#define __SHAWN_APPS_VANET_ENVIRONMENT_VALUE_TYPE_INT_READING_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "information_description.h"
#include "../utility.h"
#include "apps/reading/readings/reading.h"
#include <list>

namespace shawn
{
  class World;
}

namespace vanet
{

  struct ReadingEvent
  {
    int startTime;
    int stopTime;
    shawn::Vec location;
    InformationDescription::Type type;
    int value;
  };

  class ValueTypeIntReading : public reading::Reading
  {
  public:
    ValueTypeIntReading(const shawn::World& world,
        const std::list<ReadingEvent>& events);

    virtual
    ~ValueTypeIntReading();

    /**
     * This reading gives back its value and value type together with the
     * position of the value.
     *
     * The reading searches for a value within a predefined range. If it finds
     * one, it gives it back together with the exact position.
     *
     * @todo: The search range is in reality a property of the sensor. So maybe,
     * I should call this reading a RangeReading and add a range argument to the
     * value method.
     *
     * @todo: What happens, if there are two events within the range.
     */
    virtual triplet<shawn::Vec, InformationDescription::Type, int>
    value(const shawn::Vec& v) const throw ();

    virtual std::string
    name(void) const throw ();
    virtual std::string
    description(void) const throw ();

  private:
    static const double radius = 6.0;
    const shawn::World& world_;
    const std::list<ReadingEvent> events_;
  };

}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_ENVIRONMENT_VALUE_TYPE_INT_READING_H */
