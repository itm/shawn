/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_TASKS_EVALUATION_H
#define __SHAWN_APPS_LOCALIZATION_TASKS_EVALUATION_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "sys/simulation/simulation_task.h"
#include "apps/localization/math/localization_statistic.h"
#include <iostream>


namespace localization
{

   class SimulationTaskLocalizationEvaluation
      : public shawn::SimulationTask
   {

   public:

      class HeaderInfo;
      class Results;

      SimulationTaskLocalizationEvaluation();
      virtual ~SimulationTaskLocalizationEvaluation();
      virtual void run( shawn::SimulationController& ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();


   protected:

      ///
      virtual void print( const HeaderInfo&, const Results& )
         const throw();
      ///
      virtual void print_ps( const shawn::SimulationController&, const HeaderInfo& )
         const throw();
      ///
      virtual void write_out(
            const shawn::SimulationController&,
            const HeaderInfo&,
            const Results& )
         const throw( std::runtime_error );
      ///
      virtual void write_header( const std::string&, const HeaderInfo& )
         const throw( std::runtime_error );
      ///
      virtual void write_results( const std::string&, const Results& )
         const throw( std::runtime_error );

   };

   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------

   class SimulationTaskLocalizationEvaluation::HeaderInfo
   {

   public:

      HeaderInfo();
      virtual ~HeaderInfo();

      ///
      virtual void collect_information( shawn::SimulationController&, const SimulationTaskLocalizationEvaluation& ) throw();
      ///
      virtual void set_placement( const shawn::SimulationController&, const SimulationTaskLocalizationEvaluation& ) throw();


      std::string dist_algo, pos_algo, ref_algo;
      int unknown_cnt, anchor_cnt;

      double startup_anchor_frac;
      int placed_anchor_cnt;
      std::string pl_type;

      double world_width, world_height;
      double comm_range;
      int floodlimit;
      int idle_time;

      friend std::ostream& operator<<( std::ostream&, const HeaderInfo& ) throw();
   };

   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------

   class SimulationTaskLocalizationEvaluation::Results
   {

   public:

      Results();
      virtual ~Results();

      ///
      virtual void collect_information( shawn::SimulationController&, const SimulationTaskLocalizationEvaluation& ) throw();
      ///
      virtual void set_seed( const shawn::SimulationController&, const SimulationTaskLocalizationEvaluation& ) throw();
      ///
      virtual double coverage( void ) const throw();


      long int seed;

      int has_pos_cnt;
      int no_pos_cnt;

      double comm_range;

      LocalizationStatistic stat_real_err_anchor_dist;
      LocalizationStatistic stat_comm_err_anchor_dist;

      LocalizationStatistic stat_real_neighbor_dist;
      LocalizationStatistic stat_comm_neighbor_dist;

      LocalizationStatistic stat_abs_position;
      LocalizationStatistic stat_neighbor_cnt;     
      LocalizationStatistic stat_known_anchors;
      LocalizationStatistic stat_valid_known_anchors;

      friend std::ostream& operator<<( std::ostream&, const Results& ) throw();
   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/tasks/localization_evaluation.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_evaluation.h,v $
 *-----------------------------------------------------------------------*/
