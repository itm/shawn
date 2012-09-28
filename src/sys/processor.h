/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_PROCESSOR_H
#define __SHAWN_SYS_PROCESSOR_H

#include "shawn_config.h"
#include "sys/util/refcnt_pointable.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/message.h"
#include "sys/util/defutils.h"
#include "sys/tag_syncable.h"
#include "sys/logging/logger.h"



namespace shawn
{
   class Node;

   DECLARE_HANDLES(Processor);


   /// Message-processing protocol resp. algorithm for nodes
   /** Superclass for anything that runs inside a node and
    *  processes messages. A node can contain multiple processors, eg, one for
    *  each protocol that is simulated simultaneously.
    *
    *  Most important methods are
    *  - boot(), which is called once after startup
    *  - work(), which is called periodically
    *  - process_message(), which is called for every message a node receives
    *  - send(), which sends a message
    */
   class Processor
      : public RefcntPointable,
        public TagSyncable
#ifdef ENABLE_LOGGING
        , public Logger
#endif
   {
   public:
	   /** 
     * Collection of possible return values of the State of the radio. The are handed
     * over the a Sender's confirm method if a Sender was handed over when callign Radio::send. */
    typedef enum 
      {
		SHAWN_TX_STATE_SUCCESS = 0,             /**< Success (0x00) */
		SHAWN_TX_STATE_BUFFER_OVERFLOW = 1,		/**< Radio send queue full (0x01) */
		SHAWN_TX_STATE_RADIO_NOT_ACTIVE = 2,	/**< Radio switched off (0x00) */
		SHAWN_TX_STATE_BEACON_LOSS = 0xE0,      /**< Beacon loss after synchronisation request (0xE0) */
		SHAWN_TX_STATE_CHANNEL_ACCESS_FAILURE,  /**< CSMA/CA channel access failure (0xE1) */
		SHAWN_TX_STATE_DENIED,                  /**< GTS request denied (0xE2) */
		SHAWN_TX_STATE_DISABLE_TRX_FAILURE,     /**< Could not disable transmit or receive (0xE3) */
		SHAWN_TX_STATE_FAILED_SECURITY_CHECK,   /**< Incoming frame failed security check (0xE4) */
		SHAWN_TX_STATE_FRAME_TOO_LONG,          /**< Frame too long after security processing to be sent (0xE5) */
		SHAWN_TX_STATE_INVALID_GTS,             /**< GTS transmission failed (0xE6) */
		SHAWN_TX_STATE_INVALID_HANDLE,          /**< Purge request failed to find entry in queue (0xE7) */
		SHAWN_TX_STATE_INVALID_PARAMETER,       /**< Out-of-range parameter in primitive (0xE8) */
		SHAWN_TX_STATE_NO_ACK,                  /**< No acknowledgement received when expected (0xE9) */
		SHAWN_TX_STATE_NO_BEACON,               /**< Scan failed to find any beacons (0xEA) */
		SHAWN_TX_STATE_NO_DATA,                 /**< No response data after a data request (0xEB) */
		SHAWN_TX_STATE_NO_SHORT_ADDRESS,        /**< No allocated short address for operation (0xEC) */
		SHAWN_TX_STATE_OUT_OF_CAP,              /**< Receiver enable request could not be executed as CAP finished (0xED) */
		SHAWN_TX_STATE_PAN_ID_CONFLICT,         /**< PAN ID conflict has been detected (0xEE) */
		SHAWN_TX_STATE_REALIGNMENT,             /**< Coordinator realignment has been received (0xEF) */
		SHAWN_TX_STATE_TRANSACTION_EXPIRED,     /**< Pending transaction has expired and data discarded (0xF0) */
		SHAWN_TX_STATE_TRANSACTION_OVERFLOW,    /**< No capacity to store transaction (0xF1) */
		SHAWN_TX_STATE_TX_ACTIVE,               /**< Receiver enable request could not be executed as in transmit state (0xF2) */
		SHAWN_TX_STATE_UNAVAILABLE_KEY,         /**< Appropriate key is not available in ACL (0xF3) */
		SHAWN_TX_STATE_UNSUPPORTED_ATTRIBUTE,   /**< PIB Set/Get on unsupported attribute (0xF4) */
		SHAWN_TX_STATE_BUSY,          			/**< an other error occured (0xF5) */
		SHAWN_TX_STATE_UNKNOWN_ERROR          	/**< an other error occured (0xF6) */
      } tx_state;
			

      enum ProcessorState
         {
            Active,   ///< usual state
            Sleeping, ///< message reception stopped, but work() continues
            Inactive  ///< nonrevertable death
         };
      
      ///@name construction / destruction
      ///@{
		///
		Processor();
		///
		virtual ~Processor();
      ///@}


      ///@name node membership
      ///@{

		/** called by Node when processor gets added */
		virtual void set_owner( Node& ) throw();

		/** \return const Node that contains this Processor */
		virtual const Node& owner( void ) const throw();

		/** \return writable Node that contains this Processor */
		virtual Node& owner_w( void ) throw();

      ///@}

      ///@name processor interface
      ///@{

		/** This method is called \em once for every Processor that is
		*  contained in one special node, which is chosen arbitrarily. It
		*  can be identified by Node::is_special_node() 
		*
		*  It is called before boot() occurs. A Processor whose special_boot()
		*  is called will still get a boot() afterwards.
		*/
		virtual void special_boot( void ) throw();

		/** This method is called once for every Processor before the
		*  simulation starts (and again, if the simulation is reset)
		*/
		virtual void boot( void ) throw();

		/** This method is called if a Node receives a Message that is
		*  to be processed. Overload this method in derived
		*  Processors to produce specific protocols/algorithms.
		*
		*  \return \c true iff the Processor recognizes the passed
		*  message to be of "his own type". This makes the Node stop
		*  processing the message by passing it to all his Processors.
		*/
		virtual bool process_message( const ConstMessageHandle& ) throw();

		/** This method is called if a Node would have received a Message (with 
		 * regard to the communication model) but that message was dropped by the
		 * transmission model. Overload this method in derived
		*  Processors to produce specific protocols/algorithms.
		*
		*  \return \c true iff the Processor recognizes the passed
		*  message to be of "his own type". This makes the Node stop
		*  processing the message by passing it to all his Processors.
		*/
		virtual bool process_dropped_message( const shawn::ConstMessageHandle& ) throw() {return false;}
		/** This method is called by the transmission model 
		* if a Message has been sent out and if a processor is set as sender_proc_ 
		* of the message. Overload this method in derived
		*  Processors to produce specific protocols/algorithms.
		*
		*  \return \c true iff the Processor recognizes the passed
		*  message to be of "his own type". This makes the Node stop
		*  processing the message by passing it to all his Processors.
		*/
		virtual bool process_sent_indication( const ConstMessageHandle&, tx_state state, int tries ) throw();

		/** Called once in each simulation round. Used for periodic tasks
		*  like collecting sensor values and generating messages if someting
		*  interesting has happened.
		*/
		virtual void work( void ) throw();

      ///@}


      ///@name misc access
      ///@{

		/** \return the containing Node's id number
		*  \sa Node::id()  */
		virtual int id( void ) const throw();

		/** \return the state of the Processor
		*  \sa set_state() */
		virtual ProcessorState state( void ) const throw();

		/** \return 
		*  \sa set_auto_terminate() */
		bool auto_terminate( void ) const throw();

		/** \return the current iteration round */
		virtual int simulation_round( void ) const throw();

		/** to be implemented */
		virtual void sync_to_tags( TagContainer& ) throw();

		/** to be implemented */
		virtual void sync_from_tags( const TagContainer& ) throw( std::runtime_error );

      ///@}

		///@name State management
		///@{

		/** Sets the state of the processor, where
		 *  - Processor::Active   denotes the usual state
		 *  - Processor::Sleeping stops message reception, but work() continues
		 *  - Processor::Inactive is final
		 */
		virtual void set_state( const ProcessorState& ) throw();

		/** Sets the state of the processor, where
		 */
		void set_auto_terminate( bool enable ) throw();

		///@}
		
   protected:

      ///@name Message sending
      ///@{

		/** Enqueues a message for sending. Because the message is
		*  passed as a handle (see refcnt_pointer), a construct like
		*  \code
		*  send( new MyCoolMessage(1,2,3,4) );
		*  \endcode
		*  is fine. Whether a message is broadcast or
		*  unicast is decided by Message::is_unicast(). 
		*/
		virtual void send( const MessageHandle& ) throw();
      ///@}

   private:
      Node*          owner_;
      ProcessorState state_;
	  bool			 auto_terminate_;
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/processor.h,v $
 * Version $Revision: 320 $
 * Date    $Date: 2009-02-09 10:53:00 +0100 (Mon, 09 Feb 2009) $
 *-----------------------------------------------------------------------
 * $Log: processor.h,v $
 *-----------------------------------------------------------------------*/
