/** \file
 *  Definitions of the data types for vehicle value get and set
 *
 *  \author Björn Hendriks
 */

#ifndef VEHICLE_DATATYPE_H
#define VEHICLE_DATATYPE_H

#include "_apps_enable_cmake.h"

#ifdef ENABLE_TRACICLIENT

#include "atomic_datatypes.h"
#include "composed_datatypes.h"


namespace traci
{


namespace id
{

enum VehicleCommands
{
    /// command: get vehicle variable
    CmdGetVehicleVariable = 0xa4,
    /// command: change vehicle state
    CmdChangeVehicleState = 0xc4
};

enum VehicleVariablesGet
{
    /// vehicle variable speed
    VarVehicleSpeed = 0x40,
    /// vehicle variable position
    VarVehiclePosition = 0x42,
    /// vehicle variable angle
    VarVehicleAngle = 0x43,
    /// vehicle variable road ID
    VarVehicleRoadId = 0x50,
    /// vehicle variable lane id
    VarVehicleLaneId = 0x51,
    /// vehicle variable lane index
    VarVehicleLaneIndex = 0x52,
    /// vehicle variable type id
    VarVehicleTypeId = 0x4f,
    /// vehicle variable route ID
    VarVehicleRouteId = 0x53,
    /// vehicle variable edges
    VarVehicleEdges = 0x54,
    /// vehicle variable color
    VarVehicleColor = 0x45,
    /// vehicle variable lane position
    VarVehicleLanePosition = 0x56,
    /// speed without TraCI influence
    VarVehicleSpeedWithoutTraci = 0xb1
};

enum VehicleVariablesSet
{
    /// set vehicle state max speed
    VarVehicleSetMaxSpeed = 0x11,
    /// set vehicle state stop
    VarVehicleSetStop = 0x12,
    /// set vehicle state change lane
    VarVehicleSetLane = 0x13,
    /// set vehicle state slow down
    VarVehicleSetSlowDown = 0x14,
    /// set vehicle state change target
    VarVehicleSetTarget = 0x31,
    /// set vehicle state change route by id
    VarVehicleSetRouteId = 0x54,
    /// set vehicle state change route
    VarVehicleSetRoute = 0x57,
    /// set vehicle state change edge travel time information
    VarVehicleSetEdgeTravelTime = 0x58,
    /// set vehicle state change edge effort information
    VarVehicleSetEdgeEffort = 0x59,
    /// set vehicle state reroute by travel time
    VarVehicleSetRerouteTravelTime = 0x90,
    /// set vehicle state reroute by effort
    VarVehicleSetRerouteEffort = 0x91
};


} // namespace id


/**
 *  Composed value type for vehicle stop
 *
 *  The number of elements will be handled by MessageHandlerLeadingNum automatically.
 */
struct VehicleStop
{
public:
    StringTypeWithTypeId edgeId;
    FloatTypeWithTypeId offset;
    ByteTypeWithTypeId laneNum;
    IntegerTypeWithTypeId duration;

protected:
    /// set order of elements in TraCI type
    virtual void init( ComposedContainer &queue )
    {
        queue.push_back(&edgeId);
        queue.push_back(&offset);
        queue.push_back(&laneNum);
        queue.push_back(&duration);
    }
};
typedef CompoundTraciType< MessageHandlerLeadingNum< ComposedValue<VehicleStop> > > VehicleStopType;



/**
 *  Composed value type for vehicle change lane
 *
 *  The number of elements will be handled by MessageHandlerLeadingNum automatically.
 */
struct VehicleChangeLane
{
public:
    ByteTypeWithTypeId laneNum;
    IntegerTypeWithTypeId duration;

protected:
    /// set order of elements in TraCI type
    virtual void init( ComposedContainer &queue )
    {
        queue.push_back(&laneNum);
        queue.push_back(&duration);
    }
};
typedef CompoundTraciType< MessageHandlerLeadingNum< ComposedValue<VehicleChangeLane> > > VehicleChangeLaneType;



/// Specialization for get speed variable
template <>
struct VariableTraciType<id::CmdGetVehicleVariable, id::VarVehicleSpeed> :
    public VariableTraciTypeBase<id::CmdGetVehicleVariable, id::VarVehicleSpeed, DoubleTypeWithTypeId>
{
};


/// Specialization for vehicle variable position
template <>
struct VariableTraciType<id::CmdGetVehicleVariable, id::VarVehiclePosition> :
    public VariableTraciTypeBase<id::CmdGetVehicleVariable, id::VarVehiclePosition, Position2DTypeWithTypeId>
{
};


/// Specialization for vehicle variable angle
template <>
struct VariableTraciType<id::CmdGetVehicleVariable, id::VarVehicleAngle> :
    public VariableTraciTypeBase<id::CmdGetVehicleVariable, id::VarVehicleAngle, DoubleTypeWithTypeId>
{
};


/// Specialization for vehicle variable road ID
template <>
struct VariableTraciType<id::CmdGetVehicleVariable, id::VarVehicleRoadId> :
    public VariableTraciTypeBase<id::CmdGetVehicleVariable, id::VarVehicleRoadId, StringTypeWithTypeId>
{
};


/// Specialization for vehicle variable lane id
template <>
struct VariableTraciType<id::CmdGetVehicleVariable, id::VarVehicleLaneId> :
    public VariableTraciTypeBase<id::CmdGetVehicleVariable, id::VarVehicleLaneId, StringTypeWithTypeId>
{
};


/// Specialization for vehicle variable lane index
template <>
struct VariableTraciType<id::CmdGetVehicleVariable, id::VarVehicleLaneIndex> :
    public VariableTraciTypeBase<id::CmdGetVehicleVariable, id::VarVehicleLaneIndex, IntegerTypeWithTypeId>
{
};


/// Specialization for vehicle variable type id
template <>
struct VariableTraciType<id::CmdGetVehicleVariable, id::VarVehicleTypeId> :
    public VariableTraciTypeBase<id::CmdGetVehicleVariable, id::VarVehicleTypeId, StringTypeWithTypeId>
{
};


/// Specialization for vehicle variable route ID
template <>
struct VariableTraciType<id::CmdGetVehicleVariable, id::VarVehicleRouteId> :
    public VariableTraciTypeBase<id::CmdGetVehicleVariable, id::VarVehicleRouteId, StringTypeWithTypeId>
{
};


/// Specialization for vehicle variable edges
template <>
struct VariableTraciType<id::CmdGetVehicleVariable, id::VarVehicleEdges> :
    public VariableTraciTypeBase<id::CmdGetVehicleVariable, id::VarVehicleEdges, StringListTypeWithTypeId>
{
};


/// Specialization for vehicle variable color
template <>
struct VariableTraciType<id::CmdGetVehicleVariable, id::VarVehicleColor> :
    public VariableTraciTypeBase<id::CmdGetVehicleVariable, id::VarVehicleColor, ColorTypeWithTypeId>
{
};


/// Specialization for vehicle variable lane position
template <>
struct VariableTraciType<id::CmdGetVehicleVariable, id::VarVehicleLanePosition> :
    public VariableTraciTypeBase<id::CmdGetVehicleVariable, id::VarVehicleLanePosition, DoubleTypeWithTypeId>
{
};


/// Specialization for speed without TraCI influence
template <>
struct VariableTraciType<id::CmdGetVehicleVariable, id::VarVehicleSpeedWithoutTraci> :
    public VariableTraciTypeBase<id::CmdGetVehicleVariable, id::VarVehicleSpeedWithoutTraci, DoubleTypeWithTypeId>
{
};



/// Specialization for set vehicle state max speed
template <>
struct VariableTraciType<id::CmdChangeVehicleState, id::VarVehicleSetMaxSpeed> :
    public VariableTraciTypeBase<id::CmdChangeVehicleState, id::VarVehicleSetMaxSpeed, DoubleTypeWithTypeId>
{
};


/// Specialization for set vehicle state stop
template <>
struct VariableTraciType<id::CmdChangeVehicleState, id::VarVehicleSetStop> :
    public VariableTraciTypeBase<id::CmdChangeVehicleState, id::VarVehicleSetStop, VehicleStopType>
{
};


/// Specialization for set vehicle state change lane
template <>
struct VariableTraciType<id::CmdChangeVehicleState, id::VarVehicleSetLane> :
    public VariableTraciTypeBase<id::CmdChangeVehicleState, id::VarVehicleSetLane, VehicleChangeLaneType>
{
};


/// Specialization for set vehicle state slow down
// template <>
// struct VariableTraciType<id::CmdChangeVehicleState, id::VarVehicleSetSlowDown> :
//     public VariableTraciTypeBase<id::CmdChangeVehicleState, id::VarVehicleSetSlowDown, ???>
// {
// };


/// Specialization for set vehicle state change target
template <>
struct VariableTraciType<id::CmdChangeVehicleState, id::VarVehicleSetTarget> :
    public VariableTraciTypeBase<id::CmdChangeVehicleState, id::VarVehicleSetTarget, StringTypeWithTypeId>
{
};


/// Specialization for set vehicle state change route by id
template <>
struct VariableTraciType<id::CmdChangeVehicleState, id::VarVehicleSetRouteId> :
    public VariableTraciTypeBase<id::CmdChangeVehicleState, id::VarVehicleSetRouteId, StringTypeWithTypeId>
{
};


/// Specialization for set vehicle state change route
template <>
struct VariableTraciType<id::CmdChangeVehicleState, id::VarVehicleSetRoute> :
    public VariableTraciTypeBase<id::CmdChangeVehicleState, id::VarVehicleSetRoute, StringListTypeWithTypeId>
{
};


/// Specialization for set vehicle state change edge travel time information
// template <>
// struct VariableTraciType<id::CmdChangeVehicleState, id::VarVehicleSetEdgeTravelTime> :
//     public VariableTraciTypeBase<id::CmdChangeVehicleState, id::VarVehicleSetEdgeTravelTime, ???>
// {
// };


/// Specialization for set vehicle state change edge effort information
// template <>
// struct VariableTraciType<id::CmdChangeVehicleState, id::VarVehicleSetEdgeEffort> :
//     public VariableTraciTypeBase<id::CmdChangeVehicleState, id::VarVehicleSetEdgeEffort, ???>
// {
// };


/// Specialization for set vehicle state reroute by travel time
// template <>
// struct VariableTraciType<id::CmdChangeVehicleState, id::VarVehicleSetRerouteTravelTime> :
//     public VariableTraciTypeBase<id::CmdChangeVehicleState, id::VarVehicleSetRerouteTravelTime, ???>
// {
// };


/// Specialization for set vehicle state reroute by effort
// template <>
// struct VariableTraciType<id::CmdChangeVehicleState, id::VarVehicleSetRerouteEffort> :
//     public VariableTraciTypeBase<id::CmdChangeVehicleState, id::VarVehicleSetRerouteEffort, ???>
// {
// };






}   // namespace traci

#endif  // #ifdef ENABLE_TRACICLIENT
#endif // VEHICLE_DATATYPE_H

