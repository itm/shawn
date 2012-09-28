/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_WORLD_H
#define __SHAWN_SYS_WORLD_H

#include <vector>
#include <list>
#include <set>
#include <queue>

#include "sys/edge_model.h"
#include "sys/node.h"
#include "sys/simulation/simulation_task.h"
#include "sys/tag_container.h"
#include "sys/logging/logger.h"
#include "sys/misc/deref_iterator.h"
#include "sys/misc/skipnull_iterator.h"
#include "sys/misc/node_change_listener.h"

namespace shawn
{
   class EventScheduler;
   class EdgeModel;
   class TransmissionModel;
   class CommunicationModel;
   class SimulationController;
   class MovementController;

   class World
      : public TagContainer
#ifdef ENABLE_LOGGING
        , public Logger
#endif
   {
private:
      ///
      typedef
      std::vector<Node*>
      NodeVector;

public:      
      ///
      typedef
      SkipNullIterator<const Node,NodeVector,NodeVector::const_iterator>
      const_node_iterator;
      ///
      typedef
      SkipNullIterator<Node,NodeVector,NodeVector::iterator>
      node_iterator;
      ///
      typedef
      EdgeModel::adjacency_iterator
      adjacency_iterator;
      ///
      typedef
      EdgeModel::const_adjacency_iterator
      const_adjacency_iterator;
      ///
      typedef
      std::list<SimulationTaskHandle>
      SimulationTaskList;
      ///
      typedef
      std::list<NodeChangeListener*>
      NodeChangeListenerList;

      ///@name construction / destruction
      ///@{
      ///
      World();
      ///
      virtual ~World();
      ///
      virtual void init( void ) throw();
      ///@}


      ///@name World setup
      ///@{
      /// World considers itself node owner!
      virtual void add_node( Node& ) throw(std::logic_error);
      /** Removes a node from the world. First, the observers are informed
       *  that the node will no longer be available. Then, the associated
       *  members of the NodeChangeListenerList are informed. At last, the
       *  node is destroyed and marked as deleted in the internal
       *  representation.
       *
       *  \param n The node that is ought to be deleted.
       */
      virtual void remove_node( Node& n) throw(std::logic_error);
      /// New nodes will get the processors defined with this method attached when they are added to this world instance
      virtual void add_default_processor( const std::string& ) throw();
      /// world cares for model deletion
      virtual void set_edge_model( EdgeModel& ) throw();
      /// Set the transmission model that will be used for further messages exchanges. World cares for model deletion.
      virtual void set_transmission_model( TransmissionModel& ) throw();
      /// world cares for model deletion
      virtual void set_communication_model( CommunicationModel& ) throw();
      ///
      virtual const EdgeModel& edge_model( void ) const throw();
      ///
      virtual EdgeModel& edge_model_w( void ) throw();
      ///Returns the transmission model that is currently used in the world
      virtual const TransmissionModel& transmission_model( void ) const throw();
      ///Returns the (writable, non const) transmission model that is currently used in the world
      virtual TransmissionModel& transmission_model_w( void ) throw();
      ///
      virtual const CommunicationModel& communication_model( void ) const throw();
      ///
      virtual CommunicationModel& communication_model_w( void ) throw();
      ///
      virtual void set_simulation_controller( SimulationController& ) throw();
      ///
      virtual SimulationController& simulation_controller_w( void ) throw();
      ///
      virtual const SimulationController& simulation_controller( void ) const throw();
      ///
      virtual void set_movement_controller( MovementController* ) throw();

      virtual MovementController& movement_controller_w( void ) throw();
      ///
      virtual const MovementController& movement_controller( void ) const throw();
      ///

      const Vec&
      lower_left( void )
         const throw();
      ///
      const Vec&
      upper_right( void )
         const throw();
      ///
      virtual void add_edge( Node&,
                             Node& )
         throw();
      ///
      virtual void clear( void )
         throw();
      /** You can provide a hint to the world what a "proper" communication distance in this world
        * could be. This is NO information about the actual communication range of single nodes. This is just
        * a hint. This hint can only be set once before any node has been created. This hint will be propagated to
        * the communication model.
        */
       virtual void set_size_hint(double hint) throw(std::logic_error);
      ///
       virtual const std::string& uuid( void ) const throw();
      ///@}



      ///@name Status query
      ///@{
      ///
      virtual int active_nodes_count( void )
         const throw();
      ///
      virtual int sleeping_nodes_count( void )
         const throw();
      ///
      virtual int inactive_nodes_count( void )
         const throw();
      /// returns whether simulation is done, i.e., all nodes are inactive
      virtual bool is_done( void )
         const throw();
      ///
      virtual int simulation_round( void )
         const throw();
      ///
      virtual double current_time( void )
         const throw();
      ///Checks whether size_hint() would return a value or throws a std::runtime_error
      virtual bool has_size_hint( void )
         const throw();
      /** Returns a hint what a "proper" communication distance in this world could be.
        * This is NO information about the actual communication range of single nodes. This is just
        * a hint. Check has_size_hint() before you access this method if a size hint has been set.
        * @see set_size_hint
        * @throws std::runtime_error Thrown if no size hint has been set on this world instance
        */
      virtual double size_hint( void )
         const throw(std::runtime_error);
      ///@}


      ///@name Simulation
      ///@{
      ///
      virtual void step( void )
         throw();
      ///
      virtual void
      send_message( const Node&          source,
                    const MessageHandle& mh )
         throw();
      /// resets the simulation (ie, nodes are not deleted)
      virtual void
      reset( void )
         throw();
      ///@}


      ///@name NodeChangeListener s
      ///@{
      /// adds a NodeChangeListener
      virtual void add_node_change_listener( NodeChangeListener& ) throw();
      /// removes a NodeChangeListener
      virtual void remove_node_change_listener( NodeChangeListener& ) throw();
      ///@}



      virtual bool
      can_communicate_bidi( const Node&,
                            const Node& )
         const throw();
      virtual bool
      can_communicate_uni( const Node&,
                           const Node& )
         const throw();




      ///@name Access to nodes
      ///@{
      ///
      inline int node_count( void ) const throw()
      { return valid_node_cnt_; }
      /// returns the size of the node ID space, i.e.,
      /// all nodes have an ID in the range [0...node_id_space_size()-1]
      /// used in DynamicNodeArray and StaticNodeArray
      inline int node_id_space_size( void ) const throw()
      { return nodes_.size(); }
      ///
      node_iterator begin_nodes_w( void ) throw();
      ///
      node_iterator end_nodes_w( void ) throw();
      /// returns NULL if none found
      Node* find_node_by_id_w( int ) throw();
      /// returns NULL if none found, does *not* check for duplicates!
      Node* find_node_by_label_w( const std::string& ) throw();
      ///
      const_node_iterator begin_nodes( void ) const throw();
      ///
      const_node_iterator end_nodes( void ) const throw();
      /// returns NULL if none found
      const Node* find_node_by_id( int ) const throw();
      /// returns NULL if none found, does *not* check for duplicates!
      const Node* find_node_by_label( const std::string& ) const throw();
      ///
      inline const_adjacency_iterator begin_adjacent_nodes
		  ( const Node& v, EdgeModel::CommunicationDirection d = EdgeModel::CD_BIDI) const throw()
      { return edge_model().begin_adjacent_nodes(v, d); }
      ///
      inline const_adjacency_iterator
      end_adjacent_nodes( const Node& v )
         const throw()
      { return edge_model().end_adjacent_nodes(v); }
      ///
      inline adjacency_iterator
		  begin_adjacent_nodes_w( Node& v, EdgeModel::CommunicationDirection d = EdgeModel::CD_BIDI )
         throw()
      { return edge_model_w().begin_adjacent_nodes_w(v, d); }
      ///
      inline adjacency_iterator
      end_adjacent_nodes_w( Node& v )
         throw()
      { return edge_model_w().end_adjacent_nodes_w(v); }
      ///@}

      EventScheduler& scheduler_w( void ) throw();
      const EventScheduler& scheduler( void ) const throw();

      virtual void
      add_pre_step_task( const SimulationTaskHandle& )
         throw();
      virtual void
      add_post_step_task( const SimulationTaskHandle& )
         throw();

   protected:
      virtual void pre_step( void )
         throw();
      virtual void run_nodes( void )
         throw();
      virtual void post_step( void )
         throw();

      ///@name NodeChangeListener protected stuff
      ///@{
      ///
      virtual void invalidate_node_change_listeners( void ) throw();
      ///@}

   private:

      ///@name private access to nodes
      ///@{
      ///
      NodeVector& nodes_w( void ) throw();
      ///
      const NodeVector& nodes( void ) const throw();
      ///@}

      ///@name private data memberts
      ///@{
      ///
      NodeVector nodes_;
      ///
      int valid_node_cnt_;
      ///
      std::queue<int> marked_ids_;
      
      ///
      int round_;
      ///
      int cnt_act_;
      ///
      int cnt_slp_;
      ///
      int cnt_ina_;
      ///See set_size_hint for description
      double size_hint_;
      ///See has_size_hint for description
      bool has_size_hint_;
      ///
      Vec lower_left_;
      ///
      Vec upper_right_;

      ///
      EventScheduler* sched_;
      ///
      EdgeModel* edge_model_;
      ///
      TransmissionModel* transmission_model_;
      ///
      CommunicationModel *comm_model_;
      ///
      SimulationController *controller_;
      ///
      MovementController *movement_controller_;
      ///
      SimulationTaskList prestep_tasks_;
      ///
      SimulationTaskList poststep_tasks_;
      ///
      std::string uuid_;
      ///
      NodeChangeListenerList node_change_listeners_;
      ///
      std::set<std::string> default_processors_;
      ///@}
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/world.h,v $
 * Version $Revision: 504 $
 * Date    $Date: 2011-01-24 20:00:04 +0100 (Mon, 24 Jan 2011) $
 *-----------------------------------------------------------------------
 * $Log: world.h,v $
 *-----------------------------------------------------------------------*/
