/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANS_MODELS_CHAINABLE_H
#define __SHAWN_SYS_TRANS_MODELS_CHAINABLE_H

#include "sys/transmission_model.h"

#include <queue>

namespace shawn
{

    ///Transmission model that will accept a transmission model as a successor.
    /** Messages will flow along the chain of transmission models where messages
      * can be altered, dropped, duplicated or anything you come up with. 
      * The chain can be built from the config file using the SimulationTaskChainTransmModel task.
      * An example configuration could be:
      * <pre>
      * prepare_world edge_model=simple transm_model=stats_chain comm_model=disk_graph range=3.1
      * chain_transm_model name=reliable
      * </pre>
      * The chain is: stats_chain -> reliable
      */
    class ChainableTransmissionModel : public TransmissionModel 
    {
    public:
        ///@name Construction, destruction and lifecycle support
        ///@{

        ChainableTransmissionModel();
        virtual ~ChainableTransmissionModel();

        ///Passes the init call to the chain.
        virtual void init( void ) throw();
        ///Passes the reset call to the chain
        virtual void reset( void ) throw();

        ///@}

        ///@name Transmission model implementation
        ///@{

        ///Mobility is depending on the next transmission model in the chain.
        /** The next transmission model in the chain is asked for mobility support.
          * Override this method to alter this behaviour. This implies that there IS a next
          * transmission model in the chain if you use this implementation. If there is none, 
          * a std::logic_error is thrown
          */
        virtual bool supports_mobility( void ) const throw(std::logic_error);

        ///Default chaining implementiation. Just passes the message info to the next TransmissionModel if existing.
        /** Override this method to implement your funtionality in the chain.
          */
        virtual void send_message( TransmissionModel::MessageInfo& mi ) throw();
        
        ///Passes the deliver messages call to the chain. 
        /** Override for your own funtionality but do not forget to call ChainableTransmissionModel::deliver_messages()
          * to pass the call on to the chain.
          */
        virtual void deliver_messages() throw();

        ///@}

        ///@name Chaining stuff
        ///@{

        ///Appends a transmission model to the chain
        /** If there is no next TransmissionModel it is attached to this instance. If there is a next 
          * TransmissionModel, it is checked for chainability (instance of ChainableTransmissionModel). 
          * If it not chainable, a std::logic_error is trown. If it is chainable, the passed TransmissionModel 
          * is passed to the next ChainableTransmissionModel::append_to_chain() method.
          */
        void append_to_chain( TransmissionModel&  ) throw(std::logic_error);

        ///@}

        ///Returns true if there is another transmission model attached to the chain
        bool has_next_transm_model() const throw();
       ///
        TransmissionModel& next_transm_model_w( void ) throw();
       ///
        const TransmissionModel& next_transm_model( void ) const throw();

    protected:
        ///@name Chaining stuff
        ///@{

        ///Passes the message further to the chain for processing (only if there is a next transmission model)
        void pass_to_chain(MessageInfo& mi);

        
        ///@}

    private:
        ///Internal cleanup
        void cleanup( void ) throw();

    private:
        ///The transmission model that is attached to us. May be NULL
        TransmissionModel* next_transm_model_;

    };

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/chainable_transmission_model.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: chainable_transmission_model.h,v $
 *-----------------------------------------------------------------------*/
