/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>
#include "sys/world.h"
#include "sys/constants.h"
#include "sys/event_scheduler.h"
#include "sys/edge_model.h"
#include "sys/transmission_model.h"
#include "sys/communication_model.h"
#include "sys/node_movements/playback/movement_controller.h"
#include "sys/misc/uuid.h"


#include "sys/processors/processor_keeper.h"
#include "sys/processors/processor_factory.h"
#include "apps/reading/readings/reading_keeper.h"
#include "sys/util/handle_keeper.h"
#include "_apps_enable_cmake.h"

using namespace std;


// #define ALWAYS_SEARCH_FOR_NEIGHBORHOOD
#define _MIN_(x,y) ( (x)<(y) ? (x) : (y) )
#define _MAX_(x,y) ( (x)<(y) ? (y) : (x) )

namespace shawn
{
   World::
   World()
      : valid_node_cnt_      ( 0 ),
        round_               (-1 ),
        cnt_act_             ( 0 ),
        cnt_slp_             ( 0 ),
        cnt_ina_             ( 0 ),
        size_hint_           ( 0.0 ),
        has_size_hint_       ( false ),
        lower_left_          ( numeric_limits<double>::max(),
                               numeric_limits<double>::max() ),
        upper_right_         ( -numeric_limits<double>::max(),
                               -numeric_limits<double>::max() ),
        sched_               ( new EventScheduler ),
        edge_model_          ( NULL ),
        transmission_model_  ( NULL ),
        comm_model_          ( NULL ),
        controller_          ( NULL ),
        uuid_                ( UUIDGenerator::uuid() )
   {
      // DO NOT FORGET TO ADD INITIALIZATION TO reset() AND clear() !
#ifdef ENABLE_LOGGING
      set_logger_name( "world" );
#endif
   }
   // ----------------------------------------------------------------------
   World::
   ~World()
   {
      clear();

      if( edge_model_ != NULL )
         delete edge_model_;
      if( comm_model_ != NULL )
         delete comm_model_;
      if( transmission_model_ != NULL )
         delete transmission_model_;
      if( sched_ != NULL )
         delete sched_;
      if ( movement_controller_ != NULL )
         delete movement_controller_;
   }
   // ----------------------------------------------------------------------
   void
   World::
   init( void )
      throw()
   {
      communication_model_w().set_world( *this );
      communication_model_w().init();

      edge_model_w().set_world( *this );
      edge_model_w().set_communication_model( communication_model() );
      edge_model_w().init();

      transmission_model_w().set_world( *this );
      transmission_model_w().init();

#ifdef ENABLE_READING
//      reading::ReadingKeeper& rk_ = simulation_controller_w().reading_keeper_w();
//      for( reading::ReadingKeeper::HandleMapType::iterator it = rk_.begin_handles_w(); it != rk_.end_handles_w(); ++it )
//      {
//    	reading::Reading& r_ = *it->second;
//	    r_.set_world( *this );
//    	r_.init();
//      }
#endif
   }
   // ----------------------------------------------------------------------
   void
   World::
   add_edge( Node&,
             Node& )
      throw()
   {
      ABORT_NOT_IMPLEMENTED;
      /*assert( edge_model() == ManualEdges );
      n1.add_neighbour( n2 );
      n2.add_neighbour( n1 );*/
   }
   // ----------------------------------------------------------------------
   void
   World::
   add_node( Node& node )
      throw(std::logic_error)
   {
      // Determine and set the id; connect the node with the world
      int newid;
      if ( marked_ids_.empty() )
      {
         newid = (int) nodes_.size();
         nodes_.push_back( &node );
      }
      else
      {
         newid = marked_ids_.front();
         marked_ids_.pop();
         nodes_[newid] = &node;
      }
      node.set_id( newid );
      node.set_world( *this );
      assert( nodes_[newid] == &node );

      // Attach the default processors to the new node
      for ( std::set<std::string>::iterator
               it = default_processors_.begin(), end = default_processors_.end();
               it != end;
               ++it )
      {
         ProcessorFactoryHandle pfh = simulation_controller_w().processor_keeper_w().find_w(*it);
         if( pfh.is_not_null() )
         {
            node.add_processor( pfh->create() );
         }
         else
         {
            ostringstream os;
            os << "Default processor of type '"<< (*it) <<"' could not be created.";
            throw std::logic_error( os.str() );
         }
      }

      // Initialise the node
      node.init();
      ++cnt_act_;
      ++valid_node_cnt_;

      // Update the world's size
      const Vec& p = node.real_position();
      lower_left_ = Vec( _MIN_(lower_left_.x(),p.x()),
                         _MIN_(lower_left_.y(),p.y()),
                         _MIN_(lower_left_.z(),p.z()) );
      upper_right_ = Vec( _MAX_(upper_right_.x(),p.x()),
                          _MAX_(upper_right_.y(),p.y()),
                          _MAX_(upper_right_.z(),p.z()) );

      // Notify the registered listeners that a new node has been added
      for ( NodeChangeListenerList::iterator
               it = node_change_listeners_.begin();
               it != node_change_listeners_.end();
               ++it )
         (**it).node_added( node );
   }
   // ----------------------------------------------------------------------
   void
   World::
   remove_node( Node& node )
      throw( std::logic_error )
   {
      // Notify the registered listeners that a node is going to be removed
      for ( NodeChangeListenerList::iterator
               it = node_change_listeners_.begin();
               it != node_change_listeners_.end();
               ++it )
         (**it).node_removed( node );

      node.observers_clear();

      int rm_id = node.id();
      marked_ids_.push( rm_id );
      valid_node_cnt_--;

      delete nodes_[ rm_id ];
      nodes_[ rm_id ] = NULL;
   }
   // ----------------------------------------------------------------------
   void
       World::
       add_default_processor( const std::string& proc_name)
       throw()
   {
       default_processors_.insert( proc_name );
   }
   // ----------------------------------------------------------------------
   void
   World::
   add_pre_step_task( const SimulationTaskHandle& sth )
      throw()
   {
      prestep_tasks_.push_back(sth);
   }
   // ----------------------------------------------------------------------
   void
   World::
   add_post_step_task( const SimulationTaskHandle& sth )
      throw()
   {
      poststep_tasks_.push_back(sth);
   }
   // ----------------------------------------------------------------------
   void
   World::
   step( void )
      throw()
   {
      static int iteration_message_interval = simulation_controller().environment().
               optional_int_param( "iteration_message_interval", 1 );

      ++round_;
      if ( round_ % iteration_message_interval == 0 )
         cout << "------------------------------ BEGIN ITERATION " << round_ << endl;

      pre_step();
      transmission_model_w().deliver_messages();
      run_nodes();
      scheduler_w().playback( (double)(round_+1) );
      post_step();

      if ( round_ % iteration_message_interval == 0 )
      {
         cout << "------------------------------- DONE ITERATION " << round_ << endl
           << "  [ "
           << cnt_act_ << " active, "
           << cnt_slp_ << " sleeping, "
           << cnt_ina_ << " inactive ]" << endl << endl;
      }
   }
   // ----------------------------------------------------------------------
   void
   World::
   pre_step( void )
      throw()
   {
      for( SimulationTaskList::iterator
              it    = prestep_tasks_.begin(),
              endit = prestep_tasks_.end();
           it != endit; ++it )
         (**it).run(simulation_controller_w());
   }
   // ----------------------------------------------------------------------
   void
   World::
   run_nodes( void )
      throw()
   {
      for( node_iterator
              it    = begin_nodes_w(),
              endit = end_nodes_w();
           it != endit;
           ++it )
         it->step();
   }
   // ----------------------------------------------------------------------
   void
   World::
   post_step( void )
      throw()
   {
      for( SimulationTaskList::iterator
              it    = poststep_tasks_.begin(),
              endit = poststep_tasks_.end();
           it != endit; ++it )
         (**it).run(simulation_controller_w());

      cnt_act_=cnt_slp_=cnt_ina_=0;
      for( const_node_iterator
              it    = begin_nodes(),
              endit = end_nodes();
           it != endit; ++it )
         switch( (*it).state() )
            {
            case Processor::Active:
               ++cnt_act_; break;
            case Processor::Sleeping:
               ++cnt_slp_; break;
            case Processor::Inactive:
               ++cnt_ina_; break;
            }
   }
   // ----------------------------------------------------------------------
   int
   World::
   simulation_round( void )
      const throw()
   {
      return round_;
   }
   // ----------------------------------------------------------------------
   double
   World::
   current_time( void )
      const throw()
   {
      return scheduler().current_time();
   }
   // ----------------------------------------------------------------------
   void
   World::
   send_message( const Node& source,
                 const MessageHandle& mh )
      throw()
   {
      TransmissionModel::MessageInfo * mi = new TransmissionModel::MessageInfo;

      mi->src_     = const_cast<Node*>(&source); // TODO: ugh const_cast -- fix this!
      mi->src_pos_ = source.real_position();
      mi->time_    = current_time();
      mi->msg_     = mh;
      mi->range_   = mh.get()->transmission_range();

      transmission_model_w().send_message(*mi);
	  // Think about delete mi;
   }
   // ----------------------------------------------------------------------
   bool
   World::
   is_done( void )
      const throw()
   { return active_nodes_count()+sleeping_nodes_count() == 0; }
   // ----------------------------------------------------------------------
   int
   World::
   active_nodes_count( void )
      const throw()
   { return cnt_act_; }
   // ----------------------------------------------------------------------
   int
   World::
   sleeping_nodes_count( void )
      const throw()
   { return cnt_slp_; }
   // ----------------------------------------------------------------------
   int
   World::
   inactive_nodes_count( void )
      const throw()
   { return cnt_ina_; }
   // ----------------------------------------------------------------------
   bool
   World::
   can_communicate_bidi( const Node& u,
                         const Node& v)
      const throw()
   {
       return communication_model().can_communicate_bidi(u, v);
   }
   // ----------------------------------------------------------------------
   bool
   World::
   can_communicate_uni( const Node& u,
                        const Node& v)
      const throw()
   {
       return communication_model().can_communicate_uni(u, v);
   }
   // ----------------------------------------------------------------------
   World::NodeVector&
   World::
   nodes_w( void )
      throw()
   {
      return nodes_;
   }
   // ----------------------------------------------------------------------
   const World::NodeVector&
   World::
   nodes( void )
      const throw()
   {
      return nodes_;
   }
   // ----------------------------------------------------------------------
   World::node_iterator
   World::
   begin_nodes_w( void )
      throw()
   {
      return node_iterator( nodes_w().begin(), nodes_w() );
   }
   // ----------------------------------------------------------------------
   World::node_iterator
   World::
   end_nodes_w( void )
      throw()
   {
      return node_iterator( nodes_w().end(), nodes_w() );
   }
   // ----------------------------------------------------------------------
   Node*
   World::
   find_node_by_id_w( int i )
      throw()
   {
      if( i < 0 )
         return NULL;
      if( i >= (int)nodes_.size() )
         return NULL;

      assert( nodes_[i] != NULL );
      assert( nodes_[i]->id() == i );

      return nodes_[i];
   }
   // ----------------------------------------------------------------------
   Node*
   World::
   find_node_by_label_w( const std::string& l )
      throw()
   {
      // TODO: keep consistent map<string,nodehandle> instead of this
      // (*&#(&#) searching
      for( node_iterator
              it    = begin_nodes_w(),
              endit = end_nodes_w();
           it != endit; ++it )
         if( l == it->label() )
            return &(*it);

      return NULL;
   }
   // ----------------------------------------------------------------------
   World::const_node_iterator
   World::
   begin_nodes( void )
      const throw()
   {
      return const_node_iterator( nodes().begin(), nodes() );
   }
   // ----------------------------------------------------------------------
   World::const_node_iterator
   World::
   end_nodes( void )
      const throw()
   {
      return const_node_iterator( nodes().end(), nodes() );
   }
   // ----------------------------------------------------------------------
   const Node*
   World::
   find_node_by_id( int i )
      const throw()
   {
      if( i < 0 )
         return NULL;
      if( i >= (int)nodes_.size() )
         return NULL;

      assert( nodes_[i] != NULL );
      assert( nodes_[i]->id() == i );

      return nodes_[i];
   }
   // ----------------------------------------------------------------------
   const Node*
   World::
   find_node_by_label( const std::string& l )
      const throw()
   {
      // TODO: keep consistent map<string,nodehandle> instead of this
      // (*&#(&#) searching
      for( const_node_iterator
              it    = begin_nodes(),
              endit = end_nodes();
           it != endit; ++it )
         if( l == it->label() )
            return &(*it);

      return NULL;
   }
   // ----------------------------------------------------------------------
   const Vec&
   World::
   lower_left( void )
      const throw()
   { return lower_left_; }
   // ----------------------------------------------------------------------
   const Vec&
   World::
   upper_right( void )
      const throw()
   { return upper_right_; }
   // ----------------------------------------------------------------------
   EventScheduler&
   World::
   scheduler_w( void )
      throw()
   {
      assert( sched_ != NULL );
      return *sched_;
   }
   // ----------------------------------------------------------------------
   const EventScheduler&
   World::
   scheduler( void )
      const throw()
   {
      assert( sched_ != NULL );
      return *sched_;
   }
   // ----------------------------------------------------------------------
   void
   World::
   set_edge_model( EdgeModel& tm )
      throw()
   {
      if( !nodes_.empty() )
         {
            cerr << __FILE__ << ":" << __LINE__ << " not implemented:" << endl
                 << "set_edge_model() with existing nodes => rehashing of hoods needed."
                 << endl;
            abort();
         }
      if( edge_model_ != NULL )
         delete edge_model_;
      edge_model_=&tm;
      add_node_change_listener( tm );
   }
   // ----------------------------------------------------------------------
   const EdgeModel&
   World::
   edge_model( void )
      const throw()
   {
      assert( edge_model_ != NULL );
      return *edge_model_;
   }
   // ----------------------------------------------------------------------
   EdgeModel&
   World::
   edge_model_w( void )
      throw()
   {
      assert( edge_model_ != NULL );
      return *edge_model_;
   }
   // ----------------------------------------------------------------------
   void
   World::
   set_transmission_model( TransmissionModel& tm )
      throw()
   {
       if( transmission_model_ != NULL )
         delete transmission_model_;

       transmission_model_= &tm;
   }
   // ----------------------------------------------------------------------
   const TransmissionModel&
   World::
   transmission_model( void )
      const throw()
   {
      assert( transmission_model_ != NULL );
      return *transmission_model_;
   }
   // ----------------------------------------------------------------------
   TransmissionModel&
   World::
   transmission_model_w( void )
      throw()
   {
      assert( transmission_model_ != NULL );
      return *transmission_model_;
   }
   // ----------------------------------------------------------------------
   void
   World::
   set_communication_model( CommunicationModel& tm )
      throw()
   {
      if( !nodes_.empty() )
         {
            cerr << __FILE__ << ":" << __LINE__ << " not implemented:" << endl
                 << "set_communication_model() with existing nodes => rehashing of hoods needed."
                 << endl;
            abort();
         }
      if( comm_model_ != NULL )
         delete comm_model_;
      comm_model_=&tm;
   }
   // ----------------------------------------------------------------------
   const CommunicationModel&
   World::
   communication_model( void )
      const throw()
   {
      assert( comm_model_ != NULL );
      return *comm_model_;
   }
   // ----------------------------------------------------------------------
   CommunicationModel&
   World::
   communication_model_w( void )
      throw()
   {
      assert( comm_model_ != NULL );
      return *comm_model_;
   }
   // ----------------------------------------------------------------------
   void
   World::
   reset( void )
      throw()
   {
      round_ = -1;
      scheduler_w().clear();
      movement_controller_w().reset();
      for( node_iterator
              it    = begin_nodes_w(),
              endit = end_nodes_w();
           it != endit; ++it )
         it->reset();
   }
   // ----------------------------------------------------------------------
   void
   World::
   clear( void )
      throw()
   {
//		   cout << "world::clear: 1" << endl;
      round_ = -1;
      scheduler_w().clear();
//		   cout << "world::clear: 2" << endl;
      invalidate_node_change_listeners();
      for( node_iterator
              it    = begin_nodes_w(),
              endit = end_nodes_w();
           it != endit;
           ++it )
		   {
//			   cout << "world::clear: 2,5: " << &(*it) << endl;
         delete &(*it);
//			   cout << "world::clear: 2,6: " << &(*it) << endl;
		   }

//		   cout << "world::clear: 3" << endl;
      nodes_.clear();
//		   cout << "world::clear: 4" << endl;

      if( transmission_model_ != NULL )
         transmission_model_w().reset();

      cnt_act_ = 0;
      cnt_slp_ = 0;
      cnt_ina_ = 0;
      lower_left_ = Vec( numeric_limits<double>::max(),
                         numeric_limits<double>::max() );
      upper_right_ = Vec( numeric_limits<double>::min(),
                          numeric_limits<double>::min() );
   }
   // ----------------------------------------------------------------------
   void
   World::
   set_simulation_controller( SimulationController& sc )
      throw()
   {
      controller_ = &sc;
   }
   // ----------------------------------------------------------------------
   SimulationController&
   World::
   simulation_controller_w( void )
      throw()
   {
      assert( controller_ != NULL );
      return *controller_;
   }
   // ----------------------------------------------------------------------
   const SimulationController&
   World::
   simulation_controller( void )
      const throw()
   {
      assert( controller_ != NULL );
      return *controller_;
   }

   // ----------------------------------------------------------------------
   void
   World::
   set_movement_controller( MovementController* mc )
      throw()
   {
      movement_controller_ = mc;
   }
   // ----------------------------------------------------------------------
   MovementController&
   World::
   movement_controller_w( void )
      throw()
   {
      assert( movement_controller_ != NULL );
      return *movement_controller_;
   }
   // ----------------------------------------------------------------------
   const MovementController&
   World::
   movement_controller( void )
      const throw()
   {
      assert( movement_controller_ != NULL );
      return *movement_controller_;
   }

   // ----------------------------------------------------------------------
   void
       World::
       set_size_hint(double hint)
       throw(std::logic_error)
   {
       if( has_size_hint() )
           throw std::logic_error("A size hint has already been provided.");

       if( nodes().size() > 0 )
           throw std::logic_error("Nodes have been created in the world. No size hint can be set anymore.");

       communication_model_w().set_size_hint( hint );

       size_hint_ = hint;
       has_size_hint_ = true;
   }
   // ----------------------------------------------------------------------
   bool
       World::
       has_size_hint( void )
       const throw()
   {
       return has_size_hint_;
   }
   // ----------------------------------------------------------------------
   double
       World::
       size_hint( void )
       const throw(std::runtime_error)
   {
       if( has_size_hint() )
           return size_hint_;

       throw std::runtime_error("Size hint was requested without a size hint set on the world.");
   }
   // ----------------------------------------------------------------------
   const std::string&
   World::
   uuid( void )
      const throw()
   { return uuid_; }
   // ----------------------------------------------------------------------
   void
   World::
   add_node_change_listener( NodeChangeListener& ncl )
      throw()
   {
      node_change_listeners_.push_back(&ncl);
   }
   // ----------------------------------------------------------------------
   void
   World::
   remove_node_change_listener( NodeChangeListener& ncl )
      throw()
   {
	  assert ( std::find( node_change_listeners_.begin(), node_change_listeners_.end(), &ncl) !=
		       node_change_listeners_.end() );
      node_change_listeners_.remove(&ncl);
      if( ncl.invalidate() )
         delete &ncl;
   }
   // ----------------------------------------------------------------------
   void
   World::
   invalidate_node_change_listeners( void )
      throw()
   {
      for( NodeChangeListenerList::iterator it = node_change_listeners_.begin(); it != node_change_listeners_.end(); ++it )
         if( (**it).invalidate() )
            delete *it;
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/world.cpp,v $
 * Version $Revision: 518 $
 * Date    $Date: 2011-03-23 16:07:18 +0100 (Wed, 23 Mar 2011) $
 *-----------------------------------------------------------------------
 * $Log: world.cpp,v $
 *-----------------------------------------------------------------------*/
