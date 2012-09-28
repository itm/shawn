/** \file
 *  Definitions of the data types for traffic lights value get and set
 *
 *  \author Björn Hendriks
 */

#ifndef TRAFFIC_LIGHT_DATATYPE_H
#define TRAFFIC_LIGHT_DATATYPE_H

#include "_apps_enable_cmake.h"

#ifdef ENABLE_TRACICLIENT

#include "atomic_datatypes.h"


namespace traci
{


namespace id
{

enum TrafficLightCommands
{
    /// command: get traffic lights variable
    CmdGetTrafficLightsVariable = 0xa2,

    /// command: change traffic lights state
    CmdChangeTrafficLightsState = 0xc2
};

enum TrafficLightVariablesGet
{
};

enum TrafficLightVariablesSet
{
};


/// Traffic Light Phases
enum TrafficLightPhase
{
    /// red phase
    TlphaseRed = 0x01,
    /// yellow phase
    TlphaseYellow = 0x02,
    /// green phase
    TlphaseGreen = 0x03,
    /// TL is blinking
    TlphaseBlinking = 0x04,
    /// TL is off and not blinking
    TlphaseNosignal = 0x05
};

} // namespace id



/**
 *  Composed value type for elements of TraCI type Traffic Light Phase List.
 */
struct TrafficLightPhase
{
    StringType precRoad;
    StringType succRoad;
    UbyteType phase;

protected:
    /// set order of elements in TraCI type
    virtual void init( ComposedContainer &queue )
    {
        queue.push_back(&precRoad);
        queue.push_back(&succRoad);
        queue.push_back(&phase);
    }
};
/// TraciType for polygon elements
typedef CompoundTraciType< MessageHandlerComposed< ComposedValue<TrafficLightPhase> > > TrafficLightPhaseType;
/// Container for traffic light phases
typedef DynamicLengthValue<TrafficLightPhaseType, UbyteType> TrafficLightPhaseContainer;
/// Message handler for polygons
typedef MessageHandlerDynamicLength< TrafficLightPhaseContainer, id::TypeTlphaselist > MessageHandlerTrafficLightPhaseList;
/// TraciType for polygon without type info
typedef CompoundTraciType< MessageHandlerTrafficLightPhaseList > TrafficLightPhaseListType;
/// TraciType for polygon with type info
typedef CompoundTraciType< MessageHandlerTrafficLightPhaseList, true > TrafficLightPhaseListTypeWithTypeId;


}   // namespace traci

#endif  // #ifdef ENABLE_TRACICLIENT
#endif // TRAFFIC_LIGHT_DATATYPE_H

