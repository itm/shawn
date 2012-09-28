/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/comm_models/permalink_communication_model.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/util/defutils.h"

#include <vector>
#include <limits>
#include <iostream>

using namespace std;

namespace shawn
{

    PermalinkCommunicationModel::
        PermalinkCommunicationModel()
    {}
    // ----------------------------------------------------------------------
    PermalinkCommunicationModel::
        ~PermalinkCommunicationModel()
    {}
    // ----------------------------------------------------------------------
    void
        PermalinkCommunicationModel::
        init( void )
        throw()
    {}
    // ----------------------------------------------------------------------
    void
        PermalinkCommunicationModel::
        add_edge( std::string u, std::string v )
        throw()
    {
        static_links_[u].insert(v);
    }
    // ----------------------------------------------------------------------
    bool 
        PermalinkCommunicationModel::
        in_domain(const Node& u, const Node& v) 
        const throw ()
    {
        return can_communicate_uni(u,v);
    }
    // ----------------------------------------------------------------------
    bool
        PermalinkCommunicationModel::
        can_communicate_bidi( const Node& u, const Node& v )
        const throw()
    {
        return can_communicate_uni(u,v) && can_communicate_uni(v,u);
    }
    // ----------------------------------------------------------------------
    bool
        PermalinkCommunicationModel::
        can_communicate_uni( const Node& u, const Node& v )
        const throw()
    {
        bool cc = true;
        Linkset::const_iterator it = static_links_.find( u.label() );
        
        if( it == static_links_.end() )
            cc = false;
        else if( it->second.find(v.label()) == it->second.end() )
            cc = false;

        return cc;
    }
    // ----------------------------------------------------------------------
    bool
        PermalinkCommunicationModel::
        exists_communication_upper_bound( void )
        const throw()
    {
        return false;
    }
    // ----------------------------------------------------------------------
    double
        PermalinkCommunicationModel::
        communication_upper_bound( void )
        const throw()
    {
        assert(false);
        return std::numeric_limits<double>::max();
    }
    // ----------------------------------------------------------------------
    bool
        PermalinkCommunicationModel::
        is_status_available_on_construction( void )
        const throw()
    {
        return true;
    }
}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/comm_models/permalink_communication_model.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: permalink_communication_model.cpp,v $
 *-----------------------------------------------------------------------*/
