/**
 * @file statistics_task.cpp
 * Implements the class vanet::StatisticsTask that collects and puts out
 * the simulation results.
 *
 * @author Lu Zeng
 * @author Walter Bamberger
 *
 * $Revision: 647 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/statistics/statistics_task.cpp $
 * $Id: statistics_task.cpp 647 2012-04-30 14:33:25Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Lu Zeng<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "statistics_task.h"
#ifdef ENABLE_VANET

#include "../knowledge_base/knowledge_base.h"
#include "../trust/credit_trust_model.h"
#include "../trust/trust_bamberger_2010.h"
#include "../vehicle/vehicle_processor.h"
#include "../vehicle/value_type_int_sensor.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/processors/processor_keeper.h"
#include "sys/world.h"
#include "sys/misc/tokenize.h"
#include <boost/tokenizer.hpp>
#include <iostream>
#include <fstream>
#include <limits>
#include <map>
#include <sstream>

using namespace boost;
using namespace shawn;
using namespace std;

namespace vanet
{

StatisticsTask* StatisticsTask::instance_ = 0;

// ----------------------------------------------------------------------

StatisticsTask::StatisticsTask( const SimulationController& sc ) :
         bamberger2010Filename_("bamberger2010.csv"), creditFilename_(
                  "credit.csv"), knowledgeBaseInfoFilename_(
                  "knowledgebase-info.csv"), vehicleVehicleFilename_(
                  "vehicle-vehicle.csv"), trustWatchFilename_(
                  "trust-watch.csv"), vehicleSensorFilename_("vehicle-sensor.csv"), simulationController_(
                  sc)
{
   DEBUG( logger(), "StatisticsTask ctor");

   initFile(
            knowledgeBaseInfoFilename_,
            "\"receiver\",\"knowledgeOfNodes\",\"locationGrid\",\"newestInformation\"\n");
   initFile(vehicleSensorFilename_, "vehicle,sensor type\n");
   initFile(
            vehicleVehicleFilename_,
            "\"week\",\"receiver\",\"sender\",\"number of meetings\",\"number of contracts\"\n");
}

// ----------------------------------------------------------------------

StatisticsTask::~StatisticsTask()
{
   DEBUG( logger(), "StatisticsTask dtor");
}

// ----------------------------------------------------------------------

std::string
StatisticsTask::description( void ) const throw ()
{
   return "Collects all the data of the VANET app for the output in configurable files.";
}

// ----------------------------------------------------------------------

StatisticsTask&
StatisticsTask::create( const SimulationController& sc ) throw ( runtime_error )
{
   if ( instance_ )
      throw(runtime_error("StatisticsTask may be initialised only once."));

   instance_ = new StatisticsTask(sc);
   return *instance_;
}

// ----------------------------------------------------------------------

StatisticsTask&
StatisticsTask::instance() throw ( bad_alloc )
{
   return *StatisticsTask::instance_;
}

// ----------------------------------------------------------------------

void
StatisticsTask::integratedReport( const IntReport* report,
         const string& receiver ) throw ( runtime_error )
{
   static ofstream logStream;
   static tokenizer<> watchList(
            simulationController_.environment().optional_string_param(
                     "trust_watch", ""));
   const string& sender = report->sender;
   if ( find(watchList.begin(), watchList.end(), sender) != watchList.end() )
   {
      EvidenceIntegrator & integrator =
               VehicleProcessor::labelComponentsMap[receiver].knowledgeBase->getEvidenceIntegrator();
      if ( integrator.hasTrustComputer() )
      {
         TrustComputer& trustComputer = integrator.getTrustComputer();
         int round = simulationController_.world().simulation_round();

         if ( !logStream.is_open() )
         {
            logStream.open(trustWatchFilename_.c_str(), ios_base::app);
            if ( !logStream.is_open() )
            {
               throw(runtime_error(
                        "Could not open file " + trustWatchFilename_
                                 + " for output."));
            }
            logStream.setf(ios_base::fixed, ios_base::floatfield);
            logStream.precision(4);

            static bool isCreated = false;
            if ( !isCreated )
            {
               initFile(
                        trustWatchFilename_,
                        "\"time\",\"receiver\"," + Report::getCsvHeader() + ","
                                 + trustComputer.getCsvHeader() + "\n");
            }
         }

         logStream << round << ",\"" << receiver << "\",";
         report->writeCsvRecord(logStream) << ",";
         trustComputer.computeCsvRecord(logStream, report) << endl;
      }
   }
}

// ----------------------------------------------------------------------

std::string
StatisticsTask::name( void ) const throw ()
{
   return "statistics_task";
}

// ----------------------------------------------------------------------

void
StatisticsTask::run( shawn::SimulationController & sc )
         throw ( std::runtime_error )
{
   /*
    43200:    1/2 day
    86400:    1 day
    432000:   5 days
    604800:   1 week
    9676800:  16 weeks
    15724800: 26 weeks
    */
   int output_interval = sc.environment().optional_int_param("output_interval",
            86400);
   int round = sc.world().simulation_round();

   if ( (round + 1) % output_interval == 0 )
   {
      writeVehicleVehicleFile(sc);
      writeKnowledgeBaseInfoFile(sc);
   }
   int max_iterations = sc.environment().optional_int_param("max_iterations",
            numeric_limits<int>::max());

   if ( round + 1 == max_iterations )
   {
      string trustComputer = sc.environment().optional_string_param(
               "trust_computer", "");
      if ( trustComputer == "credit" )
         writeCreditFile(sc);
      else if ( trustComputer == "bamberger2010" )
         writeBamberger2010File(sc);
      else if ( trustComputer != "" )
         runtime_error("Unknown trust computer");

      writeVehicleSensorFile(sc);
      writeKnowledgeBaseFiles(sc);
   }
}

// ----------------------------------------------------------------------

void
StatisticsTask::initFile( string fileName, string content )
         throw ( runtime_error )
{
   ofstream file;
   file.open(fileName.c_str());
   if ( file.is_open() && file.good() )
   {
      file << content;
      file.close();
   }
   else
   {
      throw(runtime_error("Could not open file " + fileName + " for output."));
   }
}

// ----------------------------------------------------------------------

void
StatisticsTask::writeBamberger2010File( const SimulationController& sc )
         throw ( runtime_error )
{
   static ofstream bamberger2010File;
   //
   // Open the output file in append mode
   //
   if ( !bamberger2010File.is_open() )
   {
      bamberger2010File.open(bamberger2010Filename_.c_str(), ios_base::app);
      if ( !bamberger2010File.is_open() )
      {
         throw(runtime_error(
                  "Could not open file " + bamberger2010Filename_
                           + " for output."));
      }
   }
   bamberger2010File.setf(ios_base::fixed, ios_base::floatfield);
   bamberger2010File.precision(4);

   //
   // Write the list
   //

   bool isHeaderWritten = false;

   for ( std::map<std::string, VehicleProcessor::Components>::iterator itComponentsMap =
            VehicleProcessor::labelComponentsMap.begin();
            itComponentsMap != VehicleProcessor::labelComponentsMap.end();
            itComponentsMap++ )
   {
      KnowledgeBase& kb = *itComponentsMap->second.knowledgeBase;
      const string& receiver = (*itComponentsMap).first;
      TrustComputer& trustComputer =
               kb.getEvidenceIntegrator().getTrustComputer();

      if ( !isHeaderWritten )
      {
         bamberger2010File
                  << "\"receiver\",\"sender\",\"info type\",\"contracts\","
                  << trustComputer.getCsvHeader() << endl;
         isHeaderWritten = true;
      }

      const KnowledgeBase::NodeKnowledgeMap& nodeKnowledge = kb.knowledgeOfNodes;
      for ( KnowledgeBase::NodeKnowledgeMap::const_iterator nodeIt =
               nodeKnowledge.begin(); nodeIt != nodeKnowledge.end(); ++nodeIt )
      {
         IntReport report;
         const string& sender = nodeIt->first;
         report.sender = sender;
         InformationDescription::Type type = InformationDescription::IntNone;
         float trustValue = 0.0f;
         size_t numberOfContracts = 0;

         try
         {
            type = InformationDescription::IntLong;
            report.type = type;
            numberOfContracts = kb.findAllReports(sender, type).size();
            bamberger2010File << "\"" << receiver << "\",\"" << sender
                     << "\",\"" << InformationDescription::typeNames[type]
                     << "\"," << numberOfContracts << ",";
            trustComputer.computeCsvRecord(bamberger2010File, &report) << "\n";
         }
         catch ( const out_of_range& oe )
         {
            // There was no such report of that type for this vehicle.
            // But that is ok.
         }

         try
         {
            type = InformationDescription::IntMedium;
            report.type = type;
            numberOfContracts = kb.findAllReports(sender, type).size();
            bamberger2010File << "\"" << receiver << "\",\"" << sender
                     << "\",\"" << InformationDescription::typeNames[type]
                     << "\"," << numberOfContracts << ",";
            trustComputer.computeCsvRecord(bamberger2010File, &report) << "\n";
         }
         catch ( const out_of_range& oe )
         {
            // There was no such report of that type for this vehicle.
            // But that is ok.
         }

         try
         {
            type = InformationDescription::IntShort;
            report.type = type;
            trustValue = trustComputer.computeTrustValue(&report).first;
            numberOfContracts = kb.findAllReports(sender, type).size();
            bamberger2010File << "\"" << receiver << "\",\"" << sender
                     << "\",\"" << InformationDescription::typeNames[type]
                     << "\"," << numberOfContracts << ",";
            trustComputer.computeCsvRecord(bamberger2010File, &report) << "\n";
         }
         catch ( const out_of_range& oe )
         {
            // There was no such report of that type for this vehicle.
            // But that is ok.
         }
      }
      bamberger2010File.flush();
   }

   //
   // Everything is done. Clean up.
   //
   bamberger2010File.close();
}

// ----------------------------------------------------------------------

void
StatisticsTask::writeVehicleVehicleFile( const SimulationController & sc )
         throw ( runtime_error )
{
   static ofstream vehicleVehicleFile;
   static int week = 0;
   ++week;
   //
   // Open the output file in append mode
   //
   if ( !vehicleVehicleFile.is_open() )
   {
      vehicleVehicleFile.open(vehicleVehicleFilename_.c_str(), ios_base::app);
      if ( !vehicleVehicleFile.is_open() )
      {
         throw(runtime_error(
                  "Could not open file " + vehicleVehicleFilename_
                           + " for output."));
      }
   }
   vehicleVehicleFile.setf(ios_base::fixed, ios_base::floatfield);
   vehicleVehicleFile.precision(4);

   //
   // Write the list
   //
   for ( std::map<std::string, VehicleProcessor::Components>::iterator itComponentsMap =
            VehicleProcessor::labelComponentsMap.begin();
            itComponentsMap != VehicleProcessor::labelComponentsMap.end();
            itComponentsMap++ )
   {
      KnowledgeBase& kb = *itComponentsMap->second.knowledgeBase;
      string receiver = (*itComponentsMap).first;
      string sender = "";
      int numberOfMeetings = 0;
      int numberOfContracts = 0;
      for ( KnowledgeBase::NodeKnowledgeMap::iterator itMeeting =
               kb.knowledgeOfNodes.begin();
               itMeeting != kb.knowledgeOfNodes.end(); itMeeting++ )
      {
         sender = (*itMeeting).first;
         numberOfMeetings = (*itMeeting).second.meetingCounter;
         numberOfContracts = (*itMeeting).second.reports.size();
         vehicleVehicleFile << week << ",\"" << receiver << "\",\"" << sender
                  << "\"," << numberOfMeetings << "," << numberOfContracts
                  << "\n";
      }
   }

   //
   // Everything is done. Clean up.
   //
   vehicleVehicleFile.close();
}

// ----------------------------------------------------------------------

void
StatisticsTask::writeCreditFile( const SimulationController & sc )
         throw ( runtime_error )
{
   static ofstream creditFile;
   //
   // Open the output file in append mode
   //
   if ( !creditFile.is_open() )
   {
      creditFile.open(creditFilename_.c_str(), ios_base::app);
      if ( !creditFile.is_open() )
      {
         throw(runtime_error(
                  "Could not open file " + creditFilename_ + " for output."));
      }
   }
   creditFile.setf(ios_base::fixed, ios_base::floatfield);
   creditFile.precision(4);

   //
   // Write the list
   //

   bool isHeaderWritten = false;

   for ( std::map<std::string, VehicleProcessor::Components>::iterator itComponentsMap =
            VehicleProcessor::labelComponentsMap.begin();
            itComponentsMap != VehicleProcessor::labelComponentsMap.end();
            itComponentsMap++ )
   {
      KnowledgeBase& kb = *itComponentsMap->second.knowledgeBase;
      string receiver = (*itComponentsMap).first;
      InformationDescription::Type typeOfInformation =
               InformationDescription::IntNone;
      TrustComputer& trustComputer =
               kb.getEvidenceIntegrator().getTrustComputer();

      if ( !isHeaderWritten )
      {
         creditFile << "\"receiver\",\"sender\",\"error rate of sender\","
                  << trustComputer.getCsvHeader() << endl;
         isHeaderWritten = true;
      }

      const KnowledgeBase::NodeKnowledgeMap& nodeKnowledge = kb.knowledgeOfNodes;
      for ( KnowledgeBase::NodeKnowledgeMap::const_iterator nodeIt =
               nodeKnowledge.begin(); nodeIt != nodeKnowledge.end(); ++nodeIt )
      {
         IntReport report;
         const string& sender = nodeIt->first;
         int errorType =
                  VehicleProcessor::labelComponentsMap[sender].sensor->getErrorType();
         report.sender = sender;
         InformationDescription::Type type = InformationDescription::IntNone;
         float trustValue = 1.0f;

         try
         {
            type = InformationDescription::IntLong;
            report.type = type;
            creditFile << receiver << "," << sender << "," << errorType << ",";
            trustComputer.computeCsvRecord(creditFile, &report) << "\n";
         }
         catch ( const out_of_range& oe )
         {
            // There was no such report of that type for this vehicle.
            // But that is ok.
         }

         try
         {
            type = InformationDescription::IntMedium;
            report.type = type;
            creditFile << receiver << "," << sender << "," << errorType << ",";
            trustComputer.computeCsvRecord(creditFile, &report) << "\n";
         }
         catch ( const out_of_range& oe )
         {
            // There was no such report of that type for this vehicle.
            // But that is ok.
         }

         try
         {
            type = InformationDescription::IntShort;
            report.type = type;
            creditFile << receiver << "," << sender << "," << errorType << ",";
            trustComputer.computeCsvRecord(creditFile, &report) << "\n";
         }
         catch ( const out_of_range& oe )
         {
            // There was no such report of that type for this vehicle.
            // But that is ok.
         }
      }
      creditFile.flush();
   }

   //
   // Everything is done. Clean up.
   //

   creditFile.close();
}

// ----------------------------------------------------------------------

void
StatisticsTask::writeKnowledgeBaseFiles( const SimulationController& sc )
         throw ( std::runtime_error )
{
   /* Make a table with the size of each knowledge base to find
    * the largest knowledge bases. I use the product of the known number of
    * vehicles and the number of pieces of information knowledge as the size of
    * the knowledge base. */
   multimap<size_t, pair<string, const KnowledgeBase*> > kbSizes;
   for ( map<string, VehicleProcessor::Components>::iterator itComponentsMap =
            VehicleProcessor::labelComponentsMap.begin();
            itComponentsMap != VehicleProcessor::labelComponentsMap.end();
            itComponentsMap++ )
   {
      const KnowledgeBase& kb = *itComponentsMap->second.knowledgeBase;
      const string& receiver = itComponentsMap->first;
      size_t size = kb.locationGrid.size() * kb.knowledgeOfNodes.size();
      kbSizes.insert(make_pair(size, make_pair(receiver, &kb)));
   }

   /* Initialize a file for each type of table: one for information knowledge
    * records, one for report records, and one for observation records. */
   ofstream informationFile;
   informationFile.open("information.csv");
   if ( !informationFile.is_open() )
      throw runtime_error(
               "StatisticsTask: Could not open the file information.csv");
   informationFile << "id,vehicle,location,type,begin,end,value,opinion\n";

   ofstream reportFile("report.csv");
   if ( !reportFile.is_open() )
      throw runtime_error("StatisticsTask: Could not open the file report.csv");
   reportFile << "id,iid," << Report::getCsvHeader() << "\n";

   ofstream observationFile("observation.csv");
   if ( !observationFile.is_open() )
      throw runtime_error(
               "StatisticsTask: Could not open the file observation.csv");
   observationFile << "id,iid,location,type,time,value,opinion\n";

   /* Write the ten biggest knowledge bases in the files. */
   int anzahl = 0;
   multimap<size_t, pair<string, const KnowledgeBase*> >::reverse_iterator it =
            kbSizes.rbegin();
   for ( ; it != kbSizes.rend() && anzahl < 20; ++it, anzahl++ )
   {
      const KnowledgeBase& kb = *(it->second.second);
      const string& receiver = it->second.first;
      writeKnowledgeBase(receiver, kb, informationFile, reportFile,
               observationFile);
   }
}

// ----------------------------------------------------------------------

void
StatisticsTask::writeKnowledgeBase( const std::string& vehicleLabel,
         const KnowledgeBase& kb, ostream& informationFile, ostream& reportFile,
         ostream& observationFile )
{
   for ( KnowledgeBase::LocationGrid::const_iterator lgIt =
            kb.locationGrid.begin(); lgIt != kb.locationGrid.end(); ++lgIt )
   {
      IntInformationKnowledge& i =
               dynamic_cast<IntInformationKnowledge&>(*(lgIt->second));
      informationFile << &i << "," << vehicleLabel << "," << i.location << ","
               << InformationDescription::typeNames[i.type] << "," << i.begin
               << "," << i.end << "," << i.value << "," << i.opinion << "\n";

      IntReportList& reports = i.reports;
      for ( IntReportList::const_iterator rIt = reports.begin();
               rIt != reports.end(); ++rIt )
      {
         IntReport& r = **rIt;
         reportFile << &r << "," << &i << ",";
         r.writeCsvRecord(reportFile) << "\n";
      }

      IntObservationList& observations = i.observations;
      for ( IntObservationList::const_iterator oIt = observations.begin();
               oIt != observations.end(); ++oIt )
      {
         IntObservation& o = **oIt;
         observationFile << &o << "," << &i << "," << o.location << ","
                  << InformationDescription::typeNames[o.type] << "," << o.time
                  << "," << o.value << "," << o.opinion << "\n";
      }
   }
}

// ----------------------------------------------------------------------

void
StatisticsTask::writeKnowledgeBaseInfoFile( const SimulationController& sc )
         throw ( std::runtime_error )
{
   static ofstream knowledgeBaseInfoFile;
   //
   // Open the output file in append mode
   //
   if ( !knowledgeBaseInfoFile.is_open() )
   {
      knowledgeBaseInfoFile.open(knowledgeBaseInfoFilename_.c_str(),
               ios_base::app);
      if ( !knowledgeBaseInfoFile.is_open() )
      {
         throw(runtime_error(
                  "Could not open file " + knowledgeBaseInfoFilename_
                           + " for output."));
      }
   }
   knowledgeBaseInfoFile.setf(ios_base::fixed, ios_base::floatfield);
   knowledgeBaseInfoFile.precision(4);

   //
   // Write the list
   //
   for ( std::map<std::string, VehicleProcessor::Components>::iterator itComponentsMap =
            VehicleProcessor::labelComponentsMap.begin();
            itComponentsMap != VehicleProcessor::labelComponentsMap.end();
            itComponentsMap++ )
   {
      KnowledgeBase& kb = *itComponentsMap->second.knowledgeBase;
      string receiver = (*itComponentsMap).first;
      int sizeKnowledgeOfNodes = kb.knowledgeOfNodes.size();
      int sizeLocationGrid = kb.locationGrid.size();
      int sizeNewestInformation = kb.newestInformation.size();
      knowledgeBaseInfoFile << receiver << "," << sizeKnowledgeOfNodes << ","
               << sizeLocationGrid << "," << sizeNewestInformation << ","
               << "\n";
   }
   //
   // Everything is done. Clean up.
   //
   knowledgeBaseInfoFile.close();
}

void
StatisticsTask::writeVehicleSensorFile( const SimulationController& sc )
         throw ( std::runtime_error )
{

   static ofstream vehicleSensorFile;
   //
   // Open the output file in append mode
   //
   if ( !vehicleSensorFile.is_open() )
   {
      vehicleSensorFile.open(vehicleSensorFilename_.c_str(), ios_base::app);
      if ( !vehicleSensorFile.is_open() )
      {
         throw(runtime_error(
                  "Could not open file " + vehicleSensorFilename_ + " for output."));
      }
   }
   vehicleSensorFile.setf(ios_base::fixed, ios_base::floatfield);
   vehicleSensorFile.precision(4);

   //
   // Write the list
   //
   for ( std::map<std::string, VehicleProcessor::Components>::iterator itComponentsMap =
            VehicleProcessor::labelComponentsMap.begin();
            itComponentsMap != VehicleProcessor::labelComponentsMap.end();
            itComponentsMap++ )
   {
      string receiver = (*itComponentsMap).first;
      int errorType = itComponentsMap->second.sensor->getErrorType();
      vehicleSensorFile << "\"" << receiver << "\",S" << errorType << "\n";
   }
   //
   // Everything is done. Clean up.
   //
   vehicleSensorFile.close();
}

}

#endif /* ENABLE_VANET */
