/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include <algorithm>
#include <fstream>
#include <limits>
#include <sstream>

#include "apps/localization/tasks/localization_evaluation.h"
#include "sys/world.h"
#include "sys/communication_model.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/misc/random/random_seed.h"
#include "sys/misc/localization/localization_anchor_placement.h"
#include "apps/localization/localization_processor.h"
#include "apps/localization/localization_observer.h"
#include "apps/localization/util/localization_ps_writer.h"


using namespace shawn;

namespace localization
{

   SimulationTaskLocalizationEvaluation::
   SimulationTaskLocalizationEvaluation()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskLocalizationEvaluation::
   ~SimulationTaskLocalizationEvaluation()
   {}
   // ----------------------------------------------------------------------
   void
   SimulationTaskLocalizationEvaluation::
   run( SimulationController& sc )
      throw( std::runtime_error )
   {
      require_world( sc );

      HeaderInfo header;
      Results results;

      header.collect_information( sc, *this );
      results.collect_information( sc, *this );

      print( header, results );
      print_ps( sc, header );
      write_out( sc, header, results );
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskLocalizationEvaluation::
   name( void )
      const throw()
   {
      return std::string( "localization_evaluation" );
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskLocalizationEvaluation::
   description( void )
      const throw()
   {
      return std::string( "writes localization evaluation to stdout and $ls_out_file" );
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskLocalizationEvaluation::
   print( const HeaderInfo& header, const Results& results )
      const throw()
   {
	  std::stringstream oss;
	  
	  oss << "Distance algorithm: \t" << header.dist_algo << std::endl
            << "Position algorithm: \t" << header.pos_algo << std::endl
            << "Refinement algorithm: \t" << header.ref_algo << std::endl
            << std::endl
            << results.has_pos_cnt << " nodes know their position, "
            << results.no_pos_cnt << " are still unknowning. "
            << "That's a coverage of " << results.coverage() << "%." << std::endl
            << std::endl
            << "Relative anchor distances (real): "
            << "mean is " << results.stat_real_err_anchor_dist.mean()
            << ", std-dev is " << results.stat_real_err_anchor_dist.std_dev()
            << " ( from " << results.stat_real_err_anchor_dist.min()
            << " to " << results.stat_real_err_anchor_dist.max() << " )" << std::endl
            << "Relative anchor distances (comm_range): "
            << "mean is " << results.stat_comm_err_anchor_dist.mean()
            << ", std-dev is " << results.stat_comm_err_anchor_dist.std_dev()
            << " ( from " << results.stat_comm_err_anchor_dist.min()
            << " to " << results.stat_comm_err_anchor_dist.max() << " )" << std::endl
            << "Average absolute distance from real pos: "
            << results.stat_abs_position.mean()
            << " ( " << ( results.stat_abs_position.mean() / header.comm_range ) * 100
            << "% of communication range )"
            << " ( from " << results.stat_abs_position.min()
            << " to " << results.stat_abs_position.max() << " )" << std::endl
            << "Average relative neighbor distance error is "
            <<  results.stat_real_neighbor_dist.mean()
            << " ( from " << results.stat_real_neighbor_dist.min()
            << " to " << results.stat_real_neighbor_dist.max() << " )" << std::endl
            << std::endl
            << "Average number of neighbors per node is " << results.stat_neighbor_cnt.mean()
            << " ( from " << results.stat_neighbor_cnt.min()
            << " to " << results.stat_neighbor_cnt.max() << " )";

	  USER( oss.str() );
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskLocalizationEvaluation::
   print_ps( const SimulationController& sc, const HeaderInfo& header )
      const throw()
   {
      std::string fname = sc.environment().optional_string_param( "loc_ps_out", "" );
      if ( fname == "" ) return;

      std::ofstream psout( fname.c_str() );
      LocalizationPsWriter psw( psout, false );
      std::string info =
         "Dist: " + header.dist_algo
            + "; Pos: " + header.pos_algo
            + "; Ref: " + header.ref_algo;

      psw.paint_color( sc.world(), info, true );
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskLocalizationEvaluation::
   write_out( const SimulationController& sc, const HeaderInfo& header, const Results& results )
      const throw( std::runtime_error )
   {
      std::string fname = sc.environment().optional_string_param( "loc_ls_out", "" );
      if ( fname == "" ) return;

      std::string ftype = sc.environment().required_string_param( "loc_ls_type" );
      if ( ftype != "create" && ftype != "append" )
         throw std::runtime_error("Wrong argument for 'loc_ls_type'. Should be 'create' or 'append'");

      if ( ftype == "create" )
         write_header( fname, header );

      write_results( fname, results );
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskLocalizationEvaluation::
   write_header( const std::string& fname, const HeaderInfo& header )
      const throw( std::runtime_error )
   {
      std::ofstream ofs( fname.c_str(), std::ios::trunc );
      if ( !ofs )
         throw std::runtime_error( "Cannot open file " + fname );

      ofs << header;
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskLocalizationEvaluation::
   write_results( const std::string& fname, const Results& results )
      const throw( std::runtime_error )
   {
      std::ofstream ofs( fname.c_str(), std::ios::app );
      if ( !ofs )
         throw std::runtime_error( "Cannot open file " + fname );

      ofs << results;
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   SimulationTaskLocalizationEvaluation::HeaderInfo::
   HeaderInfo()
      : unknown_cnt          ( 0 ),
         anchor_cnt          ( 0 ),
         startup_anchor_frac ( 0 ),
         placed_anchor_cnt   ( 0 ),
         world_width         ( 0 ),
         world_height        ( 0 ),
         comm_range          ( 0 ),
         floodlimit          ( 0 ),
         idle_time           ( 0 )
   {}
   // ----------------------------------------------------------------------
   SimulationTaskLocalizationEvaluation::HeaderInfo::
   ~HeaderInfo()
   {}
   // ----------------------------------------------------------------------
   void
   SimulationTaskLocalizationEvaluation::HeaderInfo::
   collect_information( SimulationController& sc, const SimulationTaskLocalizationEvaluation& stle )
      throw()
   {
      LocalizationObserver observer;

      if ( sc.world().communication_model().exists_communication_upper_bound() )
         comm_range = sc.world().communication_model().communication_upper_bound();

      world_width = sc.world().upper_right().x() - sc.world().lower_left().x();
      world_height = sc.world().upper_right().y() - sc.world().lower_left().y();

      set_placement( sc, stle );

      for( World::node_iterator
              it = sc.world_w().begin_nodes_w();
           it != sc.world_w().end_nodes_w();
           ++it )
      {
         Node& v = *it;
         LocalizationProcessor* lp =
            v.get_processor_of_type_w<LocalizationProcessor>();

         if ( lp == NULL ) continue;

         if ( lp->is_anchor() )
            ++anchor_cnt;
         else
            ++unknown_cnt;

         observer.set_owner( *lp );

         startup_anchor_frac = observer.startup_anchor_frac();
         dist_algo = observer.distance_algorithm();
         pos_algo = observer.position_algorithm();
         ref_algo = observer.refinement_algorithm();
         floodlimit = observer.floodlimit();
         idle_time = observer.idle_time();
      }
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskLocalizationEvaluation::HeaderInfo::
   set_placement( const SimulationController& sc, const SimulationTaskLocalizationEvaluation& stle )
      throw()
   {
      const ConstSimulationTaskHandle& csth =
               sc.simulation_task_keeper().find( "localization_anchor_placement" );
      if ( csth != NULL )
      {
         const SimulationTaskLocalizationAnchorPlacement& stlap =
            *dynamic_cast<const SimulationTaskLocalizationAnchorPlacement*>( csth.get() );

         DEBUG( stle.logger(), "HeaderInfo: placed anchors: " << stlap.anchor_cnt() << "; " << stlap.pl_type() );

         placed_anchor_cnt = stlap.anchor_cnt();
         pl_type = stlap.pl_type();
      }
      else
      {
         placed_anchor_cnt = 0;
         pl_type = "";
      }
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   SimulationTaskLocalizationEvaluation::Results::
   Results()
      : seed         ( 0 ),
         has_pos_cnt ( 0 ),
         no_pos_cnt  ( 0 ),
         comm_range  ( 1 )
   {}
   // ----------------------------------------------------------------------
   SimulationTaskLocalizationEvaluation::Results::
   ~Results()
   {}
   // ----------------------------------------------------------------------
   void
   SimulationTaskLocalizationEvaluation::Results::
   collect_information( SimulationController& sc, const SimulationTaskLocalizationEvaluation& stle )
      throw()
   {
      LocalizationObserver observer;

      if ( sc.world().communication_model().exists_communication_upper_bound() )
         comm_range = sc.world().communication_model().communication_upper_bound();

      set_seed( sc, stle );

      for( World::node_iterator
               it = sc.world_w().begin_nodes_w();
               it != sc.world_w().end_nodes_w();
               ++it )
      {
         Node& v = *it;
         LocalizationProcessor* lp =
            v.get_processor_of_type_w<LocalizationProcessor>();

         if ( lp == NULL || lp->is_anchor() ) continue;
         observer.set_owner( *lp );

         if ( v.has_est_position() )
         {
            ++has_pos_cnt;
            double distance = euclidean_distance( v.real_position(), v.est_position() );

            stat_abs_position += distance;
         }
         else
         {
            ++no_pos_cnt;
         }

         stat_known_anchors += observer.anchor_cnt();
         stat_valid_known_anchors += observer.valid_anchor_cnt();

         observer.fill_stat_rel_anchor_distance(
            stat_real_err_anchor_dist,
            stat_comm_err_anchor_dist );

         observer.fill_stat_rel_neighbor_distance(
            stat_real_neighbor_dist,
            stat_comm_neighbor_dist );

         stat_neighbor_cnt += (double)observer.neighbor_cnt();
      }// for all nodes
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskLocalizationEvaluation::Results::
   set_seed( const SimulationController& sc, const SimulationTaskLocalizationEvaluation& stle )
      throw()
   {
      const ConstSimulationTaskHandle& csth =
               sc.simulation_task_keeper().find( "random_seed" );
      if ( csth != NULL )
      {
         const SimulationTaskRandomSeed& strs =
            *dynamic_cast<const SimulationTaskRandomSeed*>( csth.get() );

         DEBUG( stle.logger(), "Results: seed: " << strs.seed() );

         seed = strs.seed();
      }
      else
         seed = 0;
   }
   // ----------------------------------------------------------------------
   double
   SimulationTaskLocalizationEvaluation::Results::
   coverage( void )
      const throw()
   {
      if ( has_pos_cnt + no_pos_cnt == 0 )
         return 0;

      return ( (double)has_pos_cnt / (has_pos_cnt + no_pos_cnt) ) * 100;
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   std::ostream&
   operator<<(
         std::ostream& os,
         const SimulationTaskLocalizationEvaluation::HeaderInfo& header )
      throw()
   {
      os << "v1.0.4" << std::endl
         << "dist_algo; "
         << "pos_algo; "
         << "ref_algo; "
         << "anchor_cnt; "
         << "unknown_cnt; "
         << "startup_anchor_frac; "
         << "placed_anchor_cnt; "
         << "placed_type; "
         << "world_width; "
         << "world_height; "
         << "comm_range; "
         << "floodlimit; "
         << "idle;"
         << std::endl;

      os << header.dist_algo.c_str() << "; "
         << header.pos_algo.c_str() << "; "
         << header.ref_algo.c_str() << "; "
         << header.anchor_cnt << "; "
         << header.unknown_cnt << "; "
         << header.startup_anchor_frac << "; "
         << header.placed_anchor_cnt << "; "
         << header.pl_type.c_str() << "; "
         << header.world_width << "; "
         << header.world_height << "; "
         << header.comm_range << "; "
         << header.floodlimit << "; "
         << header.idle_time << "; "
         << std::endl;

      os << "seed; "
         << "has_pos_cnt; "
         << "no_pos_cnt; "
         << "avg_abs_distance; "
         << "avg_rel_distance_R; "
         << "coverage; "
         << "connectivity; "
         << "min_nbrs; "
         << "max_nbrs; "
         << "known_anchors; "
         << "valid_known_anchors; "
         << "std_dev_real_anchor_dist; "
         << "mean_real_anchor_dist; "
         << "std_dev_comm_anchor_dist; "
         << "mean_comm_anchor_dist; "
         << "std_dev_real_neighbor_dist; "
         << "mean_real_neighbor_dist; "
         << "std_dev_comm_neighbor_dist; "
         << "mean_comm_neighbor_dist; "
         << std::endl;

      return os;
   }
   // ----------------------------------------------------------------------
   std::ostream&
   operator<<(
         std::ostream& os,
         const SimulationTaskLocalizationEvaluation::Results& results )
      throw()
   {
      os << results.seed << "; "
         << results.has_pos_cnt << "; "
         << results.no_pos_cnt << "; "
         << results.stat_abs_position.mean() << "; "
         << ( results.stat_abs_position.mean() / results.comm_range ) * 100 << "; "
         << results.coverage() << "; "
         << results.stat_neighbor_cnt.mean() << "; "
         << results.stat_neighbor_cnt.min() << "; "
         << results.stat_neighbor_cnt.max() << "; "
         << results.stat_known_anchors.mean() << "; "
         << results.stat_valid_known_anchors.mean() << "; "
         << results.stat_real_err_anchor_dist.std_dev() << "; "
         << results.stat_real_err_anchor_dist.mean() << "; "
         << results.stat_comm_err_anchor_dist.std_dev() << "; "
         << results.stat_comm_err_anchor_dist.mean() << "; "
         << results.stat_real_neighbor_dist.std_dev() << "; "
         << results.stat_real_neighbor_dist.mean() << "; "
         << results.stat_comm_neighbor_dist.std_dev() << "; "
         << results.stat_comm_neighbor_dist.mean() << "; "
         << std::endl;

      return os;
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/tasks/localization_evaluation.cpp,v $
 * Version $Revision: 270 $
 * Date    $Date: 2008-10-22 13:03:27 +0200 (Wed, 22 Oct 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_evaluation.cpp,v $
 *-----------------------------------------------------------------------*/
