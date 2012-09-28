/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_VIRTUAL_LINK_TRANSMISSION_MODEL_H_
#define __SHAWN_APPS_TESTBEDSERVICE_VIRTUAL_LINK_TRANSMISSION_MODEL_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/core/testbedservice_client.h"
#include "apps/testbedservice/core/wsnapi_client.h"
#include "apps/testbedservice/sockets/socket_client.h"
#include "apps/wiselib/ext_iface_processor.h"
#include "apps/wiselib/wiselib_message.h"
#include "sys/transm_models/chainable_transmission_model.h"
#include "sys/event_scheduler.h"
#include <list>
#include <set>
#include <map>

namespace testbedservice
{

   /**
   */
   class VirtualLinkTransmissionModel
      : public shawn::ChainableTransmissionModel,
         public shawn::EventScheduler::EventHandler
   {
      class VirtualLinkInfo
      {
      public:
         VirtualLinkInfo()
            : wsnapi_client ( 0 ),
               socket_client( 0 )
         {}
         // -----------------------------------------------------------------
         int shawn_node_id;            /// Node ID in Shawn
         int virtual_node_id;          /// Node ID at another portal server
         std::string shawn_node_urn;   /// Node URN in Shawn
         std::string virtual_node_urn; /// Node URN at another portal server
         std::string remote_uri;       /// URI of remote WSN API
         WsnApiClient *wsnapi_client;  /// Remote WSN API webservice
         std::string remote_socket;    /// "host:port" of remote socket connection
         SocketClient *socket_client;  /// Remote Socket Server
      };

      typedef std::list<VirtualLinkInfo*> VirtualLinkList;
      typedef VirtualLinkList::iterator VirtualLinkListIterator;
      typedef VirtualLinkList::const_iterator ConstVirtualLinkListIterator;

      typedef std::map<std::string, WsnApiClient*> WsnApiClientMap;
      typedef WsnApiClientMap::iterator WsnApiClientMapIterator;
      typedef WsnApiClientMap::const_iterator ConstWsnApiClientMapIterator;

      typedef std::map<std::string, SocketClient*> SocketClientMap;
      typedef SocketClientMap::iterator SocketClientMapIterator;
      typedef SocketClientMap::const_iterator ConstSocketClientMapIterator;

      typedef std::map<std::string, std::set<std::string> > DeadlinksMap;
      typedef DeadlinksMap::iterator DeadlinksMapIterator;
      typedef DeadlinksMap::const_iterator ConstDeadlinksMapIterator;

   public:
      typedef wiselib::ExtIfaceProcessor::ExtIfaceWiselibMessage WiselibMessage;

      ///@name Construction, destruction and lifecycle support
      ///@{
      VirtualLinkTransmissionModel();
      virtual ~VirtualLinkTransmissionModel();
      ///@}

      ///@name Transmission model implementation
      ///@{
      ///
      virtual void init( void ) throw();
      ///
      virtual void reset( void ) throw();
      ///
      virtual void send_message( shawn::TransmissionModel::MessageInfo& mi ) throw();
      ///@}

      ///
      void pass_to_webservice_client( WiselibMessage& message, VirtualLinkInfo& info ) throw();

      ///@name Event Handler
      ///@{
      virtual void timeout( shawn::EventScheduler&,
                            shawn::EventScheduler::EventHandle,
                            double,
                            shawn::EventScheduler::EventTagHandle& ) throw();
      ///@}
      // --------------------------------------------------------------------
      void add_virtual_link( const std::string& shawn_node, const std::string& virtual_node,
                             const std::string& remote_uri,
                             const std::string& remote_socket_host = "",
                             const std::string& remote_socket_port = "" ) throw();
      void remove_virtual_link( std::string shawn_node, std::string virtual_node ) throw();
      // --------------------------------------------------------------------
      void enable_physical_link( std::string source, std::string destination ) throw();
      void disable_physical_link( std::string source, std::string destination ) throw();
      // --------------------------------------------------------------------
      int node_id_from_urn( std::string urn ) const throw();
      // --------------------------------------------------------------------
      inline void set_testbedservice_client( TestbedServiceClient& client ) throw()
      { client_ = &client; }
      // --------------------------------------------------------------------
      inline TestbedServiceClient& testbedservice_client( void ) throw()
      { assert( client_ ); return *client_; }

   private:
      VirtualLinkListIterator find_virtual_link_w( std::string shawn_node, std::string virtual_node ) throw();
      // --------------------------------------------------------------------
      TestbedServiceClient *client_;

      VirtualLinkList virtual_links_;
      WsnApiClientMap wsnapis_clients_;
      SocketClientMap socket_clients_;

      DeadlinksMap dead_links_;
   };

}

#endif
#endif
