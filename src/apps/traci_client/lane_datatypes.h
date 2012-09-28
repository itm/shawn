/** \file
 *  Definitions of the data types for lane value get and set
 *
 *  \author Björn Hendriks
 */

#ifndef LANE_DATATYPE_H
#define LANE_DATATYPE_H

#include "_apps_enable_cmake.h"

#ifdef ENABLE_TRACICLIENT

#include "atomic_datatypes.h"


namespace traci
{


namespace id
{

enum LaneCommands
{
    /// command get lane variable
    CmdGetLaneVariable = 0xa3,

    /// command change lane state
    CmdChangeLaneState = 0xc3,
};

enum LaneVariablesGet
{
    /// lane variable link number
    VarLaneLinkNumber = 0x30,
    /// lane variable edge id
    VarLaneEdge = 0x31,
    /// lane variable links
    VarLaneLinks = 0x33,
    /// lane variable allowed vehicle classes
    VarLaneAllowedVehicleClasses = 0x34,
    /// lane variable disallowed vehicle classes
    VarLaneDisallowedVehicleClasses = 0x35,
    /// lane variable length
    VarLaneLength = 0x44,
    /// lane variable vmax
    VarLaneVmax = 0x41,
    /// lane variable shape
    VarLaneShape = 0x4e
};

enum LaneVariablesSet
{
};


} // namespace id



/** \name   Structures for TraCI compound object links of command get lane variable
 *
 *  See
 *  http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Lane_Value_Retrieval
 */
//@{

/**
 *  Composed value type for lanes link as part of result of get lane variable links
 *
 *  This is one link element of the composed result of variable links of lane
 *  value retrieval.
 */
struct SingleLaneLink
{
    /// consecutive not internal lane
    StringTypeWithTypeId consecutiveNotInternal;
    /// consecutive internal lane
    StringTypeWithTypeId consecutiveInternal;
    /// has priority (true = 1, false = 0)
    UbyteTypeWithTypeId hasPriority;
    /// is opened (true = 1, false = 0)
    UbyteTypeWithTypeId isOpened;
    /// has approaching foo (true = 1, false = 0)
    UbyteTypeWithTypeId hasApproachingFoo;
    /// (current) state
    StringTypeWithTypeId currState;
    /// direction
    StringTypeWithTypeId direction;
    /// length [m] (only valid if not using internal lanes)
    DoubleTypeWithTypeId length;

protected:
    /// set order of elements in TraCI type
    virtual void init( ComposedContainer &queue )
    {
        queue.push_back(&consecutiveNotInternal);
        queue.push_back(&consecutiveInternal);
        queue.push_back(&hasPriority);
        queue.push_back(&isOpened);
        queue.push_back(&hasApproachingFoo);
        queue.push_back(&currState);
        queue.push_back(&direction);
        queue.push_back(&length);
    }
};
/// GenTraciType of LaneLink
typedef CompoundTraciType< MessageHandlerComposed< ComposedValue<SingleLaneLink> > > SingleLaneLinkType;
/// Container for single lane links
typedef DynamicLengthValue<SingleLaneLinkType, IntegerTypeWithTypeId> LaneLinksContainer;
/// Message handler for dynamic list of LaneLinkType
typedef MessageHandlerDynamicLength< LaneLinksContainer > MessageHandlerLaneLinksList;
/// TraciType for dynamic list of LaneLinkType
typedef CompoundTraciType< MessageHandlerLaneLinksList > LaneLinksListType;


/**
 *  Composed value type for the whole compound object links of command get lane variable
 *
 *  This TraCI type is only used for reading so we don't need a functions to
 *  compute length.
 */
struct LaneLinks
{
    /// length
    IntegerType length;
    /// list of LaneLinkType
    LaneLinksListType laneLinks;

protected:
    /// set order of elements in TraCI type
    virtual void init( ComposedContainer &queue )
    {
        queue.push_back(&length);
        queue.push_back(&laneLinks);
    }
};
/// GenTraciType of the whole compund object LaneLinks
typedef CompoundTraciType< MessageHandlerComposed< ComposedValue<LaneLinks> > > LaneLinksType;

//@}


}   // namespace traci

#endif  // #ifdef ENABLE_TRACICLIENT
#endif // LANE_DATATYPE_H

