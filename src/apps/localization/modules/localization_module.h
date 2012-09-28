/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MODULE_H
#define __SHAWN_APPS_LOCALIZATION_MODULE_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "sys/message.h"
#include "sys/node.h"
#include "apps/localization/localization_processor.h"
#include "apps/localization/localization_observer.h"
#include "apps/localization/neighborhood/localization_neighborhood.h"
#include "apps/localization/neighborhood/localization_local_coordinate_system.h"


namespace localization
{

   class LocalizationProcessor;
   class LocalizationObserver;


   /// Basic localization module to act with LocalizationProcessor.
   /** This is the basic localization module which has to be implemented if
    *  someone wants to add a new algorithm.
    *
    *  Functionality is like a Processor. There are the same methods boot(),
    *  process_message() and work(), which are called by the
    *  LocalizationProcessor. Each LocalizationProcessor owns two
    *  LocalizationModules; one for distance phase (get distance to anchors)
    *  and one for refinement phase (calibrating the estimated position).
    *  Moreover the modules are able to send messages by the method send().
    *
    *  Particularly a distance module has to work on given neighborhood(),
    *  because the LocalizationProcessor uses it to estimate it's position.
    *
    *  If the distance phase finished, method finished() has to return
    *  \c true, so that the LocalizationProcessor is able to estimate it's
    *  position. Same with a refinement module to make the
    *  LocalizationProcessor able to deactivate itself.
    */
   class LocalizationModule
   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      LocalizationModule();
      ///
      virtual ~LocalizationModule();
      ///@}


      ///@name standard methods startup/simulation steps
      ///@{
      /** Boot method which is called once on startup.
       *
       *  \sa LocalizationProcessor::boot(), Processor::boot()
       */
      virtual void boot( void ) throw();
      /** This method is called if the LocalizationProcessor receives a
       *  message and forward it to the modules.
       *
       *  \return \c true if message is processed, \false otherwise
       *  \sa LocalizationProcessor::process_message(),
       *    Processor::process_message()
       */
      virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
      /** This method is called once each simulation round as long as the
       *  module is not finished.
       *
       *  Moreover a refinement module's method is not called until the
       *  distance module is finished.
       *
       *  \sa LocalizationProcessor::work(), Processor::work()
       */
      virtual void work( void ) throw();
      /** This method is called if there is a rollback_period given and
       *  reached. This is done in most cases, because the nodes are mobile,
       *  so that they change their position.
       *
       *  The easiest way of implementation would be to just reset all
       *  information.
       */
      virtual void rollback( void ) throw();
      ///@}


      ///@name basic access to owner, observer, neighborhood and local coordinate system
      ///@{
      /** This method sets the owner of the module. Moreover, from the given
       *  LocalizationProcessor, the observer and neighborhood are set.
       *
       *  \param LocalizationProcessor owner of module
       */
      virtual void set_owner( LocalizationProcessor& ) throw();
      /** \return LocalizationProcessor the module belongs to
       */
      virtual const LocalizationProcessor& owner( void ) const throw();
      /** \return writable LocalizationProcessor the module belongs to
       */
      virtual LocalizationProcessor& owner_w( void ) throw();
      /** \return LocalizationObserver of the LocalizationProcessor the module
       *    belongs to
       */
      virtual const LocalizationObserver& observer( void )
         const throw();
      /** \return LocalizationNeighborhood of the LocalizationProcessor the
       *    module belongs to
       */
      virtual const LocalizationNeighborhood& neighborhood( void )
         const throw();
      /** \return writable LocalizationNeighborhood of the
       *    LocalizationProcessor the module belongs to
       */
      virtual LocalizationNeighborhood& neighborhood_w( void ) throw();
      /** \return LocalizationLocalCoordinateSystem of the
       *    LocalizationProcessor the module belongs to
       */
      virtual const LocalizationLocalCoordinateSystem& local_coord_sys( void )
         const throw();
      /** \return writableLocalizationLocalCoordinateSystem of the
       *    LocalizationProcessor the module belongs to
       */
      virtual LocalizationLocalCoordinateSystem& local_coord_sys_w( void ) throw();
      ///@}


      ///@name status information
      ///@{
      /** \return \c true, if module is finished. \c false otherwise
       */
      virtual bool finished( void ) throw() = 0;
      ///@}


   protected:

      ///@name acces to node of belonging LocalizationProcessor
      ///@{
      /** \return Node of the LocalizationProcessor the module belongs to
       */
      virtual const shawn::Node& node( void ) const throw();
      /** \return writable Node of the LocalizationProcessor the module
       *    belongs to
       */
      virtual shawn::Node& node_w( void ) throw();
      /** \return the current iteration round
       *  \sa Node::simulation_round()
       */
      virtual int simulation_round( void ) const throw();
      ///@}


      ///@name special access to owner
      ///@{
      /** This method sets the confidence of belonging LocalizationProcessor
       */
      virtual void set_confidence( double ) throw();
      ///@}


      ///@name direct access to methods of LocalizationProcessor
      ///@{
      /** Send a message.
       *  /sa LocalizationProcessor::send(), Processor::send()
       */
      virtual void send(const shawn::MessageHandle&) throw();
      /** \return estimateted distance between given nodes.
       *  \sa LocalizationProcessor::estimate_distance()
       */
      virtual double estimate_distance( const shawn::Node&, const shawn::Node& )
         const throw();
      ///@}


   private:

      LocalizationProcessor* owner_;
      LocalizationObserver* observer_;
      LocalizationNeighborhood* neighborhood_;
      LocalizationLocalCoordinateSystem* local_coord_sys_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/localization_module.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_module.h,v $
 *-----------------------------------------------------------------------*/
