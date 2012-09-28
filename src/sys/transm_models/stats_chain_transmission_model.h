/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANS_MODELS_STATS_CHAIN_H
#define __SHAWN_SYS_TRANS_MODELS_STATS_CHAIN_H

#include "sys/transm_models/chainable_transmission_model.h"
#include "sys/misc/dynamic_node_array.h"
#include <map>
#include <string>
#include <typeinfo>


namespace shawn
{

   ///Message counting chainable transmission model.
/** Messages are not altered, just counted.
 * dump_stats() will display the current counters on the console
 */
   class StatsChainTransmissionModel
      : public ChainableTransmissionModel
   {
   public:
      static const int UNKNOWN;
      enum DetailLevel
         { Disabled, Totals, ByMessageType };
      class Counter
      {
      public:
         typedef std::pair<int,int> MessageCount;
         typedef std::map<std::string,MessageCount> MessageCountByType;
         typedef std::pair<MessageCountByType,int> RefMessageCountByType;
         Counter();
         Counter( const Counter& );
         ~Counter();

         void reset( void );
         int messages( void ) const throw();
         int size( void ) const throw();
         int messages_of_type( const std::type_info& ) const throw();
         int size_of_type( const std::type_info& ) const throw();

         void add( StatsChainTransmissionModel::DetailLevel,
                   const Message& ) throw();
         void dump( std::ostream&, const std::string& ) const throw();
      private:
         MessageCount                             count_;
         RefMessageCountByType*                   count_by_type_;
      };






      ///@name Construction, destruction and lifecycle support
      ///@{
      ///
      StatsChainTransmissionModel();
      ///
      virtual ~StatsChainTransmissionModel();
      ///Init code
      virtual void init( void ) throw();
      ///Dump the current stats and reset the counters
      virtual void reset( void ) throw();
      ///@}


      ///@name Transmission model implementation
      ///@{
      ///Counts the messages per message class and passes them on
      virtual void send_message( TransmissionModel::MessageInfo& mi ) throw();
      ///@}


      ///@name Configuration
      ///@{
      ///
      inline DetailLevel sender_detail( void ) const throw()
      { return sender_detail_; }
      ///
      inline DetailLevel receiver_detail( void ) const throw()
      { return receiver_detail_; }
      ///
      inline DetailLevel edge_detail( void ) const throw()
      { return edge_detail_; }
      ///
      inline DetailLevel general_detail( void ) const throw()
      { return general_detail_; }
      ///
      inline bool split_broadcasts( void ) const throw()
      { return split_broadcasts_; }
      ///
      void set_sender_detail( DetailLevel ) throw();
      ///
      void set_receiver_detail( DetailLevel ) throw();
      ///
      void set_edge_detail( DetailLevel ) throw();
      ///
      void set_general_detail( DetailLevel ) throw();
      ///
      void set_split_broadcasts( bool ) throw();
      ///@}


      ///@name Info access
      ///@{
      ///
//       int total_sent_messages( void ) const throw();
//       int total_sent_size( void ) const throw();
//       int sent_messages( const Node& ) const throw();
//       int sent_size( const Node& ) const throw();
//       int received_messages( const Node& ) const throw();
//       int received_size( const Node& ) const throw();
//       int sent_over_messages( const Node&, const Node& ) const throw();
//       int sent_over_size( const Node&, const Node& ) const throw();
//       int total_sent_messages_of_type( const type_info& ) const throw();
//       int total_sent_size_of_type( const type_info& ) const throw();
//       int sent_messages_of_type( const Node&, const type_info&) const throw();
//       int sent_size_of_type( const Node&, const type_info&) const throw();
//       int received_messages_of_type( const Node&, const type_info&) const throw();
//       int received_size_of_type( const Node&, const type_info&) const throw();
//       int sent_over_messages_of_type( const Node&, const Node&, const type_info&) const throw();
//       int sent_over_size_of_type( const Node&, const Node&, const type_info&) const throw();
      ///@}

      ///@name Output
      ///@{
      ///Simple information display
      virtual void dump_stats( std::ostream& ) const throw();
      ///Simple information display
      virtual void dump_node_stats( const Node&, std::ostream& ) const throw();
      ///@}


   private:
   ///
      DynamicNodeArray<Counter>* sender_info_;
      ///
      DynamicNodeArray<Counter>* receiver_info_;
      ///
      Counter* general_info_;
      ///
      DetailLevel sender_detail_;
      ///
      DetailLevel receiver_detail_;
      ///
      DetailLevel edge_detail_;
      ///
      DetailLevel general_detail_;
      ///
      bool split_broadcasts_;
      ///
      bool initialized_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/stats_chain_transmission_model.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: stats_chain_transmission_model.h,v $
 *-----------------------------------------------------------------------*/
