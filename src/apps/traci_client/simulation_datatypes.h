/** \file
 *  Definitions of the data types for simulation control and value get
 *
 *  \author Björn Hendriks
 */

#ifndef SIMULATION_DATATYPE_H
#define SIMULATION_DATATYPE_H

#include "_apps_enable_cmake.h"

#ifdef ENABLE_TRACICLIENT

#include "atomic_datatypes.h"


namespace traci
{


namespace id
{

enum SimulationCommands
{
};

enum SimulationVariablesGet
{
};

enum SimulationVariablesSet
{
};


} // namespace id





}   // namespace traci

#endif  // #ifdef ENABLE_TRACICLIENT
#endif // SIMULATION_DATATYPE_H

