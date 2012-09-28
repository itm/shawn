/************************************************************************
** This file is part of the network simulator Shawn.                   **
** Copyright (C) 2007 by AutoNomos (www.auto-nomos.de)                 **
** This part of Shawn is free software; you can redistribute it and/or **
** modify it under the terms of the BSD License. Refer to the		   **
** shawn-licence.txt file in the root of the Shawn source tree for	   **
** further details.												       **
**                                                                     **
** \author Axel Wegener <wegener@itm.uni-luebeck.de>				   **
** \author Torsten Teubler <teubler@itm.uni-luebeck.de>			       **
**                                                                     **
************************************************************************/

#ifndef __SHAWN_LEGACYAPPS_AUTOCAST_PROCESSOR_H
#define __SHAWN_LEGACYAPPS_AUTOCAST_PROCESSOR_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_AUTOCAST

#include <set>
#include <map>
#include "sys/processor.h"
#include "sys/event_scheduler.h"
#include "sys/vec.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/misc/random/uniform_random_variable.h"
#include "sys/node_movements/linear_movement.h"
#include "apps/autocast/autocast_app/neighborhood.h"
#include "apps/autocast/autocast_app/dataunit.h"
#include "apps/autocast/autocast/autocast_message.h"

#define AUTOCAST_DEFAULT_STARTUP_TIME 5
#define AUTOCAST_DEFAULT_MIN_UPDATE_TIME 0.5
#define AUTOCAST_DEFAULT_MAX_UPDATE_TIME 20
#define AUTOCAST_DEFAULT_DATAUNIT_LIVETIME 300
#define AUTOCAST_DEFAULT_PACKET_SIZE 1500
#define AUTOCAST_DEFAULT_MAX_DATAUNITS_PER_PACKET 10
#define AUTOCAST_MAX_PACKET_LENGTH 1500

namespace autocast
{

   class AutoCastProcessor
      : public shawn::Processor, public shawn::EventScheduler::EventHandler
   {
   public:
      AutoCastProcessor();
      virtual ~AutoCastProcessor();

      virtual void boot( void ) throw();
      virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
      virtual void work( void ) throw();
      void timeout( shawn::EventScheduler&, shawn::EventScheduler::EventHandle,
         double, shawn::EventScheduler::EventTagHandle& ) throw();
      virtual bool send_to(const ConstDataUnitHandle&, shawn::Processor* creator = NULL) throw();
      bool booted() const throw();

      virtual bool get_active_state_at( double time ) const throw();

   protected:
      void set_state(const Processor::ProcessorState&) throw();

   private:
      /// An internal data structure to local save ONE DataUnit. This is
      /// virtually a "cell" stored in complete_DataUnits_.
      class LocalDataUnit{
      public:
         LocalDataUnit() : duh_(NULL),
            unknown_count_(0),
            last_send_time_(0),
            last_received_time_(0),
            creator_(NULL),
            refresh_before_send_(false)
         {}
         LocalDataUnit(const ConstDataUnitHandle& duh) : duh_(duh),
            unknown_count_(0),
            last_send_time_(0),
            last_received_time_(0),
            creator_(NULL),
            refresh_before_send_(false)
         {}
         LocalDataUnit(const LocalDataUnit& o) : duh_(o.dataUnit()),
            unknown_count_(o.unknown_count()),
            last_send_time_(o.last_send_time()),
            last_received_time_(o.last_received_time()),
            creator_(o.creator()),
            refresh_before_send_(o.refresh_before_send())
         {}
         ~LocalDataUnit(){}
         const ConstDataUnitHandle& dataUnit() const { return duh_; }
         int unknown_count() const { return unknown_count_; }
         double last_send_time() const { return last_send_time_; }
         double last_received_time() const { return last_received_time_; }
         shawn::Processor* creator() const { return creator_; }
         bool refresh_before_send() const { return refresh_before_send_; }
         void dataUnit(const ConstDataUnitHandle& duh) { duh_ = duh; }
         void unknown_count(int u) { unknown_count_ = u; }
         void last_send_time(double l) { last_send_time_ = l; }
         void last_received_time(double l) { last_received_time_ = l; }
         void creator(shawn::Processor* c) { creator_ = c; };
         void refresh_before_send(bool r) { refresh_before_send_ = r; }
         void refresh() {
            if (refresh_before_send_){
               autocast::AutoCastApplication* ac_app = NULL;
               if (creator_) ac_app = dynamic_cast<AutoCastApplication*>(creator_);
               if(ac_app) duh_ = ac_app->refresh(duh_);
            }
         }

      private:
         ConstDataUnitHandle duh_;
         int unknown_count_;
         double last_send_time_;
         double last_received_time_;
         shawn::Processor* creator_;
         bool refresh_before_send_;
      };

      typedef std::map<int,LocalDataUnit*> DataUnitsMap;
      DataUnitsMap complete_DataUnits_;
      DataUnitsMap stale_DataUnits_;
	  bool use_stale_hashes_;

      /// Fetching startup parameters
      void fetch_parameters();
      void clean_up() throw();
      double uniform_random(double lb = 0.0, double ub = 1.0, bool lbi = true, bool ubi = true);
      void local_update() throw();
      autocast::AutoCastProcessor::LocalDataUnit* handle_DataUnit(const ConstDataUnitHandle&, bool& is_new, bool logging) throw();
      void update() throw();
      void send_update_packet(const double) throw();
      double get_update_time() throw();
      LocalDataUnit* most_urgent_DataUnit() throw();


      /// This is the time it might take to start up a node
      double max_startup_time_;
      /// These are the time boundaries a node will wait tell it sends a update packet
      double min_update_time_;
      double max_update_time_;
      double dataUnit_max_live_time_;
      double max_update_packet_size_;
      double max_update_data_units_;

      /// Initialized with 1
      double update_time_;

      std::multiset<unsigned int> received_messages_ids_total_;
      std::set<unsigned int> unknown_DataUnit_ids_; // For request
      shawn::EventScheduler::EventHandle update_timer_;
      shawn::EventScheduler::EventHandle answer_timer_;
      shawn::EventScheduler::EventHandle request_timer_;
      shawn::EventScheduler::EventHandle flood_timer_;

      /// Statistics
      unsigned int packets_sent_total_;
      unsigned int bytes_sent_total_;
      unsigned int dataUnits_sent_total_;
      unsigned int dataUnits_bytes_sent_total_;
      std::set<unsigned int> received_DataUnit_ids_total_;
      unsigned int received_DataUnits_total_;
      int msgCountBeacon_;
      int msgCountAnswer_;
      int msgCountFlood_;
      int msgCountRequest_;
      int msgCountAnswer_only_;
      int msgCountFlood_only_;
      int msgCountRequest_only_;

      double activeTime_;

	  std::vector<int> active_times_;
	  std::vector<int>::const_iterator active_times_it_;

      /// Neighborhood stuff
      autocast::Neighborhood neighborhood_;

      int max_iterations_;
      bool booted_;
      static int uid_counter_;

      bool logging_;
      /*unsigned int neighbors_count_;
      unsigned int real_neighbors_count_;
      double velocity_count_;
      double update_time_count_;*/

   };


}

#endif
#endif
/*-----------------------------------------------------------------------
* Source  $HeadURL$
* Version $LastChangedRevision$
* Date    $LastChangedDate$
*-----------------------------------------------------------------------*/

