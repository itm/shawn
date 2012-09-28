/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/ws_handler/node_control.h"
#include "apps/testbedservice/util/ws_helpers.h"
#include "apps/testbedservice/util/types.h"
#include "apps/testbedservice/core/wsnapi_serverH.h"
#include "sys/processors/processor_keeper.h"
#include "sys/worlds/processor_world_factory.h"
#include "sys/misc/tokenize.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include "sys/edge_model.h"
#include "sys/processor.h"
#include <boost/bind.hpp>
#include <sstream>

namespace testbedservice
{
   static testbedservice::NodeControl *node_control_ = 0;
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   NodeControl::
   NodeControl()
      : controller_     ( 0 ),
         vlink_control_ ( 0 ),
         sc_            ( 0 )
   {}
   // ----------------------------------------------------------------------
   NodeControl::
   ~NodeControl()
   {}
   // ----------------------------------------------------------------------
   void
   NodeControl::
   init( shawn::SimulationController& sc, TestbedServiceClient& controller, VirtualLinkControl& vlink_control )
      throw()
   {
      sc_ = &sc;
      controller_ = &controller;
      vlink_control_ = &vlink_control;
      node_control_ = this;
   }
   // ----------------------------------------------------------------------
   void
   NodeControl::
   are_nodes_alive( std::string id, std::vector<std::string> nodes )
   {
      NodeIdVector response_nodes;
      StatusValueVector response_values;
      StatusMsgVector response_msgs;

      // TODO: Make this threadsafe with respect to Shawn; -> add to EventScheduler?
      for ( std::vector<std::string>::iterator
               it = nodes.begin();
               it != nodes.end();
               ++it )
      {
         const shawn::Node *node = simulation_controller().world().find_node_by_label( *it );
         if ( node )
         {
            if ( node->state() == shawn::Processor::Inactive )
               response_values.push_back( 0 );
            else
               response_values.push_back( 1 );
         }
         else
            response_values.push_back( -1 );

         response_nodes.push_back( *it );
         response_msgs.push_back( "" );
      }

      controller().send_receive_status( id, response_nodes, response_values, response_msgs );
   }
   // ----------------------------------------------------------------------
   void
   NodeControl::
   reset_nodes( std::string id, std::vector<std::string> nodes )
   {
      NodeIdVector response_nodes;
      StatusValueVector response_values;
      StatusMsgVector response_msgs;

      // TODO: Make this threadsafe with respect to Shawn; -> add to EventScheduler?
      for ( std::vector<std::string>::iterator
               it = nodes.begin();
               it != nodes.end();
               ++it )
      {
         const shawn::Node *node = simulation_controller().world().find_node_by_label( *it );
         if ( node )
         {
            // TODO: Add reset behavior in shawn::sys
            response_values.push_back( 0 );
         }
         else
            response_values.push_back( -1 );

         response_nodes.push_back( *it );
         response_msgs.push_back( "" );
      }

      controller().send_receive_status( id, response_nodes, response_values, response_msgs );
   }
   // ----------------------------------------------------------------------
   void
   NodeControl::
   set_node_state( std::string id, std::string urn, shawn::Processor::ProcessorState state )
   {
      NodeIdVector response_nodes;
      StatusValueVector response_values;
      StatusMsgVector response_msgs;

      response_nodes.push_back( urn );
      response_msgs.push_back( "" );
      shawn::Node *node = simulation_controller_w().world_w().find_node_by_label_w( urn );
      if ( node )
      {
         for ( shawn::Node::ProcessorList::iterator
                  it = node->begin_processors_w();
                  it != node->begin_processors_w();
                  ++it )
         {
            // TODO: do this over event scheduler to change to Shawn's main thread
            (**it).set_state( state );
         }
         response_values.push_back( 1 );
      }
      else
         response_values.push_back( -1 );

      controller().send_receive_status( id, response_nodes, response_values, response_msgs );
   }
   // ----------------------------------------------------------------------
   void
   NodeControl::
   pass_message( std::string id, NodeIdVector nodes, TestbedserviceMessage message )
   {
      NodeIdVector response_nodes;
      StatusValueVector response_values;
      StatusMsgVector response_msgs;

//       std::cout << "message:" << std::endl;
//       std::cout << "  -> from: " << message.source << std::endl;
//       std::cout << "  -> size: " << message.size << std::endl;
//       std::cout << "  -> buf : ";
//       for ( int i = 0; i < message.size; i++ )
//          std::cout << int(message.buffer[i]) << " ";
//       std::cout << std::endl;

      for ( NodeIdVector::iterator it = nodes.begin(); it != nodes.end(); ++it )
      {
         if ( message.size >= 20 &&
               ( message.buffer[0] == VIRTUAL_LINK_MESSAGE || 
#warning Quick hack to suppress isense packet type sent by testbed-runtime
                  (message.buffer[0] == 10 && message.buffer[1] == VIRTUAL_LINK_MESSAGE) ) ) // isense specific on testbed-runtime
         {
//             std::cout << "new vlink message..." << std::endl;
            virtual_link_control().add_virtual_message( *it, message );
            response_values.push_back( 1 );
            response_nodes.push_back( *it );
            response_msgs.push_back( "" );
            continue;
         }
         
         shawn::Node *node =
            simulation_controller_w().world_w().find_node_by_label_w( *it );

         if ( node )
         {
            TestbedServiceProcessor *proc =
               node->get_processor_of_type_w<TestbedServiceProcessor>();
            if ( proc )
            {
               // TODO: do this over event scheduler to change to Shawn's main thread
               proc->process_message( message );
               response_values.push_back( 1 );
            }
            else
               response_values.push_back( 0 );
         }
         else
            response_values.push_back( -1 );

         response_nodes.push_back( *it );
         response_msgs.push_back( "" );
      }

      controller().send_receive_status( id, response_nodes, response_values, response_msgs );
   }
   // ----------------------------------------------------------------------
   NodeIdVector
   NodeControl::get_neighborhood( std::string urn )
   {
      NodeIdVector neighbors;
      const shawn::Node *node = simulation_controller().world().find_node_by_label( urn );
      if ( node )
      {
         for ( shawn::EdgeModel::const_adjacency_iterator
                  it = simulation_controller().world().begin_adjacent_nodes( *node );
                  it != simulation_controller().world().end_adjacent_nodes( *node );
                  ++it )
         {
            neighbors.push_back( it->label() );
         }
      }

      return neighbors;
   }
   // ----------------------------------------------------------------------
   void
   NodeControl::
   dbg_nodevector( std::string id, std::vector<std::string> nodes )
   {
      std::cout << "node vector:" << std::endl;
      std::cout << "  -> repsonse id: " << id << std::endl;
      std::cout << "  -> node cnt   : " << nodes.size() << std::endl;
      for ( std::vector<std::string>::iterator
               it = nodes.begin();
               it != nodes.end();
               ++it )
      {
         std::cout << "    **: " << *it << std::endl;
      }
   }

}

namespace wsnapi_server
{

   // -----------------------------------------------------------------------
   // -----------------------------------------------------------------------
   // -----------------------------------------------------------------------
   static bool node_control_available( void )
   {
      if ( testbedservice::node_control_ )
      {
         return true;
      }
      else
         std::cerr << "Error processing ws call - node_control_ not set." << std::endl;

      return false;
   }
   // -----------------------------------------------------------------------
   // ------------- Web Service Calls Network Control -----------------------
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__areNodesAlive( soap*,
            ns1__areNodesAlive *ns1__areNodesAlive_,
            ns1__areNodesAliveResponse *ns1__areNodesAliveResponse_ )
   {
      if ( !node_control_available() )
         return SOAP_OK;

      int node_cnt = ns1__areNodesAlive_->__sizenodes;
      std::vector<std::string> nodes;
      for ( int i = 0; i < node_cnt; i++ )
      {
         nodes.push_back( std::string(ns1__areNodesAlive_->nodes[i]) );
      }

      std::string id = testbedservice::create_response_id();
      char *response_id = testbedservice::allocate_string( ns1__areNodesAliveResponse_->soap, id );
      ns1__areNodesAliveResponse_->return_ = response_id;

      boost::thread thrd( boost::bind(
         &testbedservice::NodeControl::are_nodes_alive,
         testbedservice::node_control_,
         id, nodes ) );

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__enableNode( soap*,
            ns1__enableNode *ns1__enableNode_,
            ns1__enableNodeResponse *ns1__enableNodeResponse_ )
   {
      std::string node = std::string( ns1__enableNode_->node );
      std::string id = testbedservice::create_response_id();
      char *response_id = testbedservice::allocate_string( ns1__enableNodeResponse_->soap, id );
      ns1__enableNodeResponse_->return_ = response_id;

      boost::thread thrd( boost::bind(
         &testbedservice::NodeControl::set_node_state,
         testbedservice::node_control_,
         id, node, shawn::Processor::Active ) );

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__disableNode( soap*,
            ns1__disableNode *ns1__disableNode_,
            ns1__disableNodeResponse *ns1__disableNodeResponse_ )
   {
      std::string node = std::string( ns1__disableNode_->node );
      std::string id = testbedservice::create_response_id();
      char *response_id = testbedservice::allocate_string( ns1__disableNodeResponse_->soap, id );
      ns1__disableNodeResponse_->return_ = response_id;

      boost::thread thrd( boost::bind(
         &testbedservice::NodeControl::set_node_state,
         testbedservice::node_control_,
         id, node, shawn::Processor::Inactive ) );

      return SOAP_OK;
   }
//    // -----------------------------------------------------------------------
//    SOAP_FMAC5 int SOAP_FMAC6 __ns1__getNeighbourhood( soap*,
//             ns1__getNeighbourhood *ns1__getNeighbourhood_,
//             ns1__getNeighbourhoodResponse *ns1__getNeighbourhoodResponse_ )
//    {
//       if ( !node_control_available() )
//          return SOAP_OK;
// 
//       std::string node( ns1__getNeighbourhood_->node );
//       std::vector<std::string> neighbors =
//          testbedservice::node_control_->get_neighborhood( node );
// 
//       char **return_neighborhood_ = 0;
//       if ( neighbors.size() > 0 )
//       {
//          struct soap *soap = ns1__getNeighbourhoodResponse_->soap;
//          return_neighborhood_ = (char**)soap_malloc( soap, neighbors.size() );
//          int idx = 0;
//          for ( std::vector<std::string>::iterator
//                   it = neighbors.begin();
//                   it != neighbors.end();
//                   ++it )
//          {
//             return_neighborhood_[idx] = testbedservice::allocate_string( soap, *it );
//             idx++;
//          }
//       }
// 
//       ns1__getNeighbourhoodResponse_->__sizereturn_ = neighbors.size();
//       ns1__getNeighbourhoodResponse_->return_ = return_neighborhood_;
// 
//       return SOAP_OK;
//    }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__resetNodes( soap*,
            ns1__resetNodes *ns1__resetNodes_,
            ns1__resetNodesResponse *ns1__resetNodesResponse_ )
   {
      if ( !node_control_available() )
         return SOAP_OK;

      int node_cnt = ns1__resetNodes_->__sizenodes;
      std::vector<std::string> nodes;
      for ( int i = 0; i < node_cnt; i++ )
      {
         nodes.push_back( std::string(ns1__resetNodes_->nodes[i]) );
      }

      std::string id = testbedservice::create_response_id();
      char *response_id = testbedservice::allocate_string( ns1__resetNodesResponse_->soap, id );
      ns1__resetNodesResponse_->return_ = response_id;

      boost::thread thrd( boost::bind(
         &testbedservice::NodeControl::reset_nodes,
         testbedservice::node_control_,
         id, nodes ) );

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__send( soap*,
            ns1__send *ns1__send_,
            ns1__sendResponse *ns1__sendResponse_)
   {
      if ( !node_control_available() )
         return SOAP_OK;
      
      int node_cnt = ns1__send_->__sizenodeIds;
      std::vector<std::string> nodes;
      for ( int i = 0; i < node_cnt; i++ )
      {
         nodes.push_back( std::string(ns1__send_->nodeIds[i]) );
// std::cout << "  --> for node " << std::string(ns1__send_->nodeIds[i]) << std::endl;
      }

      std::string id = testbedservice::create_response_id();
      std::string source( ns1__send_->message->sourceNodeId );
      time_t timestamp = ns1__send_->message->timestamp;

      testbedservice::TestbedserviceMessage message;
      message.source = source;
      message.timestamp = timestamp;
      message.size = ns1__send_->message->binaryData.__size;
      message.buffer = new uint8_t[message.size];
      memcpy( message.buffer,
               (uint8_t*)ns1__send_->message->binaryData.__ptr,
               message.size );
// std::cout << "Message size " << message.size << std::endl;
// std::cout << "  --> " << int(message.buffer[0]) << std::endl;
// std::cout << "  --> " << int(message.buffer[1]) << std::endl;
// std::cout << "  --> " << int(message.buffer[2]) << std::endl;
// std::cout << "  --> " << int(message.buffer[3]) << std::endl;

      boost::thread thrd( boost::bind(
         &testbedservice::NodeControl::pass_message,
         testbedservice::node_control_,
         id, nodes, message ) );


      char *response_id = testbedservice::allocate_string( ns1__sendResponse_->soap, id );
      ns1__sendResponse_->return_ = response_id;

      return SOAP_OK;
   }
}

#endif
