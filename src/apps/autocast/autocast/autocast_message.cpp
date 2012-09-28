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

#include "autocast_message.h"

#ifdef ENABLE_AUTOCAST

namespace autocast
{

   AutoCastMessage::
	   AutoCastMessage() :	packet_type_(AUTOCAST_TYPE_INVALID),
							uid_(0),
							autoCast_message_size_(0),
							last_hop_addr_(0),
							last_hop_x_(0),
							last_hop_y_(0),
							next_update_interval_(0)
   {}
   AutoCastMessage::AutoCastMessage(int uid,
									int packet_type,
									double next_update_interval,
									int last_hop_addr,
									double last_hop_x,
									double last_hop_y) : packet_type_(packet_type),
														 uid_(uid),
														 autoCast_message_size_(0),
														 last_hop_addr_(last_hop_addr),
														 last_hop_x_(last_hop_x),
														 last_hop_y_(last_hop_y),
														 next_update_interval_(next_update_interval)
   {}
   // ----------------------------------------------------------------------
   AutoCastMessage::
   ~AutoCastMessage()
   {}
   // ----------------------------------------------------------------------
   double
	   AutoCastMessage::
	   next_update_interval()
	   const
	   throw()
   {
	   return next_update_interval_;
   }
   // ----------------------------------------------------------------------
   int
	   AutoCastMessage::
	   packet_type()
	   const
	   throw()
   {
	   return packet_type_;
   }
   // ----------------------------------------------------------------------
   unsigned int
	   AutoCastMessage::
	   uid()
	   const
	   throw()
   {
		return uid_;
   }
   // ----------------------------------------------------------------------
   int
	   AutoCastMessage::
	   last_hop_addr()
	   const
	   throw()
   {
	   return last_hop_addr_;
   }
   // ----------------------------------------------------------------------
   double
	   AutoCastMessage::
	   last_hop_x()
	   const
	   throw()
   {
	   return last_hop_x_;
   }
   // ----------------------------------------------------------------------
   double
	   AutoCastMessage::
	   last_hop_y()
	   const
	   throw()
   {
	   return last_hop_y_;
   }
   // ----------------------------------------------------------------------
   const std::set<unsigned int>&
	   AutoCastMessage::
	   known_DataUnits()
	   const
	   throw()
   {
	   return known_DataUnits_;
   }
   // ----------------------------------------------------------------------
   const std::set<autocast::ConstDataUnitHandle>&
	   AutoCastMessage::
	   complete_DataUnits()
	   const
	   throw()
   {
	   return complete_DataUnits_;
   }

   // ----------------------------------------------------------------------
   const std::set<unsigned int>&
	   AutoCastMessage::
	   stale_ids()
	   const
	   throw()
   {
		return stale_ids_;
   }
   // ----------------------------------------------------------------------
   void
	   AutoCastMessage::
	   insert_id(const unsigned int id)
   {
	   known_DataUnits_.insert(id);
	   autoCast_message_size_ += 4;
   }
   // ----------------------------------------------------------------------
   void
	   AutoCastMessage::
	   insert_complete_DataUnit(const ConstDataUnitHandle& duh)
   {
	   complete_DataUnits_.insert(duh);
	   autoCast_message_size_ += duh->size();
   }
   // ----------------------------------------------------------------------
   void
	   AutoCastMessage::
	   insert_stale_id(unsigned int id)
   {
	   stale_ids_.insert(id);
	   autoCast_message_size_ += 4;
   }
   // ----------------------------------------------------------------------
   int
	   AutoCastMessage::
	   size()
	   const
	   throw()
   {
	   /// Returns the size of the user data AND the size of the header
	   /// which is what is sent with every message.
	   /// And the overhead is:
	   /// 4 for packet_type_ int
	   /// 4 for uid_ int
	   /// 4 last_hop_addr_ int
	   /// 8 last_hop_x_ double
	   /// 8 last_hop_y_ double
	   /// 8 next_update_interval_ double
	   /// 4 for known_id size int
	   /// 4 complete_DataUnits size int
	   /// +____________________________
	   /// Makes 44 bytes total overhead
	   /// -----------------------------
	   /// -----------------------------
	   return autoCast_message_size_ + 44;
   }
   // ----------------------------------------------------------------------
   void
	   AutoCastMessage::
	   setSize(int s)
	   throw()
   {
		autoCast_message_size_ = s;
   }
}
#endif

/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/
