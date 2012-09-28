/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/ws_handler/experiment_control.h"
#include "apps/testbedservice/processor/testbedservice_processor.h"
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
   static testbedservice::ExperimentControl *experiment_control_ = 0;
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   ExperimentControl::
   ExperimentControl()
      : controller_ (0),
         sc_( 0 )
   {}
   // ----------------------------------------------------------------------
   ExperimentControl::
   ~ExperimentControl()
   {}
   // ----------------------------------------------------------------------
   void
   ExperimentControl::
   init( shawn::SimulationController& sc, TestbedServiceClient& controller )
      throw()
   {
      sc_ = &sc;
      controller_ = &controller;
      experiment_control_ = this;
   }
   // ----------------------------------------------------------------------
   void
   ExperimentControl::
   flash_programs( std::string id, NodeIdVector nodes,
                   IndicesVector indices, FlashProgramVector programs )
      throw()
   {
      NodeIdVector response_nodes;
      StatusValueVector response_values;
      StatusMsgVector response_msgs;

      IndicesVector::iterator idx_it = indices.begin();
      for ( NodeIdVector::iterator it = nodes.begin();
               it != nodes.end();
               ++it, ++idx_it )
      {
         shawn::Node *node =
            simulation_controller_w().world_w().find_node_by_label_w( *it );

         if ( node )
         {
            TestbedServiceProcessor *proc =
               node->get_processor_of_type_w<TestbedServiceProcessor>();
            if ( proc )
            {
               proc->flash_program( programs.at( *idx_it ) );
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

}

namespace wsnapi_server
{

   static bool experiment_control_available( void )
   {
      if ( testbedservice::experiment_control_ )
      {
         return true;
      }
      else
         std::cerr << "Error processing ws call - experiment_control_ not set." << std::endl;

      return false;
   }

   // -----------------------------------------------------------------------
   // ------------- Web Service Calls Experiment Control --------------------
   // -----------------------------------------------------------------------
   static char version_[] = "2.3";
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__getVersion( soap*,
            ns1__getVersion *ns1__getVersion_,
            ns1__getVersionResponse *ns1__getVersionResponse_ )
   {
      ns1__getVersionResponse_->return_ = version_;
      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__flashPrograms( soap*,
            ns1__flashPrograms *ns1__flashPrograms_,
            ns1__flashProgramsResponse *ns1__flashProgramsResponse_ )
   {
      if ( !experiment_control_available() )
         return SOAP_OK;

      int node_cnt = ns1__flashPrograms_->__sizenodeIds;
      std::vector<std::string> nodes;
      for ( int i = 0; i < node_cnt; i++ )
      {
         nodes.push_back( std::string(ns1__flashPrograms_->nodeIds[i]) );
      }

      int idx_cnt = ns1__flashPrograms_->__sizeprogramIndices;
      std::vector<int> indices;
      for ( int i = 0; i < idx_cnt; i++ )
      {
         indices.push_back( ns1__flashPrograms_->programIndices[i] );
      }

      int program_cnt = ns1__flashPrograms_->__sizeprograms;
      std::vector<testbedservice::ExperimentControl::FlashProgram> programs;
      for ( int i = 0; i < program_cnt; i++ )
      {
         testbedservice::ExperimentControl::FlashProgram program;

         xsd__base64Binary *bin = &ns1__flashPrograms_->programs[i]->program;
         program.data = new uint8_t[bin->__size];
         memcpy( program.data, bin->__ptr, bin->__size );
         program.data_len = bin->__size;

         ns1__programMetaData *meta = ns1__flashPrograms_->programs[i]->metaData;
         program.version = std::string( meta->version );
         program.name = std::string( meta->name );
         program.platform = std::string( meta->platform );
         program.other = std::string( meta->other );

         programs.push_back(program);
      }

      std::string id = testbedservice::create_response_id();
      char *response_id = testbedservice::allocate_string( ns1__flashProgramsResponse_->soap, id );
      ns1__flashProgramsResponse_->return_ = response_id;

      boost::thread thrd( boost::bind(
         &testbedservice::ExperimentControl::flash_programs,
         testbedservice::experiment_control_,
         id, nodes, indices, programs ) );

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__getSupportedChannelHandlers( soap*,
            ns1__getSupportedChannelHandlers *ns1__getSupportedChannelHandlers_,
            ns1__getSupportedChannelHandlersResponse *ns1__getSupportedChannelHandlersResponse_ )
   {
      // TODO!!
      std::cerr << "WsnAPI: getSupportedChannelHandlers not yet implemented." << std::endl;
      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__setChannelPipeline( soap*,
            ns1__setChannelPipeline *ns1__setChannelPipeline_,
            ns1__setChannelPipelineResponse *ns1__setChannelPipelineResponse_ )
   {
      // TODO!!
      std::cerr << "WsnAPI: setChannelPipeline not yet implemented." << std::endl;
      return SOAP_OK;
   }
}

#endif
