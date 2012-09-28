/**
 * @file knowledge_base.cpp
 * Implements the simple data base class vanet::KnowledgeBase.
 *
 * @author Walter Bamberger
 *
 * $Revision: 639 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/knowledge_base/knowledge_base.cpp $
 * $Id: knowledge_base.cpp 639 2011-12-01 07:51:59Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "knowledge_base.h"
#ifdef ENABLE_VANET

#include "sys/simulation/simulation_controller.h"
#include <fstream>
#include <sstream>

using namespace std;
using namespace shawn;

namespace vanet
{
// Static class member. Used when an empty report list must be returned.
const ReportList KnowledgeBase::emptyReportList;

KnowledgeBase::KnowledgeBase( const SimulationController& sc,
         const string& vehicleLabel, const __LOGGER_OBJECT & logger ) :
         logger_(logger), simulationController_(sc), informationIntegrator(
                  *this, vehicleLabel, sc, logger), last_pruned(-1)
{
}

// ----------------------------------------------------------------------

KnowledgeBase::~KnowledgeBase()
{
   for ( LocationGrid::iterator l = locationGrid.begin();
            l != locationGrid.end(); ++l )
   {
      InformationKnowledge* i = l->second;
      IntInformationKnowledge* int_i = dynamic_cast<IntInformationKnowledge*>(i);
      if ( int_i )
      {
         IntReportList& reports = int_i->reports;
         for ( IntReportList::iterator r = reports.begin(); r != reports.end();
                  ++r )
         {
            delete *r;
         }

         IntObservationList& observations = int_i->observations;
         for ( IntObservationList::iterator o = observations.begin();
                  o != observations.end(); ++o )
         {
            delete *o;
         }
      }
      delete i;
   }
}

// ----------------------------------------------------------------------

template<>
   void
   KnowledgeBase::addReport<TrustReport>( TrustReport* newReport )
   {
      knowledgeOfNodes[newReport->reportedNode].trustReports.insert(newReport);
   }

// ----------------------------------------------------------------------

const ReportList&
KnowledgeBase::findAllReports( const std::string& nodeLabel ) const
{
   NodeKnowledgeMap::const_iterator nodeIt = knowledgeOfNodes.find(nodeLabel);
   if ( nodeIt == knowledgeOfNodes.end() )
      return emptyReportList;
   else
      return nodeIt->second.reports;
}

// ----------------------------------------------------------------------

ReportList
KnowledgeBase::findAllReports( const string& nodeLabel,
         const InformationDescription::Type& type ) const throw ()
{
   ReportList resultList;
   NodeKnowledgeMap::const_iterator nodeIt = knowledgeOfNodes.find(nodeLabel);
   if ( nodeIt != knowledgeOfNodes.end() )
   {
      const ReportList& reports = nodeIt->second.reports;
      for ( ReportList::const_iterator it = reports.begin();
               it != reports.end(); ++it )
      {
         if ( (*it)->type == type )
            resultList.insert(*it);
      }
   }
   return resultList;
}

ReportList
KnowledgeBase::findAllTrustReports( const string& nodeLabel,
         const InformationDescription::Type& type ) const throw ()
{
   // TODO constness is not true here

   ReportList resultList;
   NodeKnowledgeMap::const_iterator nodeIt = knowledgeOfNodes.find(nodeLabel);
   if ( nodeIt != knowledgeOfNodes.end() )
   {
      const ReportList& reports = nodeIt->second.trustReports;
      for ( ReportList::const_iterator it = reports.begin();
               it != reports.end(); ++it )
      {
         if ( (*it)->type == type )
            resultList.insert(*it);
      }
   }
   return resultList;
}

// ----------------------------------------------------------------------

InformationKnowledge*
KnowledgeBase::findInformation( const shawn::Vec& location,
         const InformationDescription::Type& type, const int& time ) throw ()
{
   // Start to search at the newest InformationKnowledge; this is most likely
   // very close to the right one.
   InformationKnowledge* ikPtr = findNewestInformationAbout(location, type);
   if ( ikPtr )
   {
      // The vehicle already knows something about
      // this topic (location, type). Now find the InformationKnowledge
      // that is closest in time coming from the newest one.
      while ( ikPtr->begin > time && ikPtr->previous() )
         ikPtr = ikPtr->previous();
   }

   return ikPtr;
}

// ----------------------------------------------------------------------

InformationKnowledge*
KnowledgeBase::findNewestInformationAbout( const Vec& location,
         const InformationDescription::Type& type ) throw ()
{
   InformationKnowledge* newest = 0;
   pair<LocationGrid::iterator, LocationGrid::iterator> gridNodeRange =
            locationGrid.equal_range(vecToGridKey(location));

   for ( LocationGrid::iterator it = gridNodeRange.first;
            it != gridNodeRange.second; ++it )
   {
      // Test whether this is the right tuple (location, type)
      if ( it->second->type == type && it->second->location == location )
      {
         if ( newest == 0 || newest->begin < it->second->begin )
         {
            newest = it->second;
         }
      }
   }

   return newest;
}

// ----------------------------------------------------------------------

Observation*
KnowledgeBase::findNewestObservationAbout( const Vec& location,
         const InformationDescription::Type& type ) throw ()
{
   IntInformationKnowledge * newestIK =
            dynamic_cast<IntInformationKnowledge*>(findNewestInformationAbout(
                     location, type));

   if ( newestIK != 0 && newestIK->observations.size() > 0 )
   {
      return *newestIK->observations.rbegin();
   }

   return 0;
}

// ----------------------------------------------------------------------

EvidenceIntegrator&
KnowledgeBase::getEvidenceIntegrator()
{
   return informationIntegrator;
}

// ----------------------------------------------------------------------

void
KnowledgeBase::prune() throw ( runtime_error )
{
   // Should I prune?
   static const string pruningType =
            simulationController_.environment().optional_string_param("prune",
                     "none");
   if ( pruningType == "none" )
   {
      return;
   }
   else if ( pruningType == "old_nodes" )
   {
      pruneOldNodes();
   }
   else if ( pruningType == "overfull" )
   {
      pruneOverfull();
   }

   last_pruned = simulationController_.world().simulation_round();
}

// ----------------------------------------------------------------------

void
KnowledgeBase::pruneOldNodes()
{
   // Default is 8 weeks in seconds
   static const int ageThreshold =
            simulationController_.environment().optional_int_param("prune_age",
                     3600 * 24 * 7 * 8);

   for ( NodeKnowledgeMap::iterator nodeIt = knowledgeOfNodes.begin();
            nodeIt != knowledgeOfNodes.end(); )
   {
      ReportList::index<Report::receptionTimeTag>::type& reports =
               nodeIt->second.reports.get<Report::receptionTimeTag>();
      ReportList::index<Report::receptionTimeTag>::type::reverse_iterator newestIt =
               reports.rbegin();

      int currentTime = simulationController_.world().simulation_round();
      int lastReceptionTime = (*newestIt)->receptionTime;
      size_t numberOfReports = reports.size();

      if ( (currentTime - lastReceptionTime) / numberOfReports > ageThreshold )
      {
         // Increase before erasing, in order to keep a valid iterator.
         // The postfix increment gives me the pre-increment iterator.
         NodeKnowledgeMap::iterator toErase = nodeIt++;
         removeNode(toErase);
      }
      else
      {
         // To keep a valid iterator in the previous if block, the iterator
         // must be incremented in the if branches, not in the for statement.
         ++nodeIt;
      }
   }
}

// ----------------------------------------------------------------------

void
KnowledgeBase::pruneOverfull()
{
   /*
    * Walk through the list of information knowledge elements. For every
    * element, compile a map that assigns the senders with lists of iterators
    * to the reports from that node about that information knowledge.
    *
    * Then walk through this map. For every sender,
    * take the temporal difference of the first and last report. Set time
    * counter to the time of the first report. Then walk through the reports
    * and delete every (next) report that is smaller than the time counter.
    * Save (do not delete) a report if its time is greater or equal than
    * the time counter. If a report is saved increase the time counter by
    * (time difference / 5).
    *
    * Ensure that the first and last report is not deleted due to rounding
    * errors.
    */
   for ( LocationGrid::iterator info = locationGrid.begin();
            info != locationGrid.end(); ++info )
   {
      IntInformationKnowledge* int_info =
               dynamic_cast<IntInformationKnowledge*>(info->second);
      if ( int_info )
      {
//         IntReportList& reports = int_info->reports;
//         if ( reports.size() < 10 )
//            continue;
         if (int_info->last_changed > last_pruned)
            pruneOverfullIntInformation(*int_info);
      }
      else
      {
         throw SimulationFailed(
                  "KnowledgeBase: Found an InformationKnowledge subclass, for which pruning is not implemented.");
      }
   }
}

// ----------------------------------------------------------------------

void
KnowledgeBase::pruneOverfullIntInformation(IntInformationKnowledge& ik)
{
   typedef map<string, list<IntReportList::iterator> > SortedReports;
   SortedReports sorted_reports;

   /* Sort the reports grouped by each sender and sorted
    * by the observation time within each group. */
   IntReportList& reports = ik.reports;
   for ( IntReportList::iterator report = reports.begin();
            report != reports.end(); ++report )
   {
      sorted_reports[(*report)->sender].push_back(report);
   }

   /* Go through the sorted groups and prune the reports. */
   for ( SortedReports::iterator sender_it = sorted_reports.begin();
            sender_it != sorted_reports.end(); ++sender_it )
   {
      list<IntReportList::iterator>& reports_of_sender = sender_it->second;
      if ( reports_of_sender.size() < 6 )
         continue;

      int begin = (**reports_of_sender.begin())->time;
      int end = (**reports_of_sender.rbegin())->time;
      int time_diff = end - begin;
      assert(time_diff > 0);
      int interval = time_diff / 5;
      int current_time = begin;
      list<IntReportList::iterator>::iterator before_end_it =
               --reports_of_sender.end();
      bool have_pruned = false;
      for ( list<IntReportList::iterator>::iterator report_it =
               reports_of_sender.begin(); report_it != before_end_it;
               ++report_it )
      {
         IntReport* report = **report_it;
         if ( report->time < current_time )
         {
            INFO(
                     logger_,
                     "Remove report. Sender: " << report->sender << ", type: " << report->type << ", time: " << report->time << " (current total number: " << reports_of_sender.size() << ").");
            removeReport(report);
            have_pruned = true;
            // operator--(int) is possible, because the above condition
            // never meets for the first element in the list.
            reports_of_sender.erase(report_it--);
         }
         else
         {
            current_time += interval;
         }
      }
      if ( have_pruned )
      {
         ostringstream oss;
         oss << "Remaining reports from " << sender_it->first << " (time):";
         for ( list<IntReportList::iterator>::iterator report_it =
                  reports_of_sender.begin();
                  report_it != reports_of_sender.end(); ++report_it )
         {

            oss << "  " << (**report_it)->time;
         }

         INFO(logger_, oss.str());
      }
   }
}

// ----------------------------------------------------------------------

void
KnowledgeBase::registerMeeting( const std::string& nodeLabel,
         int meetingTime ) throw ()
{
   NodeKnowledge& kn = knowledgeOfNodes[nodeLabel];
   if ( meetingTime - kn.lastMeetingTime > 20 )
      (kn.meetingCounter)++;
   kn.lastMeetingTime = meetingTime;
}

// ----------------------------------------------------------------------

void
KnowledgeBase::removeNode( NodeKnowledgeMap::iterator nodeIt )
         throw ( runtime_error )
{
   ReportList& reports = nodeIt->second.reports;

   for ( ReportList::iterator reportIt = reports.begin();
            reportIt != reports.end(); ++reportIt )
   {
      IntReport* intReportPtr = dynamic_cast<IntReport*>(*reportIt);
      if ( intReportPtr )
      {
         IntInformationKnowledge* infoPtr =
                  intReportPtr->associatedInformation();
         IntReportList& infoReports = infoPtr->reports;

         pair<IntReportList::iterator, IntReportList::iterator> timeRange =
                  infoReports.equal_range(intReportPtr->time);
         for ( IntReportList::iterator infoReportIt = timeRange.first;
                  infoReportIt != timeRange.second; ++infoReportIt )
         {
            if ( *infoReportIt == intReportPtr )
            {
               // Now, I found the report in the report list
               // of the information knowledge. Delete everything.
               infoReports.erase(infoReportIt);
               reports.erase(reportIt);
               delete intReportPtr;
               break;
            }
         }
      }
      else
      {
         throw(runtime_error(
                  "Found a type that cannot be handled when removing a node from the knowledge base."));
      }
   }

   ReportList& trustReports = nodeIt->second.trustReports;

   for ( ReportList::iterator trustReportIt = trustReports.begin();
            trustReportIt != trustReports.end(); ++trustReportIt )
   {
      TrustReport* trustReportPtr = dynamic_cast<TrustReport*>(*trustReportIt);
      if ( trustReportPtr )
      {
         trustReports.erase(trustReportIt);
         delete trustReportPtr;
      }
      else
      {
         throw(runtime_error(
                  "Found a type that cannot be handled when removing a node from the knowledge base."));
      }
   }
   // All reports are removed from the knowledge base.
   // Now, the node knowledge can be removed as well.
   knowledgeOfNodes.erase(nodeIt);
}

void
KnowledgeBase::removeReport( IntReport* report )
{
   IntReportList& info_reports = report->associatedInformation()->reports;
   IntReportList::iterator info_found = find(info_reports.begin(),
            info_reports.end(), report);
   if ( info_found != info_reports.end() )
      info_reports.erase(info_found);
   else
      throw SimulationFailed(
               "KnowledgeBase: Could not find a report in its InformationKnowledge for removal.");

   ReportList& sender_reports = knowledgeOfNodes[report->sender].reports;
   ReportList::iterator sender_found_it = find(sender_reports.begin(),
            sender_reports.end(), report);
   if ( sender_found_it != sender_reports.end() )
      sender_reports.erase(sender_found_it);
   else
      throw SimulationFailed(
               "KnowledgeBase: Could not find a report in its NodeKnowledge for removal.");

   delete report;
}

}

#endif /* ENABLE_VANET */
