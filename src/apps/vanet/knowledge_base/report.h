/**
 * @file report.h
 * Defines the data containers vanet::Report and vanet::IntReport
 * together with their lists vanet::ReportList and vanet::IntReportList.
 *
 * @author Walter Bamberger
 *
 * $Revision: 552 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/knowledge_base/report.h $
 * $Id: report.h 552 2011-04-20 00:48:14Z andylz $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_KNOWLEDGE_BASE_REPORT_H
#define __SHAWN_APPS_VANET_KNOWLEDGE_BASE_REPORT_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "../vanet_message.h"
#include "sys/node.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <string>

namespace vanet
{

class InformationKnowledge;
class IntInformationKnowledge;

/**
 * Record of a piece of information received from another node.
 *
 * A report contains all the data of an Observation and, additionally, some data
 * about the reception of the message received from the other node. The
 * reception is characterised by the sender (stored as its label in #sender),
 * the time (#receptionTime) when the message was received, and the
 * location (#receptionLocation) where the message was received.
 *
 * This class is just an interface. Sub classes must be defined for various
 * data types of the value (e.g. @c int and @c double as well as compound
 * data types).
 */
class Report
{
public:
   // Types for indexing.
   // (It seems, the declaration is enough. Do definition necessary.)
   struct timeTag;
   struct receptionTimeTag;

   /** Place where the other node has made the reported observation. */
   shawn::Vec location;

   /** Place where the message has been received. */
   shawn::Vec receptionLocation;

   /** Time when the message has been received. */
   int receptionTime;

   /** Label of the other node which sent the message. */
   std::string sender;

   /** Time when the other node has made the reported observation. */
   int time;

   /** Kind of information stored in the value. */
   InformationDescription::Type type;

   Report( const int& rcpTime, const shawn::Vec& rcpLocation,
            const VanetMessage& msg ) :
      sender(msg.source().label()), receptionTime(rcpTime),
               receptionLocation(rcpLocation), location(msg.location),
               type(msg.type), time(msg.time)
   {
   }

   Report() :
      sender(), receptionTime(0), receptionLocation(), location(),
               type(InformationDescription::IntNone), time(0)
   {
   }

   /**
    * Virtual destructor for polymorphism.
    */
   virtual
   ~Report()
   {
   }

   /// @todo There should be a virtual access method to the associated information.

   static std::string
   getCsvHeader()
   {
      return "\"location\",\"type\",\"observation time\",\"value\",\"opinion\",\"sender\",\"reception time\"";
   }

   friend std::ostream&
   operator<<( std::ostream& os, const Report& r )
   {
      return r.putOut(os);
   }

   virtual std::ostream&
   putOut( std::ostream& os ) const = 0;

   virtual std::ostream&
   writeCsvRecord( std::ostream& os ) const = 0;
};

/**
 * List of reports ordered by the observation time.
 */
typedef boost::multi_index::multi_index_container<Report*,
         boost::multi_index::indexed_by< //
                  // Index 0: observation time
                  boost::multi_index::ordered_non_unique<
                           boost::multi_index::tag<Report::timeTag>,
                           boost::multi_index::member<Report, int,
                                    &Report::time> >,
                  // Index 1: reception time
                  boost::multi_index::ordered_non_unique<
                           boost::multi_index::tag<Report::receptionTimeTag>,
                           boost::multi_index::member<Report, int,
                                    &Report::receptionTime> > //
         > > ReportList;

// ----------------------------------------------------------------------

class IntReport : public Report
{
private:
   // Links to other objects of the knowledge base.
   IntInformationKnowledge* associatedKnowledge;

public:
   IntOpinion opinion;
   int value;

   IntReport( const int& rcpTime, const shawn::Vec& rcpLocation,
            const VanetIntMessage& msg ) :
      Report(rcpTime, rcpLocation, msg), value(msg.value), opinion(msg.opinion)
   {
   }

   IntReport() :
      Report(), opinion(), value(0), associatedKnowledge(0)
   {
   }

   virtual IntInformationKnowledge*
   associatedInformation()
   {
      return associatedKnowledge;
   }

   virtual const IntInformationKnowledge*
   associatedInformation() const
   {
      return associatedKnowledge;
   }

   virtual void
   associatedInformation( IntInformationKnowledge* i )
   {
      associatedKnowledge = i;
   }

   virtual std::ostream&
   putOut( std::ostream& os ) const
   {
      return os << location << ", type:" << type << ", time:" << time
               << ", value:" << value << ", " << opinion << "; sender:"
               << sender << ", " << receptionTime;
   }

   virtual std::ostream&
   writeCsvRecord( std::ostream& os ) const
   {
      return os << "\"" << location << "\",\""
               << InformationDescription::typeNames[type] << "\"," << time
               << "," << value << ",\"" << opinion << "\",\"" << sender
               << "\"," << receptionTime;
   }
};

/**
 * List of integer reports ordered by the observation time.
 */
typedef boost::multi_index::multi_index_container<IntReport*,
         boost::multi_index::indexed_by< //
                  // Index 0: observation time
                  boost::multi_index::ordered_non_unique<
                           boost::multi_index::tag<Report::timeTag>,
                           boost::multi_index::member<Report, int,
                                    &Report::time> >,
                  // Index 1: reception time
                  boost::multi_index::ordered_non_unique<
                           boost::multi_index::tag<Report::receptionTimeTag>,
                           boost::multi_index::member<Report, int,
                                    &Report::receptionTime> > //
         > > IntReportList;

// ----------------------------------------------------------------------

class TrustReport : public Report
{
public:
   std::string reportedNode;
   float trustValue;

   TrustReport( const int& rcpTime, const shawn::Vec& rcpLocation,
            const VanetTrustMessage& msg ) :
      Report(rcpTime, rcpLocation, msg), trustValue(msg.trustValue),
      reportedNode(msg.name)
   {
   }

   TrustReport() :
      Report(), trustValue(0), reportedNode("")
   {
   }

   virtual std::ostream&
   putOut( std::ostream& os ) const
   {
      return os << location << ", type:" << type
               << ", last interaction time:" <<time
               << ", third party name:" << reportedNode
               << ", trustValue:" << trustValue << "; sender:"
               << sender << ", " << receptionTime;
   }

   virtual std::ostream&
   writeCsvRecord( std::ostream& os ) const
   {
      return os << "\"" << location << "\",\""
               << InformationDescription::typeNames[type] << "\","
               << ", last interaction time:" <<time
               << ", third party name:" << reportedNode
               << ", trustValue" << trustValue << "\",\"" << sender
               << "\"," << receptionTime;
   }
};

/**
 * @class DoubleTrustReportList
 *
 * List of integer reports ordered by the observation time.
 */
typedef boost::multi_index::multi_index_container<TrustReport*,
         boost::multi_index::indexed_by< //
                  // Index 0: observation time
                  boost::multi_index::ordered_non_unique<
                           boost::multi_index::tag<Report::timeTag>,
                           boost::multi_index::member<Report, int,
                                    &Report::time> >,
                  // Index 1: reception time
                  boost::multi_index::ordered_non_unique<
                           boost::multi_index::tag<Report::receptionTimeTag>,
                           boost::multi_index::member<Report, int,
                                    &Report::receptionTime> > //
         > > TrustReportList;

}

#endif /* VANET_ENABLE */
#endif /* __SHAWN_APPS_VANET_KNOWLEDGE_BASE_REPORT_H */
