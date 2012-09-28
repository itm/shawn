/** \file
 *  Definitions of the data types for subscription handling
 *
 *  \author Björn Hendriks
 */

#ifndef SUBSCRIPTION_DATATYPE_H
#define SUBSCRIPTION_DATATYPE_H

#include "_apps_enable_cmake.h"

#ifdef ENABLE_TRACICLIENT

#include "atomic_datatypes.h"


namespace traci
{


namespace id
{

enum SubscriptionCommands
{
};

enum SubscriptionVariablesGet
{
};

enum SubscriptionVariablesSet
{
};


} // namespace id





}   // namespace traci

#endif  // #ifdef ENABLE_TRACICLIENT
#endif // SUBSCRIPTION_DATATYPE_H

