/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/comm_models/manual_communication_model.h"
#include "sys/edge_models/manual_edge_model.h"
#include "sys/node.h"
#include "sys/world.h"

#include <cstdlib>
#include <limits>

namespace shawn
{

   ManualCommunicationModel::
   ManualCommunicationModel()
      : edge_model_ ( NULL )
   {}
   // ----------------------------------------------------------------------
   ManualCommunicationModel::
   ~ManualCommunicationModel()
   {}
   // ----------------------------------------------------------------------
   void
   ManualCommunicationModel::
   init( void )
      throw()
   {}
   // ----------------------------------------------------------------------
   void
   ManualCommunicationModel::
   add_edge( Node& u, 
             Node& v )
      throw()
   {
      edge_model_w().add_edge(u,v);
   }
   // ----------------------------------------------------------------------
   bool
   ManualCommunicationModel::
   can_communicate_bidi( const Node& u,
                         const Node& v )
      const throw()
   {
      return can_communicate_uni(u,v) && can_communicate_uni(v,u);
   }
   // ----------------------------------------------------------------------
   bool
   ManualCommunicationModel::
   can_communicate_uni( const Node& u,
                        const Node& v )
      const throw()
   {
     // ConstNeighborhoodHandle nh = u.neighborhood(1);
	  std::set<const Node*> nh = u.get_adjacent_nodes();
	  // remove the const_cast here! (why does the set need it BTW?)
      return nh.find(const_cast<Node*>(&v)) != nh.end();
   }
   // ----------------------------------------------------------------------
   bool
   ManualCommunicationModel::
   exists_communication_upper_bound( void )
      const throw()
   {
      return false;
   }
   // ----------------------------------------------------------------------
   double
   ManualCommunicationModel::
   communication_upper_bound( void )
      const throw()
   {
      assert(false);
      return std::numeric_limits<double>::max();
   }
   // ----------------------------------------------------------------------
   bool
   ManualCommunicationModel::
   is_status_available_on_construction( void )
      const throw()
   {
      return false;
   }
   // ----------------------------------------------------------------------
   ManualEdgeModel&
   ManualCommunicationModel::
   edge_model_w( void ) 
      throw()
   {
      if( edge_model_ == NULL )
         {
            edge_model_=
               dynamic_cast<ManualEdgeModel*>(&world_w().edge_model_w());

            if( edge_model_==NULL )
               {
                  std::cerr << "ManualCommunicationModel: edge model must be a ManualEdgeModel! "
                            << std::endl;
                  abort();
               }
         }
         return *edge_model_;
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/comm_models/manual_communication_model.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: manual_communication_model.cpp,v $
 *-----------------------------------------------------------------------*/
