/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_OBSERVER_H
#define __SHAWN_APPS_LOCALIZATION_OBSERVER_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/localization_processor.h"
#include "apps/localization/math/localization_statistic.h"
#include <string>


namespace localization
{

   class LocalizationProcessor;


   /// Observer of LocalizationProcessor
   /** The task of this class is to observe a LocalizationProcessor. It
    *  provides information about the used parameters and some
    *  \ref LocalizationNeighborhood "neighborhood" characteristics.
    */
   class LocalizationObserver
   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      LocalizationObserver();
      virtual ~LocalizationObserver();
      ///@}


      ///@name localization processor to observe
      ///@{
      ///
      virtual void set_owner( LocalizationProcessor& ) throw();
      virtual const LocalizationProcessor& owner( void ) const throw();
      virtual LocalizationProcessor& owner_w( void ) throw();
      ///@}


      ///@name used parameters
      ///@{
      ///
      virtual double startup_anchor_frac( void ) const throw();
      ///
      virtual const std::string& distance_algorithm( void ) const throw();
      ///
      virtual const std::string& position_algorithm( void ) const throw();
      ///
      virtual const std::string& refinement_algorithm( void ) const throw();
      ///
      virtual unsigned int floodlimit( void ) const throw();
      ///
      virtual bool check_residue( void ) const throw();
      ///
      virtual int idle_time( void ) const throw();
      ///
      virtual int idle_shutdown_time( void ) const throw();
      ///
      virtual double comm_range( void ) const throw();
      ///@}


      ///@name special information
      ///@{
      ///
      virtual double confidence( void ) const throw();
      ///@}


      ///@name neighborhood information and statistics
      ///@{
      /** \return Number of direct, that means one hop, neighbors
       */
      virtual int neighbor_cnt( void ) const throw();
      /** \return Number of known anchors. This includes ones without
       *  distance information.
       */
      virtual int anchor_cnt( void ) const throw();
      /** \return Number of valid known anchors. That means, that the distance
       *  to the anchor, respectively it's estimation, has to be known.
       */
      virtual int valid_anchor_cnt( void ) const throw();
      /** Fills the given \ref LocalizationStatistic "LocalizationStatistics"
       *  with information about estimated anchor distances.
       *  \param LocalizationStatistic relative to the real distance
       *  \param LocalizationStatistic relative to communication range
       */
      virtual void fill_stat_rel_anchor_distance(
            LocalizationStatistic&,
            LocalizationStatistic& )
         const throw();
	  virtual void fill_stat_abs_anchor_distance(
            LocalizationStatistic&)
         const throw();
      /** Fills the given \ref LocalizationStatistic "LocalizationStatistics"
       *  with information about estimated neighbor distances.
       *  \param LocalizationStatistic relative to the real distance
       *  \param LocalizationStatistic relative to communication range
       */
      virtual void fill_stat_rel_neighbor_distance(
            LocalizationStatistic&,
            LocalizationStatistic& )
         const throw();
      ///@}
	  virtual void fill_stat_abs_neighbor_distance(
            LocalizationStatistic& )
         const throw();

   private:

      LocalizationProcessor* owner_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/localization_observer.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_observer.h,v $
 *-----------------------------------------------------------------------*/
