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

#ifndef __SHAWN_PROTO_AUTOCAST_MESSAGE_H
#define __SHAWN_PROTO_AUTOCAST_MESSAGE_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_AUTOCAST

#include <set>
#include <map>
#include <vector>
#include "sys/message.h"
#include "apps/autocast/autocast_app/dataunit.h"

#define AUTOCAST_TYPE_INVALID 0
#define AUTOCAST_TYPE_UPDATE  1
#define AUTOCAST_ADDRESS_INVALID -2

namespace autocast
{

   class AutoCastMessage
       : public shawn::Message
   {
   public:
     
	double next_update_interval() const throw();

	int packet_type() const throw();

	unsigned int uid() const throw();

	int last_hop_addr() const throw();

	double last_hop_x() const throw();

	double last_hop_y() const throw();

	const std::set<unsigned int>& known_DataUnits() const throw();

	const std::set<unsigned int>& stale_ids() const throw();

	const std::set<ConstDataUnitHandle>& complete_DataUnits() const throw();

	void insert_id(const unsigned int id);

	void insert_complete_DataUnit(const ConstDataUnitHandle& duh);

	void insert_stale_id(const unsigned int id);

	int size() const throw();
	
	/// Be careful whes settig the size! The size is automatically set by appending DataUnit IDs and
	/// complete DataUnits!
	void setSize(int s) throw();
 
	
    AutoCastMessage(int uid,
					int packet_type,
					double next_update_interval,
					int last_hop_addr,
					double last_hop_x,
					double last_hop_y);
    virtual ~AutoCastMessage();
   private:
	    AutoCastMessage();
	    AutoCastMessage(const AutoCastMessage&);
	    //void compute_size() throw();
		// This are the DataUnit Id's the sender of this message knows about
		std::set<unsigned int> known_DataUnits_;
		// This are the complete DataUnits the sender sends
		std::set<ConstDataUnitHandle> complete_DataUnits_;
		std::set<unsigned int> stale_ids_;
		/// The still unused packet type
		int packet_type_;
		/// The unique ID
		unsigned int uid_;
		///// The uid counter (helper variable)
		//static int uid_counter_;
		/// The AutoCast message size (helper variable)
		int autoCast_message_size_;
		/// The following three fields should be set  
		/// to the corresponding data of the source
		/// the sender of this message.
		int last_hop_addr_;
		double last_hop_x_;
		double last_hop_y_;
		/// The next expected update time in s. The nodes are requested to send
		/// periodical updates. If this time expires, the reciever of this message wants
		/// to receive a further message at that time. If this further message isn't sent.
		/// the receiver knows, the node is out of reach respectively it left the neighborhood.
		double next_update_interval_; 

   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/

