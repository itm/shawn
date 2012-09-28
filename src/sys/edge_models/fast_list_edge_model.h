/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_EDGE_MODELS_FAST_LIST_EDGE_MODEL_H
#define __SHAWN_SYS_EDGE_MODELS_FAST_LIST_EDGE_MODEL_H

#include "sys/edge_models/list_edge_model.h"

namespace shawn
{
   class GridEdgeModel;

   class FastListEdgeModel
      : public ListEdgeModel
   {
   public:
      FastListEdgeModel( GridEdgeModel* );
      virtual ~FastListEdgeModel();

      virtual void set_world( World& ) throw();
      virtual void set_communication_model( const CommunicationModel& ) throw();
      virtual void init( void ) throw();

      virtual void node_added( Node& ) throw();
      virtual void node_removed( Node& ) throw();

   protected:
      virtual void add_node_neighbors( Node&, const Vec& pos, const Vec& velo ) throw();

   private:
      GridEdgeModel* grid_model_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/edge_models/fast_list_edge_model.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: fast_list_edge_model.h,v $
 *-----------------------------------------------------------------------*/
