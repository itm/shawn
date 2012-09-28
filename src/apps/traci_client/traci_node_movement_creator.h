/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef SHAWN_APPS_TRACI_CLIENT_TRACI_NODE_MOVEMENT_CREATOR_H
#define SHAWN_APPS_TRACI_CLIENT_TRACI_NODE_MOVEMENT_CREATOR_H

#include "_apps_enable_cmake.h"

#include <string>
#include <list>
#include <map>
#include <memory>

#include "sys/node_movements/playback/node_movement_creator.h"
#include "sys/node_movements/playback/node_movement_task.h"
#include "sys/misc/node_change_listener.h"
#include "sys/processors/processor_factory.h"

#include "apps/tcpip/socket.h"
#include "apps/tcpip/storage.h"

#include "mobility_interface_constants.h"
#include "tracitype.h"
#include "dynamic_length_datatypes.h"

#ifdef ENABLE_TRACICLIENT

namespace traci
{


class TraCIClient :
            public shawn::NodeMovementCreator,
            public shawn::NodeChangeListener
{
    friend class shawn::SimulationTaskNodeMovement;

public:
    /// Type for command ID in Traci messages
    typedef  unsigned char  CommandId;
    /// Type for variable in get(set commands
    typedef  unsigned char  VariableId;

    ~TraCIClient();

    /// Get single instance.
    static TraCIClient* instance();

    /// Executes traffic simulation until TraCIClient::target_time_.
    shawn::MovementInfo* next_movement( void ) throw( std::runtime_error );

    void reset();

    /// Is called whenever a node gets added to the world
    virtual void node_added( shawn::Node& ) throw();
    /// Is called whenever a node gets removed from the world
    virtual void node_removed( shawn::Node& ) throw();
    /// Is called whenever the id of a node has changed.
    virtual void id_changed( int, int ) throw();
    /// Is called when the world gets deleted.
    virtual bool invalidate( void ) throw();

    struct TraCIID;
    typedef std::map<const shawn::Node*, const TraCIClient::TraCIID> NodeToTraCIIDMap;
    typedef std::map<const TraCIClient::TraCIID, shawn::Node*> TraCIIDToNodeMap;

    TraCIClient::NodeToTraCIIDMap::const_iterator begin_traci_nodes() const throw();
    TraCIClient::NodeToTraCIIDMap::const_iterator end_traci_nodes() const throw();

    // Interface for listeners
    class TraCINodeChangeListener;

    /** \name Commands
     *
     *  These member functions send special TraCI-commands and analyze the answer.
     */
    //@{

    /// Send set maximum speed command (0x11).
    void command_set_maximum_speed( const shawn::Node& node, double max_speed );

    /// Send stop node command (0x12) in 2DPosition format.
    void command_stop( const shawn::Node& node, double x, double y, double radius,
                       double wait_time );

    /// Send change route command (0x30).
    void command_change_route( const shawn::Node& node, std::string road_id,
                               double travel_time );

    /**
        Send change lane command (0x30).

        Command not implemented because it's not supported by TraCI any more. Use
        appropriate vehicle state change commands instead.
    */
    void command_change_lane( const shawn::Node& node, char lane, float time );

    /// Send simulation step command and get %position of \p node
    bool command_roadmap_position( const shawn::Node& node, double targetTime,
                                   std::string& roadId_, float& relPosition_,
                                   std::string& laneID_ );

    /// Get %position of \p node by vehicle value retrieval
    void command_roadmap_position( const shawn::Node & node,
                                   std::string & laneId, float & laneOffset ) const;

    /// Convenience function to call TraCIClient::command_get_value with VariableAndId structure
    void command_get_value( CommandId command, VariableId variable, const std::string & sumoId,
                            traci::TraciType & value,
                            const traci::TraciType & extension = traci::EmptyType(),
                            CommandId response_command = 0 ) const;

    /// Convenience function which derives the value type from command and variable number
    template <CommandId Command, VariableId Variable>
    typename VariableTraciType<Command, Variable>::Type command_get_value(
                            const std::string & sumoId,
                            const traci::TraciType & extension = traci::EmptyType(),
                            CommandId response_command = 0 ) const;

    /// Convenience function with separate parameters for variable number and SUMO ID
    template < class ValueType >
    void command_set_value( CommandId command, VariableId variable, const std::string & sumoId,
                            const ValueType & value );

    /// Convenience function which derives the value type from command and variable number
    template <CommandId Command, VariableId Variable>
    void command_set_value( const std::string & sumoId, const typename VariableTraciType<Command, Variable>::Type & value );

    //@}

    /// Send arbitrary command_content and return its answer
    void send_command( CommandId command,           TraciMessage & content,     TraciMessage &answer ) const throw( tcpip::SocketException );
    /// Send arbitrary TraciType command and return its answer
    void send_command( CommandId command, const traci::TraciType & content,     TraciMessage &answer ) const throw( tcpip::SocketException );
    /// Send arbitrary TraciType command and return answer in TraciType
    void send_command( CommandId command, const traci::TraciType & content, traci::TraciType &answer ) const throw( tcpip::SocketException );
    /// Send arbitrary TraciType command without expected answer
    void send_command( CommandId command, const traci::TraciType & content                           ) const throw( tcpip::SocketException );

    /// Read length from storage
    unsigned int read_length( TraciMessage &storage ) const;

    /// Check if actual position in \p storage is as expected.
    void check_position( const TraciMessage &storage, unsigned int expectedPosition,
                         const std::string & callerName = "" ) const;

    /// Convert SUMO time to Shawn time
    double timeSumoToShawn(int sumoTime) const;
    /// Convert Shawn time to SUMO time
    int timeShawnToSumo(double shawnTime) const;

public:

    const shawn::Node* find_node_by_traci_id( const TraCIID& traci_id );
    shawn::Node* find_node_by_traci_id_w( const TraCIID& traci_id );
    const TraCIID& find_traci_id_by_node( const shawn::Node& node ) const throw( std::logic_error );
    shawn::Node* new_node( const TraCIID& traci_id, bool attach_processors_by_domain = true );
    void add_traffic_lights();
    void add_node_change_listener( TraCIClient::TraCINodeChangeListener& listener ) throw();

private:
    void do_send_command( const CommandId command, const TraciMessage &out, TraciMessage &in ) const;
    /// Write command ID and length into \p storage
    void writeCommandHead( const CommandId command, const std::size_t contentLength, TraciMessage & storage ) const;
    void checkCommandId( CommandId expectedId, CommandId actualId, const std::string &functionName = "" ) const;

    void run( shawn::SimulationController & sc ) throw();
    /// Make SUMO run until \p targetTime
    void runSumo(double targetTime);

    // Connect and disconnect to the mobility server
    // The connect method is called by startSimStepHandler
    // The close method is called by the destructor
    bool connect();

    void close();

    void fetch_processor_factories();

    bool check_domain( int domain, const TraCIID& traci_id, bool abrt = false, const std::string & caller = "" ) const;

    void check_for_unused_vehicle_nodes();

    void add_node( shawn::Node* node, const TraCIID& traci_id );

    void remove_node_from_map( shawn::Node* node );

    // Constructors
    TraCIClient();
    TraCIClient( const TraCIClient& );

    // Internal variables
    shawn::SimulationController* sc_;
    std::auto_ptr<tcpip::Socket> socket_;
    StringListTypeWithTypeId nextStepVehicles;
    std::string remotehost_;
    int remoteport_;
    double time_interval_;
    double target_time_;
    double feed_time_;
    // Every domain has it's associated processor factories map<domain,processor factories>
    std::map<int, std::set<shawn::ProcessorFactoryHandle>* > domain_processor_factories_;
    // Current nodes of the step
    std::set<TraCIID> current_vehicle_nodes_;
    // Two-way "node id" mapping from TraCI to shawn
    NodeToTraCIIDMap node_to_traci_ids_;
    TraCIIDToNodeMap traci_ids_to_node_;

    typedef std::list<TraCIClient::TraCINodeChangeListener*> TraCINodeChangeListenerList;
    TraCINodeChangeListenerList node_change_listeners_;

    /// Domain ID numbers
    // we use an enum instead of static const int to avoid static initialization problems
    enum DomainIds
    {
        DomainVehicle       = 0x01,
        DomainTrafficLight  = 0x02
    };
    static const int DOMAIN_IDS[];
    static const std::string DOMAIN_NAMES[];
};


/**
 *  If the compiler cannot derive the correct type a specialization of the template
 *  VariableTraciType<CommandId Command, VariableId Variable> for the concrete
 *  command/variable pair is missing. As a workaround use another version of this
 *  function.
 */
template <TraCIClient::CommandId Command, TraCIClient::VariableId Variable>
typename VariableTraciType<Command, Variable>::Type TraCIClient::command_get_value( const std::string & sumoId,
                                                                    const traci::TraciType & extension,
                                                                    CommandId response_command ) const
{
    typename VariableTraciType<Command, Variable>::Type result;
    command_get_value(Command, Variable, sumoId, result, extension, response_command);
    return result;
}


template < class ValueType >
inline void TraCIClient::command_set_value( CommandId command, VariableId variable,
                               const std::string & sumoId, const ValueType & value )
{
    // assemble TraciType to send
    const UbyteType variableType(variable);
    const StringType idType(sumoId);
    ConstDynamicTraciType<> out;
    out.push_back(&variableType);
    out.push_back(&idType);
    out.push_back(&value);
    send_command(command, out);
}


/**
 *  If the compiler cannot derive the correct type a specialization of the template
 *  VariableTraciType<CommandId Command, VariableId Variable> for the concrete
 *  command/variable pair is missing. As a workaround use another version of this
 *  function.
 */
template <TraCIClient::CommandId Command, TraCIClient::VariableId Variable>
inline void TraCIClient::command_set_value( const std::string & sumoId,
                               const typename VariableTraciType<Command, Variable>::Type & value )
{
    command_set_value( Command, Variable, sumoId, value );
}


struct TraCIClient::TraCIID
{
    TraCIID();
    TraCIID( int d, const std::string & i );
    TraCIID( const TraCIID& );
    virtual ~TraCIID();
    int domain() const;
    const std::string & id() const;
    TraCIClient::TraCIID& operator= ( const TraCIClient::TraCIID& rv );
private:
    //friend class traci::TraCIClient;
    friend int operator< ( const TraCIClient::TraCIID&,
                           const TraCIClient::TraCIID& );
    friend int operator> ( const TraCIClient::TraCIID&,
                           const TraCIClient::TraCIID& );
    friend int operator== ( const TraCIClient::TraCIID&,
                            const TraCIClient::TraCIID& );

    // TODO: make this an enum
    int domain_;
    std::string id_;
};


class TraCIClient::TraCINodeChangeListener
{
public:
    virtual ~TraCINodeChangeListener() {}
    virtual void node_added( shawn::Node& node, const TraCIClient::TraCIID& traci_id ) throw() = 0;
    virtual void node_removed( shawn::Node& node, const TraCIClient::TraCIID& traci_id ) throw() = 0;
protected:
    TraCINodeChangeListener() {}
private:
    TraCINodeChangeListener( const TraCINodeChangeListener& );
};


}   // namespace traci

#endif  // #ifdef ENABLE_TRACICLIENT
#endif  // #ifndef SHAWN_APPS_TRACI_CLIENT_TRACI_NODE_MOVEMENT_CREATOR_H

