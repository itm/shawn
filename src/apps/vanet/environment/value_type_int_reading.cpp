/**
 * @file value_type_int_reading.cpp
 * Implements the reading vanet::ValueTypeIntReading.
 *
 * @author Walter Bamberger
 *
 * $Revision: 536 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/environment/value_type_int_reading.cpp $
 * $Id: value_type_int_reading.cpp 536 2011-04-12 08:29:33Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "value_type_int_reading.h"
#ifdef ENABLE_VANET

#include "sys/world.h"

using namespace shawn;
using namespace std;

namespace vanet
{

ValueTypeIntReading::ValueTypeIntReading( const World& world, const list<
      ReadingEvent>& events ) :
   world_( world ), events_( events )
{
   DEBUG(logger(), "ValueTypeIntReading ctor");
}

ValueTypeIntReading::~ValueTypeIntReading()
{
   DEBUG(logger(), "ValueTypeIntReading dtor");
}

triplet<Vec, InformationDescription::Type, int>
ValueTypeIntReading::value( const Vec& v ) const throw()
{
   /// @todo Speed up with a list of events that are currently active and a value
   /// when this list expires (the earliest stop of an event)
   int time = world_.simulation_round();
   for ( list<ReadingEvent>::const_iterator it = events_.begin(); it
         != events_.end(); ++it )
   {
      if ( time >= it->startTime && time < it->stopTime
            && ( v - it->location ).euclidean_norm() < radius )
      {
         return make_triplet( it->location, it->type, it->value );
      }
   }
   return make_triplet( v, InformationDescription::IntNone, 0 );
}

string
ValueTypeIntReading::name( void ) const throw()
{
   return "ValueTypeIntReading";
}

string
ValueTypeIntReading::description( void ) const throw()
{
   return "Provides integer values and an enum for the meaning of the this value.";
}

}

#endif /* ENABLE_VANET */
