/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/comm_models/stochastic_comm_model.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/node_distance_estimate.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

namespace shawn
{

   StochasticCommunicationModel::
   StochasticCommunicationModel( double max_probabilty, double smooth_factor )
      :
        range_       ( 1.0 ),
		max_probability_( max_probabilty ),
		smooth_factor_	( smooth_factor ),
		support_mobility_(false),
		connectivity_	(NULL)
   {
		urv.set_lower_bound( 0.0);
		urv.set_upper_bound( 1.0 );
		urv.set_lower_bound_inclusive( true );
		urv.set_upper_bound_inclusive( true );
		urv.init();
   }

   // ----------------------------------------------------------------------
   StochasticCommunicationModel::
   ~StochasticCommunicationModel()
   {
	   if (connectivity_ != NULL)
		   delete connectivity_;
   }

   // ----------------------------------------------------------------------
   void
   StochasticCommunicationModel::
   init( void )
      throw()
   {
      CommunicationModel::init();
      support_mobility_ = world().edge_model().supports_mobility();
   	  cout << "StochasticCommunicationModel: Mobility support: " << support_mobility_ << endl;

      if(!support_mobility_)
      {
    	  delete connectivity_;
    	  connectivity_ = new DynamicNodeArray< set<const Node*> >(world_w());
      }

   }

	  double
	   	StochasticCommunicationModel::
	   	communication_probability(double distance) const
	  {
		  double a = max_probability_+(1-smooth_factor_)*max_probability_/(2*smooth_factor_);
		  double b = -max_probability_/(2*smooth_factor_*range_);
		  double value = a + distance*b;
		  return min(max(0.0, value), max_probability_);
	  }

	  // ----------------------------------------------------------------------
   bool
	   	StochasticCommunicationModel::
	   	has_connection( const Node& u, const Node& v ) const
		throw()
   {
	   double dist = ( u.real_position() - v.real_position() ).euclidean_norm();
	   //return dist  <=  range_ * ( 1.0 + smooth_factor_ * urv );
	   return urv <= communication_probability(dist);
   }

   // ----------------------------------------------------------------------
   void
	   	StochasticCommunicationModel::
	   	set_transmission_range( double tr )
		throw()
   {
      range_ = tr;
	  cout << "StochasticCommunicationModel: Transmission range set to ["<< range_ <<"]" << endl;
   }

   // ----------------------------------------------------------------------
	double
   		StochasticCommunicationModel::
   		transmission_range( void )
   		const throw()
	{
      return range_;
	}

   // ----------------------------------------------------------------------
   bool
   	StochasticCommunicationModel::
	can_communicate_bidi( const Node& u, const Node& v )
    const throw()
   {
      return ( can_communicate_uni(u, v) && can_communicate_uni(v, u) );
   }

   // ----------------------------------------------------------------------
   bool
   	StochasticCommunicationModel::
   	can_communicate_uni( const Node& u, const Node& v )
   	const throw()
   {
	   if( support_mobility_ )
	   {
		   return has_connection(u,v);
	   }
	   else
	   {
		   //Dirty cast to avoid constness of this
		   if( !(*connectivity_).find(u) )
			   ((StochasticCommunicationModel*)this)->calculate_connectivity();

		   set<const Node*> &m = (*connectivity_)[u];
		   set<const Node*>::iterator it = m.find(&v);
		   return it != m.end();
	   }
   }

   // ----------------------------------------------------------------------
   bool
   StochasticCommunicationModel::
   exists_communication_upper_bound( void )
      const throw()
   {
      return true;
   }

   // ----------------------------------------------------------------------
   double
   StochasticCommunicationModel::
   communication_upper_bound( void )
      const throw()
   {
      return range_ + range_ * smooth_factor_;
   }

   // ----------------------------------------------------------------------
   bool
   StochasticCommunicationModel::
   is_status_available_on_construction( void )
      const throw()
   {
      return true;
   }

   // ----------------------------------------------------------------------
   void
	   StochasticCommunicationModel::
	   calculate_connectivity()
   {
		//cout << "StochasticCommunicationModel::calculate_connectivity(): creating connectivity for " << world().node_count() << " nodes" << endl << flush;
		for( shawn::World::node_iterator it = world_w().begin_nodes_w(), endit = world_w().end_nodes_w();it != endit; ++it)
			for( shawn::World::node_iterator it2 = world_w().begin_nodes_w(), endit2 = world_w().end_nodes_w();it2 != endit2; ++it2)
			{
				Node &u = *it, &v = *it2;
				if( has_connection(u, v) )
				{
					if( !connectivity_->find(u) )
						connectivity_->node_added( u );

					if ( (*connectivity_)[u].find(&v) == (*connectivity_)[u].end() )
						(*connectivity_)[u].insert(&v);
				}

			}
   }



}

