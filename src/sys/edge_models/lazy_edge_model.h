/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_EDGE_MODELS_LAZY_EDGE_MODEL_H
#define __SHAWN_SYS_EDGE_MODELS_LAZY_EDGE_MODEL_H

#include "sys/edge_model.h"

namespace shawn
{

   class LazyEdgeModel
      : public EdgeModel
   {
   public:

	   
	   template<typename NodeType,
               typename WorldNodeIterator>
      class LazyIteratorHelper
         : public shawn::EdgeModel::AbstractIteratorHelper<NodeType>
      {
      public:
         typedef AbstractIteratorHelper<NodeType> base_type;
         LazyIteratorHelper( const LazyEdgeModel&,
			 EdgeModel::CommunicationDirection dir,
                             NodeType&,
                             const WorldNodeIterator&,
                             const WorldNodeIterator& );
         virtual ~LazyIteratorHelper();
         virtual void init( void ) throw();
         virtual void next( void ) throw();
         virtual NodeType* current( void ) const throw();
         virtual AbstractIteratorHelper<NodeType>* clone( void ) const throw();
      private:
         const LazyEdgeModel&      edge_model_;
         NodeType&                 node_;
         WorldNodeIterator         world_it_;
         WorldNodeIterator         world_end_it_;
      };

   public:
      


      ///
      LazyEdgeModel();
      ///
      virtual ~LazyEdgeModel();

      ///
      virtual int
      nof_adjacent_nodes( const Node&, CommunicationDirection d = CD_BIDI ) 
         const throw();
      ///
      virtual const_adjacency_iterator
		  begin_adjacent_nodes( const Node&, CommunicationDirection  d = CD_BIDI )
         const throw();
      ///
      virtual const_adjacency_iterator
      end_adjacent_nodes( const Node& )
         const throw();
      ///
      virtual adjacency_iterator
      begin_adjacent_nodes_w( Node&, CommunicationDirection d = CD_BIDI )
         throw();
      ///
      virtual adjacency_iterator
      end_adjacent_nodes_w( Node& )
         throw();


      ///
      virtual Box observer_initial_zone(Node&, const Vec& pos, const Vec& velo ) 
         throw();
      ///
      virtual Box observer_update_zone(Node&, const Vec& newpos, const Vec& velo ) throw();
      virtual bool supports_mobility( void ) const throw();

      virtual void node_added( Node& ) throw();
      virtual void node_removed( Node& ) throw();


   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/edge_models/lazy_edge_model.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: lazy_edge_model.h,v $
 *-----------------------------------------------------------------------*/
