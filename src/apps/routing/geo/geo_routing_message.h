/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_GEO_GEO_ROUTING_MESSAGE_H
#define __SHAWN_APPS_ROUTING_GEO_GEO_ROUTING_MESSAGE_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/vec.h"
#include "sys/message.h"

#include "apps/routing/geo/geo_routing.h"

namespace routing
{
	namespace geo
	{
		class GeoRouting;

		class GeoRoutingBeaconMessage : public shawn::Message
		{
		public:
			GeoRoutingBeaconMessage( GeoRouting& routing_instance, 
									 const shawn::Vec& source_position /* Actual position of the source where the message was created */); 
			virtual ~GeoRoutingBeaconMessage(); 
			const shawn::Vec& source_position() const throw();
			const GeoRouting* routing_instance() const throw();
			GeoRouting* routing_instance_w() const throw();
		protected:
			mutable GeoRouting* routing_instance_;
			const shawn::Vec source_position_;
			static const int MESSAGE_OVERHEAD;
		};// End of GeoRoutingBeaconMessage

		class GeoRoutingGreedyMessage : public GeoRouting::RoutingMessage
		{
			friend std::ostream& operator<<(std::ostream& os, const GeoRoutingGreedyMessage& gm);
		public:
			// For "Greedy-Init"
			GeoRoutingGreedyMessage( RoutingBase& routing_instance,
									 const shawn::ConstMessageHandle& application_message,
									 const geo::GeoRoutingAddress& destination_address,
									 const shawn::Vec& source_position,
									 int time_to_live );
			GeoRoutingGreedyMessage( const GeoRoutingGreedyMessage& other,
									 const shawn::Node& next_hop,
									 const shawn::Vec& source_position /* Actual position of the source where the message was created*/);
			virtual ~GeoRoutingGreedyMessage();
			virtual int size( void ) const throw();
			int time_to_live() const throw();
			const shawn::Node* next_hop() const throw();
			const shawn::Vec& source_position() const throw();
		protected:
			GeoRoutingGreedyMessage( const GeoRoutingGreedyMessage& other,
									 const shawn::Vec& source_position );
			int time_to_live_;
			const shawn::Node* next_hop_;
			const shawn::Vec source_position_; // For piggybacking the neighborhood information
			static const int MESSAGE_OVERHEAD;
			static const bool USE_ROUTING_MESSAGE_OVERHEAD;
		};// End of GeoRoutingGreedyMessage

		class GeoRoutingPerimeterMessage : public GeoRoutingGreedyMessage
		{
			friend class GeoRouting;
			friend std::ostream& operator<<(std::ostream& os, const GeoRoutingPerimeterMessage& pm);
		public:
			// For perimeter init
			GeoRoutingPerimeterMessage( const GeoRoutingGreedyMessage& greedy_message,
										const shawn::Node& right_hand_receiver,
										const shawn::Node& source );
			// For perimeter forward. The necessary members
			// location_entering_current_face_, first_edge_on_current_face_ and 
			// next_hop are altered via their setters
			GeoRoutingPerimeterMessage( const GeoRoutingPerimeterMessage& other,
										const shawn::Vec& source_position );
			virtual int size( void ) const throw();
			const shawn::Vec& location_entering_perimeter_mode() const throw();
			const shawn::Vec& location_entering_current_face() const throw();
			const geo::GeoRoutingDirectedLink& first_edge_on_current_face() const throw();
			const shawn::Node* next_hop() const throw();
			void location_entering_current_face( const shawn::Vec& l ) throw();
			void first_edge_on_current_face( const geo::GeoRoutingDirectedLink& e ) throw();
			void next_hop( const shawn::Node& n ) throw();
		protected:
			const shawn::Vec location_entering_perimeter_mode_; //L_p
			shawn::Vec location_entering_current_face_; // L_f
			GeoRoutingDirectedLink first_edge_on_current_face_; // e_0
			static const int MESSAGE_OVERHEAD;
			static const bool USE_GREEDY_MESSAGE_OVERHEAD;
		};// End of GeoRoutingPerimeterMessage
	}
}
#endif
#endif
