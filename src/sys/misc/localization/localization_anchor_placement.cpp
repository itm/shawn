/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <cmath>
#include <limits>
#include "sys/misc/localization/localization_anchor_placement.h"
#include "sys/misc/localization/localization_processor_type.h"
#include "sys/world.h"
#include "sys/vec.h"
#include "sys/taggings/basic_tags.h"


namespace shawn
{

   SimulationTaskLocalizationAnchorPlacement::
   SimulationTaskLocalizationAnchorPlacement()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskLocalizationAnchorPlacement::
   ~SimulationTaskLocalizationAnchorPlacement()
   {}
   // ----------------------------------------------------------------------
   void
   SimulationTaskLocalizationAnchorPlacement::
   run( SimulationController& sc )
      throw( std::runtime_error )
   {
      require_world(sc);

      anchor_cnt_ = sc.world().simulation_controller().environment().
         required_int_param( "placed_anchor_cnt" );
      pl_type_ = sc.world().simulation_controller().environment().
         required_string_param( "anchor_placement" );
      int anchor_cnt = anchor_cnt_;
      if ( anchor_cnt < 0 ) return;

      if ( pl_type_ == "inner_grid" )
         anchor_cnt = inner_grid( sc, anchor_cnt );
      else if ( pl_type_ == "outer_grid" )
         anchor_cnt = outer_grid( sc, anchor_cnt );
      else if ( pl_type_ == "random" )
		  random_set(sc,anchor_cnt);
      else if( pl_type_=="tag")
		  anchor_cnt=tag(sc);
	  else
         throw std::runtime_error( "Invalid value for 'anchor_placement'" );

     // random_set( sc, anchor_cnt );
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskLocalizationAnchorPlacement::
   name( void )
      const throw()
   {
      return std::string( "localization_anchor_placement" );
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskLocalizationAnchorPlacement::
   description( void )
      const throw()
   {
      return std::string( "places anchors on geometric figures" );
   }
   // ----------------------------------------------------------------------
   int
   SimulationTaskLocalizationAnchorPlacement::
   anchor_cnt( void )
      const throw()
   {
      return anchor_cnt_;
   }
   // ----------------------------------------------------------------------
   const std::string&
   SimulationTaskLocalizationAnchorPlacement::
   pl_type( void )
      const throw()
   {
      return pl_type_;
   }
   // ----------------------------------------------------------------------
   int
   SimulationTaskLocalizationAnchorPlacement::
   outer_grid( SimulationController& sc, int anchor_cnt )
      throw()
   {
      // get (s*s)-grid; s: (s*s) <= #anchors
      int grid_size = (int)floor( sqrt( (double)anchor_cnt ) );
      double cell_width =
         ( sc.world().upper_right().x() - sc.world().lower_left().x() ) /
         ( grid_size - 1 );
      double cell_height =
         ( sc.world().upper_right().y() - sc.world().lower_left().y() ) /
         ( grid_size - 1 );

      for ( int y = 0; y < grid_size; ++y )
         for ( int x = 0; x < grid_size; ++x )
         {
            set_nearest_anchor(
               sc,
               Vec(
                  x*cell_width + sc.world().lower_left().x(),
                  y*cell_height + sc.world().lower_left().y()) );
         }

      anchor_cnt -= grid_size * grid_size;
      return anchor_cnt;
   }
   // ----------------------------------------------------------------------
   int
   SimulationTaskLocalizationAnchorPlacement::
   inner_grid( SimulationController& sc, int anchor_cnt )
      throw()
   {
      // get (s*s)-grid; s: (s*s) <= #anchors
      int grid_size = (int)floor( sqrt( (double)anchor_cnt ) );
      double cell_width =
         ( sc.world().upper_right().x() - sc.world().lower_left().x() ) /
         ( grid_size + 1 );
      double cell_height =
         ( sc.world().upper_right().y() - sc.world().lower_left().y() ) /
         ( grid_size + 1 );

      for ( int y = 1; y <= grid_size; ++y )
         for ( int x = 1; x <= grid_size; ++x )
         {
            set_nearest_anchor(
               sc,
               Vec(
                  x*cell_width + sc.world().lower_left().x(),
                  y*cell_height + sc.world().lower_left().y()) );
         }

      anchor_cnt -= grid_size * grid_size;
      return anchor_cnt;
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskLocalizationAnchorPlacement::
   random_set( SimulationController& sc, int anchor_cnt )
      throw()
   {
      assert( anchor_cnt != 0 );
      if ( anchor_cnt == 0 )
         return;

      int cnt = 0;
      LocalizationProcessorType *lpt;

      for ( World::node_iterator
               it = sc.world_w().begin_nodes_w();
               it != sc.world_w().end_nodes_w();
               ++it )
         if ( ( lpt = it->get_processor_of_type_w<LocalizationProcessorType>() ) != NULL &&
                  !lpt->is_anchor() )
         {
            lpt->set_proc_type( LocalizationProcessorType::anchor );
            ++cnt;

            if ( cnt == anchor_cnt ) break;
         }
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskLocalizationAnchorPlacement::
   set_nearest_anchor( SimulationController& sc, const Vec& pos )
      throw()
   {
      LocalizationProcessorType *lpt, *best = NULL;
      double min_dist = std::numeric_limits<double>::max();

      for ( World::node_iterator
               it = sc.world_w().begin_nodes_w();
               it != sc.world_w().end_nodes_w();
               ++it )
         if ( ( lpt = it->get_processor_of_type_w<LocalizationProcessorType>() ) != NULL &&
                  !lpt->is_anchor() )
         {
            double distance = euclidean_distance( pos, it->real_position() );
            if ( distance < min_dist )
            {
               min_dist = distance;
               best = lpt;
            }
         }

      if ( best ) best->set_proc_type( LocalizationProcessorType::anchor );
   }
   // ----------------------------------------------------------------------
   int
	SimulationTaskLocalizationAnchorPlacement::
	tag(SimulationController& sc)
	   throw()
	{
		int cnt = 0;
      LocalizationProcessorType *lpt;

      for ( World::node_iterator
               it = sc.world_w().begin_nodes_w();
               it != sc.world_w().end_nodes_w();
					++it )
					{
						lpt = it->get_processor_of_type_w<LocalizationProcessorType>() ;
						const BoolTag* bool_tag = dynamic_cast<const BoolTag*>( it->find_tag("anchor").get() );
						if(( bool_tag!= NULL ) &&
							( bool_tag->value() ) &&  
							( !lpt->is_anchor() ) &&
							 ( lpt!= NULL ) )
								{
									++cnt;
									lpt->set_proc_type( LocalizationProcessorType::anchor );
								}
					}
			return cnt;
	}
}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/localization/localization_anchor_placement.cpp,v $
 * Version $Revision: 219 $
 * Date    $Date: 2008-06-22 23:55:01 +0200 (Sun, 22 Jun 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_anchor_placement.cpp,v $
 *-----------------------------------------------------------------------*/
