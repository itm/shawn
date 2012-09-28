/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/transm_models/traces_transmission_model.h"
#include "sys/edge_model.h"
#include "sys/world.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>


using namespace shawn;

namespace shawn
{

    // ----------------------------------------------------------------------
    TracesTransmissionModel::
       TracesTransmissionModel(std::string filename)
        : filename_(filename),
        next_timeout_(NULL)
    {}

    // ----------------------------------------------------------------------
    TracesTransmissionModel::
        ~TracesTransmissionModel()
    {}

    // ----------------------------------------------------------------------
    bool
        TracesTransmissionModel::
        supports_mobility( void )
        const throw(std::logic_error)
    {
        return world().edge_model().supports_mobility();
    }

    // ----------------------------------------------------------------------
    void
        TracesTransmissionModel::
        init( void )
        throw()
    {
        TransmissionModel::init();

        std::ifstream tracefile;
        std::string tmp;
        TracesTransmissionModel::TracesMessageInfo * mi = NULL;

        tracefile.open(filename_.c_str());
        if(!tracefile)
           return;

        while (!tracefile.eof())
        {
           getline(tracefile, tmp);

           // New message
           if(tmp.find("[message]")==0)
           {
              // Last one wasn't finished, so delete the unfinished object first
              if(mi != NULL)
                 delete(mi);
              // New message structure:
              mi = new TracesTransmissionModel::TracesMessageInfo;
              const shawn::MessageHandle &msg = new Message();
              mi->msg_ = msg;
              continue;
           }

           //read source_id
           if(tmp.find("source_id")!=std::string::npos)
           {
              tmp.erase(0,10);
              mi->src_ = tmp;
              continue;
           }

           if(tmp.find("dest_id")!=std::string::npos)
           {
              tmp.erase(0,8);
              mi->dest_ = tmp;
              continue;
           }

           if(tmp.find("send_t")!=std::string::npos)
           {
              tmp.erase(0,7);
              mi->time_ = atof(tmp.c_str());
              continue;
           }

           if(tmp.find("timestamp")!=std::string::npos)
           {
              tmp.erase(0,10);
              mi->rec_time_ = atof(tmp.c_str());
              continue;
           }

           if(tmp.find("[/message]")==0)
           {
              queued_messages_.push(mi);
              std::cout << "Message queued" << std::endl;
              mi = NULL;
              continue;
           }

        }
        tracefile.close();

        next_timeout_ = world_w().scheduler_w().new_event(*this, queued_messages_.front()->rec_time_, NULL);
    }

    // ----------------------------------------------------------------------
    void
        TracesTransmissionModel::
        reset( void )
        throw()
    {
        TransmissionModel::reset();

        while( !aired_messages_.empty() )
        {
            TracesMessageInfo* mi = aired_messages_.front();
            aired_messages_.pop();
            delete mi;
        }
    }

    // ----------------------------------------------------------------------
    void
        TracesTransmissionModel::
        send_message( TransmissionModel::MessageInfo& mi )
        throw()
    {
    	  //queued_messages_.push( &mi );
        //if( immediate_delivery_ && next_timeout_ == NULL)
        //	next_timeout_ = world_w().scheduler_w().new_event(*this, world_w().scheduler_w().current_time() + EPSILON_TIME, NULL);
    }

    // ----------------------------------------------------------------------
    void
        TracesTransmissionModel::
        deliver_messages()
        throw()
    {
       //std::cout << "Delivering..." << std::endl;
       // // copy the queued messages to the aired_messages and clear the queue.
       // while( !queued_messages_.empty() )
       // {
       //     aired_messages_.push( queued_messages_.front() );
       //     queued_messages_.pop();
       // }

       // // Deliver messages. It is possible, that a processor receives a
       // // message and resend it immediately. Therefore there are the two
       // // queues queued- and aired_messages. The following loop processes
       // // the aired_messages_, whereby messages which are sent during the
       // // loop, are packed into queued_messages_.
       // while( ! aired_messages_.empty() )
       // {
       //     TracesMessageInfo* mi = aired_messages_.front();
       //     aired_messages_.pop();
       //     deliver_one_message( *mi );
       // }
    }

    // ----------------------------------------------------------------------
	void
		TracesTransmissionModel::
		timeout (EventScheduler &, EventScheduler::EventHandle, double, EventScheduler::EventTagHandle &)
		throw ()
   	{
         TracesMessageInfo * mi = queued_messages_.front();
         queued_messages_.pop();
         mi->msg_->set_source(*world_w().find_node_by_label_w(mi->src_));
         mi->msg_->set_timestamp((int)mi->rec_time_, mi->rec_time_);
         shawn::Node *dest = world_w().find_node_by_label_w(mi->dest_);
         dest->receive(ConstMessageHandle(mi->msg_));
         delete(mi);
         if(!queued_messages_.empty())
            next_timeout_ = world_w().scheduler_w().new_event(*this, queued_messages_.front()->rec_time_, NULL);
   	}

    // ----------------------------------------------------------------------
    void
        TracesTransmissionModel::
        deliver_one_message( TracesTransmissionModel::TracesMessageInfo& mi )
        throw()
    {

    //    if( mi.msg_->is_unicast() )
    //    {
    //        ABORT_INCONSISTENT_CONFIGURATION("Unicast is not supported by the Traces transmission model. Implement it -> NOW.");
    //    }
    //    //std::cout << "deliver one, now " << world_w().scheduler_w().current_time() << std::endl;
    //
    //    for( EdgeModel::adjacency_iterator it =
    //       world_w().begin_adjacent_nodes_w( *mi.src_ ),
    //         endit = world_w().end_adjacent_nodes_w( *mi.src_ );
    //         it != endit; ++it )
    //       it->receive( ConstMessageHandle(mi.msg_) );

    //    const Message* m = mi.msg_.get();
		  //if (m->has_sender_proc())
    //    	  (m->sender_proc_w()).process_sent_indication(
    //       ConstMessageHandle(mi.msg_),
    //       shawn::Processor::SHAWN_TX_STATE_SUCCESS, 1 );
    //
		  //delete &mi;
    }


}
