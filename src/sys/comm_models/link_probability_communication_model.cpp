/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/comm_models/link_probability_communication_model.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/util/defutils.h"
#include "sys/misc/random/basic_random.h"

#include <vector>
#include <limits>
#include <iostream>

using namespace std;

namespace shawn
{

    LinkProbabilityCommunicationModel::
        LinkProbabilityCommunicationModel()
    {}
    // ----------------------------------------------------------------------
    LinkProbabilityCommunicationModel::
        ~LinkProbabilityCommunicationModel()
    {}
    // ----------------------------------------------------------------------
    void
        LinkProbabilityCommunicationModel::
        init( void )
        throw()
    {}
    // ----------------------------------------------------------------------
    void
        LinkProbabilityCommunicationModel::
        add_edge( std::string u, std::string v, double probability )
        throw()
    {
		probabilities_[u].insert(make_pair(v,probability));
    }
    // ----------------------------------------------------------------------
    bool 
        LinkProbabilityCommunicationModel::
        in_domain(const Node& u, const Node& v) 
        const throw ()
    {
        return can_communicate_uni(u,v);
    }
    // ----------------------------------------------------------------------
    bool
        LinkProbabilityCommunicationModel::
        can_communicate_bidi( const Node& u, const Node& v )
        const throw()
    {
        return can_communicate_uni(u,v) && can_communicate_uni(v,u);
    }
    // ----------------------------------------------------------------------
    bool
        LinkProbabilityCommunicationModel::
        can_communicate_uni( const Node& u, const Node& v )
        const throw()
    {
        LinkProbabilityMap::const_iterator it = probabilities_.find( u.label() );
        if(u==v) return true;

        if( it == probabilities_.end() )
            return false;
        else 
		{
			std::map<std::string, double>::const_iterator it2 = it->second.find(v.label());
			if( it2 == it->second.end() ) 
				return false;
			else 
				return (it2->second >= uniform_random_0e_1i());
		}
    }
    // ----------------------------------------------------------------------
    bool
        LinkProbabilityCommunicationModel::
        exists_communication_upper_bound( void )
        const throw()
    {
        return false;
    }
    // ----------------------------------------------------------------------
    double
        LinkProbabilityCommunicationModel::
        communication_upper_bound( void )
        const throw()
    {
        assert(false);
        return std::numeric_limits<double>::max();
    }
    // ----------------------------------------------------------------------
    bool
        LinkProbabilityCommunicationModel::
        is_status_available_on_construction( void )
        const throw()
    {
        return true;
    }
}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/comm_models/permalink_communication_model.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: permalink_communication_model.cpp,v $
 *-----------------------------------------------------------------------*/
