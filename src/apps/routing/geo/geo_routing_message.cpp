/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "geo_routing_message.h"
#ifdef ENABLE_ROUTING

#include "sys/node.h"

#include "apps/routing/geo/geo_routing_processor.h"
#include "apps/routing/geo/geo_routing_message.h"

using namespace std;
using namespace shawn;

// 1 for message type
// 4 for Message::source()
// 8 + 8 for source_position_
// 4 for routing_instance_
const int routing::geo::GeoRoutingBeaconMessage::MESSAGE_OVERHEAD = 1 + 4 + 8 + 8 + 4;
// 1 for message type
// 4 for Message::source()
// 4 for next_hop_
// 8 + 8 for source_position_
const int routing::geo::GeoRoutingGreedyMessage::MESSAGE_OVERHEAD = 1 + 4 + 4 + 8 + 8;
const bool routing::geo::GeoRoutingGreedyMessage::USE_ROUTING_MESSAGE_OVERHEAD = true;
// 8 + 8 for location_entering_perimeter_mode_
// 8 + 8 location_entering_current_face_
// 4 + 4 first_edge_on_current_face_
const int routing::geo::GeoRoutingPerimeterMessage::MESSAGE_OVERHEAD = 8 + 8 + 8 + 8 + 4 + 4;
const bool routing::geo::GeoRoutingPerimeterMessage::USE_GREEDY_MESSAGE_OVERHEAD = true;

namespace routing
{
	namespace geo
	{

		// GeoRoutingBeaconMessage:

		GeoRoutingBeaconMessage::
			GeoRoutingBeaconMessage( GeoRouting& routing_instance, const shawn::Vec& source_position ) :
		routing_instance_(&routing_instance),
		source_position_(source_position)
		{
			setSize(MESSAGE_OVERHEAD);
		}
		// ----------------------------------------------------------------------
		GeoRoutingBeaconMessage::
			~GeoRoutingBeaconMessage()
		{}
		// ----------------------------------------------------------------------
		const shawn::Vec&
			GeoRoutingBeaconMessage::
			source_position()
			const
			throw()
		{
			return source_position_;
		}
		// ----------------------------------------------------------------------
		const GeoRouting*
			GeoRoutingBeaconMessage::
			routing_instance()
			const
			throw()
		{
			return routing_instance_;
		}
		// ----------------------------------------------------------------------
		GeoRouting*
			GeoRoutingBeaconMessage::
			routing_instance_w()
			const
			throw()
		{
			return routing_instance_;
		}

		// GeoRoutingGreedyMessage:

		GeoRoutingGreedyMessage::
			GeoRoutingGreedyMessage( RoutingBase& routing_instance,
									 const shawn::ConstMessageHandle& application_message,
									 const geo::GeoRoutingAddress& destination_address,
									 const shawn::Vec& source_position,
									 int time_to_live ) :
		RoutingMessage(routing_instance,
					   application_message,
					   destination_address),
		time_to_live_(time_to_live > 0 ? time_to_live : 0),
	    next_hop_(NULL),
		source_position_(source_position)
		{}
		// ----------------------------------------------------------------------
		GeoRoutingGreedyMessage::
			GeoRoutingGreedyMessage( const GeoRoutingGreedyMessage& other,
									 const shawn::Node& next_hop,
									 const shawn::Vec& source_position ) :
		RoutingMessage(other),
		time_to_live_(other.time_to_live_ - 1),
		next_hop_(&next_hop),
		source_position_(source_position)
		{}
		// ----------------------------------------------------------------------
		GeoRoutingGreedyMessage::
			GeoRoutingGreedyMessage( const GeoRoutingGreedyMessage& other,
									 const shawn::Vec& source_position ) :
		RoutingMessage(other),
		time_to_live_(other.time_to_live_ - 1),
		next_hop_(NULL),
		source_position_(source_position)
		{}
		// ----------------------------------------------------------------------
		GeoRoutingGreedyMessage::
			~GeoRoutingGreedyMessage()
		{}
		// ----------------------------------------------------------------------
		int
			GeoRoutingGreedyMessage::
			size( void )
			const
			throw()
		{
			return (USE_ROUTING_MESSAGE_OVERHEAD ? RoutingMessage::size() :
					application_message()->size()) + MESSAGE_OVERHEAD;
		}
		// ----------------------------------------------------------------------
		int
			GeoRoutingGreedyMessage::
			time_to_live()
			const
			throw()
		{
			return time_to_live_;
		}
		// ----------------------------------------------------------------------
		const shawn::Node*
			GeoRoutingGreedyMessage::
			next_hop()
			const
			throw()
		{
			assert( next_hop_ );
			return next_hop_;
		}
		// ----------------------------------------------------------------------
		const shawn::Vec&
			GeoRoutingGreedyMessage::
			source_position()
			const
			throw()
		{
			return source_position_;
		}
		// ----------------------------------------------------------------------
		std::ostream&
			operator<<(std::ostream& os, const GeoRoutingGreedyMessage& gm)
		{
			if(gm.has_source() && gm.next_hop())
			{
				return os << "Destination address: " << gm.destination_address_ << "\n"
							<< "Time to live: " << gm.time_to_live_ << "\n"
							<< "Next hop/Id: " << gm.next_hop_ << "/" << gm.next_hop_->id() << "\n"
							<< "Source position: " << gm.source_position_;
			}
			else
			{
				return os << "Destination address: " << gm.destination_address_ << "\n"
							<< "Time to live: " << gm.time_to_live_;
			}
		}

		// GeoRoutingPerimeterMessage:

		// For perimeter init
		GeoRoutingPerimeterMessage::
			GeoRoutingPerimeterMessage( const GeoRoutingGreedyMessage& greedy_message,
										const shawn::Node& right_hand_receiver,
										const shawn::Node& source ) :
		GeoRoutingGreedyMessage(greedy_message,
								right_hand_receiver,
								source.real_position()),
		location_entering_perimeter_mode_(source.real_position()),
		location_entering_current_face_(source.real_position()),
		first_edge_on_current_face_( GeoRoutingDirectedLink(source,right_hand_receiver) ) // e_0
		{}
		// ----------------------------------------------------------------------
		// For perimeter forward. The necessary members
		// location_entering_current_face_, first_edge_on_current_face_ and
		// next_hop are altered via their setters
		GeoRoutingPerimeterMessage::
			GeoRoutingPerimeterMessage( const GeoRoutingPerimeterMessage& other,
										const shawn::Vec& source_position ) :
		GeoRoutingGreedyMessage(other,source_position),
		location_entering_perimeter_mode_(other.location_entering_perimeter_mode_),
		location_entering_current_face_(other.location_entering_current_face_),
		first_edge_on_current_face_(other.first_edge_on_current_face_)
		{}
		// ----------------------------------------------------------------------
		int
			GeoRoutingPerimeterMessage::
			size( void )
			const
			throw()
		{
			return (USE_GREEDY_MESSAGE_OVERHEAD ? GeoRoutingGreedyMessage::size() :
					USE_ROUTING_MESSAGE_OVERHEAD ? RoutingMessage::size() :
					application_message()->size()) + MESSAGE_OVERHEAD;
		}
		// ----------------------------------------------------------------------
		const shawn::Vec&
			GeoRoutingPerimeterMessage::
			location_entering_perimeter_mode()
			const
			throw()
		{
			return location_entering_perimeter_mode_;
		}
		// ----------------------------------------------------------------------
		const shawn::Vec&
			GeoRoutingPerimeterMessage::
			location_entering_current_face()
			const
			throw()
		{
			return location_entering_current_face_;
		}
		// ----------------------------------------------------------------------
		const geo::GeoRoutingDirectedLink&
			GeoRoutingPerimeterMessage::
			first_edge_on_current_face()
			const
			throw()
		{
			return first_edge_on_current_face_;
		}
		// ----------------------------------------------------------------------
		const shawn::Node*
			GeoRoutingPerimeterMessage::
			next_hop()
			const
			throw()
		{
			return next_hop_;
		}
		// ----------------------------------------------------------------------
		void
			GeoRoutingPerimeterMessage::
			location_entering_current_face( const shawn::Vec& l )
			throw()
		{
			location_entering_current_face_ = l;
		}
		// ----------------------------------------------------------------------
		void
			GeoRoutingPerimeterMessage::
			first_edge_on_current_face( const geo::GeoRoutingDirectedLink& e )
			throw()
		{
			first_edge_on_current_face_ = e;
		}
		// ----------------------------------------------------------------------
		void
			GeoRoutingPerimeterMessage::
			next_hop( const shawn::Node& n )
			throw()
		{
			next_hop_ = &n;
		}
		// ----------------------------------------------------------------------
		ostream&
			operator<<(ostream& os, const GeoRoutingPerimeterMessage& pm)
		{
			return os << static_cast<const GeoRoutingGreedyMessage&>( pm ) << "\n"
						<< "Location entering perimeter mode: " << pm.location_entering_perimeter_mode_ << "\n"
						<< "Location entering current face: " << pm.location_entering_current_face_ << "\n"
						<< "First edge on current face: " << pm.first_edge_on_current_face_;
		}

	}
}
#endif
