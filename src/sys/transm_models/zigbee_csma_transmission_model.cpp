/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/transm_models/zigbee_csma_transmission_model.h"
#include "sys/edge_model.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <algorithm>

namespace shawn
{
	ZigbeeCsmaTransmissionModel::
		ZigbeeCsmaTransmissionModel() :
			received_(0), dropped_(0), deliver_num_(0), packet_failure_(0), average_delay_(0), 
				jitter_(0), max_backoff_num_(4), min_backoff_exponent_(0), max_backoff_exponent_(0),
				backoff_period_(0.5), slot_(0), contention_window_(2)
	{}

	// ----------------------------------------------------------------------
	ZigbeeCsmaTransmissionModel::
		~ZigbeeCsmaTransmissionModel()
	{}

	// ----------------------------------------------------------------------
	void ZigbeeCsmaTransmissionModel::init()
		throw()
	{
		TransmissionModel::init();

		const SimulationEnvironment& se = world().simulation_controller().environment();
        max_backoff_num_ = se.optional_int_param("max_backoff_num", 4);
		min_backoff_exponent_ = se.optional_int_param( "min_backoff_exponent", 0 );
		max_backoff_exponent_ = se.optional_int_param( "max_backoff_exponent", 0 );
		backoff_period_ = se.optional_double_param( "backoff_period", 0.5 );

		slot_ = se.optional_double_param( "slot", 0 );
		contention_window_ = se.optional_int_param( "contention_window", 2);

//		srand( 10 );

        std::cout << "ZigBee CSMA: Initialised. ";
		if( slot_>0 )
			std::cout << "It is a slotted CSMA, slot is " << slot_ 
			<< ", contention window length is " << contention_window_ << ". ";
		else
			std::cout << "It is a unslotted CSMA. ";
		std::cout << "backoff period=" << backoff_period_ << ", max backoff number=" << max_backoff_num_ << ", " 
			<< min_backoff_exponent_ << "<=backoff_exponent<=" << max_backoff_exponent_ << ". " << std::endl;
	}

	// ----------------------------------------------------------------------
	void ZigbeeCsmaTransmissionModel::reset()
		throw()
	{
		average_delay_ = deliver_num_>0? average_delay_/deliver_num_ : 0;
		jitter_ = deliver_num_>1? jitter_/(deliver_num_-1) :0;

		std::cout << "ZigBee CSMA: "<<  
            received_ <<" msgs. to be sent. "<< dropped_ << " msgs. dropped." 
			<< packet_failure_ << " packets fail to reach the destination." << " And average delay is "
            << average_delay_ << ", jitter is "<< jitter_ << std::endl;

		TransmissionModel::reset();
		received_ = 0;
		dropped_ = 0;
		deliver_num_= 0;
		packet_failure_ = 0;
		average_delay_ = 0;
		jitter_ = 0;
		max_backoff_num_ = 4;
		min_backoff_exponent_ = max_backoff_exponent_ = 0;
		backoff_period_ = 0.5;
		slot_ = 0;
		contention_window_ = 2;
	}

	// ----------------------------------------------------------------------
    bool
        ZigbeeCsmaTransmissionModel::
        supports_mobility( void )
        const throw(std::logic_error)
    {
        return world().edge_model().supports_mobility();
    }

	// ----------------------------------------------------------------------
	void ZigbeeCsmaTransmissionModel::send_message( MessageInfo &mi )
		throw()
	{
		received_++;
		csma_msg* new_msg = new csma_msg(&mi, min_backoff_exponent_, contention_window_ );
		find_destinations( new_msg );
		// initial backoff
		std::cout << "Initial backoff of msg from node" << new_msg->pmi->src_->id() << std::endl
				<< "\tsend time: " << new_msg->pmi->time_;
		random_backoff( new_msg );
		std::cout << ", deliver time after backoff: " << new_msg->deliver_time << std::endl;

		aired_messages_.push_back( new_msg );
	}

	// ----------------------------------------------------------------------
	void ZigbeeCsmaTransmissionModel::deliver_messages()
		throw()
	{
		if( aired_messages_.empty() )
			return;

		sort( aired_messages_.begin(),aired_messages_.end(), msg_pre() );
		int cur_round = (aired_messages_.empty())? 0 : (int)aired_messages_.front()->deliver_time;

		//// initial backoff
		//for(std::vector<csma_msg*>::iterator it=aired_messages_.begin(); it!=aired_messages_.end(); it++ )
		//{
		//	std::cout << "Initial backoff of msg from node" << (*it)->pmi->src_->id() << std::endl
		//		<< "\tsend time: " << (*it)->pmi->time_;
		//	random_backoff( *it );
		//	std::cout << ", deliver time after backoff: " << (*it)->deliver_time << std::endl;
		//}
		
		std::vector<csma_msg*> cur_round_msg;
		
		while( !aired_messages_.empty() && cur_round<this->world().simulation_round() ){
			
			if( slot_>0 )
				arrange_one_round_slotted( cur_round, cur_round_msg );
			else
				arrange_one_round_unslotted( cur_round, cur_round_msg );
			for(std::vector<csma_msg*>::iterator cur_it = cur_round_msg.begin();
				cur_it!=cur_round_msg.end(); cur_it++ )
			{
				deliver_one_message( *cur_it );
				deliver_num_++;
				double cur_delay = (*cur_it)->deliver_time-(*cur_it)->pmi->time_;
				average_delay_ += cur_delay;

				if(deliver_num_>1)
					jitter_ += (cur_delay>last_delay_)? cur_delay-last_delay_ : last_delay_-cur_delay;
				
				last_delay_ = cur_delay;
				delete (*cur_it);
			}
			cur_round++;
		}

		//average_delay_ = deliver_num_>0? average_delay_/deliver_num_ : 0;
		//jitter_ = deliver_num_>1? jitter_/(deliver_num_-1) :0;
	}

	// ----------------------------------------------------------------------
	void ZigbeeCsmaTransmissionModel::
		random_backoff( csma_msg* msg)
		throw()
	{
		int random_backoff = rand()%(int)pow(2.0,msg->BE);
		std::cout << "\t" << msg->pmi->src_->id() << "get backoff " << random_backoff;
		//locate the backoff boundry
		if(slot_>0)
			msg->deliver_time = (int)(msg->deliver_time/slot_)*slot_==msg->deliver_time? 
				msg->deliver_time : ((int)(msg->deliver_time/slot_)+1)*slot_;

		msg->deliver_time += (backoff_period_*random_backoff);
	}

	// ----------------------------------------------------------------------
	void ZigbeeCsmaTransmissionModel::
		arrange_one_round_slotted( int round, std::vector<csma_msg*>& cur_round_msg )
		throw()
	{
		sort( aired_messages_.begin(),aired_messages_.end(), msg_pre() );
		
		///
		std::cout << "msgs. after sort\n";
		for(std::vector<csma_msg*>::iterator jj=aired_messages_.begin(); jj!=aired_messages_.end(); jj++)
		{
			std::cout << "\tmsg from node " << (*jj)->pmi->src_->id() << std::endl;
		}
		////

		cur_round_msg.clear();

		std::cout << "arrange round " << round << std::endl;

		for(std::vector<csma_msg*>::iterator it = aired_messages_.begin(); 
			it!=aired_messages_.end()&&(int)(*it)->deliver_time==round; )
		{
			std::cout << "arrange msg. from node " << (*it)->pmi->src_->id() << std::endl;
			(*it)->deliver_time = get_backoff_boundary( slot_, (*it)->deliver_time );
			std::vector<csma_msg*>::iterator start_it;

			for(start_it=cur_round_msg.begin(); start_it!=cur_round_msg.end(); start_it++)
			{
				if( need_delay((*it), (*start_it)) )
				{
					// collision
					if( (*it)->CW==1 && (*it)->deliver_time==(*start_it)->deliver_time )
					{
						std::cout << "collision: node " << (*it)->pmi->src_->id() << " and " 
								<< (*start_it)->pmi->src_->id() << std::endl;
						remove_collided_destinations( (*it), (*start_it) );
						cur_round_msg.insert( start_it+1, (*it) );
						it = aired_messages_.erase( it );
					}
					// delay or drop
					else if( (*it)->CW>1 && (*it)->deliver_time==(*start_it)->deliver_time )
						continue;
					else
					{
						(*it)->CW = contention_window_;
						(*it)->NB += 1;
						(*it)->BE = std::min<int>( (*it)->BE+1, max_backoff_exponent_ );

						// Drop the msg. if it cannot occupy the channel for too long, this msg. has never been delivered
						if( (*it)->NB > max_backoff_num_ )
						{
							std::cout << "msg. from node " << (*it)->pmi->src_->id() 
									<< " is dropped for waiting too long." << std::endl;
							dropped_++;
							for(std::vector<csma_msg::msg_destination*>::iterator dest_it=(*it)->destinations_.begin();
								dest_it!=(*it)->destinations_.end();
								dest_it++)
							{
								if((*dest_it)->valid_)
									packet_failure_++;
							}
							it = aired_messages_.erase( it );
						}
						// Get a new congestion backoff
						else
						{
							std::cout << "msg. from node " << (*it)->pmi->src_->id() << " need delay." << std::endl;
							std::cout << "\told deliver time: " << (*it)->deliver_time;
							random_backoff( *it );
								
							std::cout << "\tnew deliver time=" << (*it)->deliver_time <<std::endl;

							csma_msg* tmp_msg = *it;
							aired_messages_.erase( it );
							std::vector<csma_msg*>::iterator ii = find_if( aired_messages_.begin(), 
								aired_messages_.end(), later_than(tmp_msg->deliver_time) );
							aired_messages_.insert( ii, tmp_msg );
							it = aired_messages_.begin();

							for(ii=aired_messages_.begin();	ii!=aired_messages_.end(); ii++)
							{
								std::cout << "\tmsg from node " << (*ii)->pmi->src_->id() << std::endl;
							}
						}
					}
					
					break;
				}
			}
			// channel idle
			if( start_it==cur_round_msg.end() )
			{
				if( --(*it)->CW == 0 )
				{
					std::cout << "msg. from node " << (*it)->pmi->src_->id() << " move to cur_round_msg.\n";
					cur_round_msg.push_back( *it );
					it = aired_messages_.erase( it );
				}
				else
				{
					(*it)->deliver_time = ((int)((*it)->deliver_time/slot_)+1)*slot_;
					csma_msg* tmp_msg = *it;
					////
					std::cout << (*it)->pmi->src_->id() << " has a new deliver time of " 
						<< (*it)->deliver_time << " after CW--.\n";
					/////
					aired_messages_.erase( it );
					std::vector<csma_msg*>::iterator ii = find_if( aired_messages_.begin(), 
						aired_messages_.end(), later_than(tmp_msg->deliver_time) );
					aired_messages_.insert( ii, tmp_msg );
					it = aired_messages_.begin();
					
				}
			}
		}
	}

	// ----------------------------------------------------------------------
	double ZigbeeCsmaTransmissionModel::
		get_backoff_boundary( double slot, double deliver_time )
	{
		return ((int)(deliver_time/slot)*slot==deliver_time? deliver_time : ((int)(deliver_time/slot)+1)*slot);
	}

	// ----------------------------------------------------------------------
	void
		ZigbeeCsmaTransmissionModel::
		arrange_one_round_unslotted( int round, std::vector<csma_msg*>& cur_round_msg )
		throw()
	{
		sort( aired_messages_.begin(),aired_messages_.end(), msg_pre() );
		cur_round_msg.clear();

		std::cout << "arrange round " << round << std::endl;

//		std::vector<csma_msg*>::iterator it=find_if(aired_messages_.begin(), aired_messages_.end(), same_round(round));

		std::vector<csma_msg*>::iterator it = aired_messages_.begin();
		bool first_msg=true;
		while(  it!=aired_messages_.end() && (int)(*it)->deliver_time==round )
		{
			if( first_msg )
			{
				std::cout << "The first msg of this round from node " << (*it)->pmi->src_->id() << std::endl;
				cur_round_msg.push_back( *it );
				it = aired_messages_.erase( it );
				first_msg = false;
			}
			else
			{
				std::cout << "arrange this msg from " << (*it)->pmi->src_->id() << std::endl;
				std::vector<csma_msg*>::iterator start_it;
				 
				for(start_it=cur_round_msg.begin(); start_it!=cur_round_msg.end(); start_it++)
				{
					if( need_delay( (*it), (*start_it) ) )
					{
						if( (*it)->deliver_time==(*start_it)->deliver_time )
						{
							// Collision occurs	
							std::cout << "collision: node " << (*it)->pmi->src_->id() << " and " 
								<< (*start_it)->pmi->src_->id() << std::endl;
							remove_collided_destinations( (*it), (*start_it) );

							continue;
							//cur_round_msg.insert( start_it+1, *it );
							//it = aired_messages_.erase(it);
						}
						else
						{
							std::cout << "msg. from node " << (*it)->pmi->src_->id() << " need delay." << std::endl;
							// Delay
							(*it)->NB += 1;
							(*it)->BE = std::min( (*it)->BE+1, max_backoff_exponent_ );
							
							// Drop the msg. if it cannot occupy the channel for too long, this msg. has never been delivered
							if( (*it)->NB > max_backoff_num_ )
							{
								std::cout << "msg. from node " << (*it)->pmi->src_->id() 
									<< " is dropped for waiting too long." << std::endl;
								dropped_++;
								for(std::vector<csma_msg::msg_destination*>::iterator dest_it=(*it)->destinations_.begin();
									dest_it!=(*it)->destinations_.end();
									dest_it++)
								{
									if((*dest_it)->valid_)
										packet_failure_++;
								}
								it = aired_messages_.erase( it );
							}
							else
							{
								std::cout << "old deliver time: " << (*it)->deliver_time;
								random_backoff( *it );
								
								std::cout << "\tnew deliver time=" << (*it)->deliver_time <<std::endl;

								csma_msg* tmp_msg = *it;
								aired_messages_.erase( it );
								std::vector<csma_msg*>::iterator ii = find_if( aired_messages_.begin(), 
									aired_messages_.end(), later_than(tmp_msg->deliver_time) );
								aired_messages_.insert( ii, tmp_msg );
								it = aired_messages_.begin();

								for(ii=aired_messages_.begin();	ii!=aired_messages_.end(); ii++)
								{
									std::cout << "\tmsg from node " << (*ii)->pmi->src_->id() << std::endl;
								}

							}
							
						}
						break;
					}
				}
				if( start_it==cur_round_msg.end() )
				{
					cur_round_msg.push_back( *it );
					it = aired_messages_.erase( it );
				}
			}
		}
	}

	// ----------------------------------------------------------------------
	void ZigbeeCsmaTransmissionModel::
		find_destinations( csma_msg* pmsg )
	{
		EdgeModel::adjacency_iterator it, endit;
		for( it = world_w().begin_adjacent_nodes_w( *pmsg->pmi->src_ ),
             endit = world_w().end_adjacent_nodes_w( *pmsg->pmi->src_ );
            it != endit;
            ++it )
		{
         Node *cur_dst = &(*it);
         if(transmission_in_range(pmsg->pmi->src_, cur_dst, pmsg->pmi))
         {
			   pmsg->push_new_destination( &(*it) );
         }
		}

		//print destination info and othe info of a msg.
		std::cout << "\nSource Node: " << pmsg->pmi->src_->id() << " send time: " << pmsg->pmi->time_ <<std::endl;
		std::cout << "The neighbours: "  << std::endl;
		shawn::Node* pNode;
		for( unsigned int i=0; i<pmsg->destinations_.size(); i++)
		{
			pNode = pmsg->destinations_[i]->dest_node_;
			std::cout << "[neighbour" << i << "]: " << pNode->id() << "; ";
		}
		std::cout << std::endl;
		//
	}

	// ----------------------------------------------------------------------
	bool
		ZigbeeCsmaTransmissionModel::
		remove_collided_destinations( csma_msg* msg1, csma_msg* msg2 )
		throw()
	{
		bool removed_ = false;

		for( std::vector<csma_msg::msg_destination*>::iterator it1=msg1->destinations_.begin(); 
			it1 != msg1->destinations_.end(); it1++)
		{
			// Find the common destination node and set the valid_ tag of collided destination to be false
			for( std::vector<csma_msg::msg_destination*>::iterator it2=msg2->destinations_.begin();
				it2 != msg2->destinations_.end(); it2++ )
			{
				if( (*it1)->dest_node_==(*it2)->dest_node_ && ((*it1)->valid_||(*it2)->valid_))
				{
					packet_failure_ = (*it1)->valid_? packet_failure_+1 : packet_failure_;
					packet_failure_ = (*it2)->valid_? packet_failure_+1 : packet_failure_;
					(*it1)->valid_ = (*it2)->valid_ = false;
					removed_ = true;
					break;
				}
			}
		}
		return removed_;
	}

	// ----------------------------------------------------------------------
	bool
		ZigbeeCsmaTransmissionModel::
		deliver_one_message( csma_msg* msg )
		throw()
	{
		if( msg->pmi->msg_->is_unicast() )
        {
            ABORT_INCONSISTENT_CONFIGURATION("Unicast is not supported by the csma transmission model. Implement it -> NOW.");
        }

		// print msg. info.
		std::cout << "Source Node: " <<  msg->pmi->src_->id() 
			<< ". Msg. send time: " <<  msg->pmi->time_ 
			<< ". Msg. deliver time: " <<  msg->deliver_time
			<< std::endl;
		std::cout << "The valid neighbours:\n";

		bool no_destination = true;
		for( std::vector<csma_msg::msg_destination*>::iterator it=msg->destinations_.begin();
			it != msg->destinations_.end();
            ++it )
		{
			if( (*it)->valid_ )
			{
				(*it)->dest_node_->receive( ConstMessageHandle(msg->pmi->msg_) );
				std::cout << (*it)->dest_node_->id() << std::endl;
				no_destination = false;
			}
		}
		dropped_ = no_destination? dropped_+1 : dropped_;
		return !no_destination;
	}

	// ----------------------------------------------------------------------
	bool 
		ZigbeeCsmaTransmissionModel::
		need_delay( csma_msg *new_msg, csma_msg* msg )
		throw()
	{
		std::cout << "see if msgs from node " << new_msg->pmi->src_->id() << " and " << msg->pmi->src_->id() 
			<< " will interfere." << std::endl;
		 
		//if( ( slot_==0 && (int)new_msg->deliver_time != (int)msg->deliver_time ) ||
		//	(slot_>0 && (int)(new_msg->deliver_time/slot_)!=(int)(msg->deliver_time/slot_)) )
		if( (int)new_msg->deliver_time != (int)msg->deliver_time )
		{
			std::cout << "\tno interfere" << std::endl;
			return false;
		}

		// The new message need to be delayed if the source nodes of new_msg and msg can communicate with each other,
		// that means the source node of new_msg(N) should have received a RTS message from the source node of msg(O).
		if( world_w().can_communicate_bidi( *new_msg->pmi->src_, *msg->pmi->src_ ) )
		{
			std::cout << "\t interfere" << std::endl;
			return true;
		}

		// new_msg should also be delayed if N and O have common neighbor, that means N should have received a CTS
		// message from that neighbor.
		for( EdgeModel::adjacency_iterator it=world_w().begin_adjacent_nodes_w( *new_msg->pmi->src_ ), 
			endit=world_w().end_adjacent_nodes_w( *new_msg->pmi->src_ );
			it != endit;
			++it)
		{
			for( EdgeModel::adjacency_iterator it1=world_w().begin_adjacent_nodes_w( *msg->pmi->src_ ), 
			endit1=world_w().end_adjacent_nodes_w( *msg->pmi->src_ );
			it1 != endit1;
			++it1)
			{
				// compare the address of Node structure
				if( &(*it) == &(*it1))
				{
					std::cout << "\tinterfere" << std::endl;
					return true;
				}
			}
		}
		std::cout << "\tno interfere" << std::endl;
		return false;
	}
}
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/zigbee_csma_transmission_model.cpp,v $
* Version $Revision: 370 $
* Date    $Date: 2009-11-12 14:12:56 +0100 (Thu, 12 Nov 2009) $
*-----------------------------------------------------------------------
* $Log: zigbee_csma_transmission_model.cpp,v $
 *-----------------------------------------------------------------------*/
