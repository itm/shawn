/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANS_MODELS_TRACES_H
#define __SHAWN_SYS_TRANS_MODELS_TRACES_H

#include "sys/transmission_model.h"
#include "sys/event_scheduler.h"

#include <queue>
#include <string>

namespace shawn
{

    /// The TracesTransmissionModel delivers messages based on a trace file
    /** 
    */
    class TracesTransmissionModel
		: public TransmissionModel,
		  public EventScheduler::EventHandler
    {

    public:

        /// Container for message delivery related stuff
        struct TracesMessageInfo
        {
            std::string       src_;
            std::string       dest_;
            Vec               src_pos_;
            double            time_;
            double            rec_time_;
            MessageHandle     msg_;
        };

    public:
        ///@name Construction, destruction and lifecycle support
        ///@{

       TracesTransmissionModel(std::string filename);
        virtual ~TracesTransmissionModel();

        ///@}
        ///@name Transmission model implementation
        ///@{

        ///
        virtual void init( void ) throw();
        ///
        virtual void reset( void ) throw();

        ///Mobility is depending on mobility support from the edge model
        /** The edge model is used to determine the 1-hop neighbours
        * which will receive the message
        */
        virtual bool supports_mobility( void ) const throw(std::logic_error);

        /// Stores each message in a list for delivery at the next simulation round start.
        virtual void send_message( TransmissionModel::MessageInfo& mi ) throw();

        /// Delivers all messages which are in the list
        virtual void deliver_messages() throw();

        ///@}

        virtual void timeout (EventScheduler &, EventScheduler::EventHandle, double, EventScheduler::EventTagHandle &) throw ();

    private:
        std::string filename_;

        ///Delivers one message reliably to one node
        virtual void deliver_one_message( TracesMessageInfo& mi ) throw();

        ///The messages that have been sent by the nodes and are waiting for delivery
        std::queue<TracesMessageInfo*> aired_messages_;
        std::queue<TracesMessageInfo*> queued_messages_;

        EventScheduler::EventHandle next_timeout_;

    };

}

#endif
