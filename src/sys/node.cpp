/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <iostream>
#include <sstream>
#include <math.h>
#include <limits>

#include "sys/node.h"
#include "sys/world.h"
#include "sys/misc/box.h"
#include "sys/misc/uuid.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/node_movements/no_movement.h"

using namespace std;

namespace shawn
{
   const std::string Node::WARN_HOOD_STORAGE_PARAMETER = "warn_hood_storage";

   // ----------------------------------------------------------------------
   Node::
   Node()
      : is_initialised_            ( false ),
        has_est_pos_               ( false ),
        world_                     ( NULL ),
        id_                        ( -1 ),
        label_                     ( "?" ),
        has_label_                 ( false ),
        min_storage_               ( 0 ),
        movement_                  ( NULL ),
	nh_data_is_valid_          ( true ),
	nh_time_stamp_             ( 0.0 ),
	has_fixpoint_              ( false ),
	max_hop_component_extent_  ( std::numeric_limits<int>::max() ),
	max_hop_count_             (0),
	transmission_range_        (1.0)
   {
      //NeighborhoodHandle nh0 = new Neighborhood;
      //nh0->insert( this );
      //neighborhoods_.push_back( nh0 );
      //assert( neighborhoods_.size() == 1 );
   }
   // ----------------------------------------------------------------------
   Node::
   ~Node()
   {
//      set_minimum_storage( 0 );
//      drop_neighborhood( 0 );
   }
   // ----------------------------------------------------------------------
   void
   Node::
   set_real_position( const Vec& p )
      throw()
   {
	   if( movement_.is_null() )
		   real_pos_ = p;
	   else
	   {
		   NoMovement* nm = new NoMovement();
		   nm->set_position(p);
		   set_movement(nm);
	   }
   }
   // ----------------------------------------------------------------------
   void
   Node::
   set_est_position( const Vec& p )
      throw()
   {
      est_pos_ = p;
      has_est_pos_ = true;
   }
   // ----------------------------------------------------------------------
   void
   Node::
   clear_est_position( void )
      throw()
   {
      est_pos_ = Vec(0.0,0.0);
      has_est_pos_ = false;
   }
   // ----------------------------------------------------------------------
   bool
   Node::
   has_est_position( void )
      const throw()
   {
      return has_est_pos_;
   }
   // ----------------------------------------------------------------------
   void
   Node::
   set_label( const std::string& s )
      throw()
   {
      has_label_ = true; // prevents set_id() from overwriting label
      label_=s;
   }
   // ----------------------------------------------------------------------
   void
   Node::
   set_id( int i )
      throw()
   {
      id_ = i;
      if( !has_label_ )
         {
            ostringstream oss;
            oss << "v" << i << "-" << UUIDGenerator::uuid();
            label_=oss.str();
         }
   }
   // ----------------------------------------------------------------------
   void
   Node::
   add_processor( const ProcessorHandle& ph )
      throw()
   {
      ph->set_owner( *this );
      // If this node is initialised, boot and add this processor directly.
      // If it is not initialised, queue the processor for later booting
      // and addition
      if (is_initialised_)
      {
         if( is_special_node() )
            ph->special_boot();
         ph->boot();
         processors_.push_back(ph);
      }
      else
      {
         new_processors_.push(ph);
      }
   }
   // ----------------------------------------------------------------------
   void
   Node::
   set_world( World& w )
      throw()
   {
      world_ = &w;
   }
   // ----------------------------------------------------------------------
   void
   Node::
   init()
      throw()
   {
      assert( world_ != NULL );
      assert( id_ >= 0 );
      is_initialised_ = true;

      //Hack until NodeMovement initialization is complete...
      NoMovement* nm = new NoMovement;
      nm->set_position(real_pos_);
      set_movement( nm );

      // TODO: do not boot processors when simulation has not yet started.
      // this is more a quick hack to still have processors booted at
      // simulation round 0.
      if ( world().simulation_round() < 0 )
         return;

      // Boot all queued processors
      while( !new_processors_.empty() )
      {
         ProcessorHandle new_processor = new_processors_.front();
         if( is_special_node() )
            new_processor->special_boot();
         new_processor->boot();
         processors_.push_back(new_processor);
         new_processors_.pop();
      }
   }
   // ----------------------------------------------------------------------
   Vec
   Node::
   real_position( void )
      const throw()
   {
      //cout << "CURRENT POS " << id() << " " << movement().position(*this) << endl;
      return movement().position();
   }
   // ----------------------------------------------------------------------
   const Vec&
   Node::
   est_position( void )
      const throw(/*no_gps*/)
   {
      return est_pos_;
   }
   // ----------------------------------------------------------------------
   int
   Node::
   id( void )
      const throw()
   {
      return id_;
   }
   // ----------------------------------------------------------------------
   bool
   Node::
   is_special_node( void )
      const throw()
   { return id()==0; }
   // ----------------------------------------------------------------------
   void
   Node::
   step( void )
      throw()
   {
      // Boot all queued processors
      while( !new_processors_.empty() )
      {
         ProcessorHandle new_processor = new_processors_.front();
         if( is_special_node() )
            new_processor->special_boot();
         new_processor->boot();
         processors_.push_back(new_processor);
         new_processors_.pop();
      }

	   for( ProcessorList::iterator it = processors_.begin(), endit = processors_.end(); it != endit; ++it )
		   if( (**it).state() != Processor::Inactive )
			   (**it).work();
   }
   // ----------------------------------------------------------------------
   void
   Node::
   receive( const ConstMessageHandle& cmh )
      throw()
   {
      for( ProcessorList::iterator it = processors_.begin(), endit = processors_.end(); it != endit; ++it )
         if( (**it).state() == Processor::Active )
            if( (**it).process_message(cmh) )
               return;
   }
   // ----------------------------------------------------------------------
   void
   Node::
   receive_dropped( const ConstMessageHandle& cmh )
      throw()
   {
      for( ProcessorList::iterator it = processors_.begin(), endit = processors_.end(); it != endit; ++it )
         if( (**it).state() == Processor::Active )
            if( (**it).process_dropped_message(cmh) )
               return;
   }
   // ----------------------------------------------------------------------
   void
   Node::
   send( const MessageHandle& mh )
      throw()
   {
      mh->set_source( *this );
      mh->set_timestamp( simulation_round(), current_time() );
      world_->send_message( *this, mh );
   }
   // ----------------------------------------------------------------------
   double
   Node::
   transmission_range()
      const throw()
   {
      return transmission_range_;
   }
   // ----------------------------------------------------------------------
   void
   Node::
   set_transmission_range(double range)
      throw()
   {
      transmission_range_ = range;
      if(transmission_range_ < 0.0)
         transmission_range_ = 0.0;
   }
   // ----------------------------------------------------------------------
   int
   Node::
   simulation_round( void )
      const throw()
   {
      return world().simulation_round();
   }
   // ----------------------------------------------------------------------
   double
   Node::
   current_time( void )
      const throw()
   {
      return world().current_time();
   }
   // ----------------------------------------------------------------------
   const World&
   Node::
   world( void )
      const throw()
   {
      assert( world_ != NULL );
      return *world_;
   }
   // ----------------------------------------------------------------------
   World&
   Node::
   world_w( void )
      throw()
   {
      assert( world_ != NULL );
      return *world_;
   }
   // ----------------------------------------------------------------------
   Processor::ProcessorState
   Node::
   state( void )
      const throw()
   {
	   bool have_act   = false;
	   bool have_sleep = false;

	   for( ProcessorList::const_iterator it=processors_.begin(), endit=processors_.end(); it!=endit; ++it)
	   {
		   //Only look at processors that will not auto_terminate with others
		   if( !(**it).auto_terminate() )
		   {
			   switch( (**it).state() )
			   {
			   case Processor::Active:
				   have_act = true;
				   break;

			   case Processor::Sleeping:
				   have_sleep = true;
				   break;

			   default:
				   break;
			   }
		   }
	   }
	   if( have_act )
		   return Processor::Active;
	   else if( have_sleep )
		   return Processor::Sleeping;
	   else
		   return Processor::Inactive;
   }
   // ----------------------------------------------------------------------
/*   void
   Node::
   add_neighbour( Node& v )
      throw()
   {
      // if the following assert fails, someone's trying to use add_neighbour
      // directly. If you want to manually set which nodes are neighboured,
      // use "manual" for the world's communication model, "list" for the edge
      // model and finally use add_edge() of the comm. model!
      assert( min_storage_ >= 1 );

      if( neighborhoods_.size()<2 )
         {
            assert( neighborhoods_.size()==1 );
            neighborhoods_.push_back( new Neighborhood );
         }
      else if( neighborhoods_[1] == NULL )
         {
            neighborhoods_[1] = new Neighborhood;
         }
      neighborhoods_[1]->insert(&v);
   }*/
   // ----------------------------------------------------------------------
   const std::string&
   Node::
   label( void )
      const throw()
   {
      return label_;
   }
   // ----------------------------------------------------------------------
/*   void
   Node::
   drop_neighborhood( int dist )
      const throw()
   {
      if( dist < min_storage_ ) dist = min_storage_;

      for( int i = neighborhoods_.size()-1;
           i>=dist;
           --i )
	 neighborhoods_[i] = NULL;
   }*/
   // ----------------------------------------------------------------------
/*   void
   Node::
   dynamic_neighborhood( int maxdist )
     const throw()
   {
      assert( maxdist>=0 );
      if( maxdist == 0 )
         return;

      const_cast<Node*>(this)->revalidate_data();

      if( ( neighborhoods_[maxdist] == NULL ) ||
	  ( neighborhoods_.size() <= (unsigned int)maxdist ))
      {

	 if( neighborhoods_.size() <= (unsigned int)maxdist )
	 {
	    neighborhoods_.resize( maxdist + 1 , NULL);
	 }


//	 copy fixpoint
	 if( has_fixpoint_ && maxdist >= max_hop_component_extent_ )
	 {
	    assert( neighborhoods_[ max_hop_component_extent_ ] != NULL );
	    neighborhoods_[maxdist] = neighborhoods_[ max_hop_component_extent_ ];
	    return;
	 }

	 const double log2_maxdist = log((double)maxdist)/log(2.0);
	 const int floor_log2_maxdist = (int)floor( log2_maxdist );

	 NeighborhoodHandle newhood = new Neighborhood;

	 if( maxdist == 1 )
	 {
	    assert( neighborhoods_[0] != NULL );
	    //look for adjacent nodes asking the EdgeModel
	    for ( Node::adjacency_iterator adjaIt = (const_cast<Node*>(this))->begin_adjacent_nodes_w();
		  adjaIt != (const_cast<Node*>(this))->end_adjacent_nodes_w(); ++adjaIt )
	    {
	       newhood->insert( &(*adjaIt) );
	    }
	    neighborhoods_[maxdist] = newhood;
	    return;
	 }
	 else
	 {
	    int level;
	    int rest;

	    if( 0.0 == fmod(log2_maxdist,1.0) )
	    {
// 	       iff maxdist == 2^k ==> generate neighborhoods( 2^(k-1) )
// 	       neighborhood(v, maxdist) =
//             neighborhood(v,halfdist) + neighborhood(w,halfdist)

	       level = maxdist/2;
	       rest  = maxdist/2;
	    }
	    else
	    {
// 	       neighborhood(v, maxdist) = neighborhood(v,Level) + neighborhood(w,Rest)
// 	       for all  w \in neighborhood(v,Level)
// 	       Level = 2^(floor( ln( maxdist ) ) )       // ln(x) = log(x)/log(2)
// 	       Rest = maxdist - Level

	       level =(int)pow( 2.0, floor_log2_maxdist );
	       rest = maxdist - level;
	    }

// 	    recursive call for neighborhood(level)
	    dynamic_neighborhood( level );

	    assert( neighborhoods_[ level ] != NULL );

	    newhood->insert( neighborhoods_[ level ]->begin(),
			     neighborhoods_[ level ]->end() );

// 	    generate recursive rest_neighborhoods for all nodes
//	    and add them to newhood
	    for( Neighborhood::iterator it = neighborhoods_[ level ]->begin();
		 it != neighborhoods_[ level ]->end(); ++it )
	    {
	       (*it)->dynamic_neighborhood( rest );
	       assert( ( (*it)->neighborhoods_[ rest ] )!= NULL );
	       newhood->insert( (*it)->neighborhoods_[ rest ]->begin(),
				(*it)->neighborhoods_[ rest ]->end() );
	    }

	    neighborhoods_[maxdist] = newhood;

//          fixpoint detection
	    if( neighborhoods_[ max_hop_count_ ]->size() == newhood->size() )
	    {
	       has_fixpoint_ = true;
	       max_hop_component_extent_ = ( maxdist <= max_hop_count_ ) ? maxdist : max_hop_count_ ;
	    }
	    if( maxdist > max_hop_count_ ) max_hop_count_= maxdist;

	    return;
	 }
      }
   }
*/
   // ----------------------------------------------------------------------
/*   void Node::revalidate_data() throw()
   {
      if( nh_data_is_valid_ )
      {
	 if( current_time() == nh_time_stamp_ )
	 {
	    return;
	 }
//	 else
//	 {
// 	    if( no change in (max-hop-, 1-hop- or) maxdist-neighborhood-NODESET )
// 	    {
// 	       return;
// 	    }
//	 }
      }
      // data is not valid --> inform "all" other nodes in NODESET(nh_time_stamp_)
      // and NODESET(current_time) to invalidate informations

      // simple correct method: allways
      // mark nodes in neighborhoods_[max] as not_valid
      vector<NeighborhoodHandle>::reverse_iterator maxPos;
      for(  maxPos = neighborhoods_.rbegin();
	    ( ( maxPos != neighborhoods_.rend() ) && ( (*maxPos) == NULL ) );
	    ++maxPos ){ }

      for_each((*maxPos)->begin(),(*maxPos)->end(),
	       mem_fun(&Node::invalidate_neighborhoods_data));

      for( int i = neighborhoods_.size()-1; i > 0; --i )
      {
	    neighborhoods_[i] = NULL;
      }
      nh_data_is_valid_ = true;
      nh_time_stamp_ = current_time();
   }*/
   // ----------------------------------------------------------------------
/*     void
     Node::
     invalidate_neighborhoods_data( )
	 throw()
     {
	if( current_time() != nh_time_stamp_ )
	{
	   nh_data_is_valid_           = false;
	   has_fixpoint_               = false;
	   max_hop_component_extent_   = std::numeric_limits<int>::max();
	   max_hop_count_              = 0;
	}
     }*/
   // ----------------------------------------------------------------------
/*   void
   Node::
   set_minimum_storage( int mindist )
      throw()
   {
      assert( (mindist==0) || (mindist==1) );
      min_storage_ = mindist;
   }*/
   // ----------------------------------------------------------------------
   /*void
   Node::
   prepare_neighborhood( int maxdist )
      const throw()
   {
      assert( maxdist>=0 );
      if( maxdist == 0 )
         return;



      if( (neighborhoods_.size() <= (unsigned int)maxdist) ||
          (neighborhoods_[maxdist]==NULL) )
         {
#ifndef NDEBUG
            try {
               if( maxdist >=
                   world().simulation_controller().environment().optional_int_param(WARN_HOOD_STORAGE_PARAMETER,1) )
                  {
                     cerr << "[WARNING: N_" << maxdist << "(" << label() << ") gets stored..." << endl
                          << " set " << WARN_HOOD_STORAGE_PARAMETER << " to a large number to suppress warning]" << endl;
                  }
            }
            catch( std::runtime_error& re ) {
               cerr << re.what() << endl;
               abort();
            }
#endif

            prepare_neighborhood( maxdist-1 );
            if( neighborhoods_.size() <= (unsigned int)maxdist )
               {
//                   if( neighborhoods_.size()!=(unsigned int)maxdist )
//                      cout << (unsigned int)neighborhoods_.size() << "==" << (unsigned int)maxdist << "?" << endl;
                  assert( neighborhoods_.size()==(unsigned int)maxdist );
                  neighborhoods_.push_back( NULL );
               }

            assert( neighborhoods_[maxdist]   == NULL );
            assert( neighborhoods_[maxdist-1] != NULL );

            NeighborhoodHandle newhood = new Neighborhood;
            newhood->insert( neighborhoods_[maxdist-1]->begin(),
                             neighborhoods_[maxdist-1]->end() );
            for( Neighborhood::iterator
                    it    = neighborhoods_[maxdist-1]->begin(),
                    endit = neighborhoods_[maxdist-1]->end();
                 it != endit; ++it )
               //look for adjacent nodes while asking the EdgeModel
               for ( Node::adjacency_iterator adjaIt=(*it)->begin_adjacent_nodes_w();
                     adjaIt != (*it)->end_adjacent_nodes_w(); ++adjaIt)
                  newhood->insert( &(*adjaIt) );

            neighborhoods_[maxdist] = newhood;
         }
   }*/
   // ----------------------------------------------------------------------
/*  NeighborhoodHandle
   Node::
   neighborhood_w( int maxdist )
      throw()
   {
      if (! world_->edge_model().supports_mobility() )
      {
	 prepare_neighborhood( maxdist );
      	 assert( neighborhoods_.size() > (unsigned int)maxdist );
      }
      else
      {
	 dynamic_neighborhood( maxdist );
      }
      assert( neighborhoods_[maxdist].is_not_null());
      return neighborhoods_[maxdist];
   }*/
   // ----------------------------------------------------------------------
/*   ConstNeighborhoodHandle
   Node::
   neighborhood( int maxdist )
      const throw()
   {
      if (! world_->edge_model().supports_mobility() )
      {
	 prepare_neighborhood( maxdist );
	 assert( neighborhoods_.size() > (unsigned int)maxdist );
      }
      else
      {
	 dynamic_neighborhood( maxdist );
      }
      assert( neighborhoods_[maxdist].is_not_null());
      return neighborhoods_[maxdist];
   }*/


   // ----------------------------------------------------------------------
  NeighborhoodHandle
	  Node::
	  neighborhood_w( int maxdist, EdgeModel::CommunicationDirection direction )
      throw()
  {
	NeighborhoodHandle newhood = new Neighborhood;
	NeighborhoodHandle s1 = new Neighborhood;
	NeighborhoodHandle s2 = new Neighborhood;

	s1->insert(this);
	newhood->insert(this);

	for(int d = 1; d <= maxdist; ++d)
	{
		for(Neighborhood::iterator set1it = s1->begin(), end1it = s1->end(); set1it!=end1it; ++set1it) {

			for(Node::adjacency_iterator it = (**set1it).begin_adjacent_nodes_w(direction),
				end = (**set1it).end_adjacent_nodes_w(); it!=end; ++it)
			{
				if( newhood->find(&(*it)) == newhood->end() )
				{
					s2->insert(&(*it));
					newhood->insert(&(*it));
				}
			}
		}
		(*s1) = (*s2);
		s2->clear();

	}
	return newhood;
  }

  // ----------------------------------------------------------------------
  NeighborhoodConstHandle
	  Node::
	  neighborhood( int maxdist, EdgeModel::CommunicationDirection direction)
	  const throw()
  {
	NeighborhoodConstHandle newhood = new NeighborhoodConst;
	NeighborhoodConstHandle s1 = new NeighborhoodConst;
	NeighborhoodConstHandle s2 = new NeighborhoodConst;

	s1->insert(this);
	newhood->insert(this);

	for(int d = 1; d <= maxdist; ++d)
	{
		for(NeighborhoodConst::iterator set1it = s1->begin(), end1it = s1->end(); set1it!=end1it; ++set1it) {

			for(Node::const_adjacency_iterator it = (**set1it).begin_adjacent_nodes(direction),
				end = (**set1it).end_adjacent_nodes(); it!=end; ++it)
			{
				if( newhood->find(&(*it)) == newhood->end() )
				{
					s2->insert(&(*it));
					newhood->insert(&(*it));
				}
			}

		}
		(*s1) = (*s2);
		s2->clear();

	}
	return newhood;
  }


   // ----------------------------------------------------------------------
   void
   Node::
   reset( void )
      throw()
   {
	   for( ProcessorList::iterator it = processors_.begin(), endit = processors_.end(); it != endit; ++it )
		   new_processors_.push(*it);
   }
   // ----------------------------------------------------------------------
   int
   Node::
   degree( EdgeModel::CommunicationDirection d )
      const throw()
   {
      //ABORT_NOT_IMPLEMENTED;
      //return (int)neighborhood(1)->size() - 1;

	  return world().edge_model().nof_adjacent_nodes(*this, d )-1;
   }
   // ----------------------------------------------------------------------
   void
   Node::
   sync_to_tags( void )
      throw()
   { ABORT_NOT_IMPLEMENTED;
   }
   // ----------------------------------------------------------------------
   void
   Node::
   sync_from_tags( void )
      throw( std::runtime_error )
   { ABORT_NOT_IMPLEMENTED;
   }
   // ----------------------------------------------------------------------
   void
   Node::
   set_movement( const NodeMovementHandle& nm )
      throw()
   {
      assert( nm.is_not_null() );

	  //Initialize the movement either from scratch or from a previous movement
      if( movement_.is_null() )
	  {
            movement_ = nm;
            movement_->init( *this );
      }
      else
      {
            NodeMovementHandle old = movement_;
            movement_=nm;
            movement_->init_from( *this, *old );
      }

	  //Add the movement observers, always add the current edge model as observer
	  nm->observer_add( world_w().edge_model_w() );
	  for(ObserverIterator it = observers_begin(), end = observers_end(); it!=end; ++it)
		  nm->observer_add(**it);
   }

   // ----------------------------------------------------------------------
   void
	   Node::
	   observers_added(MovementObserver& obs)
	   throw()
   {
	   obs.observer_initial_zone(*this, movement_w().position(), movement_w().velocity() );
	   movement_update_boxes(*this, movement_w().position(), movement_w().velocity());
   }

   // ----------------------------------------------------------------------
   NodeMovement&
   Node::
   movement_w( void )
      throw()
   {
      assert( movement_.is_not_null() );
      return *movement_;
   }
   // ----------------------------------------------------------------------
   const NodeMovement&
   Node::
   movement( void )
      const throw()
   {
      assert( movement_.is_not_null() );
      return *movement_;
   }
   // ----------------------------------------------------------------------
   Node::const_adjacency_iterator
   Node::
   begin_adjacent_nodes( EdgeModel::CommunicationDirection d )
      const throw()
   { return world().edge_model().begin_adjacent_nodes(*this, d); }
   // ----------------------------------------------------------------------
   Node::const_adjacency_iterator
   Node::
   end_adjacent_nodes( void )
      const throw()
   { return world().edge_model().end_adjacent_nodes(*this); }
   // ----------------------------------------------------------------------
   Node::adjacency_iterator
   Node::
   begin_adjacent_nodes_w( EdgeModel::CommunicationDirection d )
      throw()
   { return world_w().edge_model_w().begin_adjacent_nodes_w(*this, d); }
   // ----------------------------------------------------------------------
   Node::adjacency_iterator
   Node::
   end_adjacent_nodes_w( void )
      throw()
   { return world_w().edge_model_w().end_adjacent_nodes_w(*this); }

   // ----------------------------------------------------------------------
	std::set<const Node*>
		Node::get_adjacent_nodes( EdgeModel::CommunicationDirection d ) const
	{
			set<const Node*> neighbors;
			for(shawn::Node::const_adjacency_iterator itnb = begin_adjacent_nodes(d), end = end_adjacent_nodes(); itnb != end; ++itnb) {
				neighbors.insert( &(*itnb) );
			}
			return neighbors;

	}

   // ----------------------------------------------------------------------
	std::set<Node*>
		Node::get_adjacent_nodes_w( EdgeModel::CommunicationDirection d )
	{
			set<Node*> neighbors;
			for(shawn::Node::adjacency_iterator itnb = begin_adjacent_nodes_w(d), end = end_adjacent_nodes_w(); itnb != end; ++itnb) {
				neighbors.insert( &(*itnb) );
			}
			return neighbors;

	}
	// ----------------------------------------------------------------------
	const Node::ProcessorList&
		Node::processors() const
		throw()
	{
		return processors_;
	}
	// ----------------------------------------------------------------------
	Node::ProcessorList&
		Node::processors_w()
		throw()
	{
		return processors_;
	}
	// ----------------------------------------------------------------------
	Node::ProcessorList::iterator
		Node::begin_processors_w()
		throw()
	{
		return processors_w().begin();
	}
	// ----------------------------------------------------------------------
	Node::ProcessorList::const_iterator
		Node::begin_processors()
		throw()
	{
		return processors().begin();
	}
	// ----------------------------------------------------------------------
	Node::ProcessorList::iterator
		Node::end_processors_w()
		throw()
	{
		return processors_w().end();
	}
	// ----------------------------------------------------------------------
	Node::ProcessorList::const_iterator
		Node::end_processors()
		throw()
	{
		return processors().end();
	}

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node.cpp,v $
 * Version $Revision: 646 $
 * Date    $Date: 2012-04-30 15:55:19 +0200 (Mon, 30 Apr 2012) $
 *-----------------------------------------------------------------------
 * $Log: node.cpp,v $
 *-----------------------------------------------------------------------*/
