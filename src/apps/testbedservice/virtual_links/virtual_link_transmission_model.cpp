/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/virtual_links/virtual_link_transmission_model.h"
#include "apps/testbedservice/virtual_links/virtual_link_message.h"
#include "apps/testbedservice/virtual_links/virtual_link_message_in.h"
#include "apps/testbedservice/util/types.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "apps/wiselib/ext_iface_processor.h"
#include "sys/edge_model.h"
#include "sys/world.h"
#include <algorithm>
#include <sstream>


namespace testbedservice
{

   // ----------------------------------------------------------------------
   VirtualLinkTransmissionModel::
   VirtualLinkTransmissionModel()
      : client_ ( 0 )
   {}
   // ----------------------------------------------------------------------
   VirtualLinkTransmissionModel::
   ~VirtualLinkTransmissionModel()
   {}
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   init( void )
      throw()
   {
      ChainableTransmissionModel::init();
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   reset( void )
      throw()
   {
      ChainableTransmissionModel::reset();
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   send_message( shawn::TransmissionModel::MessageInfo& mi )
      throw()
   {
      WiselibMessage *message =
         dynamic_cast<WiselibMessage*>( mi.msg_.get() );
      if ( message )
      {
         for ( VirtualLinkListIterator
                  it = virtual_links_.begin();
                  it != virtual_links_.end();
                  ++it )
            if ( (*it)->shawn_node_id == message->source().id() )
            {
               pass_to_webservice_client( *message, *(*it) );
            }

         DeadlinksMapIterator it = dead_links_.find(message->source().label());
         if ( it != dead_links_.end() )
         {
            // TODO: vlinkmessage.destination() != URN !!!!
            std::stringstream ss;
            ss << message->destination();
            if ( it->second.find(ss.str()) != it->second.end() )
               return;
         }
      }

      pass_to_chain( mi );
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   pass_to_webservice_client( testbedservice::VirtualLinkTransmissionModel::WiselibMessage& message,
                              testbedservice::VirtualLinkTransmissionModel::VirtualLinkInfo& info )
      throw()
   {
      VirtualLinkMessageIn vmsg;
//       vmsg.type = NODE_OUTPUT_VIRTUAL_LINK;
      vmsg.type = VIRTUAL_LINK_MESSAGE;
      vmsg.rssi = 127;
      vmsg.lqi = 127;
      vmsg.payload_size = message.payload_size();
      vmsg.destination = message.destination();
      vmsg.source = message.source().id();
      vmsg.payload = new uint8_t[vmsg.payload_size];
      memcpy( vmsg.payload, message.payload(), vmsg.payload_size );
//       std::cout << "Send " << vmsg << std::endl;

      NodeIdVector dests;
      dests.push_back( info.virtual_node_urn );

      uint8_t len = vmsg.length();
      uint8_t *bytes = vmsg.to_bytes();
      // for debugging purposes, send to controller
      testbedservice_client().send_message( info.shawn_node_urn, len, bytes );

      TestbedserviceMessage tsm;
      tsm.source = info.shawn_node_urn;
      tsm.timestamp = time(0);
      tsm.size = len;
      tsm.buffer = bytes;
      // send to remote wsn api and socket connection - if they exist
      if ( info.wsnapi_client )
         info.wsnapi_client->send_message( dests, tsm );
      if ( info.socket_client )
         info.socket_client->send_binary_data( info.shawn_node_urn, info.virtual_node_urn, bytes, len );

      delete bytes;
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   timeout( shawn::EventScheduler& ev, shawn::EventScheduler::EventHandle eh,
            double t, shawn::EventScheduler::EventTagHandle& eth )
      throw ()
   {
      VirtualLinkMessage *msg = dynamic_cast<VirtualLinkMessage*>( eth.get() );
      if ( msg )
      {
//          std::cout << "Rcvd " << *msg << std::endl;
// 
//          std::cout << " -> deliver to: " << msg->pass_to
//                   << "; from " << (int)msg->source
//                   << "; addressed to " << (int)msg->destination << std::endl;
         shawn::Node* node = world_w().find_node_by_id_w( msg->pass_to );
         if ( node )
         {
            wiselib::ExtIfaceProcessor *extiface =
               node->get_processor_of_type_w<wiselib::ExtIfaceProcessor>();
            if ( extiface )
            {
               extiface->receive_vlink( msg );
//                std::cout << "*** ok, delivered " << std::endl;
            }
         }
      }
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   add_virtual_link( const std::string& shawn_node, const std::string& virtual_node,
                     const std::string& remote_uri,
                     const std::string& remote_socket_host,
                     const std::string& remote_socket_port )
      throw()
   {
      if ( find_virtual_link_w( shawn_node, virtual_node ) != virtual_links_.end() )
         return;

      std::string remote_socket = remote_socket_host + ":" + remote_socket_port;
      VirtualLinkInfo *link = new VirtualLinkInfo();
      link->shawn_node_urn = shawn_node;
      link->virtual_node_urn = virtual_node;
      link->shawn_node_id = node_id_from_urn( shawn_node );
      link->virtual_node_id = node_id_from_urn( virtual_node );
      link->remote_uri = remote_uri;
      link->remote_socket = remote_socket;
      std::cout << "add vlink from " << link->shawn_node_urn << " to " << link->virtual_node_urn << std::endl;

      if ( remote_uri != "" )
      {
         if ( wsnapis_clients_.find( remote_uri ) == wsnapis_clients_.end() )
         {
            std::cout << "create new wsn api to " << remote_uri << std::endl;
            
            WsnApiClient *client = new WsnApiClient();
            client->init( remote_uri );
            std::string sendmode = world().simulation_controller().environment().optional_string_param( "wsnapi_send_mode", SEND_MODE_DEFAULT );
            client->set_sendmode( sendmode );
            
            wsnapis_clients_[remote_uri] = client;
            link->wsnapi_client = client;
         }
         else
         {
            std::cout << "use existing wsn api to " << remote_uri << std::endl;
            link->wsnapi_client = wsnapis_clients_[remote_uri];
         }
      }

      if ( remote_socket != ":" )
      {
         if ( socket_clients_.find( remote_socket ) == socket_clients_.end() )
         {
            std::cout << "create new socket client to " << remote_uri << std::endl;
            SocketClient *client = new SocketClient();
            client->init( world().simulation_controller(), remote_socket_host, remote_socket_port );
            socket_clients_[remote_socket] = client;
            link->socket_client = client;
         }
         else
         {
            std::cout << "use existing socket client to " << remote_socket << std::endl;
            link->socket_client = socket_clients_[remote_socket];
         }
      }

      virtual_links_.push_back( link );
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   remove_virtual_link( std::string shawn_node, std::string virtual_node )
      throw()
   {
      VirtualLinkListIterator it =
         find_virtual_link_w( shawn_node, virtual_node );

      if ( it == virtual_links_.end() )
         return;

      VirtualLinkInfo *link = *it;
      virtual_links_.erase( it );
      delete link;
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   enable_physical_link( std::string source, std::string destination )
      throw()
   {
      dead_links_[source].insert( destination );
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   disable_physical_link( std::string source, std::string destination )
      throw()
   {
      dead_links_[source].erase( destination );
   }
   // ----------------------------------------------------------------------
   int
   VirtualLinkTransmissionModel::
   node_id_from_urn( std::string urn )
      const throw()
   {
      int pos = urn.find_last_of( ":" );
      if ( pos == urn.npos )
         return -1;

      std::string urn_part = urn.substr( pos + 1 );
      std::istringstream int_stream( urn_part );

      int id;
      if ( !(int_stream >> id) )
         return -1;

      return id;
   }
   // ----------------------------------------------------------------------
   VirtualLinkTransmissionModel::VirtualLinkListIterator
   VirtualLinkTransmissionModel::
   find_virtual_link_w( std::string shawn_node, std::string virtual_node )
      throw()
   {
      for ( VirtualLinkListIterator
               it = virtual_links_.begin();
               it != virtual_links_.end();
               ++it )
      {
         if ( (*it)->shawn_node_urn == shawn_node &&
                (*it)->virtual_node_urn == virtual_node )
            return it;
      }

      return virtual_links_.end();
   }

}

#endif
