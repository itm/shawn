/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/simulation/simulation_controller.h"
#include "sys/transmission_model.h"
#include "sys/transm_models/chainable_transmission_model.h"
#include "sys/edge_model.h"
#include "sys/world.h"

#include <string>

using namespace std;

namespace shawn
{

    // ----------------------------------------------------------------------
    ChainableTransmissionModel::
        ChainableTransmissionModel()
        : next_transm_model_(NULL)
    {
    }

    // ----------------------------------------------------------------------
    ChainableTransmissionModel::
        ~ChainableTransmissionModel() 
    {
        cleanup();
    }


    // ----------------------------------------------------------------------
    void
        ChainableTransmissionModel::
        init( void ) 
        throw()
    {
        TransmissionModel::init();
    }

    // ----------------------------------------------------------------------
    bool 
        ChainableTransmissionModel::
        supports_mobility( void ) 
        const throw(std::logic_error)
    {
        if( !has_next_transm_model() )
            throw std::logic_error("Attach a transmission model to me that answers supports_mobility().");
        
        return next_transm_model_->supports_mobility();
    }

    // ----------------------------------------------------------------------
    void
        ChainableTransmissionModel::
        append_to_chain( TransmissionModel& tm ) 
        throw(std::logic_error)
    {
        if( next_transm_model_ == NULL ) 
        {
            next_transm_model_ = &tm;
        }
        else
        {

            ChainableTransmissionModel* ctm = dynamic_cast<ChainableTransmissionModel*>(next_transm_model_);
            if( ctm != NULL ) 
            {
                ctm->append_to_chain(tm);
            } 
            else
            {
                throw std::logic_error("Cannot append to chain. Last transmission model is not chainable.");
            }

        }
    }

    // ----------------------------------------------------------------------
    void
        ChainableTransmissionModel::
        pass_to_chain(MessageInfo& mi)
    {        
        if( has_next_transm_model() )
            next_transm_model_->send_message(mi);
    }

    // ----------------------------------------------------------------------
    void
        ChainableTransmissionModel::
        send_message( TransmissionModel::MessageInfo& mi ) 
        throw()
    {
        pass_to_chain(mi);
    }

    // ----------------------------------------------------------------------
    void
        ChainableTransmissionModel::
        deliver_messages() 
        throw()
    {
        if( has_next_transm_model() )
            next_transm_model_->deliver_messages();
    }

    // ----------------------------------------------------------------------
    void
        ChainableTransmissionModel::
        reset( void ) 
        throw()
    {        
        TransmissionModel::reset();
        
        if( has_next_transm_model() )
            next_transm_model_->reset();
    }

    // ----------------------------------------------------------------------
    void
        ChainableTransmissionModel::
        cleanup( void ) 
        throw()
    {
        if( has_next_transm_model() ) 
        {
            delete next_transm_model_;
            next_transm_model_ = NULL;
        }
    }

    // ----------------------------------------------------------------------
    bool
        ChainableTransmissionModel::
        has_next_transm_model()
        const throw()
    {
        return next_transm_model_ != NULL;
    
    }
   // ----------------------------------------------------------------------
   TransmissionModel&
   ChainableTransmissionModel::
   next_transm_model_w( void )
      throw()
   {
      assert( next_transm_model_ != NULL );
      return *next_transm_model_;
   }
   // ----------------------------------------------------------------------
   const TransmissionModel& 
   ChainableTransmissionModel::
   next_transm_model( void )
      const throw()
   {
      assert( next_transm_model_ != NULL );
      return *next_transm_model_;
   }



}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/chainable_transmission_model.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: chainable_transmission_model.cpp,v $
 *-----------------------------------------------------------------------*/
