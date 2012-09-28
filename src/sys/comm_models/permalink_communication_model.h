/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_COMM_MODELS_PERMALINK_COMMUNICATION_MODEL_H
#define __SHAWN_SYS_COMM_MODELS_PERMALINK_COMMUNICATION_MODEL_H

#include "sys/communication_model.h"

#include <string>
#include <utility>
#include <set>
#include <map>

namespace shawn
{
    class GroupTag;
    class Node;

	//----------------------------------------------------------------------------
    /// CommunicationModel that provides static links between nodes to mimic fixed communication channels.
	/** Unidirectional edges can be added manually by using add_edge(). 
      * Alternatively, see PermalinkCommunicationModelFactory on how to configure the static links
      * by using Tag entries in the SimulationEnvironment.
      */
    class PermalinkCommunicationModel
        : public CommunicationModel
    {
    public:

        ///@name construction / destruction / init
        ///@{
        ///
        PermalinkCommunicationModel();
        ///
        virtual ~PermalinkCommunicationModel();
        ///
        virtual void init( void ) throw();
        ///@}


        ///@name edges
        ///@{
        ///
        virtual void add_edge(std::string, std::string ) throw();
        ///@}

        ///@name CommunicationModel interface
        ///@{
        ///
        virtual bool can_communicate_bidi( const Node&, const Node& ) const throw();

        ///
        virtual bool can_communicate_uni( const Node&, const Node& ) const throw();

        /// returns whether communication_upper_bound() returns a useful value
        virtual bool exists_communication_upper_bound( void ) const throw();

        ///
        virtual double communication_upper_bound( void ) const throw();

        ///
        virtual bool is_status_available_on_construction( void ) const throw();

        ///
        virtual bool in_domain (const Node &, const Node &) const throw ();

        ///@}

    private:
        ///
        typedef std::map<std::string, std::set<std::string> > Linkset;
        ///
        Linkset static_links_;
    };

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/comm_models/permalink_communication_model.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: permalink_communication_model.h,v $
 *-----------------------------------------------------------------------*/
