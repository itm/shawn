/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "simplified_geo_routing_message.h"
#ifdef ENABLE_ROUTING

#include "sys/node.h"

#include "apps/routing/simplified_geo/simplified_geo_routing_processor.h"
#include "apps/routing/simplified_geo/simplified_geo_routing_message.h"

using namespace std;
using namespace shawn;

// 1 for message type
// 4 for Message::source()
// 4 for next_hop_
// 8 + 8 for source_position_ (conceptual)
const int routing::geo::SimplifiedGeoRoutingGreedyMessage::MESSAGE_OVERHEAD = 1 + 4 + 4 + 8 + 8;
const bool routing::geo::SimplifiedGeoRoutingGreedyMessage::USE_ROUTING_MESSAGE_OVERHEAD = true;
// 8 + 8 for location_entering_perimeter_mode_
// 8 + 8 location_entering_current_face_
// 4 + 4 first_edge_on_current_face_
const int routing::geo::SimplifiedGeoRoutingPerimeterMessage::MESSAGE_OVERHEAD = 8 + 8 + 8 + 8 + 4 + 4;
const bool routing::geo::SimplifiedGeoRoutingPerimeterMessage::USE_GREEDY_MESSAGE_OVERHEAD = true;

namespace routing
{
	namespace geo
	{
		// SimplifiedGeoRoutingGreedyMessage:

		SimplifiedGeoRoutingGreedyMessage::
			SimplifiedGeoRoutingGreedyMessage( RoutingBase& routing_instance,
											   const shawn::ConstMessageHandle& application_message,
											   const geo::GeoRoutingAddress& destination_address,
											   int time_to_live ) :
		RoutingMessage(routing_instance,
					   application_message,
					   destination_address),
		time_to_live_(time_to_live > 0 ? time_to_live : 0),
	    next_hop_(NULL)
		{}
		// ----------------------------------------------------------------------
		SimplifiedGeoRoutingGreedyMessage::
			SimplifiedGeoRoutingGreedyMessage( const SimplifiedGeoRoutingGreedyMessage& other,
											   const shawn::Node& next_hop ) :
		RoutingMessage(other),
		time_to_live_(other.time_to_live_ - 1),
		next_hop_(&next_hop)
		{}
		// ----------------------------------------------------------------------
		SimplifiedGeoRoutingGreedyMessage::
			SimplifiedGeoRoutingGreedyMessage( const SimplifiedGeoRoutingGreedyMessage& other, bool ) :
		RoutingMessage(other),
		time_to_live_(other.time_to_live_ - 1),
		next_hop_(NULL)
		{}
		// ----------------------------------------------------------------------
		SimplifiedGeoRoutingGreedyMessage::
			~SimplifiedGeoRoutingGreedyMessage()
		{}
		// ----------------------------------------------------------------------
		int
			SimplifiedGeoRoutingGreedyMessage::
			size( void )
			const
			throw()
		{
			return (USE_ROUTING_MESSAGE_OVERHEAD ? RoutingMessage::size() :
					application_message()->size()) + MESSAGE_OVERHEAD;
		}
		// ----------------------------------------------------------------------
		int
			SimplifiedGeoRoutingGreedyMessage::
			time_to_live()
			const
			throw()
		{
			return time_to_live_;
		}
		// ----------------------------------------------------------------------
		const shawn::Node*
			SimplifiedGeoRoutingGreedyMessage::
			next_hop()
			const
			throw()
		{
			assert( next_hop_ );
			return next_hop_;
		}
		// ----------------------------------------------------------------------
		std::ostream&
			operator<<(std::ostream& os, const SimplifiedGeoRoutingGreedyMessage& gm)
		{
			return os << "Destination address: " << gm.destination_address_ << "\n"
						<< "Time to live: " << gm.time_to_live_ << "\n"
						<< "Next hop/Id: " << gm.next_hop_ << "/" << gm.next_hop_->id();
		}

		// SimplifiedGeoRoutingPerimeterMessage:

		// For perimeter init
		SimplifiedGeoRoutingPerimeterMessage::
			SimplifiedGeoRoutingPerimeterMessage( const SimplifiedGeoRoutingGreedyMessage& greedy_message,
												  const shawn::Node& right_hand_receiver,
												  const shawn::Node& source ) :
		SimplifiedGeoRoutingGreedyMessage(greedy_message,
										  right_hand_receiver),
		location_entering_perimeter_mode_(source.real_position()),
		location_entering_current_face_(source.real_position()),
		first_edge_on_current_face_( GeoRoutingDirectedLink(source,right_hand_receiver) ) // e_0
		{}
		// ----------------------------------------------------------------------
		// For perimeter forward. The necessary members
		// location_entering_current_face_, first_edge_on_current_face_ and
		// next_hop are altered via their setters
		SimplifiedGeoRoutingPerimeterMessage::
			SimplifiedGeoRoutingPerimeterMessage( const SimplifiedGeoRoutingPerimeterMessage& other ) :
		SimplifiedGeoRoutingGreedyMessage(other,true),
		location_entering_perimeter_mode_(other.location_entering_perimeter_mode_),
		location_entering_current_face_(other.location_entering_current_face_),
		first_edge_on_current_face_(other.first_edge_on_current_face_)
		{}
		// ----------------------------------------------------------------------
		int
			SimplifiedGeoRoutingPerimeterMessage::
			size( void )
			const
			throw()
		{
			return (USE_GREEDY_MESSAGE_OVERHEAD ? SimplifiedGeoRoutingGreedyMessage::size() :
					USE_ROUTING_MESSAGE_OVERHEAD ? RoutingMessage::size() :
					application_message()->size()) + MESSAGE_OVERHEAD;
		}
		// ----------------------------------------------------------------------
		const shawn::Vec&
			SimplifiedGeoRoutingPerimeterMessage::
			location_entering_perimeter_mode()
			const
			throw()
		{
			return location_entering_perimeter_mode_;
		}
		// ----------------------------------------------------------------------
		const shawn::Vec&
			SimplifiedGeoRoutingPerimeterMessage::
			location_entering_current_face()
			const
			throw()
		{
			return location_entering_current_face_;
		}
		// ----------------------------------------------------------------------
		const geo::GeoRoutingDirectedLink&
			SimplifiedGeoRoutingPerimeterMessage::
			first_edge_on_current_face()
			const
			throw()
		{
			return first_edge_on_current_face_;
		}
		// ----------------------------------------------------------------------
		const shawn::Node*
			SimplifiedGeoRoutingPerimeterMessage::
			next_hop()
			const
			throw()
		{
			return next_hop_;
		}
		// ----------------------------------------------------------------------
		void
			SimplifiedGeoRoutingPerimeterMessage::
			location_entering_current_face( const shawn::Vec& l )
			throw()
		{
			location_entering_current_face_ = l;
		}
		// ----------------------------------------------------------------------
		void
			SimplifiedGeoRoutingPerimeterMessage::
			first_edge_on_current_face( const geo::GeoRoutingDirectedLink& e )
			throw()
		{
			first_edge_on_current_face_ = e;
		}
		// ----------------------------------------------------------------------
		void
			SimplifiedGeoRoutingPerimeterMessage::
			next_hop( const shawn::Node& n )
			throw()
		{
			next_hop_ = &n;
		}
		// ----------------------------------------------------------------------
		ostream&
			operator<<(ostream& os, const SimplifiedGeoRoutingPerimeterMessage& pm)
		{
			return os << static_cast<const SimplifiedGeoRoutingGreedyMessage&>( pm ) << "\n"
						<< "Location entering perimeter mode: " << pm.location_entering_perimeter_mode_ << "\n"
						<< "Location entering current face: " << pm.location_entering_current_face_ << "\n"
						<< "First edge on current face: " << pm.first_edge_on_current_face_;
		}

	}
}
#endif
