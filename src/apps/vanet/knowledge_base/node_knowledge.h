/**
 * @file node_knowledge.h
 * Defines the data containers vanet::NodeKnowledge.
 *
 * @author Walter Bamberger
 *
 * $Revision: 630 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/knowledge_base/node_knowledge.h $
 * $Id: node_knowledge.h 630 2011-10-28 12:52:43Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_KNOWLEDGE_BASE_NODE_KNOWLEDGE_H
#define __SHAWN_APPS_VANET_KNOWLEDGE_BASE_NODE_KNOWLEDGE_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "report.h"
#include <climits>

namespace vanet
{

/**
 * Record about all properties known of another node. It contains some
 * information collected from previous interactions. The label of the node is
 * saved in a map as the key of the map. This record is the value of this map.
 */
struct NodeKnowledge
{
   /** How often this nodes has been met already. */
   int meetingCounter;
   /** When this node has been met the last time. */
   int lastMeetingTime;
   /** List of all reports received from this node. */
   ReportList reports;
   /** List of recommendations about this node. */
   ReportList trustReports;

   NodeKnowledge() :
      meetingCounter(0), lastMeetingTime(0)
   {
   }
};

}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_KNOWLEDGE_BASE_NODE_KNOWLEDGE_H */
