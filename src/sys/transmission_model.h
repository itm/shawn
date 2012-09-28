/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANSMISSION_MODEL_H
#define __SHAWN_SYS_TRANSMISSION_MODEL_H

#include "sys/vec.h"

#include "sys/message.h"
#include <stdexcept>

namespace shawn
{
    class World;
    class Node;

   /// Model deciding how messages are delivered, use it to model transmission channel characteristics. 
   /** Abstract superclass for transmission models. A message is accepted for delivery by send_message(). 
    * An implementation is free to decide how to handle the message. It may be e.g. delayed, altered, dropped 
    * or duplicated. Periodically deliver_messages() is invoked by the world before a new simulation round
    * starts. Message delivery is not bound to deliver_messages(), any suitable method can be used. One possibility
    * is to use the EventScheduler to delay packets. 
    *
    * @todo Currently there is no list of recipients for unicast.
    */
    class TransmissionModel
    {

    public:

        /// Container for message delivery related stuff
        struct MessageInfo
        {
            Node*             src_;
            Vec               src_pos_;
            double            time_;
            MessageHandle     msg_;
            double            range_;
        };

    public:

        ///@name Construction, destruction and lifecycle support
        ///@{

        ///
        TransmissionModel();
        ///
        virtual ~TransmissionModel();
        ///
        const World& world( void ) const throw();
        ///
        virtual void init( void ) throw();
        ///
        virtual void reset( void ) throw();

        ///@}
        ///@name Configuration and data access
        ///@{

        ///
        virtual void set_world( World& ) throw();
        ///
        World& world_w( void ) throw();

        /// Return true if your model supports node mobility.
        virtual bool supports_mobility( void ) const throw(std::logic_error) = 0;

        ///@}
        ///@name Transmission model implementation
        ///@{

        /// Receives a message for delivery 
        virtual void send_message( MessageInfo& mi ) throw() = 0;

        /// Periodically invoked by the world before a new simulation round starts. 
        /** Delivery of messages is not bound to this method. It can e.g. also be
          * scheduled by the EventScheduler.
          */
        virtual void deliver_messages() throw() = 0;

        ///@}

        ///@name Transmission model helper
        ///@{
        virtual bool transmission_in_range(Node*, Node*, MessageInfo*);
        ///@}

    private:
        World* world_;  ///< The world we live in

    };

}

#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transmission_model.h,v $
* Version $Revision: 370 $
* Date    $Date: 2009-11-12 14:12:56 +0100 (Thu, 12 Nov 2009) $
*-----------------------------------------------------------------------
* $Log: transmission_model.h,v $
 *-----------------------------------------------------------------------*/
