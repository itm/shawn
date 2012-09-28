/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/

#ifndef __SHAWN_SYS_EDGE_MODEL_H
#define __SHAWN_SYS_EDGE_MODEL_H

#include "sys/node_movements/movement_observer.h"
#include "sys/misc/node_change_listener.h"
#include <functional>

namespace shawn
{
	class World;
	class Node;
	class CommunicationModel;
	class Box;
	class Vec;


	//---------------------------------------------------------------------
	/**
	  *
	  */
	class EdgeModel 
		: public MovementObserver,
		public NodeChangeListener
	{
	public:
		/**
		  */
		enum CommunicationDirection 
		{
			CD_IN = 0, CD_OUT, CD_BIDI, CD_ANY, 
			CD___DO_NOT_USE_COUNT__ ///Always leave this at the end. Used to count the elements of this enum
		};

		//---------------------------------------------------------------------
		/**
		*
		*/
		template<typename NodeType> 
		class AbstractIteratorHelper
		{
		protected:
			EdgeModel::CommunicationDirection direction_;

		public:
			AbstractIteratorHelper(EdgeModel::CommunicationDirection dir) 
				: direction_(dir) {}
			virtual ~AbstractIteratorHelper();
			virtual void init( void ) throw() = 0;
			virtual void next( void ) throw() = 0;
			virtual NodeType* current( void ) const throw() = 0;
			virtual AbstractIteratorHelper<NodeType>* clone( void ) const throw() = 0;
		};

	public:

		//---------------------------------------------------------------------
		/**
		*
		*/
		template<typename NodeType>
		class AbstractAdjacencyIterator
		{
		public:
			typedef AbstractIteratorHelper<NodeType> Helper;
			AbstractAdjacencyIterator();
			AbstractAdjacencyIterator( const AbstractAdjacencyIterator<NodeType>& );
			AbstractAdjacencyIterator( Helper* );
			~AbstractAdjacencyIterator();

			AbstractAdjacencyIterator& operator++ ( void ) throw();
			NodeType& operator* ( void ) const throw();
			NodeType* operator -> ( void ) const throw();

			bool operator == ( const AbstractAdjacencyIterator<NodeType>& ) const throw();
			bool operator != ( const AbstractAdjacencyIterator<NodeType>& ) const throw();

			AbstractAdjacencyIterator& operator = ( const AbstractAdjacencyIterator<NodeType>& ) throw();

		private:
			Helper* helper_;
		};

	public:
		typedef AbstractAdjacencyIterator<Node> adjacency_iterator;
		typedef AbstractAdjacencyIterator<const Node> const_adjacency_iterator;

		///@name 
		///@{

			///
			EdgeModel();
			
			///
			virtual ~EdgeModel();
			
			///
			virtual void set_world( World& ) throw();
			
			///
			virtual void set_communication_model( const CommunicationModel& ) throw();
			
			///
			World& world_w( void ) throw();
			
			///
			const World& world( void ) const throw();
			
			///
			const CommunicationModel& communication_model( void ) const throw();
			
			///
			virtual void init( void ) throw();

			///
			virtual bool supports_mobility( void ) const throw() = 0;

		///@}

		///@name 
		///@{

			///
			virtual int nof_adjacent_nodes( const Node&, CommunicationDirection d = CD_BIDI)  const throw() = 0;

			///
			virtual const_adjacency_iterator begin_adjacent_nodes( const Node&, CommunicationDirection  d = CD_BIDI) const throw() = 0;

			///
			virtual const_adjacency_iterator end_adjacent_nodes( const Node& ) const throw() = 0;

			///
			virtual adjacency_iterator begin_adjacent_nodes_w( Node&, CommunicationDirection d = CD_BIDI) throw() = 0;

			///
			virtual adjacency_iterator end_adjacent_nodes_w( Node& ) throw() = 0;

		///@}



		///@name 
		///@{

			/// Just for completeness -- may query CommunicationModel!
			virtual bool can_communicate_bidi( const Node&, const Node& ) const throw();

			/// 
			virtual bool can_communicate_uni( const Node& u, const Node& v ) const throw();

			/// 
			virtual bool are_adjacent( const Node& u, const Node& v, CommunicationDirection d = CD_BIDI ) const throw();

		///@}


		///@name methods inherited from World::NodeChangeListener
		///@{

			///
			virtual void node_added( Node& ) throw() = 0;
			///
			virtual void node_removed( Node& ) throw() = 0;
			///
			virtual void id_changed( int, int ) throw();
			///
			virtual bool invalidate( void ) throw();

		///@}

	private:
		World* world_;
		const CommunicationModel* comm_model_;
	};

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/edge_model.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: edge_model.h,v $
*-----------------------------------------------------------------------*/
