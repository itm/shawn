/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISELIB_EXAMPLE_PROCESSOR_H
#define __SHAWN_APPS_WISELIB_EXAMPLE_PROCESSOR_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISELIB

#include "apps/wiselib/ext_iface_processor.h"
#include "sys/processor.h"
#include "sys/event_scheduler.h"

#include "external_interface/shawn/shawn_os.h"
#include "external_interface/shawn/shawn_radio.h"
#include "external_interface/shawn/shawn_timer.h"
#include "external_interface/shawn/shawn_debug.h"
#include "internal_interface/routing_table/routing_table_static_array.h"
#include "internal_interface/routing_table/routing_table_stl_map.h"
#include "algorithms/routing/dsdv/dsdv_routing.h"
#include "algorithms/routing/dsr/dsr_routing.h"
#include "algorithms/routing/tree/tree_routing.h"
#include "algorithms/routing/flooding/flooding_algorithm.h"

typedef wiselib::ShawnOsModel Os;

typedef wiselib::StaticArrayRoutingTable<Os, Os::Radio, 8, wiselib::DsdvRoutingTableValue<Os, Os::Radio> > DsdvRoutingTable;
// typedef wiselib::StlMapRoutingTable<Os, Os::Radio, wiselib::DsdvRoutingTableValue<Os, Os::Radio> > DsdvRoutingTable;
typedef wiselib::DsdvRouting<Os, DsdvRoutingTable> dsdv_routing_t;

typedef wiselib::vector_static<Os, Os::Radio::node_id_t, 8> NodeVector;
typedef wiselib::StaticArrayRoutingTable<Os, Os::Radio, 10, wiselib::DsrRoutingTableValue<Os::Radio, NodeVector> > DsrRoutingTable;
typedef wiselib::DsrRouting<Os, DsrRoutingTable> dsr_routing_t;

typedef wiselib::TreeRouting<Os, Os::Radio, Os::Timer> tree_routing_t;

typedef wiselib::StaticArrayRoutingTable<Os, Os::Radio, 8, uint16_t> FloodingStaticMap;
typedef wiselib::StlMapRoutingTable<Os, Os::Radio, uint16_t> FloodingStlMap;
typedef wiselib::FloodingAlgorithm<Os, FloodingStaticMap> flooding_algorithm_t;

namespace wiselib
{

   /**
    */
   class WiselibExampleProcessor
       : public virtual ExtIfaceProcessor
   {
   public:
      
      ///@name Constructor/Destructor
      ///@{
      WiselibExampleProcessor();
      virtual ~WiselibExampleProcessor();
      ///@}

      ///@name Inherited from Processor
      ///@{
      /**
       */
      virtual void boot( void ) throw();
      ///@}

      virtual void work( void ) throw();
      void rcv_routing_message( int from, long len, unsigned char* data) throw();
      
   private:
      std::string rt_algo_;

      flooding_algorithm_t routing_;
      dsdv_routing_t dsdv_routing_;
      dsr_routing_t dsr_routing_;
      tree_routing_t tree_routing_;
      ShawnOs os_;
      Os::Radio wiselib_radio_;
      Os::Timer wiselib_timer_;
      Os::Debug wiselib_debug_;
   };


}

#endif
#endif
