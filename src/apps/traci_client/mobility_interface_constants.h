/** \file
 *  Definitions of constants for TraCI
 *
 *  \author Axel Wegener <wegener@itm.uni-luebeck.de>
 *  \author Björn Hendriks
 */

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef TRACICONSTANTS_H
#define TRACICONSTANTS_H


namespace traci
{


/**
 *  This sub namespace contains all integer IDs of the TraCI protocol.
 *
 *  They are collected in a namespace instead a class type to allow extending
 *  the set of IDs in other files.
 */
namespace id
{

enum Command
{
    /// command: get version
    CmdGetVersion = 0x00,

    /// command: simulation step (2)
    CmdSimstep2 = 0x02,

    /// command: set maximum speed
    CmdSetmaxspeed = 0x11,

    /// command: stop node
    CmdStop = 0x12,

    /// command: set lane
    CmdChangelane = 0x13,

    /// command: slow down
    CmdSlowdown = 0x14,

    /// command: change route
    CmdChangeroute = 0x30,

    /// command: change target
    CmdChangetarget = 0x31,

    /// command: simulation parameter
    CmdSimparameter = 0x70,

    /// command: position conversion
    CmdPositionconversion = 0x71,

    /// command: distance request
    CmdDistancerequest = 0x72,

    /// command: add vehicle
    CmdAddVehicle = 0x74,

    /// command: move node
    CmdMovenode = 0x80,

    /// command: close sumo
    CmdClose = 0x7f,

    /// command:
    CmdUpdatecalibrator = 0x50,

    /// command: get all traffic light ids
    CmdGetalltlids = 0x40,

    /// command: get traffic light status
    CmdGettlstatus = 0x41,

    /// command: report traffic light id
    CmdTlidlist = 0x90,

    /// command: report traffic light status switch
    CmdTlswitch = 0x91,

    /// command: get GUI variable
    CmdGetGuiVariable = 0xac,
    /// response command: get lane variable
    CmdGetLaneVariableResponse = 0xb3
};


enum DataType
{
    EmptyType = 0x00,
    /// omit position
    PositionNone    = 0x00,
    /// 2d Cartesian coordinates
    Position2d      = 0x01,
    /// 2.5d Cartesian coordinates
    Position25d    = 0x02,
    /// 3d Cartesian coordinates
    Position3d      = 0x03,
    /// position on road map
    PositionRoadmap = 0x04,
    /// boundary box
    TypeBoundingbox = 0x05,
    /// polygon
    TypePolygon = 0x06,
    /// unsigned byte
    TypeUbyte = 0x07,
    /// signed byte
    TypeByte = 0x08,
    /// 32 bit integer
    TypeInteger = 0x09,
    /// color
    TypeColor = 0x11,
    /// float
    TypeFloat = 0x0a,
    /// double
    TypeDouble = 0x0b,
    /// 8 bit ascii string
    TypeString = 0x0c,
    /// list of traffic light phases
    TypeTlphaselist = 0x0d,
    /// list of strings
    TypeStringlist = 0x0e,
    /// compound object
    TypeCompound = 0x0f
};


/** General Value Retrieval Variable IDs
    *
    *  These variable IDs are common for several value retrieval commands.
    */
enum GenValueRetrievalVariable
{
    /// variable id list for value retrieval commands
    VarIdList = 0x00,
    /// variable CO2 emissions
    VarCo2Emissions = 0x60,
    /// variable CO emissions
    VarCoEmissions = 0x61,
    /// variable HC emissions
    VarHcEmissions = 0x62,
    /// variable PMx emissions
    VarPmxEmissions = 0x63,
    /// variable NOx emissions
    VarNoxEmissions = 0x64,
    /// variable fuel consumption
    VarFuelConsumption = 0x65,
    /// variable noise emission
    VarNoiseEmission = 0x66,
    /// variable last step vehicle number
    VarLastStepVehicleNumber = 0x10,
    /// variable last step mean speed
    VarLastStepMeanSpeed = 0x11,
    /// variable last step vehicle IDs
    VarLastStepVehicleIds = 0x12,
    /// variable last step vehicle occupancy
    VarLastStepOccupancy = 0x13,
    /// variable last step mean vehicle length
    VarLastStepMeanVehicleLength = 0x15
};

/// Get GUI Variable IDs
enum GuiValueRetrievalVariable
{
    /// zoom
    VarGuiZoom = 0xa0,
    /// offset
    VarGuiOffset = 0xa1,
    ///schema
    VarGuiSchema = 0xa2,
    /// boundary
    VarGuiBoundary = 0xa3
};

}   // namespace id


}   // namespace traci

#endif  // #ifndef TRACICONSTANTS_H



