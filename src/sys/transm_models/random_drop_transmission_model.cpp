/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/transm_models/random_drop_transmission_model.h"
#include "sys/edge_model.h"
#include "sys/world.h"
#include "sys/misc/random/basic_random.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

namespace shawn
{

    // ----------------------------------------------------------------------
    RandomDropTransmissionModel::
        RandomDropTransmissionModel() :
            probability_(0.0),
            received_(0),
            dropped_(0)
    {}

    // ----------------------------------------------------------------------
    RandomDropTransmissionModel::
        ~RandomDropTransmissionModel() 
    {}

    // ----------------------------------------------------------------------
    void
        RandomDropTransmissionModel::
        init( void ) 
        throw() 
    {
        ChainableTransmissionModel::init();
        const SimulationEnvironment& se = world().simulation_controller().environment();
        probability_ = se.required_double_param("probability");

        std::cout << "random_drop: Initialised. Drop probability is " <<  probability_ << std::endl;
    }

    // ----------------------------------------------------------------------
    void
        RandomDropTransmissionModel::
        reset( void ) 
        throw() 
    {
        double ratio = received_ > 0 ? 100 * (double(dropped_) / double(received_)) : 0.0;

        std::cout << "random_drop: "<<  
            received_ <<" msgs. to be sent, "<< 
            dropped_ <<" of them dropped_, "<< 
            ratio <<"% dropped_; desired probabilty was: " << 
            (100*probability_) << "%" << std::endl;
        
        ChainableTransmissionModel::reset();
        received_ = 0;
        dropped_ = 0;
        probability_ = 0.0;
    }

    // ----------------------------------------------------------------------
    void
        RandomDropTransmissionModel::
        send_message( TransmissionModel::MessageInfo& mi )
        throw() 
    {
        received_++;
        
        // TODO: What should be done if there are several transmission models in use?
        const Message* m = mi.msg_.get();
		if (m->has_sender_proc())
        	(m->sender_proc_w()).process_sent_indication( ConstMessageHandle(mi.msg_), shawn::Processor::SHAWN_TX_STATE_SUCCESS, 1 );
        
        if( drop_message(mi) )
            dropped_++;
        else
            pass_to_chain(mi);
    }

    // ----------------------------------------------------------------------
    bool
        RandomDropTransmissionModel::
        drop_message( TransmissionModel::MessageInfo&)
        throw() 
    {
        return uniform_random_0e_1i() <= probability_;  
    }

    // ----------------------------------------------------------------------
    int
        RandomDropTransmissionModel::
        msgs_received( void ) 
        const throw ()
    {
        return received_;
    }
    
    // ----------------------------------------------------------------------
    int
        RandomDropTransmissionModel::
        msgs_dropped( void ) 
        const throw ()
    {
        return dropped_;
    }


}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/random_drop_transmission_model.cpp,v $
* Version $Revision: 212 $
* Date    $Date: 2008-06-13 11:26:06 +0200 (Fri, 13 Jun 2008) $
*-----------------------------------------------------------------------
* $Log: random_drop_transmission_model.cpp,v $
 *-----------------------------------------------------------------------*/
