/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/transm_models/stats_chain_transmission_model.h"
#include "sys/edge_model.h"
#include "sys/message.h"
#include "sys/world.h"
#include "sys/util/defutils.h"

using namespace std;

namespace shawn
{
   const int StatsChainTransmissionModel::UNKNOWN( -1 );
   // ----------------------------------------------------------------------
   StatsChainTransmissionModel::Counter::
   Counter( void )
      : count_         ( 0,0 ),
        count_by_type_ ( NULL )
   {}
   // ----------------------------------------------------------------------
   StatsChainTransmissionModel::Counter::
   Counter( const StatsChainTransmissionModel::Counter& c )
      : count_         ( c.count_ ),
        count_by_type_ ( c.count_by_type_ )
   { 
      if( count_by_type_ != NULL ) 
         ++count_by_type_->second;
   }
   // ----------------------------------------------------------------------
   StatsChainTransmissionModel::Counter::
   ~Counter()
   {
      if( count_by_type_ != NULL )
         if( (--count_by_type_->second) == 0 )
            delete count_by_type_;
   }
   // ----------------------------------------------------------------------
   void
   StatsChainTransmissionModel::Counter::
   reset( void )
   {
      if( count_by_type_ != NULL )
         count_by_type_->first.clear();
   }
   // ----------------------------------------------------------------------
   int
   StatsChainTransmissionModel::Counter::
   messages( void ) 
      const throw()
   { return count_.first; }
   // ----------------------------------------------------------------------
   int
   StatsChainTransmissionModel::Counter::
   size( void ) 
      const throw()
   { return count_.second; }
   // ----------------------------------------------------------------------
   int
   StatsChainTransmissionModel::Counter::
   messages_of_type( const std::type_info& t )
      const throw()
   {
      if( count_by_type_ == NULL )
         return StatsChainTransmissionModel::UNKNOWN;

      MessageCountByType::const_iterator it = count_by_type_->first.find(string(t.name()));
      return it==count_by_type_->first.end() ? 0 : it->second.first;
   }
   // ----------------------------------------------------------------------
   int
   StatsChainTransmissionModel::Counter::
   size_of_type( const std::type_info& t )
      const throw()
   {
      if( count_by_type_ == NULL )
         return StatsChainTransmissionModel::UNKNOWN;

      MessageCountByType::const_iterator it = count_by_type_->first.find(string(t.name()));
      return it==count_by_type_->first.end() ? 0 : it->second.second;
   }
   // ----------------------------------------------------------------------
   void
   StatsChainTransmissionModel::Counter::
   add( StatsChainTransmissionModel::DetailLevel dl,
        const Message& msg )
      throw()
   {
      ++count_.first;
      count_.second += msg.size();

      if( dl == StatsChainTransmissionModel::ByMessageType ) {
         if( count_by_type_==NULL ) {
            count_by_type_ = new RefMessageCountByType;
            count_by_type_->second = 1;
         }
         
         string n(typeid(msg).name());
         MessageCountByType::iterator it = 
            count_by_type_->first.find(n);
         if( it==count_by_type_->first.end() )
            count_by_type_->first[n] = make_pair(1,msg.size());
         else {
            ++it->second.first;
            it->second.second += msg.size();
         }
      }
   }
   // ----------------------------------------------------------------------
   void
   StatsChainTransmissionModel::Counter::
   dump( std::ostream& os,
         const std::string& prefix )
      const throw()
   {
      os << prefix << " all_types messages " << count_.first << endl
         << prefix << " all_types size " << count_.second << endl;
      if( count_by_type_ != NULL ) {
         for( MessageCountByType::iterator
                 it    = count_by_type_->first.begin(),
                 endit = count_by_type_->first.end();
              it != endit; ++it )
            os << prefix << " " << it->first << " messages " << it->second.first << endl
               << prefix << " " << it->first << " size " << it->second.second << endl;
      }
   }




   // ----------------------------------------------------------------------
   StatsChainTransmissionModel::
   StatsChainTransmissionModel()
      : sender_info_      ( NULL ),
        receiver_info_    ( NULL ),
        general_info_     ( NULL ),
        sender_detail_    ( ByMessageType ),
        receiver_detail_  ( Disabled ),
        edge_detail_      ( Disabled ),
        general_detail_   ( ByMessageType ),
        split_broadcasts_ ( true ),
        initialized_      ( false )
   {}
   // ----------------------------------------------------------------------
   StatsChainTransmissionModel::
   ~StatsChainTransmissionModel() 
   {
      //TODO
   }
   // ----------------------------------------------------------------------
   void
   StatsChainTransmissionModel::
   init( void ) 
      throw() 
   {
      assert( !initialized_ );
      ChainableTransmissionModel::init();

      initialized_=true;
      if( sender_detail() != Disabled )
         sender_info_ = new DynamicNodeArray<Counter>(world_w());
      if( receiver_detail() != Disabled )
         receiver_info_ = new DynamicNodeArray<Counter>(world_w());
      if( general_detail() != Disabled )
         general_info_ = new Counter;
   }

   // ----------------------------------------------------------------------
   void
   StatsChainTransmissionModel::
   reset( void ) 
      throw() 
   {
      //dump_stats();
      ChainableTransmissionModel::reset();     
      //msg_count_.clear();
   }
   // ----------------------------------------------------------------------
   void
   StatsChainTransmissionModel::
   dump_stats( std::ostream& os )
      const throw()
   {
      if( !initialized_ ) return;

      os << "---- stats_chain transmission model information -----------------" << endl;

      if( general_detail() != Disabled )
         general_info_->dump( os, "general" );

// comments by tbaum.
// by using the concurrent adapter with OpenSG/Glut the user is allowed
// to show all messages sent by a single node (up to now: cursor on the
// appropriate node and press 'm').
// if i forgot to uncomment the following lines before checking the code into cvs, sorry.
// //       if( sender_detail() != Disabled ) {
// //          string pref("sender ");
// //          for( World::const_node_iterator
// //                  it    = world().begin_nodes(),
// //                  endit = world().end_nodes();
// //               it != endit; ++it )
// //             (*sender_info_)[*it].dump( os, pref+it->label() );
// //       }
// //
// //       if( receiver_detail() != Disabled ) {
// //          string pref("receiver ");
// //          for( World::const_node_iterator
// //                  it    = world().begin_nodes(),
// //                  endit = world().end_nodes();
// //               it != endit; ++it )
// //             (*receiver_info_)[*it].dump( os, pref+it->label() );
// //       }

//          sender_info_ = new DynamicNodeArray<Counter>(world_w());
//       if( receiver_detail() != Disabled )
//          receiver_info_ = new DynamicNodeArray<Counter>(world_w());

//       int overall_count = 0;

//       cout << "---- stats_chain transmission model information -----------------" << endl;

//       for(StringIntMapIt it = msg_count_.begin(); it != msg_count_.end(); ++it)
//          {
//             cout << it->first << ": " << it->second << endl;
//             overall_count += it->second;
//          }

//       cout << "Different message types: " << msg_count_.size() << endl;
//       cout << "Number of messages seen: " << overall_count << endl;

      os << "-----------------------------------------------------------------" << endl;
   }
   // ----------------------------------------------------------------------
   void
   StatsChainTransmissionModel::
   dump_node_stats( const Node& node, std::ostream& os )
      const throw()
   {
      os << "---- stats_chain transmission model node information" << endl;
      os << "---- for " << node.label() << " ----" << endl;

      if( sender_detail() != Disabled )
      {
         assert( sender_info_ != NULL );
         string pref_sen("sender ");
         (*sender_info_)[node].dump( os, pref_sen+node.label() );
      }
      else
         os << "SenderDetail is disabled." << endl;

      if( receiver_detail() != Disabled )
      {
         assert( receiver_info_ != NULL );
         string pref_rec("receiver ");
         (*receiver_info_)[node].dump( os, pref_rec+node.label() );
      }
      else
         os << "ReceiverDetail is disabled." << endl;

      os << "----" << endl;
   }


   // ----------------------------------------------------------------------
   void
   StatsChainTransmissionModel::
   send_message( TransmissionModel::MessageInfo& mi )
      throw()
   {
      if( sender_detail() != Disabled )
         {
            assert( sender_info_ != NULL );
            (*sender_info_)[*mi.src_].add( sender_detail(), *(mi.msg_) );
         }
//       if( receiver_detail() != Disabled )
//          {
//             assert( receiver_info_ != NULL );
//             if( mi.msg_->is_unicast() )
//                { ABORT_NOT_IMPLEMENTED; }
//             else if( split_broadcasts_ ) {
//                for( Node::const_adjacency_iterator
//                        ait = mi.src_->begin_adjacent_nodes
//             }
//             receiver_info_->add( sender_detail(), *(mi.msg_) );
//          }
      if( general_detail() != Disabled )
         {
            assert( general_info_ != NULL );
            general_info_->add( general_detail(), *(mi.msg_) );
         }

      pass_to_chain(mi);
   }
   // ----------------------------------------------------------------------
   void
   StatsChainTransmissionModel::
   set_sender_detail( DetailLevel dl )
      throw()
   {
      if( !initialized_ ) sender_detail_=dl;
   }
   // ----------------------------------------------------------------------
   void
   StatsChainTransmissionModel::
   set_receiver_detail( DetailLevel dl )
      throw()
   {
      if( !initialized_ ) receiver_detail_=dl;
   }
   // ----------------------------------------------------------------------
   void
   StatsChainTransmissionModel::
   set_edge_detail( DetailLevel dl )
      throw()
   {
      if( !initialized_ ) edge_detail_=dl;
   }
   // ----------------------------------------------------------------------
   void
   StatsChainTransmissionModel::
   set_general_detail( DetailLevel dl )
      throw()
   {
      if( !initialized_ ) general_detail_=dl;
   }
   // ----------------------------------------------------------------------
   void
   StatsChainTransmissionModel::
   set_split_broadcasts( bool b )
      throw()
   {
      if( !initialized_ ) split_broadcasts_=b;
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/stats_chain_transmission_model.cpp,v $
 * Version $Revision: 504 $
 * Date    $Date: 2011-01-24 20:00:04 +0100 (Mon, 24 Jan 2011) $
 *-----------------------------------------------------------------------
 * $Log: stats_chain_transmission_model.cpp,v $
 *-----------------------------------------------------------------------*/
