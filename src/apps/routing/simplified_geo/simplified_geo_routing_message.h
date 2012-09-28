/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_SIMPLIFIED_GEO_SIMPLIFIED_GEO_ROUTING_MESSAGE_H
#define __SHAWN_APPS_ROUTING_SIMPLIFIED_GEO_SIMPLIFIED_GEO_ROUTING_MESSAGE_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/vec.h"
#include "sys/message.h"

#include "apps/routing/simplified_geo/simplified_geo_routing.h"

namespace routing
{
	namespace geo
	{
		class SimplifiedGeoRouting;

		class SimplifiedGeoRoutingGreedyMessage : public SimplifiedGeoRouting::RoutingMessage
		{
			friend std::ostream& operator<<(std::ostream& os, const SimplifiedGeoRoutingGreedyMessage& gm);
		public:
			// For "Greedy-Init"
			SimplifiedGeoRoutingGreedyMessage( RoutingBase& routing_instance,
											   const shawn::ConstMessageHandle& application_message,
											   const geo::GeoRoutingAddress& destination_address,
											   int time_to_live );
			SimplifiedGeoRoutingGreedyMessage( const SimplifiedGeoRoutingGreedyMessage& other,
											   const shawn::Node& next_hop );
			virtual ~SimplifiedGeoRoutingGreedyMessage();
			virtual int size( void ) const throw();
			int time_to_live() const throw();
			const shawn::Node* next_hop() const throw();
		protected:
			SimplifiedGeoRoutingGreedyMessage( const SimplifiedGeoRoutingGreedyMessage& other, bool );
			int time_to_live_;
			const shawn::Node* next_hop_;
			static const int MESSAGE_OVERHEAD;
			static const bool USE_ROUTING_MESSAGE_OVERHEAD;
		};// End of SimplifiedGeoRoutingGreedyMessage

		class SimplifiedGeoRoutingPerimeterMessage : public SimplifiedGeoRoutingGreedyMessage
		{
			friend class SimplifiedGeoRouting;
			friend std::ostream& operator<<(std::ostream& os, const SimplifiedGeoRoutingPerimeterMessage& pm);
		public:
			// For perimeter init
			SimplifiedGeoRoutingPerimeterMessage( const SimplifiedGeoRoutingGreedyMessage& greedy_message,
												  const shawn::Node& right_hand_receiver,
												  const shawn::Node& source );
			// For perimeter forward. The necessary members
			// location_entering_current_face_, first_edge_on_current_face_ and 
			// next_hop are altered via their setters
			SimplifiedGeoRoutingPerimeterMessage( const SimplifiedGeoRoutingPerimeterMessage& other );
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
		};// End of SimplifiedGeoRoutingPerimeterMessage
	}
}
#endif
#endif
