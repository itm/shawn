/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_CLIENT_H_
#define __SHAWN_APPS_TESTBEDSERVICE_CLIENT_H_

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/util/types.h"
#include "sys/simulation/simulation_controller.h"
#include <boost/thread.hpp>
#include <boost/threadpool.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <string>
#include <list>
#include <queue>
#include <vector>

class ControllerPortBindingProxy;
class ns1__receiveStatus;
class ns1__receive;

namespace testbedservice
{

   /** Connection to Controller.
    *
    *  This client allows the sending of messages and responses to the
    *  Controller. The Controller is webservice based and defined by the
    *  appropriate Wisebed WSDL - it basically provides two methods:
    *  <i>receiveStatus</i> and </i>receive</i>.
    *
    *  The former is for sending back responses to previously received
    *  commands (such as setVirtuaslLink, resetNodes, ...), whereas the latter
    *  can be used to send text or binary messages to the Controller. This can
    *  be, for example, debug messages of nodes in case of text, or virtual
    *  link messages in case of binary.
    */
   class TestbedServiceClient
   {

   public:
      TestbedServiceClient();
      ~TestbedServiceClient();
      // --------------------------------------------------------------------
      void init( const shawn::SimulationController& );
      // --------------------------------------------------------------------
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
      // --------------------------------------------------------------------
      void set_controller_uri( std::string ) throw();
      std::string controller_uri( std::string ) throw();
      // --------------------------------------------------------------------
      void send_receive_status( std::string id,
                                NodeIdVector& nodes,
                                StatusValueVector& values,
                                StatusMsgVector& msgs  ) throw();
      void send_text_message( std::string& source, std::string& msg ) throw();
      void send_message( std::string& source, int len, uint8_t* buf ) throw();
      bool send_test_data( void );
      // --------------------------------------------------------------------
      ControllerPortBindingProxy& controller( void ) throw()
      { assert(controller_); return *controller_; }

   private:
      void call_receive_status( ns1__receiveStatus *rcv_status );
      void call_receive_message( ns1__receive *receive );
      // --------------------------------------------------------------------
      std::string controller_uri_;
      int max_pool_size_;

      ControllerPortBindingProxy *controller_;
      boost::mutex pool_mutex_, send_mutex_;
      boost::threadpool::pool pool_;
   };

}

#endif
#endif
