/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_RANDOM_CONSTANT_EDGE_RANDOM_H
#define __SHAWN_SYS_MISC_RANDOM_CONSTANT_EDGE_RANDOM_H

#include "sys/misc/dynamic_node_array.h"
#include <stdexcept>


namespace shawn
{

   class ConstantEdgeRandom
      : public DynamicNodeArray<int>
   {
   public:
      typedef DynamicNodeArray<int> base_type;

      /** constructs a ConstantEdgeRandom. If tag is not empty,
       *  it will save write state information to node tags (with the
       *  given name) so that the random values survive a save&load cycle
       */
      ConstantEdgeRandom( World&, 
                          const std::string& tag = "" );
      virtual ~ConstantEdgeRandom();

      /** returns a number uniformly distributed between 0.0 and 1.0
       *  (both inclusive). For the same node pair, it always returns the same
       *  number. If directed==true, ()(u,v) and ()(v,u) are different. */
      double operator() ( const Node&, const Node&, bool directed=false ) const throw();

      virtual void node_added( Node& v ) throw();

   protected:
      virtual void assign_seed( Node& ) throw();

   private:

      std::string tag_;
      bool store_seeds_;
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/random/constant_edge_random.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: constant_edge_random.h,v $
 *-----------------------------------------------------------------------*/
