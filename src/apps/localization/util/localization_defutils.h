/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_UTIL_DEFUTILS_H
#define __SHAWN_APPS_LOCALIZATION_UTIL_DEFUTILS_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "sys/node.h"
#include "sys/util/defutils.h"
#include <set>
#include <list>
#include <map>
#include <cmath>
#include <limits>


#ifndef M_PI
   #define M_PI 3.14159265358
#endif


namespace localization
{
   class LocalizationLocalCoordinateSystem;
   DECLARE_HANDLES(LocalizationNeighborInfo);

   const double UNKNOWN_DISTANCE = std::numeric_limits<double>::max();
   const shawn::Vec UNKNOWN_POSITION =
      shawn::Vec(
         std::numeric_limits<double>::min(),
         std::numeric_limits<double>::min(),
         std::numeric_limits<double>::min() );
   const double UNKNOWN_ANGLE = std::numeric_limits<double>::min();
   const double UNKNOWN_AVG_HOP_DIST = std::numeric_limits<double>::max();
   const int UNKNOWN_HOP_CNT = std::numeric_limits<int>::max();

   const std::string SEL_NODE_TAG_NAME = "LOCALIZATION_SELECTED_NODE_TAG";

   typedef std::set<const shawn::Node*> NodeSet;

   typedef std::list<const shawn::Node*> NodeList;
   typedef std::list<const shawn::Node*>::iterator NodeListIterator;
   typedef std::list<const shawn::Node*>::const_iterator ConstNodeListIterator;

   typedef std::map<const shawn::Node*, shawn::Vec> LocationMap;
   typedef LocationMap::iterator LocationMapIterator;
   typedef LocationMap::const_iterator ConstLocationMapIterator;

   typedef std::map<const shawn::Node*, const LocalizationLocalCoordinateSystem*> LCSMap;
   typedef LCSMap::iterator LCSMapIterator;
   typedef LCSMap::const_iterator ConstLCSMapIterator;

   typedef std::map<const shawn::Node*, double> DistanceMap;
   typedef DistanceMap::iterator NeighborIterator;
   typedef DistanceMap::const_iterator ConstNeighborIterator;

   typedef std::pair<double, double> DistancePair;

   typedef std::map<const shawn::Node*, LocalizationNeighborInfoHandle> NeighborInfoMap;
   typedef NeighborInfoMap::const_iterator ConstNeighborhoodIterator;
   typedef NeighborInfoMap::iterator NeighborhoodIterator;

   typedef std::list<ConstLocalizationNeighborInfoHandle> NeighborInfoList;
   typedef NeighborInfoList::const_iterator ConstNeighborInfoListIterator;
   typedef NeighborInfoList::iterator NeighborInfoListIterator;

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/util/localization_defutils.h,v $
 * Version $Revision: 347 $
 * Date    $Date: 2009-04-24 18:37:30 +0200 (Fri, 24 Apr 2009) $
 *-----------------------------------------------------------------------
 * $Log: localization_defutils.h,v $
 *-----------------------------------------------------------------------*/
