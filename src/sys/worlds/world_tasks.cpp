/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "shawn_config.h"
#include "sys/worlds/world_tasks.h"
#include "sys/worlds/processor_world_factory.h"
#include "sys/processors/processor_keeper.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/worlds/rect_world_factory.h"
#include "sys/worlds/cuboid_world_factory.h"
#include "sys/worlds/load_world_factory.h"
#include "sys/worlds/flegsens_world_factory.h"
#include "sys/util/defutils.h"
#include "sys/misc/tokenize.h"

#include <iostream>

using namespace shawn::xml;
using namespace std;

namespace shawn
{
    void init_world_tasks( SimulationController& sc ) throw()
    {
        sc.simulation_task_keeper_w().add( new SimulationTaskRectWorldFactory );
        sc.simulation_task_keeper_w().add( new SimulationTaskCuboidWorldFactory );
        sc.simulation_task_keeper_w().add( new SimulationTaskLoadWorldFactory );
        sc.simulation_task_keeper_w().add( new SimulationTaskFlegsensWorldFactory );
        
    }



    SimulationTaskProcessorWorldFactory::
        SimulationTaskProcessorWorldFactory()
    {}
    // ----------------------------------------------------------------------
    SimulationTaskProcessorWorldFactory::
        ~SimulationTaskProcessorWorldFactory()
    {}
    // ----------------------------------------------------------------------
    void
        SimulationTaskProcessorWorldFactory::
        run( SimulationController& sc )
        throw( std::runtime_error )
    {
        if( !sc.has_world() )
            throw std::runtime_error(std::string("World-filling task '") +
            name() +
            std::string("' needs a world to fill.") );

        World& w = sc.world_w();
        ProcessorWorldFactory* pwf = create_factory(sc);
        assert( pwf != NULL );

        try {
            set_node_count( sc, *pwf );
            add_processor_factories( sc, *pwf );
        }
        catch( std::runtime_error& ) {
            delete pwf;
            throw;
        }

        pwf->fill_world(w);

        delete pwf;
    }
    // ----------------------------------------------------------------------
    void
        SimulationTaskProcessorWorldFactory::
        set_node_count( SimulationController&  sc,
        ProcessorWorldFactory& pwf )
        throw( std::runtime_error )
    {
        pwf.set_node_count( sc.environment().required_int_param("count") );
    }
    // ----------------------------------------------------------------------
    void
        SimulationTaskProcessorWorldFactory::
        add_processor_factories( SimulationController&  sc,
        ProcessorWorldFactory& pwf )
        throw( std::runtime_error )
    {
        StrTok tok(sc.environment().optional_string_param("processors",""), ", ");
        for(StrTok::iterator it = tok.begin(), end = tok.end(); it!=end; ++it)
        {
            ProcessorFactoryHandle pfh = sc.processor_keeper_w().find_w( *it );
            assert( pfh != NULL ); // not found throws...
            pwf.add_processor_factory(pfh);
        }
    }










    SimulationTaskRectWorldFactory::
        SimulationTaskRectWorldFactory()
    {}
    // ----------------------------------------------------------------------
    SimulationTaskRectWorldFactory::
        ~SimulationTaskRectWorldFactory()
    {}
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskRectWorldFactory::
        name( void )
        const throw()
    {
        return "rect_world";
    }
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskRectWorldFactory::
        description( void )
        const throw()
    {
        return "fills a rectangle with random points";
    }
    // ----------------------------------------------------------------------
    ProcessorWorldFactory*
        SimulationTaskRectWorldFactory::
        create_factory( SimulationController& sc )
        throw( std::runtime_error )
    {
        RectWorldFactory* rwf = new RectWorldFactory;

        // todo: make this doubles...
        rwf->set_size( double(sc.environment().required_int_param("width")),
            double(sc.environment().required_int_param("height")) );

        return rwf;
    }










    SimulationTaskCuboidWorldFactory::
        SimulationTaskCuboidWorldFactory()
    {}
    // ----------------------------------------------------------------------
    SimulationTaskCuboidWorldFactory::
        ~SimulationTaskCuboidWorldFactory()
    {}
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskCuboidWorldFactory::
        name( void )
        const throw()
    {
        return "cuboid_world";
    }
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskCuboidWorldFactory::
        description( void )
        const throw()
    {
        return "fills a cuboid with random points";
    }
    // ----------------------------------------------------------------------
    ProcessorWorldFactory*
        SimulationTaskCuboidWorldFactory::
        create_factory( SimulationController& sc )
        throw( std::runtime_error )
    {
        CuboidWorldFactory* cwf = new CuboidWorldFactory;

        cwf->set_size(
            sc.environment().required_double_param("width"),
            sc.environment().required_double_param("height"),
            sc.environment().required_double_param("depth") );

        return cwf;
    }


    


    
    
    
    

    SimulationTaskFlegsensWorldFactory::
    SimulationTaskFlegsensWorldFactory()
     {}
     // ----------------------------------------------------------------------
    SimulationTaskFlegsensWorldFactory::
         ~SimulationTaskFlegsensWorldFactory()
     {}
     // ----------------------------------------------------------------------
     std::string
     SimulationTaskFlegsensWorldFactory::
         name( void )
         const throw()
     {
         return "flegsens_world";
     }
     // ----------------------------------------------------------------------
     std::string
     SimulationTaskFlegsensWorldFactory::
         description( void )
         const throw()
     {
         return "flegsens world creator";
     }
     // ----------------------------------------------------------------------
     ProcessorWorldFactory*
     SimulationTaskFlegsensWorldFactory::
         create_factory( SimulationController& sc )
         throw( std::runtime_error )
     {
    	 FlegsensWorldFactory* cwf = new FlegsensWorldFactory;
    	 // Get those arrays from the environment:    	 
    	 int in_gps_row1[5] = {	 sc.environment().optional_int_param( "gps_row1", -1),
						    	 		sc.environment().optional_int_param( "gps_row1_interval", 5),
						    	 		sc.environment().optional_int_param( "gps_row1_offset", 0),
						    	 		sc.environment().optional_int_param( "gps_row1_force_first", 0),
						    	 		sc.environment().optional_int_param( "gps_row1_force_last", 0)};
    	 int in_gps_row2[5] = {	 sc.environment().optional_int_param( "gps_row2", -1),
    							    	 sc.environment().optional_int_param( "gps_row2_interval", 5),
    							    	 sc.environment().optional_int_param( "gps_row2_offset", 0),
    							    	 sc.environment().optional_int_param( "gps_row2_force_first", 0),
    							    	 sc.environment().optional_int_param( "gps_row2_force_last", 0)}; 
    	 int in_gps_row3[5] = {	 sc.environment().optional_int_param( "gps_row3", -1),
    							    	 sc.environment().optional_int_param( "gps_row3_interval", 5),
    							    	 sc.environment().optional_int_param( "gps_row3_offset", 0),
    							    	 sc.environment().optional_int_param( "gps_row3_force_first", 0),
    							    	 sc.environment().optional_int_param( "gps_row3_force_last", 0)};
    	 int in_gps_row4[5] = {	 sc.environment().optional_int_param( "gps_row4", -1),
    							    	 sc.environment().optional_int_param( "gps_row4_interval", 5),
    							    	 sc.environment().optional_int_param( "gps_row4_offset", 0),
    							    	 sc.environment().optional_int_param( "gps_row4_force_first", 0),
    							    	 sc.environment().optional_int_param( "gps_row4_force_last", 0)};
    	 
    	 int in_gateway_row1[5] = {	 sc.environment().optional_int_param( "gateway_row1", -1),
 						    	 		sc.environment().optional_int_param( "gateway_row1_interval", 5),
 						    	 		sc.environment().optional_int_param( "gateway_row1_offset", 0),
 						    	 		sc.environment().optional_int_param( "gateway_row1_force_first", 0),
 						    	 		sc.environment().optional_int_param( "gateway_row1_force_last", 0)};
     	 int in_gateway_row2[5] = {	 sc.environment().optional_int_param( "gateway_row2", -1),
     							    	 sc.environment().optional_int_param( "gateway_row2_interval", 5),
     							    	 sc.environment().optional_int_param( "gateway_row2_offset", 0),
     							    	 sc.environment().optional_int_param( "gateway_row2_force_first", 0),
     							    	 sc.environment().optional_int_param( "gateway_row2_force_last", 0)}; 
     	 int in_gateway_row3[5] = {	 sc.environment().optional_int_param( "gateway_row3", -1),
     							    	 sc.environment().optional_int_param( "gateway_row3_interval", 5),
     							    	 sc.environment().optional_int_param( "gateway_row3_offset", 0),
     							    	 sc.environment().optional_int_param( "gateway_row3_force_first", 0),
     							    	 sc.environment().optional_int_param( "gateway_row3_force_last", 0)};
     	 int in_gateway_row4[5] = {	 sc.environment().optional_int_param( "gateway_row4", -1),
     							    	 sc.environment().optional_int_param( "gateway_row4_interval", 5),
     							    	 sc.environment().optional_int_param( "gateway_row4_offset", 0),
     							    	 sc.environment().optional_int_param( "gateway_row4_force_first", 0),
     							    	 sc.environment().optional_int_param( "gateway_row4_force_last", 0)};
    	 /*
     	cout << "GPS1: " << in_gps_row1[0] << " " << in_gps_row1[1] << " " << in_gps_row1[2] << " " << in_gps_row1[3] << " " << in_gps_row1[4] << " " << endl;
 		cout << "GPS2: " << in_gps_row2[0] << " " << in_gps_row2[1] << " " << in_gps_row2[2] << " " << in_gps_row2[3] << " " << in_gps_row2[4] << " " << endl;
 		cout << "GPS3: " << in_gps_row3[0] << " " << in_gps_row3[1] << " " << in_gps_row3[2] << " " << in_gps_row3[3] << " " << in_gps_row3[4] << " " << endl;
 		cout << "GPS4: " << in_gps_row4[0] << " " << in_gps_row4[1] << " " << in_gps_row4[2] << " " << in_gps_row4[3] << " " << in_gps_row4[4] << " " << endl;
 		
 		cout << "Gateway1: " << in_gateway_row1[0] << " " << in_gateway_row1[1] << " " << in_gateway_row1[2] << " " << in_gateway_row1[3] << " " << in_gateway_row1[4] << " " << endl;
 		cout << "Gateway2: " << in_gateway_row2[0] << " " << in_gateway_row2[1] << " " << in_gateway_row2[2] << " " << in_gateway_row2[3] << " " << in_gateway_row2[4] << " " << endl;
 		cout << "Gateway3: " << in_gateway_row3[0] << " " << in_gateway_row3[1] << " " << in_gateway_row3[2] << " " << in_gateway_row3[3] << " " << in_gateway_row3[4] << " " << endl;
 		cout << "Gateway4: " << in_gateway_row4[0] << " " << in_gateway_row4[1] << " " << in_gateway_row4[2] << " " << in_gateway_row4[3] << " " << in_gateway_row4[4] << " " << endl;
    	  */

     	 
     	 
    	 cwf->set_parameters(	sc.environment().required_int_param("x_count"),
			    			 	sc.environment().required_int_param("y_count"),
			    	            sc.environment().required_double_param("x_dist"),
			    	            sc.environment().required_double_param("y_dist"),
			    	            sc.environment().required_double_param("x_off"),
			    	            in_gps_row1, in_gps_row2, in_gps_row3, in_gps_row4,
			    	            in_gateway_row1, in_gateway_row2, in_gateway_row3, in_gateway_row4);
    	 
         return cwf;
     }

     void
     SimulationTaskFlegsensWorldFactory::
         set_node_count( SimulationController&,
         ProcessorWorldFactory& )
         throw( std::runtime_error )
     {
         // nothing -- FlegSens needs no count (see xml)
     }

     
     
     
     
     
     
     
     
     

    SimulationTaskLoadWorldFactory::
        SimulationTaskLoadWorldFactory()
    {}
    // ----------------------------------------------------------------------
    SimulationTaskLoadWorldFactory::
        ~SimulationTaskLoadWorldFactory()
    {}
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskLoadWorldFactory::
        name( void )
        const throw()
    {
        return "load_world";
    }
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskLoadWorldFactory::
        description( void )
        const throw()
    {
        return "Reads topology from a XML file";
    }
    // ----------------------------------------------------------------------
    ProcessorWorldFactory*
        SimulationTaskLoadWorldFactory::
        create_factory( SimulationController& sc )
        throw( std::runtime_error )
    {
        LoadWorldFactory* xmlf = NULL;

        try 
        {
        	std::string file = sc.environment().optional_string_param("file","");
        	if( file == "" )
        		file = sc.environment().required_string_param("world_in_file");
        	
            xmlf = new LoadWorldFactory;
            xmlf->set_document_uri( file );
            xmlf->set_snapshot_id( sc.environment().optional_string_param("snapshot","0") );
        }
        catch( std::runtime_error& ) 
        {
            delete xmlf;
            throw;
        }

        return xmlf;
    }
    // ----------------------------------------------------------------------
    void
        SimulationTaskLoadWorldFactory::
        set_node_count( SimulationController&,
        ProcessorWorldFactory& )
        throw( std::runtime_error )
    {
        // nothing -- xml factory needs no count
    }



}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/worlds/world_tasks.cpp,v $
* Version $Revision: 287 $
* Date    $Date: 2008-11-16 11:35:44 +0100 (Sun, 16 Nov 2008) $
*-----------------------------------------------------------------------
* $Log: world_tasks.cpp,v $
 *-----------------------------------------------------------------------*/
