/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "sys/misc/random/constant_edge_random.h"
#include "sys/taggings/basic_tags.h"

using namespace std;

#define SEED_MAX_1 (1<<20)
#define PARAM_A 117

namespace shawn
{

   ConstantEdgeRandom::
   ConstantEdgeRandom( World& w, 
                       const std::string& tag )
      : base_type    ( w ),
        tag_         ( tag ),
        store_seeds_ ( !tag.empty() )
   {
      for( World::node_iterator
              it    = w.begin_nodes_w(),
              endit = w.end_nodes_w();
           it != endit; ++it )
         assign_seed(*it);
   }
   // ----------------------------------------------------------------------
   ConstantEdgeRandom::
   ~ConstantEdgeRandom()
   {}
   // ----------------------------------------------------------------------
   double
   ConstantEdgeRandom::
   operator() ( const Node& u,
                const Node& v,
                bool directed )
      const throw()
   {
      int v1 = operator[](u);
      int v2 = operator[](v);

      if( (!directed) && (v1<v2) ) std::swap(v1,v2);

      return
         double( (PARAM_A*v1 + v2)%SEED_MAX_1 )/double(SEED_MAX_1-1);
   }
   // ----------------------------------------------------------------------
   void
   ConstantEdgeRandom::
   node_added( Node& v )
      throw()
   {
      base_type::node_added(v);
      assign_seed(v);
   }
   // ----------------------------------------------------------------------
   void
   ConstantEdgeRandom::
   assign_seed( Node& v )
      throw()
   {
      if( store_seeds_ )
         operator[](v) = rand()%SEED_MAX_1;
      else {
         ConstTagHandle th = v.find_tag(tag_);
         if( th.is_not_null() )
            {
               const IntegerTag* it = dynamic_cast<const IntegerTag*>(th.get());
               if( it == NULL )
                  { 
                     WARN( world_w().logger(),
                           "Ignoring node tag '"<<tag_<<"' of type '" << th->type_identifier() << "'; should be int" );
                  }
               else
                  {
                     operator[](v) = it->value();
                     return;
                  }
            }
         
         // if we reach this, the tag is either missing or bogus
         // crrate new one
         int val;
         operator[](v) = val = rand()%SEED_MAX_1;
         IntegerTag* it = new IntegerTag( tag_, val );
         it->set_persistency(true);
         v.add_tag( it );
      }
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/random/constant_edge_random.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: constant_edge_random.cpp,v $
 *-----------------------------------------------------------------------*/
