/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/localization_processor.h"
#include "apps/localization/modules/localization_nop_module.h"
#include "apps/localization/modules/distance/localization_sum_dist_module.h"
#include "apps/localization/modules/distance/localization_dv_hop_module.h"
#include "apps/localization/modules/distance/localization_euclidean_module.h"
#include "apps/localization/modules/distance/localization_gpsfree_lcs_module.h"
#include "apps/localization/modules/position/localization_dls_module.h"
#include "apps/localization/modules/refinement/localization_iter_dls_module.h"
#include "apps/localization/modules/position/localization_minmax_module.h"
#include "apps/localization/modules/position/localization_lateration_module.h"
#include "apps/localization/modules/position/localization_gpsfree_ncs_module.h"
#include "apps/localization/modules/position/localization_gpsfree_ncs_extended_module.h"
#include "apps/localization/modules/refinement/localization_iter_lateration_module.h"
#include "apps/localization/util/localization_utils.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/misc/random/random_variable_keeper.h"
#include "sys/misc/random/uniform_random_variable.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/communication_model.h"

#include "apps/localization/modules/distance/localization_test_module.h"
using namespace shawn;

namespace localization
{

   LocalizationProcessor::
   LocalizationProcessor()
      : dist_algo_            ( dv_hop ),
         pos_algo_            ( min_max ),
         ref_algo_            ( none ),
         phase_               ( distance ),
         isServer_            (false),
         startup_anchor_frac_ ( 0 ),
         check_residue_       ( true ),
         floodlimit_          ( 4 ),
         idle_time_           ( 5 ),
         idle_shutdown_time_  ( 0 ),
         comm_range_          ( 0 ),
         rollback_iteration_  ( 0 ),
         rollback_period_     ( std::numeric_limits<int>::max() ),
         rollback_cnt_        ( 0 ),
         rollback_limit_      ( 0 ),
         confidence_          ( 0 ),
         dist_est_            ( NULL )
   {}
   // ----------------------------------------------------------------------
   LocalizationProcessor::
   ~LocalizationProcessor()
   {
      if ( dist_module_ ) delete dist_module_;
      if ( pos_module_ ) delete pos_module_;
      if ( ref_module_ ) delete ref_module_;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationProcessor::
   boot( void )
      throw()
   {
      neighborhood_.set_source( owner_w() );

      random_ = owner().world().simulation_controller().
                     random_variable_keeper().find( "uni[0;1]" );

      init_parameters();
      init_modules();
      init_proc_type();
      assert( dist_module_ && pos_module_ && ref_module_ );

      dist_module_->set_owner( *this );
      dist_module_->boot();

      pos_module_->set_owner( *this );
      pos_module_->boot();

      ref_module_->set_owner( *this );
      ref_module_->boot();
   }
      /**
   * This is only proccessed if the positioning algorithm is DLS.
   * This fakes the step of processing the Matrix Ap and the vector d.
   * under "normal" circumstances beacon processors have to transmit their
    * positions to the "server";
   **/
    void
   LocalizationProcessor::
   special_boot( void )
		throw()
	{
	}

   // ----------------------------------------------------------------------
   bool
   LocalizationProcessor::
   process_message( const ConstMessageHandle& mh )
      throw()
   {
      if ( dist_module_->process_message( mh ) ||
            pos_module_->process_message( mh ) ||
            ref_module_->process_message( mh ) )
         return true;

      return Processor::process_message( mh );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationProcessor::
   work( void )
      throw()
   {
      rollback_iteration_++;

      if ( phase_ == distance )
      {
         dist_module_->work();

         if ( dist_module_->finished() )
            phase_ = position;
            if(dynamic_cast<LocalizationTestModule*>( dist_module_ ) !=NULL)
            {
               phase_ = distance;
               set_state( Inactive );
            }
      }

      if ( phase_ == position )
      {
         pos_module_->work();

         if ( pos_module_->finished() )
            phase_ = refinement;
      }

      if ( phase_ == refinement )
      {
         ref_module_->work();
      }

      if ( rollback_iteration_ == rollback_period_ )
      {
         rollback_iteration_ = 0;
         rollback_cnt_++;

         phase_ = distance;
         dist_module_->rollback();
         pos_module_->rollback();
         ref_module_->rollback();
      }

      if ( rollback_limit_ != 0 && rollback_cnt_ == rollback_limit_ )
         set_state( Inactive );

      if ( rollback_limit_ == 0 && dist_module_->finished() && pos_module_->finished() && ref_module_->finished() )
         set_state( Inactive );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationProcessor::
   init_parameters( void )
      throw()
   {
      const SimulationEnvironment& se = owner().world().simulation_controller().environment();

      dist_algo_ = DistanceAlgorithm(
         get_index(
            se.optional_string_param( "loc_dist_algo", "" ),
            DIST_ALGOS,
            sizeof( DIST_ALGOS ) / sizeof( DIST_ALGOS[0] ),
            int( dist_algo_ ) ) );
      pos_algo_ = PositionAlgorithm(
         get_index(
            se.optional_string_param( "loc_pos_algo", "" ),
            POS_ALGOS,
            sizeof( POS_ALGOS ) / sizeof( POS_ALGOS[0] ),
            int( pos_algo_ ) ) );
      ref_algo_ = RefinementAlgorithm(
         get_index(
            se.optional_string_param( "loc_ref_algo", "" ),
            REF_ALGOS,
            sizeof( REF_ALGOS ) / sizeof( REF_ALGOS[0] ),
            int( ref_algo_ ) ) );

      std::string dist_est_name = se.required_string_param( "loc_est_dist" );
      dist_est_ = owner().world().simulation_controller().
                     distance_estimate_keeper().find( dist_est_name );
      assert( dist_est_ != NULL );

      check_residue_ = se.optional_bool_param( "loc_check_residue", check_residue_ );
      floodlimit_ = se.optional_int_param( "loc_floodlimit", floodlimit_ );
      idle_time_ = se.optional_int_param( "loc_idle_time", idle_time_ );
      idle_shutdown_time_ = se.optional_int_param( "loc_idle_shutdown_time", idle_shutdown_time_ );

      startup_anchor_frac_ = se.optional_double_param( "loc_startup_anchor_frac", startup_anchor_frac_ );

      rollback_period_ = se.optional_int_param( "loc_rollback_period", rollback_period_ );
      rollback_limit_ = se.optional_int_param( "loc_rollback_limit", rollback_limit_ );

      if ( owner().world().communication_model().exists_communication_upper_bound() )
         comm_range_ = owner().world().communication_model().communication_upper_bound();

      assert( startup_anchor_frac_ >= 0 && startup_anchor_frac_ <= 1 );
      assert( floodlimit_ >= 0 && floodlimit_ <= std::numeric_limits<unsigned int>::max() );
      assert( idle_time_ >= 0 && idle_time_ <= std::numeric_limits<int>::max() );
      assert( rollback_period_ >= 0 && rollback_period_ <= std::numeric_limits<int>::max() );
      assert( rollback_limit_ >= 0 && rollback_limit_ <= std::numeric_limits<int>::max() );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationProcessor::
   init_modules( void )
      throw()
   {
      switch ( dist_algo_ )
      {
         case sum_dist:
            dist_module_ = new LocalizationSumDistModule();
            break;

         case dv_hop:
            dist_module_ = new LocalizationDVhopModule();
            break;

         case euclidean:
            dist_module_ = new LocalizationEuclideanModule();
            break;

         case gpsless_lcs:
            dist_module_ = new LocalizationGPSfreeLCSModule();
            break;
		 case test:
			 dist_module_ = new LocalizationTestModule();
      }

      switch ( pos_algo_ )
      {
          case min_max:
            pos_module_ = new LocalizationMinMaxModule();
            break;

         case lateration:
            pos_module_ = new LocalizationLaterationModule();
            break;

         case gpsless_ncs:
            pos_module_ = new LocalizationGPSfreeNCSModule();
            break;

         case gpsless_ncs_extended:
            pos_module_ = new LocalizationGPSfreeNCSExtendedModule();
            break;

		 case DLS:
            pos_module_ =new LocalizationDLSModule();
            break;
      }

      switch ( ref_algo_ )
      {
         case none:
            ref_module_ = new LocalizationNopModule();
            break;

         case iter_lateration:
            ref_module_ = new LocalizationIterLaterationModule();
            break;

		 case iDLS:
            ref_module_ = new LocalizationiDLSModule();
            break;
      }
   }
   // ----------------------------------------------------------------------
   void
   LocalizationProcessor::
   init_proc_type( void )
      throw()
   {
      if ( startup_anchor_frac_ > 0 && startup_anchor_frac_ > *random_ )
      {
         DEBUG( logger(), "set " << owner().label() << " as anchor on startup" );
         set_proc_type( anchor );
      }
	//  if( owner().is_special_node())
	//	  set_proc_type( server);

      switch ( proc_type() )
      {
         case anchor:
         {
            confidence_ = 1;
			double position_error= owner().world().simulation_controller().environment().optional_double_param("anchor_pos_err",-1);
			if(position_error>0.0){
				/*shawn::UniformRandomVariable* urv = new shawn::UniformRandomVariable();
				urv->set_upper_bound(2*position_error);
				urv->set_upper_bound_inclusive(false);
				urv->set_lower_bound_inclusive(false);
				urv->init();
				double dx = (*urv)-position_error;
				double dy = (*urv) - position_error;
				double dz =(*urv) - position_error;
				*/

				shawn::UniformRandomVariable urv;
				urv.set_upper_bound(2*position_error);
				urv.set_upper_bound_inclusive(false);
				urv.set_lower_bound_inclusive(false);
				urv.init();
				double dx = (urv)-position_error;
				double dy = (urv) - position_error;
				double dz =(urv) - position_error;

				Vec tmp=owner().real_position();
				if(tmp.z()==0)
					dz=0;
				Vec pos(tmp.x()+dx, tmp.y()+dy,tmp.z()+dz);
				owner_w().set_est_position(pos);
			}
			else
				owner_w().set_est_position( owner().real_position() );
         }

         case unknown:
         {
            confidence_ = 0.1;
         }
		 case server:
		 {
		 }
      }
   }
   // ----------------------------------------------------------------------
   double
   LocalizationProcessor::
   estimate_distance( const Node& src, const Node& dest )
      const throw()
   {
//TODO: remove method estimate_distance and put it either in the Module or in
//       the Observer by giving access to ConstNodeDistanceEstimateHandle.
//       Moreover it should be checked, when and why estimate_distance
//       returns false.
      double distance;
      //assert( dist_est_->estimate_distance( src, dest, distance ) );
	  bool line_of_sight_only = owner().world().simulation_controller().environment().optional_bool_param("line_of_sight_only",false);
	  bool estimated = dist_est_->estimate_distance( src, dest, distance );
	  if(line_of_sight_only && !estimated )
		distance = UNKNOWN_DISTANCE;
      return distance;
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/localization_processor.cpp,v $
 * Version $Revision: 510 $
 * Date    $Date: 2011-03-01 20:37:06 +0100 (Tue, 01 Mar 2011) $
 *-----------------------------------------------------------------------
 * $Log: localization_processor.cpp,v $
 *-----------------------------------------------------------------------*/
