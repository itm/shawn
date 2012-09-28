/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/comm_models/link_probability_communication_model.h"
#include "sys/comm_models/link_probability_communication_model_factory.h"

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

#include "sys/util/defutils.h"
#include "sys/taggings/map_tags.h"
#include "sys/taggings/group_tag.h"

using namespace std;

namespace shawn
{

    // ----------------------------------------------------------------------
    LinkProbabilityCommunicationModelFactory::
        ~LinkProbabilityCommunicationModelFactory()
    {}

    // ----------------------------------------------------------------------
    std::string 
        LinkProbabilityCommunicationModelFactory::
        name(void) 
        const throw()
    {
        return "link_probability";
    }

    // ----------------------------------------------------------------------
    std::string 	
        LinkProbabilityCommunicationModelFactory::
        description(void) 
        const throw()
    {
        return "Probability Link Communication Model";
    }

    // ----------------------------------------------------------------------
    CommunicationModel* 
        LinkProbabilityCommunicationModelFactory::
        create(const SimulationController& sc) 
        const throw()
    {
        LinkProbabilityCommunicationModel* plcm = new LinkProbabilityCommunicationModel();
        init_from_tags(sc, *plcm);
        return plcm;
    }

    // ----------------------------------------------------------------------
    void
        LinkProbabilityCommunicationModelFactory::
        init_from_tags(const SimulationController& sc, LinkProbabilityCommunicationModel& plcm) 
        const throw()
    {
        /* First look for a group tag named 'link_probability'
        Check each tag for the type StringBoolMapTag. Each map will contain the
        neighboring nodes of one node. The name of the map tag corresponds to the 
        node for which neighbor entries are present. Therefore we iterate over all 
        tags instead of a lookup by name.
        */

	//cout << "link probability communication model"<< endl;
        ConstTagHandle pl_tag_group = sc.environment().find_tag("link_probability");
        const GroupTag* gt = pl_tag_group.is_not_null() ? dynamic_cast<const GroupTag*> (pl_tag_group.get()) : NULL;


        if( gt != NULL )
        {

	    //cout << "group link_probability found"<< endl;
            for(Tag::tag_iterator it = gt->begin_tags(), end = gt->end_tags(); it!=end; ++it)
            {
		StringDoubleMapTag* mt = dynamic_cast<StringDoubleMapTag*> (it->second.get());
                if( mt != NULL )
                    for( StringDoubleMapTag::Map::const_iterator mit = mt->value().begin(),
                        mend = mt->value().end(); mit!=mend; ++mit )
                    {
			double probability = mit->second;

			plcm.add_edge( it->first, mit->first, probability );
			//cout << "inserting link " << it->first << " to " << mit->first << " with p=" << probability<< endl;

                    }
            }
        }
    }
}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/comm_models/permalink_communication_model_factory.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: permalink_communication_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/
