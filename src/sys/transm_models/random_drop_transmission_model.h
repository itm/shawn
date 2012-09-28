/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANS_MODELS_RANDOM_DROP_H
#define __SHAWN_SYS_TRANS_MODELS_RANDOM_DROP_H

#include "sys/transm_models/chainable_transmission_model.h"

namespace shawn
{

    ///Randomly drops messages with a given $probability ranging from 0.0 to 1.0
    class RandomDropTransmissionModel : public ChainableTransmissionModel 
    {
    public:
        ///@name Construction, destruction and lifecycle support
        ///@{
        RandomDropTransmissionModel();
        virtual ~RandomDropTransmissionModel();
        virtual void init( void ) throw();
        virtual void reset( void ) throw();
        ///@}

        ///@name Transmission model implementation
        ///@{

        /// 
        virtual void send_message( TransmissionModel::MessageInfo& mi ) throw();

        /// Returns true if this message should be dropped
        virtual bool drop_message( TransmissionModel::MessageInfo& mi ) throw();

        ///@}

        ///@name Transmission model implementation
        ///@{
        
        ///Returns the number of messages that have been submitted to this transmission model
        virtual int msgs_received( void ) const throw (); 
        ///Returns the number of messages that have been dropped by this transmission model
        virtual int msgs_dropped( void ) const throw (); 
  
        ///@}

    private:
        ///Drop probability
        double probability_; 
        ///Number of received messages
        int received_;
        ///Number of dropped messages
        int dropped_;
    };

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/random_drop_transmission_model.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: random_drop_transmission_model.h,v $
 *-----------------------------------------------------------------------*/
