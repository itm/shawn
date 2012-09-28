/** \file
 *  Definitions of the data types for lane vehicle type get
 *
 *  \author Björn Hendriks
 */

#ifndef VEHICLE_TYPE_DATATYPE_H
#define VEHICLE_TYPE_DATATYPE_H

#include "_apps_enable_cmake.h"

#ifdef ENABLE_TRACICLIENT

#include "atomic_datatypes.h"


namespace traci
{


namespace id
{

enum VehicleTypeCommands
{
    /// command get vehicle type variable
    CmdGetVehicleTypeVariable = 0xa5,
};

enum VehicleTypeVariablesGet
{
    /// vehicle type variable count
    VarVehicleTypeCount = 0x01,
    /// vehicle type variable length
    VarVehicleTypeLength = 0x44,
    /// vehicle type variable vMax
    VarVehicleTypeVMax = 0x41,
    /// vehicle type variable accel
    VarVehicleTypeAccel = 0x46,
    /// vehicle type variable decel
    VarVehicleTypeDecel = 0x47,
    /// vehicle type variable tau
    VarVehicleTypeTau = 0x48,
    /// vehicle type variable vClass
    VarVehicleTypeVClass = 0x49,
    /// vehicle type variable emission class
    VarVehicleTypeEmissionClass = 0x4a,
    /// vehicle type variable shape
    VarVehicleTypeShape = 0x4b,
    /// vehicle type variable offset
    VarVehicleTypeOffset = 0x4c,
    /// vehicle type variable GUI width
    VarVehicleTypeGuiWidth = 0x4d,
    /// vehicle type variable color
    VarVehicleTypeColor = 0x45
};



} // namespace id





}   // namespace traci

#endif  // #ifdef ENABLE_TRACICLIENT
#endif // VEHICLE_TYPE_DATATYPE_H

