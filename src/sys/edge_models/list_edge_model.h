/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#ifndef __SHAWN_SYS_EDGE_MODELS_LIST_EDGE_MODEL_H
#define __SHAWN_SYS_EDGE_MODELS_LIST_EDGE_MODEL_H

#include "sys/edge_models/manual_edge_model.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/refcnt_pointable.h"
#include <sys/misc/dynamic_node_array.h>
#include <set>

namespace shawn
{

	//---------------------------------------------------------------------
	/** This edge model stores all neighborhood information in memory. Hence, it offers an impressive
	* message dispatching and topology lookup speed but consumes quite some memory compared to e.g. the
	* LazyEdgeModel.
	*/
	class ListEdgeModel
		: public ManualEdgeModel
	{
	public:
		DECLARE_HANDLES(NodeInfo);

		//---------------------------------------------------------------------
		/** Store the communication relation to one single other node in memory. For each communication
		* pattern, a boolean value is stored. Whenever you change the CD_IN or CD_OUT property, invoke 
		* update() to reflect the changes to CD_ANY and CD_BIDI
		*/
		class NodeInfo
			: public RefcntPointable
		{ 
		public:
			///The neighboring node
			Node* node_;
			///For each communication direction, one boolean value
			bool comm_dir_[CD___DO_NOT_USE_COUNT__];

			///Sets all values to false and node to NULL or to the given parameter
			NodeInfo(Node* node = NULL) 
				: node_(node) 
			{ 
				for(int i = 0; i < CD___DO_NOT_USE_COUNT__; ++i) 
					comm_dir_[i] = false; 
			}
			virtual ~NodeInfo();

			///Comparison function for std::set, uses ONLY the node pointer to compare two instances
			bool operator==(const NodeInfo& o) { return node_ == o.node_; }

			///Update the CD_ANY and CD_BIDI state based on CD_IN and CD_OUT
			void update() 
			{ 
				comm_dir_[CD_ANY]  = comm_dir_[CD_IN] || comm_dir_[CD_OUT];
				comm_dir_[CD_BIDI] = comm_dir_[CD_IN] && comm_dir_[CD_OUT];    
			}
		};

		//---------------------------------------------------------------------
		/// Helper class to sort containers with NodeInfoHandle / NodeInfo* instances.
		struct NodeInfoSort
		{ 
			///Compares two NodeInfoHandle / NodeInfo* instances using the member node_
			bool operator() ( const NodeInfoHandle& n1, const NodeInfoHandle& n2 ) const
			{ 
				return n1->node_ < n2->node_; 
			}
		};

		///
		typedef std::set<NodeInfoHandle, NodeInfoSort> NodeInfoSet;

	public:

		//---------------------------------------------------------------------
		/** Iterator helper used by the edge model to iterate over neighborhoods.
		*/
		template<typename NodeType, typename NodeHoodIt> 
		class ListIteratorHelper
			: public AbstractIteratorHelper<NodeType>
		{
		public:

			///
			typedef AbstractIteratorHelper<NodeType> base_type;

			/** Creates a new iterator helper instance for use with the EdgeModel
			* @param lm The list model that created this iterator helper
			* @param dir Communication direction that this iterator will deliver
			* @param n The node of which the neighbors shall be returned
			* @param s The start iterator of the list edge models internal data structure containing the complete node neighborhood
			* @param e The end iterator of the list edge models internal data structure containing the complete node neighborhood
			*/
			ListIteratorHelper( const ListEdgeModel& lm, EdgeModel::CommunicationDirection dir, NodeType& n, NodeHoodIt s, NodeHoodIt e );

			virtual ~ListIteratorHelper();
			///Skips to the first/next node that is reachable by the communication direction 
			virtual void init( void ) throw();
			///Skips to the next node that is reachable by the communication direction 
			virtual void next( void ) throw();
			///Returns the current neighboring node or NULL if the end has been reached
			virtual NodeType* current( void ) const throw();
			///
			virtual AbstractIteratorHelper<NodeType>* clone( void ) const throw();

		private:
			const ListEdgeModel& edge_model_;
			NodeHoodIt hood_it_;
			NodeHoodIt hood_end_it_;
			NodeType& node_;
		};

	public:

		///
		ListEdgeModel();
		///
		virtual ~ListEdgeModel();

		///
		virtual void set_world( World& ) throw();

		/** Do not use directly -- use add_edge() of the communication
		*  model, which should be a ManualEdgeCommunicationModel
		*/
		virtual void add_edge( Node&, Node& ) throw();

		/** Do not use directly -- use add_edge() of the communication
		*  model, which should be a ManualEdgeCommunicationModel
		*/
		virtual void add_dedge( Node&, Node& ) throw();

		///Called by the World whenever a node has been added
		virtual void node_added( Node& ) throw();

		///Called by the World whenever a node has been removed
		virtual void node_removed( Node& ) throw();

		///Returns false
		bool supports_mobility( void ) const throw();



		///Returns the number of neighboring nodes with a certain communication direction
		virtual int nof_adjacent_nodes( const Node&, EdgeModel::CommunicationDirection d = CD_BIDI  )  const throw();

		/// 
		virtual const_adjacency_iterator begin_adjacent_nodes( const Node&, EdgeModel::CommunicationDirection d = CD_BIDI  ) const throw();

		/// 
		virtual const_adjacency_iterator end_adjacent_nodes( const Node& ) const throw();

		///
		virtual adjacency_iterator begin_adjacent_nodes_w( Node&, EdgeModel::CommunicationDirection d = CD_BIDI ) throw();

		///
		virtual adjacency_iterator end_adjacent_nodes_w( Node& ) throw();


		///
		virtual Box observer_initial_zone(Node&, const Vec& pos, const Vec& velo ) throw();

		///
		Box observer_update_zone(Node&, const Vec& newpos, const Vec& velo ) throw();


	protected:
		///
		ListEdgeModel::NodeInfo& node_info(Node& u, Node& v);

		///	
		virtual void add_node_neighbors( Node& v, const Vec& pos, const Vec& velo) throw();

	private:
		///
		DynamicNodeArray<NodeInfoSet>*  neighbors_;

	};


}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/edge_models/list_edge_model.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: list_edge_model.h,v $
*-----------------------------------------------------------------------*/
