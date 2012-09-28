/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_EDGE_MODELS_MANUAL_EDGE_MODEL_H
#define __SHAWN_SYS_EDGE_MODELS_MANUAL_EDGE_MODEL_H

#include "sys/edge_model.h"

namespace shawn
{
   /** ManualEdgeModel is the superclass for all EdgeModels that
    *  support arbitrary edges via add_edge()
    */
   class ManualEdgeModel
      : public EdgeModel
   {
   public:
      ///
      ManualEdgeModel();
      ///
      virtual ~ManualEdgeModel();

      ///
      virtual void add_edge( Node&, Node& ) throw() = 0;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/edge_models/manual_edge_model.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: manual_edge_model.h,v $
 *-----------------------------------------------------------------------*/
