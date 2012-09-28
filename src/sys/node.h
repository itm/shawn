/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_H
#define __SHAWN_SYS_NODE_H

#include "shawn_config.h"
#include "sys/processor.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/refcnt_pointable.h"
#include "sys/vec.h"
#include "sys/message.h"
#include "sys/util/defutils.h"
#include "sys/tag_container.h"
#include "sys/node_movement.h"
#include "sys/edge_model.h"
#include "sys/world.h"
#include "sys/node_movements/movement_observable.h"

#include <typeinfo>
#include <list>
#include <queue>
#include <set>
#include <string>

#define ASSERT_SANE_NODE_IDS(u,v) \
  assert( (u==v) || (u==NULL) || (v==NULL) || (u->id()!=v->id()) )

namespace shawn
{
   DECLARE_HANDLES( Neighborhood );
   DECLARE_HANDLES( NeighborhoodConst );

   class World;

   /** Represents a sensor node. Information about the neighborhood can be queried and messages can be sent using
     * the Node's methods. A node contains so called Processor instances that are used to implement the sensor nodes 
	 * funtionality. Multiple processors can be set on a node. See Processor for more information.
     *
	 * Subclasses TagContainer to be able to contain so called tags. See TagContainer for more information 
	 * of this powerfull concept.
     * 
	 * Subclasses MovementObservable to enable MovementObserver instances to trace this nodes movements. When
	 * set_movement is called, all registered MovementObserver instances (added by MovementObservable::observer_add) 
	 * are added on the provided NodeMovement. The EdgeModel is implicitely added as an observer. 
	 */

   class Neighborhood 
      : public std::set<Node*>, public RefcntPointable{};

   class NeighborhoodConst
      : public std::set<const Node*>, public RefcntPointable{};

   class Node
      : public TagContainer,
        public MovementObservable
   {
   public:
      static const std::string WARN_HOOD_STORAGE_PARAMETER;
      typedef std::list<ProcessorHandle> ProcessorList;
      typedef EdgeModel::const_adjacency_iterator const_adjacency_iterator;
      typedef EdgeModel::adjacency_iterator adjacency_iterator;

	  //---------------------------------------------------------------------
      ///@name construction / destruction / init / reset
      ///@{

      ///
      Node();
      ///
      virtual ~Node();
      /** Set the position of a node. 
	    * Before the node has been initialized, only real_pos_ is set. 
		* After init, a call to set_real_position will result in the creation 
		* of a NoMovement NodeMovement instance with this position. This will 
		* remove any previous NodeMovement with set_movement.
		*/
      virtual void set_real_position( const Vec& ) throw();
      ///Adds a processor to this node
      virtual void add_processor( const ProcessorHandle& ) throw();
      ///Sets the current world instance on this node
      virtual void set_world( World& ) throw();
      ///Do not use directly, only for init purposes. Sets the node id
      virtual void set_id( int ) throw();
      ///Do not use directly, only for init purposes. Sets the node label
      virtual void set_label( const std::string& ) throw();
      ///Do not use directly, only for init purposes. 
      virtual void init() throw();
      ///Reset the nodes state.
      virtual void reset( void ) throw();
      ///Do not call directly. Called whenever a new round starts.
      virtual void step( void ) throw();

	  ///@}
	  //---------------------------------------------------------------------
      ///@name Message sending and receiving
      ///@{

      ///Send a message to all 1-hop neighbors
      virtual void send( const MessageHandle& ) throw();
      ///Called whenever the node receives a message.
      virtual void receive( const ConstMessageHandle& ) throw();
		/** This method is called if a Node would have received a Message (with 
		 * regard to the communication model) but that message was dropped by the
		 * transmission model. Distributes the information to all processors
		 */
      virtual void receive_dropped( const ConstMessageHandle& ) throw();

	   /** Individual transmission ranges.
	    * Allows individual transmission ranges for nodes.
	    */
	  ///Return the individual transmission range
	  virtual double transmission_range() const throw();
	  ///Sets the individual transmission range value for this node
	  virtual void set_transmission_range(double) throw();
	  
	  ///@}
	  //---------------------------------------------------------------------
      ///@name Node movements and position 
      ///@{

      ///Set the movement for this node. This will determine the nodes position
      virtual void set_movement( const NodeMovementHandle& ) throw();
      ///Get the nodes movement 
      virtual NodeMovement& movement_w( void ) throw();
      ///Get the nodes movement as const
      virtual const NodeMovement& movement( void ) const throw();
	  ///Convenience method. Calls movement().position()
      virtual Vec real_position( void ) const throw();
      ///True if the node has an estimated position
	  bool has_est_position( void ) const throw();
      ///Removes the estimated position
      void clear_est_position( void ) throw();
      ///Returns the nodes estimated position
	  virtual const Vec& est_position( void ) const throw();
      ///Sets the nodes estimated position
      void set_est_position( const Vec& ) throw();

	  ///@}
	  //---------------------------------------------------------------------
      ///@name Accelerated tag access
      ///@{
      ///
      virtual void sync_to_tags( void ) throw();
      ///
      virtual void sync_from_tags( void ) throw( std::runtime_error );

	  ///@}
	  //---------------------------------------------------------------------
      ///@name Node and other information
      ///@{
      ///Returns the nodes internal storage id. Use label() for reliable node identification
      virtual int id( void ) const throw();
      ///Returns the unique node label
      const std::string& label( void ) const throw();
	  ///Convenience method for World::simulation_round()
	  virtual int simulation_round( void ) const throw();
      ///Convenience method for World::current_time()
      virtual double current_time( void ) const throw();
      /** Returns the nodes state. The returned state depends on the state of the 
	    * Processor instances attached to the node. If at least one processor is active,
		* Active is returned, if at least on processor is sleeping, Sleeping is returned. 
		* Otherwise Inactive will be returned.
		*/
      virtual Processor::ProcessorState state( void ) const throw();
      ///Returns the const World in which the Node is living.
      virtual const World& world( void ) const throw();
      ///Returns the World in which the Node is living.
      virtual World& world_w( void ) throw();
      /** Returns true if the node is special. Special means that special_boot() will be invoked at
	    * the beginning of the simulation. This can be used e.g. to start a certain protocol. 
		*/
	  bool is_special_node( void ) const throw();
	  /// Returns an iterator to the beginning of the ProcessorList
	  ProcessorList::iterator begin_processors_w( void ) throw();
	  /// Returns a const iterator to the beginning of the ProcessorList
	  ProcessorList::const_iterator begin_processors( void ) throw();
	  /// Returns an iterator to the beginning of the ProcessorList
	  ProcessorList::iterator end_processors_w( void ) throw();
      /// Returns a const iterator to the beginning of the ProcessorList
	  ProcessorList::const_iterator end_processors( void ) throw();
	  ///Returns a Processor of a desired type. Returns NULL if no processor of this type is registered
      template<typename T>
      T* get_processor_of_type_w( void ) throw()
      {
         T* res = NULL;
         for( ProcessorList::iterator
                 it    = processors_.begin(),
                 endit = processors_.end();
              it != endit; ++it )
            if( (res=dynamic_cast<T*>(it->get())) != NULL )
               return res;
         return NULL;
      }
	  ///Returns a const Processor of a desired type. Returns NULL if no processor of this type is registered
      template<typename T>
      const T* get_processor_of_type( void ) const throw()
      {
         const T* res = NULL;
         for( ProcessorList::const_iterator
                 it    = processors_.begin(),
                 endit = processors_.end();
              it != endit; ++it )
            if( (res=dynamic_cast<const T*>(it->get())) != NULL )
               return res;
         return NULL;
      }
 
	  ///@}
	  //---------------------------------------------------------------------
      ///@name Node Neighborhood access
      ///@{
      ///
	  const_adjacency_iterator begin_adjacent_nodes( EdgeModel::CommunicationDirection d = EdgeModel::CD_BIDI ) const throw();
      ///
      const_adjacency_iterator end_adjacent_nodes( void ) const throw();
      ///
      adjacency_iterator begin_adjacent_nodes_w( EdgeModel::CommunicationDirection d = EdgeModel::CD_BIDI ) throw();
      ///
      adjacency_iterator end_adjacent_nodes_w( void ) throw();
      ///Size of the one hop neighborhood (not counting the 0-hop neighborhood)
	  int degree( EdgeModel::CommunicationDirection d = EdgeModel::CD_BIDI ) const throw();
      /** Makes the node store all neighborhoods up to size mindist.
       *  works currently only for mindist == 0 or 1.
       */
      void set_minimum_storage( int mindist ) throw();

      ///Makes the node collect and store all neighborhoods up to hop distance maxdist
      //void prepare_neighborhood( int maxdist ) const throw();
      ///Hints the node to drop stored neighborhoods of hop distance fromdist and above
      //void drop_neighborhood( int fromdist ) const throw();
      ///As prepare_neighborhood(..), but more sophisticated, supports nodes movement
      //void dynamic_neighborhood( int maxdist ) const throw();    
	  ///If neighborhood changed,  stored informations will be moved to garbage collector
      //void revalidate_data() throw();
      /** indicates for information droping, sets nh_data_is_valid_
       *  to false --> data will be calculated once again */ 
      //void invalidate_neighborhoods_data() throw();
      ///
      //virtual void add_neighbour( Node& ) throw();
      ///
      NeighborhoodHandle neighborhood_w( int maxdist, EdgeModel::CommunicationDirection d = EdgeModel::CD_BIDI ) throw();
      ///
      NeighborhoodConstHandle neighborhood( int maxdist, EdgeModel::CommunicationDirection d = EdgeModel::CD_BIDI ) const throw();

	  ///@}
	  //---------------------------------------------------------------------
      ///@name Node comparison
      ///@{
	  ///
      bool operator== ( const Node& n ) const throw()
      { return this == &n; }
      bool operator!= ( const Node& n ) const throw()
      { return this != &n; }
	  ///@}

      //---------------------------------------------------------------------
      
      ///@name Node component information
      ///@{
      ///
      const bool has_fixpoint() const throw()
      { return has_fixpoint_; }
      /**Indicates if true, that the nodes component members are computed already,
       * i.e. a fixpoint of a (multi-hop-) neighborhood is computed. */
      const int component_extension() const throw()
      { return max_hop_component_extent_; }
	 ///This returns an upper bound of hops, which are needed for this node component coverage. 
	 ///@}
  
	  std::set<const Node*> get_adjacent_nodes( EdgeModel::CommunicationDirection d = EdgeModel::CD_BIDI ) const;

	  std::set<Node*> get_adjacent_nodes_w( EdgeModel::CommunicationDirection d = EdgeModel::CD_BIDI );

   protected:

      ///@name Node movement
      ///@{
	  virtual void observers_added(MovementObserver& obs) throw();
	  ///@}

   private:
      Node( const Node& );

	  /// Returns the ProcessorList
	  const Node::ProcessorList& processors( void ) const throw();
	  /// Returns the ProcessorList
	  Node::ProcessorList& processors_w( void ) throw();

	  /// True if init has been called.
	  bool is_initialised_;
      	
	  ///Used to store the initial node position before init() has completed - Always use real_position()
      Vec    real_pos_;
	  ///
      Vec    est_pos_;
	  ///
      bool   has_est_pos_;
	  ///
      ProcessorList processors_;
      std::queue<ProcessorHandle> new_processors_;
      mutable std::vector<NeighborhoodHandle> neighborhoods_;
      World* world_;
      int id_;
      std::string label_;
      bool has_label_;
      int min_storage_;
      NodeMovementHandle movement_;
      mutable bool nh_data_is_valid_;
      ///While nh_data_is_valid_ neighborhoods_ contains valid data.
      double nh_time_stamp_;
      ///While nodes are mobile, neighborhood data may change; nh_time_stamp_ indicates the time, 
      ///when the data was generated.
      mutable bool has_fixpoint_;
      ///Is true, if the whole component has been computed, i.e. a fixpoint has been found.
      mutable int max_hop_component_extent_;
      ///Extension of this node component is at most max_hop_component_extent_ hops.
      mutable int max_hop_count_;
      ///Maximum of all values of maxdist.
	  double transmission_range_;
	  ///Individual transmission range.

  };
}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node.h,v $
 * Version $Revision: 518 $
 * Date    $Date: 2011-03-23 16:07:18 +0100 (Wed, 23 Mar 2011) $
 *-----------------------------------------------------------------------
 * $Log: node.h,v $
 *-----------------------------------------------------------------------*/
