/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/ws_handler/virtual_link_control.h"
#include "apps/testbedservice/virtual_links/virtual_link_transmission_model.h"
#include "apps/testbedservice/virtual_links/virtual_link_message.h"
#include "apps/testbedservice/util/ws_helpers.h"
#include "apps/testbedservice/core/wsnapi_serverH.h"
#include "sys/processors/processor_keeper.h"
#include "sys/worlds/processor_world_factory.h"
#include "sys/misc/random/basic_random.h"
#include "sys/logging/logger.h"
#include "sys/misc/tokenize.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include "sys/processor.h"
#include <boost/bind.hpp>

namespace testbedservice
{

   static testbedservice::VirtualLinkControl *virtual_link_control_ = 0;
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   VirtualLinkControl::
   VirtualLinkControl()
      : vltm_        ( 0 ),
         controller_ ( 0 ),
         sc_         ( 0 )
   {}
   // ----------------------------------------------------------------------
   VirtualLinkControl::
   ~VirtualLinkControl()
   {}
   // ----------------------------------------------------------------------
   void
   VirtualLinkControl::
   timeout( shawn::EventScheduler&,
            shawn::EventScheduler::EventHandle,
            double,
            shawn::EventScheduler::EventTagHandle& eth )
      throw()
   {
      std::cout << "TIMEOUT AT " << sc_->world().current_time() << std::endl;

      VirtualLinkMessage *msg = dynamic_cast<VirtualLinkMessage*>( eth.get() );
      if ( msg )
      {
         std::cout << "Got VirtualLinkMessage from " << msg->source  << std::endl;
      }
      else
         std::cout << "ETH of unknown type" << std::endl;
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkControl::
   init( shawn::SimulationController& sc, TestbedServiceClient& controller, VirtualLinkTransmissionModel *vltm )
      throw()
   {
      sc_ = &sc;
      controller_ = &controller;
      vltm_ = vltm;
      virtual_link_control_ = this;
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkControl::
   set_virtual_link( std::string id, std::string source, std::string destination, std::string uri )
      throw()
   {
      std::cout << "set virtual link from " << source << " to "
         << destination << " at " << uri << std::endl;

      NodeIdVector response_nodes;
      StatusValueVector response_values;
      StatusMsgVector response_msgs;

      virtual_link_transmission_model_w().add_virtual_link( source, destination, uri );

      response_nodes.push_back( source );
      response_values.push_back( 1 );
      response_msgs.push_back( "" );
      controller_w().send_receive_status( id, response_nodes, response_values, response_msgs );
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkControl::
   destroy_virtual_link( std::string id, std::string source, std::string destination )
      throw()
   {
      std::cout << "destroy virtual link from " << source << " to "
         << destination << std::endl;

      NodeIdVector response_nodes;
      StatusValueVector response_values;
      StatusMsgVector response_msgs;

      virtual_link_transmission_model_w().remove_virtual_link( source, destination );

      response_nodes.push_back( source );
      response_values.push_back( 1 );
      response_msgs.push_back( "" );
      controller_w().send_receive_status( id, response_nodes, response_values, response_msgs );
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkControl::
   enable_physical_link( std::string id, std::string source, std::string destination )
   {
      std::cout << "enable physical link from " << source << " to "
         << destination << std::endl;

      NodeIdVector response_nodes;
      StatusValueVector response_values;
      StatusMsgVector response_msgs;

      virtual_link_transmission_model_w().enable_physical_link( source, destination );

      response_nodes.push_back( source );
      response_values.push_back( 1 );
      response_msgs.push_back( "" );
      controller_w().send_receive_status( id, response_nodes, response_values, response_msgs );
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkControl::
   disable_physical_link( std::string id, std::string source, std::string destination )
   {
      std::cout << "disable physical link from " << source << " to "
         << destination << std::endl;

      NodeIdVector response_nodes;
      StatusValueVector response_values;
      StatusMsgVector response_msgs;

      virtual_link_transmission_model_w().disable_physical_link( source, destination );

      response_nodes.push_back( source );
      response_values.push_back( 1 );
      response_msgs.push_back( "" );
      controller_w().send_receive_status( id, response_nodes, response_values, response_msgs );
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkControl::
   add_virtual_message( std::string dest, TestbedserviceMessage message )
      throw()
   {
      boost::lock_guard<boost::mutex> sc_lock( sc_mutex_ );

      if ( sc_ )
      {
         // TODO: check if buffer is of correct size for vlink message
         VirtualLinkMessage *vlink_msg;

         if ( message.buffer[0] == NODE_OUTPUT_VIRTUAL_LINK )
            vlink_msg = new VirtualLinkMessage( message.buffer );
         else if ( message.buffer[0] == 10 &&
                     message.buffer[1] == VIRTUAL_LINK_MESSAGE )
         {
#warning Quick hack to suppress isense packet type sent by testbed-runtime
            vlink_msg = new VirtualLinkMessage( message.buffer + 2 );
            vlink_msg->type = NODE_OUTPUT_VIRTUAL_LINK;
         }
         else if (message.buffer[0] == VIRTUAL_LINK_MESSAGE )
         {
            vlink_msg = new VirtualLinkMessage( message.buffer + 1 );
            vlink_msg->type = NODE_OUTPUT_VIRTUAL_LINK;
         }
         else
         {
            std::cerr << "Received unknown virtual link message. Skip." << std::endl;
            return;
         }

         vlink_msg->pass_to = virtual_link_transmission_model().node_id_from_urn( dest );

//          std::cout << "CALLED AT " << sc_->world().current_time() << std::endl;
         double now = sc_->world().current_time();
         sc_->world_w().scheduler_w().new_event( virtual_link_transmission_model_w(), now, vlink_msg );
//          std::cout << "ADD EVENT REMOTELY AT " << now << std::endl;
      }
   }
}

namespace wsnapi_server
{

   static bool virtual_link_control_available( void )
   {
      if ( testbedservice::virtual_link_control_ )
      {
         return true;
      }
      else
         std::cerr << "Error processing ws call - virtual_link_control_ not set." << std::endl;

      return false;
   }

   // -----------------------------------------------------------------------
   // ------------- Web Service Calls Network Control -----------------------
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__setVirtualLink( soap*,
            ns1__setVirtualLink *ns1__setVirtualLink_,
            ns1__setVirtualLinkResponse *ns1__setVirtualLinkResponse_ )
   {
      if ( !virtual_link_control_available() )
         return SOAP_OK;

      std::string source = std::string( ns1__setVirtualLink_->sourceNode );
      std::string destination = std::string( ns1__setVirtualLink_->targetNode );
      std::string remote_uri = std::string( ns1__setVirtualLink_->remoteServiceInstance );

      std::string id = testbedservice::create_response_id();
      char *response_id = testbedservice::allocate_string( ns1__setVirtualLinkResponse_->soap, id );
      ns1__setVirtualLinkResponse_->return_ = response_id;

      boost::thread thrd( boost::bind(
         &testbedservice::VirtualLinkControl::set_virtual_link,
         testbedservice::virtual_link_control_,
         id, source, destination, remote_uri ) );

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__destroyVirtualLink( soap*,
            ns1__destroyVirtualLink *ns1__destroyVirtualLink_,
            ns1__setVirtualLinkResponse *ns1__destroyVirtualLinkResponse_)
   {
      if ( !virtual_link_control_available() )
         return SOAP_OK;

      std::string source = std::string( ns1__destroyVirtualLink_->sourceNode );
      std::string destination = std::string( ns1__destroyVirtualLink_->targetNode );

      std::string id = testbedservice::create_response_id();
      char *response_id = testbedservice::allocate_string(
         ns1__destroyVirtualLinkResponse_->soap, id );
      ns1__destroyVirtualLinkResponse_->return_ = response_id;

      boost::thread thrd( boost::bind(
         &testbedservice::VirtualLinkControl::destroy_virtual_link,
         testbedservice::virtual_link_control_,
         id, source, destination ) );

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__enablePhysicalLink( soap*,
            ns1__enablePhysicalLink *ns1__enablePhysicalLink_,
            ns1__enablePhysicalLinkResponse *ns1__enablePhysicalLinkResponse_)
   {
      if ( !virtual_link_control_available() )
         return SOAP_OK;

      std::string source = std::string( ns1__enablePhysicalLink_->nodeA );
      std::string destination = std::string( ns1__enablePhysicalLink_->nodeB );

      std::string id = testbedservice::create_response_id();
      char *response_id = testbedservice::allocate_string(
         ns1__enablePhysicalLinkResponse_->soap, id );
      ns1__enablePhysicalLinkResponse_->return_ = response_id;

      boost::thread thrd( boost::bind(
         &testbedservice::VirtualLinkControl::enable_physical_link,
         testbedservice::virtual_link_control_,
         id, source, destination ) );

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__disablePhysicalLink( soap*,
            ns1__disablePhysicalLink *ns1__disablePhysicalLink_,
            ns1__disablePhysicalLinkResponse *ns1__disablePhysicalLinkResponse_)
   {
      if ( !virtual_link_control_available() )
         return SOAP_OK;

      std::string source = std::string( ns1__disablePhysicalLink_->nodeA );
      std::string destination = std::string( ns1__disablePhysicalLink_->nodeB );

      std::string id = testbedservice::create_response_id();
      char *response_id = testbedservice::allocate_string(
         ns1__disablePhysicalLinkResponse_->soap, id );
      ns1__disablePhysicalLinkResponse_->return_ = response_id;

      boost::thread thrd( boost::bind(
         &testbedservice::VirtualLinkControl::disable_physical_link,
         testbedservice::virtual_link_control_,
         id, source, destination ) );

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__getFilters( soap*,
            ns1__getFilters *ns1__getFilters_,
            ns1__getFiltersResponse *ns1__getFiltersResponse_)
   {
      // TODO !
      std::cout << "shawn::testbedservice::getFilters" << std::endl;
      return SOAP_OK;
   }

}

#endif
