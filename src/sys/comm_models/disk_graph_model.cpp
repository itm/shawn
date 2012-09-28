/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/comm_models/disk_graph_model.h"
#include "sys/node.h"
#include "sys/world.h"

#include <cassert>
#include <iostream>

using namespace std;

namespace shawn
{

   DiskGraphModel::
   DiskGraphModel()
      : initialized_ ( false ),
        range_       ( 1.0 ),
        has_range_   ( false )
   {}
   // ----------------------------------------------------------------------
   DiskGraphModel::
   ~DiskGraphModel()
   {}
   // ----------------------------------------------------------------------
   void
   DiskGraphModel::
   init( void )
      throw()
   {
      CommunicationModel::init();
      initialized_ = true;
   }
   // ----------------------------------------------------------------------
   void
   DiskGraphModel::
   set_transmission_range( double tr )
      throw()
   {
      assert( !initialized_ );
      range_ = tr;
      has_range_ = true;
      cout << "DiskGraphModel: Transmission range set to ["<< range_ <<"]" << endl;
   }
   // ----------------------------------------------------------------------
   double
   DiskGraphModel::
   transmission_range( void )
      const throw()
   {
      return range_;
   }
   // ----------------------------------------------------------------------
   bool
   DiskGraphModel::
   can_communicate_bidi( const Node& u,
                         const Node& v )
      const throw()
   {
      return euclidean_distance( u.real_position(), v.real_position() )
         <= range_;
   }
   // ----------------------------------------------------------------------
   bool
   DiskGraphModel::
   can_communicate_uni( const Node& u,
                        const Node& v )
      const throw()
   {
      return euclidean_distance( u.real_position(), v.real_position() )
         <= range_;
   }
   // ----------------------------------------------------------------------
   bool
   DiskGraphModel::
   exists_communication_upper_bound( void )
      const throw()
   {
      return true;
   }
   // ----------------------------------------------------------------------
   double
   DiskGraphModel::
   communication_upper_bound( void )
      const throw()
   {
      return range_;
   }
   // ----------------------------------------------------------------------
   bool
   DiskGraphModel::
   is_status_available_on_construction( void )
      const throw()
   {
      return true;
   }
   // ----------------------------------------------------------------------
   void 
   DiskGraphModel::
   set_size_hint(double size_hint)
   throw()
   {
        if(has_range_) 
            return;

        has_range_ = true;
        range_ = size_hint;
        cout << "DiskGraphModel: Using size hint ["<< range_ <<"] as comm range" << endl;
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/comm_models/disk_graph_model.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: disk_graph_model.cpp,v $
 *-----------------------------------------------------------------------*/
