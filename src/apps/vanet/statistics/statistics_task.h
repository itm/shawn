/**
 * @file statistics_task.h
 * Defines the class vanet::StatisticsTask that collects and puts out
 * the simulation results.
 *
 * @author Lu Zeng
 * @author Walter Bamberger
 *
 * $Revision: 647 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/statistics/statistics_task.h $
 * $Id: statistics_task.h 647 2012-04-30 14:33:25Z simsiem $
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

#ifndef __SHAWN_APPS_VANET_STATISTICS_STATISTICS_TASK_H
#define __SHAWN_APPS_VANET_STATISTICS_STATISTICS_TASK_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "sys/simulation/simulation_task.h"

namespace vanet
{

class IntReport;
class KnowledgeBase;

class StatisticsTask : public shawn::SimulationTask
{
public:
   virtual
   ~StatisticsTask();

   static StatisticsTask&
   create( const shawn::SimulationController& sc ) throw ( std::runtime_error );

   static StatisticsTask&
   instance() throw ( std::bad_alloc );

   virtual std::string
   name( void ) const throw ();

   virtual std::string
   description( void ) const throw ();

   virtual void
   run( shawn::SimulationController& sc ) throw ( std::runtime_error );

   virtual void
   integratedReport( const IntReport* report, const std::string& receiver )
            throw ( std::runtime_error );

private:
   static StatisticsTask* instance_;

   const std::string bamberger2010Filename_;
   const std::string creditFilename_;
   const std::string knowledgeBaseInfoFilename_;
   const std::string trustWatchFilename_;
   const std::string vehicleSensorFilename_;
   const std::string vehicleVehicleFilename_;
   const shawn::SimulationController& simulationController_;

   StatisticsTask( const shawn::SimulationController& sc );

   void
   initFile( std::string fileName, std::string content )
            throw ( std::runtime_error );

   void
   writeVehicleVehicleFile( const shawn::SimulationController& sc )
            throw ( std::runtime_error );

   void
   writeCreditFile( const shawn::SimulationController& sc )
            throw ( std::runtime_error );

   void
   writeBamberger2010File( const shawn::SimulationController& sc )
            throw ( std::runtime_error );

   void
   writeModifyingDisseminatorVehicleFile(
            const shawn::SimulationController& sc ) throw ( std::runtime_error );

   void
   writeKnowledgeBaseFiles( const shawn::SimulationController& sc )
            throw ( std::runtime_error );

   void
   writeKnowledgeBase( const std::string& vehicleLabel,
            const KnowledgeBase& kb, std::ostream& informationFile,
            std::ostream& reportFile, std::ostream& observationFile );

   void
   writeKnowledgeBaseInfoFile( const shawn::SimulationController& sc )
            throw ( std::runtime_error );

   void
   writeVehicleSensorFile( const shawn::SimulationController& sc )
            throw ( std::runtime_error );
};

}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_STATISTICS_STATISTICS_TASK_H */
