/** \file
 *  Definitions of the data types for induction loop and detector value get
 *
 *  \author Björn Hendriks
 */

#ifndef DETECTOR_DATATYPE_H
#define DETECTOR_DATATYPE_H

#include "_apps_enable_cmake.h"

#ifdef ENABLE_TRACICLIENT

#include "atomic_datatypes.h"


namespace traci
{


namespace id
{

enum DetectorCommands
{
};

enum DetectorVariablesGet
{
};

enum DetectorVariablesSet
{
};


} // namespace id





}   // namespace traci

#endif  // #ifdef ENABLE_TRACICLIENT
#endif // DETECTOR_DATATYPE_H

