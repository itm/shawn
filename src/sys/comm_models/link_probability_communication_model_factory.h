/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_COMM_MODELS_LINK_PROBABILITY_COMM_MODEL_FACTORY_H
#define __SHAWN_SYS_COMM_MODELS_LINK_PROBABILITY_COMM_MODEL_FACTORY_H

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"
#include "sys/comm_models/communication_model_factory.h"

#include <string>
#include <vector>

namespace shawn
{
   class CommunicationModel;
   class LinkProbabilityCommunicationModel;
    class SimulationController;

	//----------------------------------------------------------------------------
    /// Creates a new PermalinkCommunicationModel instance
	/** This instance will be configured by tags attached to the SimulationEnvironment. 
      * A TagGroup named 'link_probability' can  be present in the SimulationEnvironment. If not present,
      * no permanent links will be set. You can add these link manually by calling 
      * PermalinkCommunicationModel::add_edge().
      * 
      * If present, this tag group must contain StringDoubleMapTag instances for each 
      * node that should have a permanent connection to another node.
      * The name of the map must match Node::label(). Each StringDoubleMapTag entry creates 
      * a link with the corresponding communication probability, e.g. In a map named v00, the entry [v02,0.8] will enable a unidirectional 
      * link from node v00 to node v02 where 80% of the packet will go through and 20% are dropped.
      *
      * If you want to specify the links in the xml configuration file, an example configuration
      * might look like this:
      *
      * &lt;scenario&gt;
      *     
      *     &lt;environment&gt;
      *     
      *         &lt;tag type="group" name="permalink"&gt;
      *         
      *             &lt;tag type="map-string-double" name="v00"&gt;
      *     			&lt;entry index="v01" value="1.0" /&gt;
      *     			&lt;entry index="v02" value="0.8" /&gt;
      *             &lt;/tag&gt;
      *             
      *             &lt;tag type="map-string-double" name="v01"&gt;
      *     			&lt;entry index="v00" value="1.0" /&gt;
      *             &lt;/tag&gt;
      *             
      *         &lt;/tag&gt;
      *         
      *     &lt;/environment&gt;
	  *
	  * To load these tags into the SimulationEnvironment, use the prepare_world task with the configuration option $env_config and
	  * specify the file. This may also be an arbitrary xml file as long as an xml fragment is present that resembles the above xml 
	  * file. Example:
	  * <pre>
	  *   prepare_world [other options] env_config=mylinks.xml
	  * </pre>
	  */
	class LinkProbabilityCommunicationModelFactory
		: public CommunicationModelFactory
	{
	public:
		virtual ~LinkProbabilityCommunicationModelFactory();
		virtual CommunicationModel* create( const SimulationController& ) const throw();
		virtual std::string name(void) const throw();
		virtual std::string description(void) const throw();
    protected:
        virtual void init_from_tags(const SimulationController&, LinkProbabilityCommunicationModel& plcm) const throw();
	};

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/comm_models/permalink_communication_model_factory.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: permalink_communication_model_factory.h,v $
 *-----------------------------------------------------------------------*/
