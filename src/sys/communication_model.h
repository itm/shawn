/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_COMMUNICATION_MODEL_H
#define __SHAWN_SYS_COMMUNICATION_MODEL_H

namespace shawn
{
	class Node;
	class World;

	/// Model deciding whether nodes can hear each other
	/** Abstract superclass for communication models. The central method is can_communicate_bidi(), 
	* which determines for two specific nodes whether they may exchange messages.
	*/
	class CommunicationModel
	{
	public:

		///@name construction / destruction
		///@{
		///
		CommunicationModel();
		///
		virtual ~CommunicationModel();

		/** Sets the world that contains this model,
		*  used by World
		*  \internal */
		virtual void set_world( World& ) throw();

		/** called after world is setup and before first node is created.
		*  used by World
		*  \internal */
		virtual void init( void ) throw();
		///@}


		///@name world access
		///@{
		/** \return the containing writable World */
		virtual World& world_w( void ) throw();

		/** \return the containing const World */
		virtual const World& world( void ) const throw();
		///@}

		///@name main interface
		///@{
		/** \return \c true iff can_communicate_uni(u,v) and can_communicate_uni(v,u),
		*  therefore, whether the two nodes may exchange messages.
		*/
		virtual bool can_communicate_bidi( const Node&, const Node& ) const throw() = 0;

		/** \return \c true if the first passed node can send messages to the second.
		*  \warning currently, unidirectional links are unsupported. This method
		*           is therefore pretty much useless
		*/
		virtual bool can_communicate_uni( const Node&, const Node& ) const throw() = 0;

		/** \return whether communication_upper_bound() returns a useful value */
		virtual bool exists_communication_upper_bound( void ) const throw() = 0;

		/** if exists_communication_upper_bound(), nodes whose euclidean
		*  distance exceeds communication_upper_bound() can never communicate
		*  in any direction.
		* 
		*  Providing such a value is useful because EdgeModel instances might
		*  use the information for geographical caching.
		*/
		virtual double communication_upper_bound( void ) const throw() = 0;

		/** \return whether the status (i.e., whether two nodes can communicate)
		*  is available instantly when nodes are created.
		*/
		virtual bool is_status_available_on_construction( void ) const throw() = 0;

		/** A size hint MAY be provided by the World to the communication model what a "normal" distance in this world
		* could be. This is NO information about the actual communication range of nodes. This is just
		* a hint. Implement this method in your communication model if you want to react to this information.
		*/
		virtual void  set_size_hint(double) throw();

		///Returns true if this instance is able to anser the can_communicate_uni() and can_communicate_bidi() questions
		/** The default implementation returns true. Override this method to implement communication model specific 
		  * intelligence. This is mainly used in conjunction with MultipleCommunicationModel to combine several
		  * CommunicationModels
		  */
		virtual bool in_domain(const Node&, const Node&) const throw(); 

		///@}

	private:
		///Reference to the world
		World* world_;
	};

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/communication_model.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: communication_model.h,v $
 *-----------------------------------------------------------------------*/
