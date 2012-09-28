/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/message.h"
#include "sys/node.h"


namespace shawn
{

   Message::
   Message()
      : source_      ( NULL ),
        sender_proc_ ( NULL ),
        stamp_round_ ( -1 ),
        stamp_time_  ( -1.0 ),
        size_( 1 ),
        ack_(false)
   {}
   // ----------------------------------------------------------------------
   Message::
   ~Message()
   {}
   // ----------------------------------------------------------------------
   bool
   Message::
   is_unicast( void )
      const throw()
   { return false; }
   // ----------------------------------------------------------------------
   int
   Message::
   unicast_target( void )
      const throw()
   { return -1; }
   // ----------------------------------------------------------------------
   bool
   Message::
   is_loopback_message( void )
      const throw()
   { return false; }
   // ----------------------------------------------------------------------
   const Node&
   Message::
   source( void )
      const throw()
   {
      assert( source_ != NULL );
      return *source_;
   }
   // ----------------------------------------------------------------------
   Node&
   Message::
   source_w( void )
      const throw()
   {
      assert( source_ != NULL );
      return *source_;
   }
   // ----------------------------------------------------------------------
   bool
   Message::
   has_source( void )
      const throw()
   {
      return source_ != NULL;
   }
   // ----------------------------------------------------------------------
   double
   Message::
   transmission_range()
   {
      if(has_source())
      {
         return source_->transmission_range();
      }
      else
      {
         return 1.0;
      }
   }
   // ----------------------------------------------------------------------
   const Processor&
   Message::
   sender_proc( void )
      const throw()
   {
      assert( sender_proc_ != NULL );
      return *sender_proc_;
   }
   // ----------------------------------------------------------------------
   Processor&
   Message::
   sender_proc_w( void )
      const throw()
   {
      assert( sender_proc_ != NULL );
      return *sender_proc_;
   }
   // ----------------------------------------------------------------------
   int
   Message::
   timestamp_simulation_round( void )
      const throw()
   {
      assert( stamp_round_ >= 0 );
      return stamp_round_;
   }
   // ----------------------------------------------------------------------
   double
   Message::
   timestamp_time( void )
      const throw()
   {
      assert( stamp_time_ > -.5 );
      return stamp_time_;
   }
   // ----------------------------------------------------------------------
   void
   Message::
   set_source( Node& v ) 
      throw()
   { source_=&v; }
   // ----------------------------------------------------------------------
   void
   Message::
   set_sender_proc( Processor& v ) 
      throw()
   { sender_proc_=&v; }
   // ----------------------------------------------------------------------
   void
   Message::
   set_timestamp( int tsr, double tst ) 
      throw()
   { 
      assert( tsr >= 0 );
      assert( tst > -.5 );
      stamp_round_=tsr;
      stamp_time_=tst;
   }
   // ----------------------------------------------------------------------
   int
   Message::
   size( void )
   const throw()
   { 
	   return size_;
   }
   // ----------------------------------------------------------------------
   void 
   Message::
   setSize(int size)
   throw()
   {
		size_=size;
   }

   // ----------------------------------------------------------------------
   void  
   Message::
   set_ack( bool ack )
   {
	   ack_=ack;
   }
   
   // ----------------------------------------------------------------------
   bool  
   Message::
   is_ack( ) const
   {
	   return ack_;
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/message.cpp,v $
 * Version $Revision: 370 $
 * Date    $Date: 2009-11-12 14:12:56 +0100 (Thu, 12 Nov 2009) $
 *-----------------------------------------------------------------------
 * $Log: message.cpp,v $
 *-----------------------------------------------------------------------*/
