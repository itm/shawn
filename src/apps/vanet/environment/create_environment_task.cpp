/**
 * @file create_environment_task.cpp
 * Implements the task vanet::CreateEnvironmentTask.
 *
 * @author Walter Bamberger
 *
 * $Revision: 536 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/environment/create_environment_task.cpp $
 * $Id: create_environment_task.cpp 536 2011-04-12 08:29:33Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "create_environment_task.h"
#ifdef ENABLE_VANET

#include "value_type_int_reading.h"
#include "apps/reading/readings/integer_reading_random_factory.h"
#include "apps/reading/readings/reading_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/xml/sax_reader.h"
#include <iostream>
#include <sstream>

using namespace reading;
using namespace shawn;
using namespace shawn::xml;
using namespace std;

namespace vanet
{

class CreateEnvironmentTask::ReadingsFileReader : public shawn::xml::SAXReader
{
public:
   ReadingsFileReader( const std::string& fileName,
            std::list<ReadingEvent>* events ) :
      events_(*events), fileName_(fileName)
   {
      set_document_uri(fileName);
   }

protected:
   virtual void
   handle_start_element( std::string name, shawn::xml::AttList& atts )
            throw ( std::runtime_error )
   {
      if ( name == "value" )
      {
         ReadingEvent event;

         string startTimeString = attribute("startTime", atts, "0");
         istringstream(startTimeString) >> event.startTime;
         /// @todo Test the resulting stream with .fail() (or directly the stream in if?)

         string stopTimeString = attribute("stopTime", atts, "0");
         istringstream(stopTimeString) >> event.stopTime;

         /// @todo Is there a better way for tokenising the position attribute?
         string positionString = attribute("position", atts, "0,0");
         double x, y;
         istringstream positionStream(positionString);
         positionStream >> x;
         positionStream.ignore();
         positionStream >> y;
         event.location = Vec(x, y, 0.0);

         string typeString = attribute("type", atts, "INT_LONG");
         if ( typeString == "IntShort" )
         {
            event.type = InformationDescription::IntShort;
         }
         else if ( typeString == "IntMedium" )
         {
            event.type = InformationDescription::IntMedium;
         }
         else if ( typeString == "IntLong" )
         {
            event.type = InformationDescription::IntLong;
         }
         else
         {
            throw(runtime_error("Unknown event type " + typeString
                     + " in readings file " + fileName_));
         }

         /// @todo value should be text data (as the element name indicates)
         string valueString = attribute("value", atts, "0");
         istringstream(valueString) >> event.value;

         events_.push_back(event);
      }

   }

private:
   const string fileName_;
   std::list<ReadingEvent>& events_;
};

CreateEnvironmentTask::CreateEnvironmentTask()
{
   DEBUG(logger(), "CreateEnvironmentTask ctor");
}

// ----------------------------------------------------------------------

CreateEnvironmentTask::~CreateEnvironmentTask()
{
   DEBUG(logger(), "CreateEnvironmentTask dtor");
}

// ----------------------------------------------------------------------

std::string
CreateEnvironmentTask::name( void ) const throw ()
{
   return "create_environment";
}

// ----------------------------------------------------------------------

std::string
CreateEnvironmentTask::description( void ) const throw ()
{
   return "Sets up the environment (the readings) for the VANET simulation";
}

// ----------------------------------------------------------------------

void
CreateEnvironmentTask::run( shawn::SimulationController& sc )
         throw ( std::runtime_error )
{
   INFO( logger(), "Run create_environment task");
   const string readingsFileName = sc.environment().required_string_param(
            "readings_file");

   list<ReadingEvent> events;
   ReadingsFileReader(readingsFileName, &events).parse();
   for ( list<ReadingEvent>::const_iterator it = events.begin(); it
            != events.end(); ++it )
   {
      DEBUG(logger(), it->startTime << " " << it->stopTime << " " << it->type
               << " " << it->value << " " << it->location);
   }

   /// @todo Test if sc has world
   ValueTypeIntReading* testIntegerReading = new ValueTypeIntReading(
            sc.world(), events);
   ReadingKeeper* readingKeeper = sc.keeper_by_name_w<ReadingKeeper> (
            "ReadingKeeper");
   if ( readingKeeper != NULL )
   {
      readingKeeper->add(testIntegerReading);
   }
   else
   {
      ERROR( logger(), "ReadingKeeper not found");
   }

   INFO( logger(), "Created test_integer_reading" );
}

}

#endif /* ENABLE_VANET */
