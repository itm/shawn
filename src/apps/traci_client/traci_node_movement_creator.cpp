/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/node_movements/linear_movement.h"
#include "sys/node_movements/jump_movement.h"
#include "sys/processors/processor_keeper.h"
#include "sys/misc/tokenize.h"

#include "traci_node_movement_creator.h"
#include <iomanip>
#include <sstream>
#include "atomic_datatypes.h"
#include "vehicle_datatypes.h"
#include "traffic_light_datatypes.h"


#ifdef ENABLE_TRACICLIENT


using namespace tcpip;
using namespace shawn;

namespace traci
{

// Add domains here.
// Make sure that:
// -the counts of elements in both arrays are the same and
// -the order of the domain_ids corresponds to the order of corresponding domain_names
const int TraCIClient::DOMAIN_IDS[] = {DomainVehicle, DomainTrafficLight};
const std::string TraCIClient::DOMAIN_NAMES[] = {"vehicle", "traffic_light"};

TraCIClient::TraCIClient() :
        socket_(),
        nextStepVehicles(),
        remotehost_( "localhost" ),
        remoteport_( 8888 ),
        time_interval_( 1.0 ),
        target_time_( 0.0 ),
        feed_time_( 0.0 )
{
    std::cout << "TraCIClient ctor" << std::endl;
}


//---------------------------------------------------------------------
TraCIClient::~TraCIClient( void )
{
    if ( socket_.get() )
        close();
    reset();
    std::cout << "TraCIClient dtor" << std::endl;
}


//---------------------------------------------------------------------
TraCIClient* TraCIClient::instance()
{
    static TraCIClient* inst = new TraCIClient();
    return inst;
}


//---------------------------------------------------------------------
void TraCIClient::node_added( Node& )
throw()
{
    // Do nothing here
}


//---------------------------------------------------------------------
void TraCIClient::node_removed( Node& node )
throw()
{
    remove_node_from_map( &node );
}


//---------------------------------------------------------------------
/**
 *  This occurs, for
 *  example, if the method reorganize_nodes() of the world is called,
 *  because the internal NodeVector is optimized (compressed) there
 *  and ids may change.
 *
 *  \param int Old ID
 *  \param int New ID
 */
void TraCIClient::id_changed( int, int )
throw()
{
    //ABORT_NOT_IMPLEMENTED;
}


//---------------------------------------------------------------------
/**
 *  \return     \c true, if the world should delete the listener
 */
bool TraCIClient::invalidate( void )
throw()
{
    // Do nothing here
    return false;
}


//---------------------------------------------------------------------
/**
 *  \todo   Check traffic light initialization if feed_time is zero.
 */
void TraCIClient::run( SimulationController& sc )
throw()
{
    sc_ = &sc;
    sc_->world_w().add_node_change_listener( *this );
    // Fetch parameters
    remoteport_ = sc_->environment().optional_int_param( "remote_port", remoteport_ );
    remotehost_ = sc_->environment().optional_string_param( "remote_host", remotehost_ );
    feed_time_ =  sc_->environment().optional_double_param( "feed_time", feed_time_ );
    //target_time_ = sc_->environment().optional_double_param("target_time",target_time_);
    time_interval_ = sc_->environment().optional_double_param( "time_interval", time_interval_ );

    fetch_processor_factories();
    // Connect it!
    if ( !connect() )
        abort();

    if ( feed_time_ - time_interval_ > 0.0 )
    {
        runSumo(feed_time_ - time_interval_);

        bool tln = sc_->environment_w().optional_bool_param( "traffic_light_nodes", false );
        if ( tln )
            add_traffic_lights();
    }
}


//---------------------------------------------------------------------
/**
 * \todo        For unknown reason SUMO returns some data even when no subscriptions
 *              are set. Find out why and improve this code.
 *
 * \param       targetTime      Shawn time until which to run
 */
void TraCIClient::runSumo(double targetTime)
{
    // send simulation step command
    const IntegerType targetTimeTraciType(timeShawnToSumo(targetTime));
    // throw away returned data
    TraciMessage dummy;
    send_command(id::CmdSimstep2, targetTimeTraciType, dummy);
}


//---------------------------------------------------------------------
/**
 *  After executing the traffic simulation further calls will iterate
 *  over the moved vehicles to return their shawn::MovementInfo. After
 *  this iteration it returns a shawn::MovementInfo with
 *  shawn::MovementInfo::Urgency set to shawn::MovementInfo::Delayed.
 *
 *  \todo       Check if this function can be simplified by by using
 *              other Shawn APIs.
 *
 *  \return shawn::MovementInfo for current node.
 */
// next_movement = command_simulation_step -> scheduled "magically"
MovementInfo* TraCIClient::next_movement()
throw( std::runtime_error )
{
    std::auto_ptr<MovementInfo> result(new MovementInfo());

    if ( !socket_.get() )
    {
        std::cerr << "Error in method TraCIClient::next_movement, socket_ == NULL!" << std::endl;
        abort();
    }

    const double now = sc_->world().scheduler().current_time();

    // If in-storage is empty ask for new data from movement simulator (e.g. SUMO)
    if ( nextStepVehicles.empty() )
    {
        // If the right simulation time for sending the next question is not yet arrived ...
        if ( now < target_time_ )
        {
            //... wait until then
            result->set_urgency( MovementInfo::Delayed );
            result->set_dispatch_time( target_time_ );
            result->set_node( NULL );
            result->set_node_movement( NULL );
            return result.release();
        }
        // If target_time is reached, ask for new movements
        target_time_ = now + time_interval_;
        check_for_unused_vehicle_nodes();
        current_vehicle_nodes_.clear();

        // send simulation step command
        runSumo(target_time_);

        // get new list of vehicles
        command_get_value(id::CmdGetVehicleVariable, id::VarIdList, "", nextStepVehicles);
    }

    // no more vehicles on the road?
    if ( nextStepVehicles.empty() )
    {
        // No equipped vehicle is reported
        result->set_urgency( MovementInfo::Delayed );
        result->set_dispatch_time( target_time_ );
        result->set_node( NULL );
        result->set_node_movement( NULL );
        return result.release();
    }

    // handle movement of last new vehicle (last because that's an O(1) operation)
    const std::string node_id = nextStepVehicles.back();
    nextStepVehicles.pop_back();

    // get new position
    Position2DTypeWithTypeId newPosition;
    command_get_value(id::CmdGetVehicleVariable, id::VarVehiclePosition, node_id, newPosition);

    bool is_new_node = false;
    Node * node = find_node_by_traci_id_w( TraCIID( DomainVehicle, node_id ) );
    // If node doesn't exist create new one
    if ( !node )
    {
        node = new_node( TraCIID( DomainVehicle, node_id ) );
        is_new_node = true;
    }

    current_vehicle_nodes_.insert( TraCIID( DomainVehicle, node_id ) );
    result->set_urgency( shawn::MovementInfo::Immediately );
    result->set_dispatch_time( now );
    result->set_node( node );
    const Vec destination(newPosition.x, newPosition.y, 0.0);
    if ( is_new_node )
    {
        // "Fresh" node
        std::auto_ptr<JumpMovement> jm( new JumpMovement );
        jm->set_dimensions(&destination);
        result->set_node_movement( jm.release() );
    }
    else
    {
        // Old node
        // Set node exactly to old destination
        const LinearMovement* old_lm = dynamic_cast<const LinearMovement*>( &node->movement() );
        if ( old_lm )
            node->set_real_position( old_lm->destination() );
        std::auto_ptr<LinearMovement> lm( new LinearMovement );
        if ( target_time_ > now )
        {
            const double velocity = euclidean_distance( node->real_position(), destination ) / ( target_time_ - now );
            lm->set_parameters( velocity, destination, sc_->world_w() );
        }
        result->set_node_movement( lm.release() );
    }

    return result.release();
}


//---------------------------------------------------------------------
/**
 *  Add traffic light nodes
 *
 *  The new TraCI API has no way to find out the geographical location of traffic
 *  lights, which is essential for Shawn nodes. To point this out the function
 *  always throws a std::logic_error.
 *
 *  \todo   Find out why this function is public and what happens if it's called twice.
 */
void TraCIClient::add_traffic_lights()
{
    std::logic_error("TraCIClient::add_traffic_lights: cannot determine traffic light positions");
/*
    StringListTypeWithTypeId tlIds;
    command_get_value(id::CmdGetTrafficLightsVariable, TraciConstants::VarIdList, "", tlIds);
    for(StringListTypeWithTypeId::ValueType::const_iterator tlIt = tlIds.begin(); tlIt != tlIds.end(); ++tlIt)
    {
        Node * node = new_node( TraCIID( DomainTrafficLight, *tlIt ) );
        node->set_real_position( Vec( x, y, z ) );
    }
*/
}


//---------------------------------------------------------------------
void TraCIClient::checkCommandId( CommandId expectedId, CommandId actualId, const std::string &functionName ) const
{
    if ( expectedId != actualId )
    {
        std::ostringstream msg;
        msg << std::hex << std::setfill('0')
            << "traci::TraCIClient";
        if ( !functionName.empty() )
            msg << "::" << functionName;
        msg << ": got unexpected response command, "
            << "expected 0x" << std::setw(2) << static_cast<unsigned int>( expectedId )
            << "(Scenario), got 0x" << std::setw(2) << static_cast<unsigned int>( actualId );
        throw std::runtime_error( msg.str() );
    }
}


//---------------------------------------------------------------------
void TraCIClient::reset()
{
    for ( std::map<int, std::set<ProcessorFactoryHandle>* >::iterator it = domain_processor_factories_.begin();
          it != domain_processor_factories_.end();
          ++it )
    {
        it->second->clear();
        delete it->second;
    }
    node_change_listeners_.clear();
    current_vehicle_nodes_.clear();
    node_to_traci_ids_.clear();
    traci_ids_to_node_.clear();
}


//---------------------------------------------------------------------
bool TraCIClient::connect()
{
    if ( socket_.get() )
        close();

    try
    {
        socket_.reset(new Socket( remotehost_, remoteport_ ));
        socket_->set_blocking( true );
        socket_->connect();
    }
    catch ( SocketException &e )
    {
        std::cerr << "Error in method TraCIClient::connect(): "
             << e.what() << std::endl;
        socket_.reset();
        return false;
    }
    return true;
}


//---------------------------------------------------------------------
void TraCIClient::close()
{
    if ( !socket_.get() )
    {
        std::cerr << "Error in method TraCIClient::close(): socket_ == NULL" << std::endl;
        abort();
    }
    Storage out;
    writeCommandHead(id::CmdClose, 0, out);     // 0 -> no content data
    try
    {
        socket_->sendExact( out );
    }
    catch ( SocketException &e )
    {
        std::cerr << "Error in method TraCIClient::close(): "
             << e.what() << std::endl;
        socket_.reset();
        abort();
    }

    socket_.reset();
}


//---------------------------------------------------------------------
void TraCIClient::fetch_processor_factories()
{
    for ( unsigned int i = 0; i < sizeof( DOMAIN_IDS ) / sizeof( int ); ++i )
    {
        ProcessorFactoryHandle pfh;
        std::set<ProcessorFactoryHandle>* processor_factories = new std::set<ProcessorFactoryHandle>();
        StrTok tok( sc_->environment().optional_string_param( DOMAIN_NAMES[i] + "_processors", "" ), ", " );
        for ( StrTok::iterator it = tok.begin(); it != tok.end(); ++it )
        {
            pfh = sc_->processor_keeper_w().find_w( *it );
            assert( pfh != NULL );
            processor_factories->insert( pfh );
        }
        domain_processor_factories_.
        insert( std::pair<int, std::set<ProcessorFactoryHandle>*> ( DOMAIN_IDS[i], processor_factories ) );
    }
}


//---------------------------------------------------------------------
void TraCIClient::add_node_change_listener( TraCINodeChangeListener& listener )
throw()
{
    node_change_listeners_.push_back( &listener );
}


//---------------------------------------------------------------------
Node* TraCIClient::new_node( const TraCIID& traci_id, bool attach_processors_by_domain )
{
    // If node exists return NULL
    if ( find_node_by_traci_id_w( traci_id ) )
        return NULL;

    Node* node = new Node();
    // this will pass ownership of the pointer
    add_node( node, traci_id );

    if ( attach_processors_by_domain )
    {
        std::map<int, std::set<ProcessorFactoryHandle>* >::const_iterator it1 =
            domain_processor_factories_.find( traci_id.domain() );

        if ( it1 != domain_processor_factories_.end() )
        {
            std::set<ProcessorFactoryHandle>* processor_factories = it1->second;
            for ( std::set<ProcessorFactoryHandle>::const_iterator it2 = processor_factories->begin();
                    it2 != processor_factories->end(); ++it2 )
            {
                ProcessorHandle ph = it2->get()->create();
                node->add_processor( ph );
            }
        }
    }
    return node;
}


//---------------------------------------------------------------------
void TraCIClient::check_for_unused_vehicle_nodes()
{
    for ( World::node_iterator ni = sc_->world_w().begin_nodes_w();
            ni != sc_->world_w().end_nodes_w(); ++ni )
    {

        TraCIID ti;
        try
        {
            ti = find_traci_id_by_node( *ni );
        }
        catch ( std::logic_error &e )
        {
            // Not found
            continue;
        }
        if ( ti.domain() == DomainVehicle && current_vehicle_nodes_.find( ti ) == current_vehicle_nodes_.end() )
        {
            sc_->world_w().remove_node( *ni );
        }
    }
}


//---Movement-Simulator-Commands---
//---------------------------------------------------------------------
/**
 *  see http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Mobility-related_commands
 *
 *  Calls \c abort() in case of problems after sending a message to \c stderr.
 *
 *  \param[in]  node        node to set max speed for
 *  \param[in]  max_speed   max speed to set (negative -> original value)
 */
void TraCIClient::command_set_maximum_speed( const Node& node, double max_speed )
{
    // Node ID
    TraCIID ti;
    try
    {
        ti = find_traci_id_by_node( node );
    }
    catch ( std::logic_error &e )
    {
        std::cerr << "Error in method TraCIClient::command_set_maximum_speed: " << e.what() << std::endl;
        abort();
    }
    check_domain( DomainVehicle, ti, true, "command_set_maximum_speed" );

    command_set_value( id::CmdChangeVehicleState, id::VarVehicleSetMaxSpeed, ti.id(), FloatTypeWithTypeId(max_speed) );
}


//---------------------------------------------------------------------
/**
 *  see http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Mobility-related_commands
 *
 *  Calls \c abort() in case of problems after sending a message to \c stderr.
 *
 *  \param[in]  node        node to set stop position for
 *  \param[in]  x           \c x of 2DPosition
 *  \param[in]  y           \c y of 2DPosition
 *  \param[in]  radius      \c Radius of stop node command
 *  \param[in]  wait_time   \c WaitTime of stop node command
 */
void TraCIClient::command_stop( const Node& node, double x, double y, double radius, double wait_time )
{
    TraCIID ti;
    try
    {
        //out.writeInt(find_traci_id_by_node(node).id());
        ti = find_traci_id_by_node( node );
    }
    catch ( std::logic_error &e )
    {
        std::cerr << "Error in method TraCIClient::command_stop: " << e.what() << std::endl;
        abort();
    }
    check_domain( DomainVehicle, ti, true, "command_stop" );

    // convert x/y to roadmap position
    ConstDynamicTraciType<> out;

    Position2DTypeWithTypeId stopPosXy;
    stopPosXy.x = x;
    stopPosXy.y = y;
    out.push_back(&stopPosXy);

    // result
    RoadMapPositionTypeWithTypeId stopPosRoadmap;
    const UbyteType resultType(stopPosRoadmap.getTraciTypeId());
    out.push_back(&resultType);

    send_command(id::CmdPositionconversion, out, stopPosRoadmap);

    // send stop command
    VehicleStopType stop;
    stop.edgeId = stopPosRoadmap.road;
    stop.offset = stopPosRoadmap.pos;
    stop.laneNum = stopPosRoadmap.lane;
    stop.duration = timeShawnToSumo(wait_time);
    command_set_value(id::CmdChangeVehicleState, id::VarVehicleSetStop, ti.id(), stop);
}


//---------------------------------------------------------------------
/**
 *  see http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Mobility-related_commands
 *
 *  Calls \c abort() in case of problems after sending a message to \c stderr.
 *
 *  \param[in]  node    node (vehicle) to set lane constraint for
 *  \param[in]  lane    lane to get vehicle to change to (0 for rightmost)
 *  \param[in]  time    duration of lane constraint
 */
void TraCIClient::command_change_lane( const Node& node, char lane, float time )
{
    TraCIID ti;
    try
    {
        ti = find_traci_id_by_node( node );
    }
    catch ( std::logic_error &e )
    {
        std::cerr << "Error in method TraCIClient::command_change_lane: " << e.what() << std::endl;
        abort();
    }
    check_domain( DomainVehicle, ti, true, "command_change_lane" );

    VehicleChangeLaneType changeLane;
    changeLane.laneNum = lane;
    changeLane.duration = timeShawnToSumo(time);
    command_set_value(id::CmdChangeVehicleState, id::VarVehicleSetLane, ti.id(), changeLane);
}


//---------------------------------------------------------------------
/**
 *  Calls \c abort() in case of problems after sending a message to \c stderr.
 *
 *  \attention  If \p targetTime is not equal to current time SUMO will proceed
 *              simulation until \p targetTime without updating Shawn's node
 *              positions.
 *
 *  \deprecated Due to the above risk this command will be removed.
 *
 *  \param[in]      node            node which's %position to get
 *  \param[in]      targetTime      target time for simulation step TraCI-command
 *  \param[out]     roadId_         edge of SUMO network containing \p node
 *  \param[out]     relPosition_    %position of \p node on edge \p roadId_
 *  \param[out]     laneId_         lane containing \p node
 *
 *  \return     always \c true
 */
bool TraCIClient::command_roadmap_position( const Node& node, double targetTime,
        std::string & roadId_, float & relPosition_, std::string & laneID_ )
{
    try
    {
        target_time_ = targetTime;
        runSumo(targetTime);

        // get new position of node
        command_roadmap_position(node, laneID_, relPosition_);

        // get roadId_ (edge ID)
        const TraCIID & ti = find_traci_id_by_node( node );
        StringTypeWithTypeId traciRoadId;
        command_get_value( id::CmdGetVehicleVariable, id::VarVehicleRoadId, ti.id(), traciRoadId );
        roadId_ = traciRoadId;
    }
    catch ( std::exception & e )
    {
        std::cerr << "Error in method TraCIClient::command_roadmap_position: catched std::exception: " << e.what() << std::endl;
        abort();
    }

    return true;
}


//---------------------------------------------------------------------
/**
 *  \param[in]      node        node which's %position to get
 *  \param[out]     laneId      lane containing \p node
 *  \param[out]     laneOffset  %position of \p node on lane \p laneId
 */
void TraCIClient::command_roadmap_position( const shawn::Node & node,
                                std::string & laneId, float & laneOffset ) const
{
    const TraCIID & ti = find_traci_id_by_node( node );

    StringTypeWithTypeId traciLaneId;
    command_get_value( id::CmdGetVehicleVariable, id::VarVehicleLaneId, ti.id(), traciLaneId );

    IntegerTypeWithTypeId traciLaneOffset;
    command_get_value( id::CmdGetVehicleVariable, id::VarVehicleLaneIndex, ti.id(), traciLaneOffset );

    laneId = traciLaneId;
    laneOffset = traciLaneOffset;
}


//---------------------------------------------------------------------
/**
 *  Compare actual position in \p storage with \p expected_position and throw
 *  a std::runtime_error with an appropriate message if not.
 *
 *  \param[in]  storage             storage to check position of
 *  \param[in]  expectedPosition    expected position
 *  \param[in]  callerName          (optional) name of calling function for error message
 */
void TraCIClient::check_position( const TraciMessage &storage, unsigned int expectedPosition,
                                  const std::string & callerName ) const
{
    if ( expectedPosition != storage.position() )
    {
        std::ostringstream msg;
        msg << ( callerName.empty() ? "traci::TraCIClient::check_position" : callerName );
        msg << ": unexpected position, ";
        msg << "expected = " << expectedPosition;
        msg << ", actual = " << storage.position();
        throw std::runtime_error( msg.str() );
    }
}


//---------------------------------------------------------------------
/**
 *  Calls other TraCIClient::command_get_value with \p variable and \p sumoId
 *  put into a VariableId structure.
 *
 *  \param[in]  command             command ID to send
 *  \param[in]  variable            TraCI variable number to request
 *  \param[in]  sumoId              SUMO element ID
 *  \param[out] response            TraciMessage for reponse, will be positioned
 *                                  to start of returned value (behind ID)
 *  \param[in]  response_command    (optional) command ID of response (if 0 it
 *                                  will be computed from \p command)
 *
 *  \return     expected position of \p response after reading the whole answer command
 */
void TraCIClient::command_get_value( CommandId command, VariableId variable, const std::string & sumoId,
                            traci::TraciType & value, const traci::TraciType & extension,
                            CommandId response_command ) const
{
    if ( 0 == response_command )
        response_command = command | 0x10;

    // assemble TraciType to send
    const UbyteType variableType(variable);
    const StringType idType(sumoId);
    ConstDynamicTraciType<> out;
    out.push_back(&variableType);
    out.push_back(&idType);
    out.push_back(&extension);

    // assemble TraciType to receive
    UbyteType commandId;
    UbyteType answerVariable;
    StringType answerId;
    DynamicTraciType<> in;
    in.push_back(&commandId);
    in.push_back(&answerVariable);
    in.push_back(&answerId);
    // value retrieval answers are always with type info
    const bool oldWithTypeId = value.setWithTypeId( true );
    in.push_back(&value);

    // send and receive
    send_command( command, out, in );

    // reset withTypeId of answer
    value.setWithTypeId( oldWithTypeId );

    // check answer command
    checkCommandId( response_command, commandId, "command_get_value" );

    // check answer variable and ID
    if ( (answerVariable != variable) || (answerId != sumoId) )
    {
        std::ostringstream msg;
        msg << "traci::TraCIClient::command_get_value: got wrong answer (variable, ID), expected (";
        msg << static_cast<unsigned int>(variable) << ", " << sumoId << "), got (";
        msg << static_cast<unsigned int>(answerVariable) << ", " << answerId << ")";
        throw std::runtime_error( msg.str() );
    }
}


//---------------------------------------------------------------------
/**
 *  Also considers protocol definition for lengths > 255.
 *
 *  \param[in,out]  storage     storage to read from (position will be advanced)
 *
 *  \return     read length
 */
unsigned int TraCIClient::read_length( TraciMessage &storage ) const
{
    const UbyteType short_length( storage );
    if ( 0 == short_length )
        return IntegerType( storage );
    return short_length;
}


void TraCIClient::do_send_command( const CommandId command,
                                   const TraciMessage &out,
                                   TraciMessage &in ) const
{
    // Preconditions
    if ( ! socket_.get() )
    {
        std::cerr << "TraCIClient::do_send_command: no socket" << std::endl;
        abort();
    }

    // Send command
    try
    {
        socket_->sendExact( out );
        socket_->receiveExact( in );
    }
    catch ( SocketException &e )
    {
        std::cerr << "Error in TraCIClient::do_send_command: " << e.what() << std::endl;
        abort();
    }

    // check response
    unsigned int command_start = in.position();
    unsigned int command_length = read_length( in );

    // check received command ID
    UbyteType rcvdCommandId(in);
    if ( command != rcvdCommandId )
    {
        std::cerr << sc_->world().scheduler().current_time();
        std::cerr << ": Error in method TraCIClient::do_send_command, Server answered to command: ";
        std::cerr << std::hex << "0x" << static_cast<unsigned int>(rcvdCommandId);
        std::cerr << ". Expected command: 0x" << static_cast<unsigned int>(command) << std::dec << std::endl;
        abort();
    }

    // check response status
    const StatusResponseType response(in);
    if ( !response.isSuccess() )
    {
        std::cerr << sc_->world().scheduler().current_time()
             << ": Error in method TraCIClient::do_send_command, Server returned error "
             << "[0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<unsigned int>(response.result) << "]: "
             << response.description << std::dec << std::setfill(' ') << std::endl;
        abort();
    }

    // Right length?
    check_position( in, command_start + command_length, "TraCIClient::do_send_command" );
}


//---------------------------------------------------------------------
/**
 * \param       contentLength   length of command's content without command identifier
 * \param       storage         message storage to write length into
 */
void TraCIClient::writeCommandHead( const CommandId command, const std::size_t contentLength, TraciMessage & storage ) const
{
    if (contentLength <= 253)
    {
        UbyteType(contentLength + 2).write(storage);
    }
    else
    {
        UbyteType(0).write(storage);
        IntegerType(contentLength + 6).write(storage);
    }

    UbyteType(command).write(storage);
}


//---------------------------------------------------------------------
/**
 *  see http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Protocol
 *
 *  For historical reasons subfunctions might call \c abort() in case of problems
 *  after sending a message to \c stderr instead of throwing an exception.
 *
 *  \param[in]  command         TraCI command ID to send
 *  \param[in]  content         part of TraCI command after the command ID
 *  \param[out] answer          TraciMessage for command answer, position will
 *                              be set behind status response at first answer command
 */
void TraCIClient::send_command( CommandId command,
                                TraciMessage & content,
                                TraciMessage & answer ) const
throw(SocketException)
{
    Storage out;
    writeCommandHead(command, content.size(), out);

    // write content
    out.writeStorage(content);

    do_send_command(command, out, answer);
}


//---------------------------------------------------------------------
/**
 * We could have put the \p content into a TraciMessage and call the other
 * send_command but that would require another copy operation, so, we avoid
 * that.
 */
void TraCIClient::send_command( CommandId command,
                                const traci::TraciType & content,
                                TraciMessage & answer ) const
throw( tcpip::SocketException )
{
    Storage out;
    writeCommandHead(command, content.getLength(), out);

    // write content
    content.write(out);

    do_send_command(command, out, answer);
}


//---------------------------------------------------------------------
void TraCIClient::send_command( CommandId command,
                                const traci::TraciType & content,
                                traci::TraciType & answer ) const
throw( tcpip::SocketException )
{
    Storage in;
    send_command(command, content, in);

    if ( !in.valid_pos() )
        throw std::runtime_error( "traci::TraCIClient::send_command: missing response command" );

    const unsigned int commandStart = in.position();
    const unsigned int commandLength = read_length(in);

    answer.read(in);

    check_position( in, commandStart + commandLength, "TraCIClient::command_get_value" );
}


//---------------------------------------------------------------------
void TraCIClient::send_command( CommandId command, const traci::TraciType & content ) const
throw( tcpip::SocketException )
{
    Storage answer;
    send_command( command, content, answer );

    // more data in answer?
    if ( answer.valid_pos() )
        throw std::runtime_error( "traci::TraCIClient::send_command: unexpected response content" );
}


//---------------------------------------------------------------------
const Node* TraCIClient::find_node_by_traci_id( const TraCIID& traci_id )
{
    return find_node_by_traci_id_w( traci_id );
}


//---------------------------------------------------------------------
Node* TraCIClient::find_node_by_traci_id_w( const TraCIID& traci_id )
{
    TraCIIDToNodeMap::iterator it = traci_ids_to_node_.find( traci_id );
    return it != traci_ids_to_node_.end() ? it->second : NULL;
}


//---------------------------------------------------------------------
const TraCIClient::TraCIID& TraCIClient::find_traci_id_by_node( const Node& node ) const
throw( std::logic_error )
{
    NodeToTraCIIDMap::const_iterator it = node_to_traci_ids_.find( &node );
    if ( it == node_to_traci_ids_.end() )
        throw std::logic_error( "node not found" );
    return it->second;
}


//---------------------------------------------------------------------
void TraCIClient::add_node( Node* node, const TraCIID& traci_id )
{
    assert( node );
    // this will pass ownership of the pointer
    sc_->world_w().add_node( *node );
    assert( node_to_traci_ids_.find( node ) == node_to_traci_ids_.end() );
    assert( traci_ids_to_node_.find( traci_id ) == traci_ids_to_node_.end() );

    node_to_traci_ids_.insert( std::make_pair<Node*, TraCIID> ( node, traci_id ) );
    traci_ids_to_node_.insert( std::make_pair<TraCIID, Node*> ( traci_id, node ) );
    assert( traci_ids_to_node_.size() == node_to_traci_ids_.size() );

    // Inform the listeners
    for ( TraCINodeChangeListenerList::iterator it = node_change_listeners_.begin();
            it != node_change_listeners_.end(); ++it )
    {
        ( *it )->node_added( *node, traci_id );
    }
}
//---------------------------------------------------------------------
void TraCIClient::remove_node_from_map( Node* node )
{
    assert( node );

    TraCIID ti;
    try
    {
        ti = find_traci_id_by_node( *node );
    }
    catch ( std::logic_error )
    {
        assert( traci_ids_to_node_.size() == node_to_traci_ids_.size() );
        // Not found, nothing to remove!
        return;
    }

    // Inform the listeners
    for ( TraCINodeChangeListenerList::iterator it = node_change_listeners_.begin();
            it != node_change_listeners_.end(); ++it )
    {
        ( *it )->node_removed( *node, ti );
    }

    TraCIIDToNodeMap::iterator ttn = traci_ids_to_node_.find( ti );
    NodeToTraCIIDMap::iterator ntt = node_to_traci_ids_.find( node );
    if ( ttn != traci_ids_to_node_.end() )
    {
        //assert(ttn != traci_ids_to_node_.end());
        traci_ids_to_node_.erase( ttn );
    }
    if ( ntt != node_to_traci_ids_.end() )
    {
        //assert(ntt != node_to_traci_ids_.end());
        node_to_traci_ids_.erase( ntt );
    }
    assert( traci_ids_to_node_.size() == node_to_traci_ids_.size() );
}


//---------------------------------------------------------------------
bool TraCIClient::check_domain( int domain, const TraCIID& traci_id, bool abrt,
                                const std::string & caller ) const
{
    if ( domain == traci_id.domain() )
        return true;
    if ( abrt )
    {
        std::cerr << "Error in method TraCIClient::" << caller << ": "
                  << "Command not applicable for elements in domain " << traci_id.domain()
                  << std::endl;
        abort();
    }
    return false;
}


//---------------------------------------------------------------------
TraCIClient::NodeToTraCIIDMap::const_iterator TraCIClient::begin_traci_nodes() const
throw()
{
    return node_to_traci_ids_.begin();
}


//---------------------------------------------------------------------
TraCIClient::NodeToTraCIIDMap::const_iterator TraCIClient::end_traci_nodes() const
throw()
{
    return node_to_traci_ids_.end();
}


//---------------------------------------------------------------------
double TraCIClient::timeSumoToShawn(int sumoTime) const
{
    return static_cast<double>(sumoTime / 1000);
}


//---------------------------------------------------------------------
int TraCIClient::timeShawnToSumo(double shawnTime) const
{
    return static_cast<int>(shawnTime * 1000.0);
}


//---------------------------------------------------------------------
//---TraCIID---
//---------------------------------------------------------------------
TraCIClient::TraCIID::TraCIID() :
        domain_( -1 ),
        id_()
{
}


//---------------------------------------------------------------------
TraCIClient::TraCIID::TraCIID( int d, const std::string & i ) :
        domain_( d ),
        id_( i )
{
}


//---------------------------------------------------------------------
TraCIClient::TraCIID::TraCIID( const TraCIID& o ) :
        domain_( o.domain() ),
        id_( o.id() )
{
}


//---------------------------------------------------------------------
TraCIClient::TraCIID::~TraCIID()
{
}


//---------------------------------------------------------------------
int TraCIClient::TraCIID::domain() const
{
    return domain_;
}


//---------------------------------------------------------------------
const std::string & TraCIClient::TraCIID::id() const
{
    return id_;
}


//---------------------------------------------------------------------
TraCIClient::TraCIID& TraCIClient::TraCIID::operator= ( const TraCIClient::TraCIID& rv )
{
    if ( this == & rv )
        return *this;
    id_ = rv.id();
    domain_ = rv.domain();
    return *this;
}


//---------------------------------------------------------------------
int operator< ( const TraCIClient::TraCIID& left,
                const TraCIClient::TraCIID& right )
{
    if ( left.domain_ < right.domain_ )
        return 1;
    if ( left.domain_ > right.domain_ )
        return 0;
    if ( left.id_ < right.id_ )
        return 1;
    if ( left.id_ > right.id_ )
        return 0;
    return 0;
}


//---------------------------------------------------------------------
int operator> ( const TraCIClient::TraCIID& left,
                const TraCIClient::TraCIID& right )
{
    return ( right < left );
}


//---------------------------------------------------------------------
int operator== ( const TraCIClient::TraCIID& left,
                 const TraCIClient::TraCIID& right )
{
    if ( left.domain_ == right.domain_ && left.id_ == right.id_ )
        return 1;
    return 0;
}


}   // namespace traci

#endif  // #ifdef ENABLE_TRACICLIENT

