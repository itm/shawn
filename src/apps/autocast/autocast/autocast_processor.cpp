/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2007 by AutoNomos (www.auto-nomos.de)                **
 ** This part of Shawn is free software; you can redistribute it and/or**
 ** modify it under the terms of the BSD License. Refer to the		   **
 ** shawn-licence.txt file in the root of the Shawn source tree for	   **
 ** further details.												   **
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>				   **
 ** \author Torsten Teubler <teubler@itm.uni-luebeck.de>			   **
 **                                                                    **
 ************************************************************************/

#include "autocast_processor.h"
#ifdef ENABLE_AUTOCAST

#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <sstream>
#include <fstream>
#include <string>
#include "sys/node.h"
#include "sys/vec.h"
#include "sys/util/string_conv.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/transmission_model.h"
#include "apps/autocast/autocast/autocast_processor.h"
#include "apps/autocast/autocast/autocast_message.h"
#include "apps/autocast/autocast_task/autocast_task.h"

using namespace std;
using namespace shawn;

namespace autocast
{

   int autocast::AutoCastProcessor::uid_counter_ = 0;

   AutoCastProcessor::
	   AutoCastProcessor() : use_stale_hashes_(false),
							 max_startup_time_(AUTOCAST_DEFAULT_STARTUP_TIME),
							 min_update_time_(AUTOCAST_DEFAULT_MIN_UPDATE_TIME),
							 max_update_time_(AUTOCAST_DEFAULT_MAX_UPDATE_TIME),
							 dataUnit_max_live_time_(AUTOCAST_DEFAULT_DATAUNIT_LIVETIME),
							 max_update_packet_size_(AUTOCAST_MAX_PACKET_LENGTH),
							 max_update_data_units_(AUTOCAST_DEFAULT_MAX_DATAUNITS_PER_PACKET),
							 update_time_(1),
							 update_timer_(NULL),
							 answer_timer_(NULL),
							 request_timer_(NULL),
							 /*NEW*/flood_timer_(NULL),
							 packets_sent_total_(0),
							 bytes_sent_total_(0),
							 dataUnits_sent_total_(0),
							 dataUnits_bytes_sent_total_(0),
							 received_DataUnits_total_(0),
                        msgCountBeacon_(0),
                        msgCountAnswer_(0),
                        msgCountFlood_(0),
                        msgCountRequest_(0),
                        msgCountAnswer_only_(0),
                        msgCountFlood_only_(0),
                        msgCountRequest_only_(0),
							 activeTime_(0.0),
                             neighborhood_(0.0),
							 logging_(true)
							 /*FOR DEBUG PURPOSES*/
							 /*neighbors_count_(0),
							 real_neighbors_count_(0),
							 velocity_count_(0),
							 update_time_count_(0)*/

   {}
	// ----------------------------------------------------------------------
	AutoCastProcessor::
		~AutoCastProcessor()
	{
		clean_up();
	}
	// ----------------------------------------------------------------------
	void
		AutoCastProcessor::
	   boot( void )
		throw()
	{
		neighborhood_.set_owner(owner());
		fetch_parameters();
		double now = owner().world().scheduler().current_time();

		double lb2 = owner().world().simulation_controller().environment().optional_double_param("_lb2__boot",0.9);
		double ub1 = owner().world().simulation_controller().environment().optional_double_param("_ub1__boot",1.1);
		update_timer_ = owner_w().world_w().scheduler_w().new_event(*this,now+fabs(max_startup_time_) * uniform_random(lb2,ub1),NULL);
		max_iterations_ = owner().world().simulation_controller().environment().required_int_param("max_iterations");
		logging_ = owner().world().simulation_controller().environment().optional_bool_param("autocast_logging", logging_);
		use_stale_hashes_ = owner().world().simulation_controller().environment().optional_bool_param("autocast_stale_hashes", use_stale_hashes_);

		std::string activity_file = owner().world().simulation_controller().environment().optional_string_param("activity_file", "");
		if (activity_file != "")
		{
			std::ifstream * inputfile = new ifstream(activity_file.c_str());
			if (inputfile->is_open())
			{
				std::ostringstream oss;
				oss << "$g(" << owner().id() << ")";
				string id(oss.str());
				std::cerr << "\nSearching for " << id << " in " << activity_file << " ";
				char buffer[250];
				std::string line;
				while (! inputfile->eof())
				{
					inputfile->getline(buffer,250);
					line = string(buffer);
					if (line.find(id) != std::string::npos)
					{
						//std::cerr << line << " " << "\n";
						line.assign(line,line.find("$ns_ at ")+8,std::string::npos);
						//std::cerr << line << "\n";
						line.assign(line,0,line.find("."));
						//std::cerr << line << "\n";
						int time = shawn::conv_string_to_int(line);
						active_times_.push_back(time);

						std::cerr << " " << time << " ";
					}
				}
				inputfile->close();

				// If an activity file is given, we let the processors sleep until the first given time
				this->set_state(Processor::Sleeping);
				active_times_it_ = active_times_.begin();
			}
		}
	}
   // ----------------------------------------------------------------------
   bool
      AutoCastProcessor::
      get_active_state_at( double time )
      const
      throw()
   {
      if (active_times_.size() == 0) return true;
      bool toggle = false;
      for (std::vector<int>::const_iterator it=active_times_.begin(); it != active_times_.end(); ++it)
      {
         if ((*it) < time) toggle = !toggle; else break;
      }
      return toggle;
   }
	// ----------------------------------------------------------------------
   bool
      AutoCastProcessor::
      process_message( const ConstMessageHandle& mh )
      throw()
   {
	   if (state() != Processor::Active) return false;

	   const autocast::AutoCastMessage * acm = dynamic_cast<const autocast::AutoCastMessage*>(mh.get());
	   if (!acm){
		   std::cerr << "Cast to AutoCastMessage failed!" << std::endl;
		   return false;
	   }
	   if (acm->last_hop_addr() == owner().id()){
		   /// Prevent processor from receiving his sent messages
		   return false;
	   }

      if (logging_)
      {
	      if(received_messages_ids_total_.find(acm->uid()) != received_messages_ids_total_.end()){
		      /// For debug purposes only
		      std::cerr << "Message with id " << acm->uid() << " already received!" << std::endl;
		      return false;
	      }
	      /// Now receive the packet!
	      received_messages_ids_total_.insert(acm->uid());
      }
	   double now = owner().world().scheduler().current_time();
	   /// Update the local DataUnits
	   local_update();

	   neighborhood_.append_to_neighborhood( acm->last_hop_addr(),acm->next_update_interval() );

	   if (acm->packet_type() != AUTOCAST_TYPE_UPDATE){
		   std::cerr << "_" << acm->source().id() << "_received UNKNOWN packet @ " << owner().world().simulation_controller().world().scheduler().current_time() << std::endl;
		   std::cerr << "Type is " << acm->packet_type() << " and not " << AUTOCAST_TYPE_UPDATE << std::endl;
		   abort();
	   }

	   /// Two of the neighborhood should flood the information or answer with unknown data
	   int neighborhood_size = neighborhood_.neighborhood_size();
	   int barker = owner().world().simulation_controller().environment().optional_int_param("_barker__process_message",5);
	   bool answer_decision = (neighborhood_size <= barker || uniform_random(0.0,1.0) <= (1.0 * barker / neighborhood_size));

	   /// Get the known ID's from the sender
	   /// received_ids saves the ID's received in this "receiving round"
	   std::set<unsigned int> received_ids(acm->known_DataUnits());
	   /// Get the stale ID's
	   std::set<unsigned int> stale_ids(acm->stale_ids());

	   /// Fetching the complete DataUnits and give it to the applications or
	   /// hold them in the transport layer
	   bool need_to_flood = false;
	   for (std::set<ConstDataUnitHandle>::const_iterator du_it = acm->complete_DataUnits().begin();
		   du_it != acm->complete_DataUnits().end(); du_it++){
			// Add also the ID's of the complete DataUnits
		   received_ids.insert((*du_it)->id());
			if (logging_) received_DataUnit_ids_total_.insert((*du_it)->id());
			received_DataUnits_total_++;

			/// Give to applications
			bool application_responsible = false;
			for (Node::ProcessorList::iterator app_it = owner_w().begin_processors_w(); app_it != owner_w().end_processors_w(); app_it++){
					autocast::AutoCastApplication* ac_app = dynamic_cast<autocast::AutoCastApplication*>((*app_it).get());
					if (ac_app){
						application_responsible = ac_app->receive_DataUnit(*du_it);
					}
			}
			if ( !application_responsible && (*du_it)->distribution_area() &&
				( now < (*du_it)->time() + (*du_it)->max_life_time() ) ){
				if (stale_DataUnits_.find((*du_it)->id()) == stale_DataUnits_.end()){
					/// Set the flag if the DataUnit is new
					bool is_new_DataUnit = false;
               /// Interested in if is a new DataUnit and enable logging (if logging_ is true)
					autocast::AutoCastProcessor::LocalDataUnit * ldu = handle_DataUnit(*du_it,is_new_DataUnit,logging_);
					if (ldu){
						/// DataUnit is now known locally
						unknown_DataUnit_ids_.erase(ldu->dataUnit()->id());
                  if ( unknown_DataUnit_ids_.size() == 0 && request_timer_ )
                  {
                     owner_w().world_w().scheduler_w().delete_event(request_timer_);
                     request_timer_ = NULL;
                  }
                  // New data unit?
					   if (answer_decision && is_new_DataUnit){
						   ldu->unknown_count(ldu->unknown_count() + 1);
						   need_to_flood = true;
                  }
//					}else if(ldu && !is_new_DataUnit){
//						/// else old object or no answer_decision
//						if(ldu->unknown_count() > 0) ldu->unknown_count(ldu->unknown_count() - 1);
					}/// else out of region
				}/// else no one feels responsible
			}
	   }

	   bool need_to_answer = false;
	   if(answer_decision)
      {
		   for (std::map<int,autocast::AutoCastProcessor::LocalDataUnit*>::iterator it = complete_DataUnits_.begin();
			   it != complete_DataUnits_.end(); it++){
			   if ( received_ids.find( it->second->dataUnit()->id() ) == received_ids.end() &&
				    stale_ids.find(it->second->dataUnit()->id()) == stale_ids.end())
            {
				    it->second->unknown_count(it->second->unknown_count() + 1);
				    need_to_answer = true;
			   }
		   }
	   }

      bool need_to_request = false;
      if (answer_decision)
      {
	      /// Mark data units for request
	      for(std::set<unsigned int>::iterator it = received_ids.begin(); it != received_ids.end(); ++it){
		      if( complete_DataUnits_.find(*it) == complete_DataUnits_.end() &&
			      stale_DataUnits_.find(*it) == stale_DataUnits_.end())
            {
			      unknown_DataUnit_ids_.insert(*it);
               need_to_request = true;
		      }
	      }
      }

      // Schedule events
      double delta = owner().world().simulation_controller().environment().optional_double_param("delay_delta",0.03);
      double delta_variance = owner().world().simulation_controller().environment().optional_double_param("delay_delta_variance",0.0025);
      shawn::EventScheduler& es = owner_w().world_w().scheduler_w();

	   if ( (answer_decision && need_to_answer) || answer_timer_ ){
			if ( answer_timer_ )	es.delete_event(answer_timer_);
			answer_timer_ = es.new_event(*this, now + delta + uniform_random(-delta_variance,delta_variance), NULL);
	   }

	   if( (answer_decision && need_to_flood) || flood_timer_ ){
			if ( flood_timer_ ) es.delete_event(flood_timer_);
			flood_timer_ = es.new_event(*this, now + 2 * delta + uniform_random(-delta_variance,delta_variance), NULL);
	   }


	   if ( (answer_decision && need_to_request) || request_timer_ )
      {
		   if ( request_timer_ ) es.delete_event(request_timer_);
	      request_timer_ = es.new_event(*this, now + 3 * delta + uniform_random(-delta_variance,delta_variance), NULL);
      }
	   return true;
   }
   // ----------------------------------------------------------------------
   void
   AutoCastProcessor::
   work( void )
      throw()
   {
	   /*neighbors_count_+=neighborhood_.neighborhood_size();
	   real_neighbors_count_+=owner().get_adjacent_nodes().size();
	   velocity_count_+=owner().movement().velocity().euclidean_norm();
	   update_time_count_+=update_time_;*/

	   if (active_times_.size() > 0)
	   {
		   // If activeTimes were read from file
		   int now = static_cast<int>(owner().world().scheduler().current_time());
		   if (active_times_it_ != active_times_.end() && *active_times_it_ == now)
		   {
			   // it's time o switch the State
			   if (this->state() == Processor::Active)
			   {
				   this->set_state(Processor::Sleeping);
			   } else {
				   this->set_state(Processor::Active);
	            if ( update_timer_ )
	            {
		            owner_w().world_w().scheduler_w().delete_event(update_timer_);
		            update_timer_ = NULL;
	            }
	            update_timer_ = owner_w().world_w().scheduler_w().new_event(*this,now + max_update_time_ * uniform_random(0.0,1.0),NULL);

			   }
			   ++active_times_it_;
		   }
	   }

	   if (logging_ && state() == Processor::Active) activeTime_++;

	   if (logging_ && (activeTime_ > 0) && (simulation_round() == max_iterations_ - 1)){
		    KeeperManagedHandle kmh = owner_w().world_w().simulation_controller_w().simulation_task_keeper_w().find_managed_w("AutoCastTask");
			assert(kmh.is_not_null());
			autocast::AutoCastTask* act = dynamic_cast<autocast::AutoCastTask*>( kmh.get() );
			if (act){
				act->process_sent_statistic(
               &owner(),
               packets_sent_total_,
               bytes_sent_total_,
               dataUnits_sent_total_,
               dataUnits_bytes_sent_total_,
               received_messages_ids_total_.size(),
               received_DataUnits_total_,
               received_DataUnit_ids_total_.size(),
               activeTime_,
               msgCountBeacon_,
               msgCountAnswer_, msgCountAnswer_only_,
               msgCountFlood_, msgCountFlood_only_,
               msgCountRequest_, msgCountRequest_only_
               );
			}

			// And at least
			clean_up();
	   }
   }
   // ----------------------------------------------------------------------
   bool
	   AutoCastProcessor::
	   send_to(const ConstDataUnitHandle& duh, shawn::Processor* creator)
	   throw()
   {
   	  if (state() != Processor::Active) return false;

	   LocalDataUnit* ldu = NULL;
      bool is_new = false;
	   /// Not interested in if is a new DataUnit and disable logging
	   ldu = handle_DataUnit(duh, is_new, false);
	   if (ldu != NULL){
		   ldu->creator(creator);
		   if (creator) ldu->refresh_before_send(true);
		   int unknown_count = owner().world().simulation_controller().environment().optional_int_param("_unknown_count__send",10);
		   ldu->unknown_count(unknown_count);

		   update();
		   return true;
	   }
	   return false;
   }
   // ----------------------------------------------------------------------
   void
		AutoCastProcessor::
		timeout( shawn::EventScheduler& es, shawn::EventScheduler::EventHandle eh,
					double time, shawn::EventScheduler::EventTagHandle& eth)
		throw()
   {
	   if(eh == update_timer_){
         msgCountBeacon_++;
		   // Set the pointer NULL
		   update_timer_ = NULL;
	   }
	   if(eh == answer_timer_){
         msgCountAnswer_only_++;
         msgCountAnswer_++;
		   // Set the pointer NULL
		   answer_timer_ = NULL;
	   }
	   if(eh == flood_timer_){
         msgCountFlood_only_++;
         msgCountFlood_++;
		    // Set the pointer NULL
			flood_timer_ = NULL;
	   }
	   if(eh == request_timer_){
         msgCountRequest_only_++;
         msgCountRequest_++;
		   // Set the pointer NULL
		   request_timer_ = NULL;
	   }

      if (answer_timer_) msgCountAnswer_++;
      if (flood_timer_) msgCountFlood_++;
      if (request_timer_) msgCountRequest_++;

	   update();
   }
   // ----------------------------------------------------------------------
   void
	   AutoCastProcessor::
	   update()
	   throw()
   {

	   if( answer_timer_ ){
		   owner_w().world_w().scheduler_w().delete_event(answer_timer_);
		   answer_timer_ = NULL;
	   }

	   if( flood_timer_ ){
		   owner_w().world_w().scheduler_w().delete_event(flood_timer_);
		   flood_timer_ = NULL;
	   }

	   if ( update_timer_ )
	   {
		   owner_w().world_w().scheduler_w().delete_event(update_timer_);
		   update_timer_ = NULL;
	   }

	   if ( request_timer_ ){
			owner_w().world_w().scheduler_w().delete_event(request_timer_);
			request_timer_ = NULL;
	   }


	   double now = owner().world().simulation_controller().world().scheduler().current_time();
	   double lb = owner().world().simulation_controller().environment().optional_double_param("_lb__update",0.95);
	   double ub = owner().world().simulation_controller().environment().optional_double_param("_ub__update",1.05);
	   double time_interval = get_update_time() * uniform_random(lb,ub);

      if (state() == Processor::Active)
      {
         local_update();
         unknown_DataUnit_ids_.clear();
         send_update_packet(time_interval);
      }

	   for (DataUnitsMap::iterator it = complete_DataUnits_.begin(); it != complete_DataUnits_.end(); it++){
		   it->second->unknown_count(0);
	   }

      if (state() == Processor::Active) update_timer_ = owner_w().world_w().scheduler_w().new_event(*this,now + time_interval,NULL);
   }
   // ----------------------------------------------------------------------
   double
		AutoCastProcessor::
		uniform_random(double lb, double ub, bool lbi, bool ubi)
   {
	    shawn::UniformRandomVariable urv;
		urv.set_lower_bound(lb);
		urv.set_upper_bound(ub);
		urv.set_lower_bound_inclusive(lbi);
		urv.set_upper_bound_inclusive(ubi);
		urv.init();
		return urv;
   }

   // ----------------------------------------------------------------------
	void
		AutoCastProcessor::
		local_update()
	throw()
    {
		double now = owner().world().scheduler().current_time();

		/// Throw away old DataUnits (Old behavior)
		/*for (DataUnitsMap::iterator it = complete_DataUnits_.begin(); it != complete_DataUnits_.end(); it++){
			if ( (it->second->dataUnit()->max_life_time() >= 0) &&
				(now >= it->second->dataUnit()->time() + it->second->dataUnit()->max_life_time()) ){
					delete it->second;
					complete_DataUnits_.erase(it);
			}
		}*/

		for (DataUnitsMap::iterator it = complete_DataUnits_.begin(); it != complete_DataUnits_.end();){
			if (((it->second->dataUnit()->max_life_time() >= 0) &&
				(now >= it->second->dataUnit()->time() + it->second->dataUnit()->max_life_time())) ||
				(it->second->dataUnit()->distribution_area() &&
				!it->second->dataUnit()->distribution_area()->is_inside(owner().real_position().x(),owner().real_position().y()) )){
					if (use_stale_hashes_) stale_DataUnits_.insert(std::make_pair(it->first,it->second));
					complete_DataUnits_.erase(it++);
			}else{
				++it;
			}
		}

		for (DataUnitsMap::iterator it = stale_DataUnits_.begin(); it != stale_DataUnits_.end(); ){
			if (((it->second->dataUnit()->max_life_time() >= 0) &&
				(now >= it->second->dataUnit()->time() + it->second->dataUnit()->max_life_time()+5)) ||
				(it->second->dataUnit()->distribution_area() &&
				!it->second->dataUnit()->distribution_area()->is_inside(
				owner().real_position().x(),
				owner().real_position().y(),
				owner().world().simulation_controller().environment().required_double_param("range")*2) )){
					delete it->second;
					stale_DataUnits_.erase(it++);
			}else{
				++it;
			}
		}
	}
	// ----------------------------------------------------------------------
	/// Retruns NULL if DataUnit is out of region
	autocast::AutoCastProcessor::LocalDataUnit*
		AutoCastProcessor::
		handle_DataUnit(const ConstDataUnitHandle& du, bool& is_new, bool logging)
		throw()
	{
		double now = owner().world().scheduler().current_time();
		autocast::AutoCastProcessor::LocalDataUnit* ldu = NULL;
		// First we assume an old DataUnit
		is_new = false;
		// DataUnit out of region returning NULL
		if ( !(du->distribution_area() && du->distribution_area()->is_inside(owner().real_position().x(), owner().real_position().y())) ){
			return NULL;
		}
		DataUnitsMap::const_iterator old_du = complete_DataUnits_.find(du->id());
		if ( old_du == complete_DataUnits_.end() ){
			// Completely new DataUnit
			ldu = new LocalDataUnit(du);
			// The DataUnit is flaged as new
			is_new = true;
			ldu->last_received_time(now);
			complete_DataUnits_.insert(std::pair<int,LocalDataUnit*>(ldu->dataUnit()->id(),ldu));
			if (logging){
				// Do the logging of completely new DataUnit
				KeeperManagedHandle kmh = owner_w().world_w().simulation_controller_w().simulation_task_keeper_w().find_managed_w("AutoCastTask");
				assert(kmh.is_not_null());
				AutoCastTask* act = dynamic_cast<AutoCastTask*>( kmh.get() );
				assert(act != NULL);
				act->process_latencies(du,&owner(),owner().world().simulation_controller());
			}
		}else{
			/// Old DataUnit
			/// With newer timestamp?
			if ( old_du->second->dataUnit()->time() < du->time() ){
				if (logging){
					//Do the logging of old DataUnit with newer timestamp
					KeeperManagedHandle kmh = owner_w().world_w().simulation_controller_w().simulation_task_keeper_w().find_managed_w("AutoCastTask");
					assert(kmh.is_not_null());
						autocast::AutoCastTask* act = dynamic_cast<autocast::AutoCastTask*>( kmh.get() );
					assert(act != NULL);
						act->process_latencies(du,&owner(),owner().world().simulation_controller());
				}

				/// If the timestap is newer replace the DataUnit in the LocalDataUnit
				old_du->second->dataUnit(du);
				ldu = old_du->second;
				/// The DataUnit is flaged as new
				is_new = true;
			}
			/// DataUnit is really old
			/// Set the last_received_time of the LocalDataUnit
			old_du->second->last_received_time(now);
			ldu = old_du->second;
		}
		return ldu;
	}
	// ----------------------------------------------------------------------
	void
		AutoCastProcessor::
		send_update_packet(const double update_time)
		throw()
	{
		assert(stale_DataUnits_.size() == 0 || use_stale_hashes_ == false);

		double now = owner().world().scheduler().current_time();
		uid_counter_++;
		AutoCastMessage * acm = new AutoCastMessage(uid_counter_,AUTOCAST_TYPE_UPDATE,update_time,owner().id(),owner().real_position().x(),owner().real_position().y());
		autocast::AutoCastProcessor::LocalDataUnit * ldu = most_urgent_DataUnit();
		/// Append complete DataUnits
		int dataUnits_bytes = 0;
		// NEW: The stale DataUnits
		int id_space = 4 * complete_DataUnits_.size() + 4 * stale_DataUnits_.size();
		while( ldu && ( acm->complete_DataUnits().size() <= max_update_data_units_ ) &&
			 ( ( acm->size() + id_space + ldu->dataUnit()->size() ) <= max_update_packet_size_ ) ){
			ldu->refresh();
			ldu->last_send_time(now);
			ldu->unknown_count(0);
			/// Detaching DataUnit
			DataUnit * du = new DataUnit( *(ldu->dataUnit()) );
			/// Incrementing DataUnits hop count
			du->hop_count( ldu->dataUnit()->hop_count() + 1 );
			dataUnits_bytes += du->size();
			// Assertion
			if (now > du->time() + du->max_life_time()){
				std::cerr << "object " << ldu->dataUnit()->id() << " is too old. Stop!" << std::endl;
				abort();
			}

			acm->insert_complete_DataUnit(du);
			id_space -= 4;
			ldu = most_urgent_DataUnit();
		}

		/// Append list with also known objects
		for (DataUnitsMap::iterator it = complete_DataUnits_.begin(); it != complete_DataUnits_.end(); it++){
  		    if (it->second->last_send_time() < now) acm->insert_id(it->second->dataUnit()->id());
		}

		/// Stale DataUnits
		for (DataUnitsMap::const_iterator it = stale_DataUnits_.begin();
			it != stale_DataUnits_.end(); ++it){
				acm->insert_stale_id(it->second->dataUnit()->id());
		}

		/// Assertion
		if (acm->size() > max_update_packet_size_){
			assert(max_update_packet_size_ == AUTOCAST_MAX_PACKET_LENGTH);
			std::cerr << "Time: " << now << "  Node: " << owner().id()
            << " wants to send " << acm->size()
            << " byte, but only " << max_update_packet_size_ << " are allowed!";
			exit(1);
		}

		/// Statistics
		packets_sent_total_++;
		bytes_sent_total_ += acm->size();
		dataUnits_sent_total_ += acm->complete_DataUnits().size();
		dataUnits_bytes_sent_total_ += dataUnits_bytes;

		owner_w().send(acm);
	}
	// ----------------------------------------------------------------------
	double
		AutoCastProcessor::
		get_update_time()
		throw()
	{
		if(min_update_time_ == max_update_time_) return max_update_time_;

		//int neighborhood_size = neighborhood_.neighborhood_size();

		double range = owner().world().simulation_controller().environment().required_double_param("range");
		//double standard_neighbors = owner().world().simulation_controller().environment().optional_double_param("_standard_neighbors__get_update_time",4.0);
		double alpha = owner().world().simulation_controller().environment().optional_double_param("_alpha__get_update_time",1.0);
		double delta_r = owner().world().simulation_controller().environment().optional_double_param("_delta_r__get_update_time",0.1);
		double add_to_velocity = owner().world().simulation_controller().environment().optional_double_param("_add_to_velocity__get_update_time",0.01);

      double v = owner().movement().velocity().euclidean_norm();

		//update_time_ = (1 - alpha) * update_time_ + alpha * (neighborhood_size + 1)*(neighborhood_size + 1)/((double)standard_neighbors + 1) * (range * delta_r)/(owner().movement().velocity().euclidean_norm() + add_to_velocity);
		update_time_ = (1-alpha) * update_time_ + alpha * (range*delta_r)/(v + add_to_velocity);

		if (update_time_ < min_update_time_) update_time_ = min_update_time_;
		if (update_time_ > max_update_time_) update_time_ = max_update_time_;
		return update_time_;
	}
	// ----------------------------------------------------------------------
	void
		AutoCastProcessor::
		fetch_parameters()
	{
		max_startup_time_ = owner().world().simulation_controller().environment().optional_double_param("max_startup_time",AUTOCAST_DEFAULT_STARTUP_TIME);
		min_update_time_ = owner().world().simulation_controller().environment().optional_double_param("min_update_time",AUTOCAST_DEFAULT_MIN_UPDATE_TIME);
		max_update_time_ = owner().world().simulation_controller().environment().optional_double_param("max_update_time",AUTOCAST_DEFAULT_MAX_UPDATE_TIME);
		dataUnit_max_live_time_ = owner().world().simulation_controller().environment().optional_double_param("dataUnit_max_live_time",AUTOCAST_DEFAULT_DATAUNIT_LIVETIME);
		max_update_packet_size_ = owner().world().simulation_controller().environment().optional_int_param("max_update_packet_size",AUTOCAST_MAX_PACKET_LENGTH);
		max_update_data_units_ = owner().world().simulation_controller().environment().optional_double_param("max_update_data_units",AUTOCAST_DEFAULT_MAX_DATAUNITS_PER_PACKET);
	}
	// ----------------------------------------------------------------------
	autocast::AutoCastProcessor::LocalDataUnit *
		AutoCastProcessor::
		most_urgent_DataUnit()
		throw()
	{
		double now = owner().world().scheduler().current_time();

		std::vector<LocalDataUnit*> tmp_objects;

		for (DataUnitsMap::const_iterator it = complete_DataUnits_.begin(); it != complete_DataUnits_.end(); ++it){
			if (it->second->last_send_time() < now && it->second->unknown_count() > 0){
				for (int i = 0; i < it->second->unknown_count(); i++){
					tmp_objects.push_back(it->second);
				}
			}
		}
		if (tmp_objects.size() == 0) return NULL;
		int idx = static_cast<int>(uniform_random(0.0,tmp_objects.size(),true,false));
		return tmp_objects.at(idx);
	}
	// ----------------------------------------------------------------------
	bool
		AutoCastProcessor::
		booted()
		const
		throw()
	{
		return booted_;
	}
	// ----------------------------------------------------------------------
	void
		AutoCastProcessor::
		clean_up()
		throw()
	{
		/// Cleaning up!
		for(DataUnitsMap::iterator it = complete_DataUnits_.begin(); it != complete_DataUnits_.end(); ++it){
			if (it->second) delete it->second;
		}
        complete_DataUnits_.clear();

		for(DataUnitsMap::iterator it = stale_DataUnits_.begin(); it != stale_DataUnits_.end(); ++it){
			if (it->second) delete it->second;
		}
        stale_DataUnits_.clear();
		complete_DataUnits_.clear();
		stale_DataUnits_.clear();
		received_messages_ids_total_.clear();
		received_DataUnit_ids_total_.clear();
		//if ( owner().is_special_node() ) owner_w().world_w().scheduler_w().clear();
	}
	// ----------------------------------------------------------------------
	void
		AutoCastProcessor::
		set_state(const Processor::ProcessorState& state)
		throw()
	{
		if (state == Inactive || state == Sleeping){
			if ( flood_timer_ ){
				owner_w().world_w().scheduler_w().delete_event(flood_timer_);
				flood_timer_ = NULL;
			}
			if ( update_timer_ ){
			   owner_w().world_w().scheduler_w().delete_event(update_timer_);
			   update_timer_ = NULL;
			}
			if ( answer_timer_ ){
				owner_w().world_w().scheduler_w().delete_event(answer_timer_);
				answer_timer_ = NULL;
			}
			if ( request_timer_ ){
				owner_w().world_w().scheduler_w().delete_event(request_timer_);
				request_timer_ = NULL;
			}
		}
		Processor::set_state(state);
	}
}
#endif

/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/
