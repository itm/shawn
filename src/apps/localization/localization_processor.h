/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_PROCESSOR_H
#define __SHAWN_APPS_LOCALIZATION_PROCESSOR_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/neighborhood/localization_neighbor_info.h"
#include "apps/localization/neighborhood/localization_neighborhood.h"
#include "apps/localization/neighborhood/localization_local_coordinate_system.h"
#include "apps/localization/modules/localization_module.h"
#include "apps/localization/util/localization_defutils.h"
#include "sys/misc/localization/localization_processor_type.h"
#include "sys/misc/random/random_variable.h"
#include "sys/node_distance_estimate.h"
#include "sys/node.h"
#include "sys/processor.h"

#ifdef ENABLE_VFDB
#include "apps/vfdb/vfdb_postscript_task.h"
#endif


namespace localization
{

   ///@name localization processor parameters
   ///@{
   const std::string DIST_ALGOS[] = { "euclidean", "dv_hop", "sum_dist", "gpsless_lcs","test" };
   const std::string POS_ALGOS[] = { "lateration", "min_max", "gpsless_ncs", "gpsless_ncs_extended","DLS" };
   const std::string REF_ALGOS[] = { "none", "iterative_lateration","iDLS" };
   ///@}


   class LocalizationModule;


   /// Processor, that executes different localization algorithms
   /** There are anchorbased and anchorless algorithms. Anchorbased means, that
    *  unknown nodes, which do not know their position, need some nodes in the
    *  topology, which know their position. Anchorless algorithms build a
    *  relative coordinate system without the need of position aware nodes.
    *  Generally you should not mix these algorithms because potentially
    *  there will be no result.
    *
    *  Localization is cut in three phases depending on using anchors or not.
    *
    *  Anchorbased: At first, unknown nodes try to
    *  estimate their distance to the anchors. Second, with the resulting
    *  information the position is calculated. Third, an optional one, the
    *  estimated position is calibrated, e.g. with help of neighbors
    *  information.
    *
    *  Anchorless: At first, nodes try to get information about their
    *  neighborhood and build a local coordinate system. Second, they will
    *  build a global, or rather network, coordinate system. Third phase
    *  is an optional one where the positions and coordinate systems are
    *  calibrated.
    *
    *  The algorithms are not designed for mobility primary, but there is a
    *  chance to do so. In general each phase is processed one time before
    *  the processor deactivate itself. Therefor you could give an rollback
    *  number, which rerun the phases after the given number of rounds.
    *  Moreover there is an rollback_limit which limit the number of
    *  maximal rollbacks.
    *
    *  The algorithms are implemented in so called
    *  \ref LocalizationModule "modules", one designed for each algorithm in
    *  each phase. All algorithms and calculations work on the same
    *  \ref LocalizationNeighborhood "neighborhood".
    *  <hr>
    *
    *  <b>Using this processor with parameters</b>
    *
    *  TODO: Write this; do same in each module, too.
    *  <hr>
    *
    *  <b>Writing your own modules</b>
    *
    *  To write an own module, you have to create a new class derived from
    *  LocalizationModule in the appropriate directory,
    *  namely <em>module/distance</em>, <em>module/position</em> or
    *  <em>module/refinement</em> and put all the wanted intelligence in it.
    *  If there are messages to be sent, there is an corresponding directory
    *  order in <em>messages/</em>.
    *
    *  To include this module in this \ref LocalizationProcessor "Processor",
    *  you should give a name in DIST_ALGOS[], POS_ALGOS[] or REF_ALGOS[]
    *  used by commandline or configuration file. Moreover there are
    *  enumerations in the private part of this class, namely DistanceAlgorithm,
    *  PositionAlgorithm and RefinementAlgorithm. You should give a name here,
    *  too, which has to be in the order as in the above mentioned arrays.
    *
    *  After you have done this, the module has to be inserted in the method
    *  LocalizationProcessor::init_modules just like the existing ones.
    *  <hr>
    *
    *  <b>Comment by the author</b>
    *
    *  This implementation was originally designed just for anchorbased
    *  algorithms devided in mentioned three phases. Done this, the
    *  anchorless algorithm(s) followed. Perhaps it would be an
    *  advantage to avoid this static phase-behavior and use the shawn-like
    *  factory structure to use more or less modules per processor
    *  (or will it be done later?).
    *
    *  Moreover, with increasing the number of modules, the
    *  \ref LocalizationNeighborhood "neighborhood" became more and more
    *  complicated. It would be appreciated to change this structure, because
    *  easy and fast algorithms like DV-hop and Min-Max have to carry a very
    *  big and useless overhead.
    */
   class LocalizationProcessor
      : public shawn::Processor,
         public shawn::LocalizationProcessorType
   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      LocalizationProcessor();
      ///
      virtual ~LocalizationProcessor();
      ///@}


      ///@name basic methods inherited from Processor
      ///@{
      /** This method reads parameters given by the simulation or a
       *  configuration file and take some initial settings.
       *  Beyond parameter settings, the \ref LocalizationModule "modules"
       *  for distance and refinement phase were created and their boot
       *  methods called.
       *  \sa shawn::Processor::boot()
       */
      virtual void boot( void ) throw();

	  virtual void special_boot( void ) throw();

      /** At first, this method checks, if the message should gone lost.
       *  If not, message is forwarded to
       *  \ref LocalizationModule "distance and refinement module".
       *  \return \c true, if message is processed, \c false, if not.
       *  \sa shawn::Processor::process_message()
       */
      virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
      /** TODO: modules had been renewed!
       *  At first, this method executes the work method of both
       *  \ref LocalizationModule "distance and refinement module".
       *  Second, if the distance phase is finished, this method tries to
       *  estimate the node's position.
       *  \sa shawn::Processor::work()
       */
      virtual void work( void ) throw();
      ///@}


   protected:

      ///@name initial methods
      ///@{
      /** Read the given parameters, which have been set via simulator
       *  commands or configuration file.
       */
      virtual void init_parameters( void ) throw();
      /** Init the \ref LocalizationModule "distance and refinement module" to
       *  use one of the different algorithms.
       */
      virtual void init_modules( void ) throw();
      /** Some initial configuration for both processor types anchor and
       *  unknown. To deal with the different algorithms and modules,
       *  an anchor, e.g., set it's estimated to real position.
       */
      virtual void init_proc_type( void ) throw();
      ///@}


      ///@name distance estimation
      ///@{
      /** Estimates distance between given nodes. The are different choices like
       *  uniform and normal distributions, which can be set via parameters
       *  in configuration file.
       */
      virtual double estimate_distance( const shawn::Node&, const shawn::Node& )
         const throw();
      ///@}


   private:

      enum DistanceAlgorithm { euclidean, dv_hop, sum_dist, gpsless_lcs, test};
      enum PositionAlgorithm { lateration, min_max, gpsless_ncs, gpsless_ncs_extended,DLS };
      enum RefinementAlgorithm { none, iter_lateration,iDLS };

      enum LocalizationPhase { distance, position, refinement };


      DistanceAlgorithm dist_algo_;
      PositionAlgorithm pos_algo_;
      RefinementAlgorithm ref_algo_;
      LocalizationPhase phase_;
	  bool isServer_;

      double startup_anchor_frac_;

      bool check_residue_;
      unsigned int floodlimit_;
      int idle_time_;
      int idle_shutdown_time_;
      double comm_range_;

      int rollback_iteration_;
      int rollback_period_;
      int rollback_cnt_;
      int rollback_limit_;

      double confidence_;

      shawn::ConstNodeDistanceEstimateHandle dist_est_;
      shawn::ConstRandomVariableHandle random_;

      LocalizationModule* dist_module_;
      LocalizationModule* pos_module_;
      LocalizationModule* ref_module_;

      LocalizationNeighborhood neighborhood_;
      LocalizationLocalCoordinateSystem local_coord_sys_;

      friend class LocalizationObserver;
      friend class LocalizationModule;
	  friend class LocalizationiDLSModule;
	  friend class LocalizationDLSModule;
	  friend class LocalizationTestModule;
	  #ifdef ENABLE_VFDB
	  friend class vfdb::VfdbPostscriptTask;
      #endif
   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/localization_processor.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_processor.h,v $
 *-----------------------------------------------------------------------*/
