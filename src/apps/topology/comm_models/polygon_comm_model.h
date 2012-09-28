
#ifndef __SHAWN_APPS_TOPOLOGY_COMM_MODELS_TOPOLOGY_COMM_MODEL_H
#define __SHAWN_APPS_TOPOLOGY_COMM_MODELS_TOPOLOGY_COMM_MODEL_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY
#include "shawn_config.h"


#include "sys/communication_model.h"
#include "sys/node_distance_estimate.h"
#include "apps/topology/topology/polygon_topology.h"
#include "sys/misc/random/uniform_random_variable.h"
#include <set>
#include <string>

namespace topology
{

/**
*	Base class for either the Communication Model
*	and the Distance Estimation Task.
*	These classes  need:
*	- the name of a topology,
*	- lower bound defining the minimal RSSI value with which a message will be send
*	- upper_bound defining the maximal transmission range
*	- optional: Defining the RSSI to DISTANCE and DISTANCE to RSSI functions. 
*		If no file is given a default function is used.
*/
	class CommonTaggedPolygonBase
	{
		struct cmp
		{
			bool operator()(const std::pair<double,double>p1, const std::pair<double,double> p2)const{
				return p1.first <=p2.first; 
			}
		};
	public:

		///@name construction / destruction
		///@{
		///
		CommonTaggedPolygonBase(const topology::PolygonTopology& topo, double, double, std::string dist2rssi, std::string rssi2dist);

		///
		virtual ~CommonTaggedPolygonBase();
		///
		virtual void init( void ) throw();
		///@}

		///@name TaggedPolygon implementation
		///@{
		/**
		*
		*/
		///@}

		///@name Communication range
		///@{
		///
		virtual void set_transmission_range( double ) throw();
		///
		virtual double transmission_range( void ) const throw();
		///@}

		/**
		* Reads the values for distance and RSSI from a file.
		* On the first line is an integer defining the scale. e.g 100 means that the following 
		* values will be divided by 100.
		* On the following lines there have to be two decimals. The first is the distance and the 
		* second is the RSSI value.
		*/
		virtual bool readFile() throw(std::runtime_error);

		virtual void create_default_map() throw();
		/**
		* Calculates the RSSI value for a given distance. 
		**/
		virtual double getRSSI(double distance) const throw();

		/*
		* Calculates the distance for a given RSSI value
		*/
		double get_distance(double measuredRssi) const throw();

	protected:
		const topology::PolygonTopology& topo_;
		bool   initialized_;

		double upper_bound_;
		double lower_bound_;
		bool   has_range_;
		std::string name_;
		typedef std::set<std::pair<double,double>,cmp> value_set;
		value_set rssi_;
		value_set distance_estimate_rssi_dist_;
		std::string fname_;
		std::string distance_estimate_rssi_dist_file_;
		shawn::UniformRandomVariable urv_;
	};
	/**
	*	Communication Model. Different nodes can communicate up to a 
	*	defined RSSI value. 
	*/
	class PolygonTopologyCommunicationModel
	: public shawn::CommunicationModel, public CommonTaggedPolygonBase 
	{
	public:

		PolygonTopologyCommunicationModel(const topology::PolygonTopology& topo, double upper, double lower, std::string fn, std::string rssi2distfile)
			: CommonTaggedPolygonBase(topo, upper, lower, fn, rssi2distfile)
			{}
			virtual ~PolygonTopologyCommunicationModel() {}

			virtual void init( void ) throw()
			{	
				CommunicationModel::init();
				CommonTaggedPolygonBase::init();
			}

			///@name CommunicationModel interface
			///@{
			///
			virtual bool can_communicate_bidi( const shawn::Node&, const shawn::Node& ) const throw();
			///
			virtual bool can_communicate_uni( const shawn::Node&, const shawn::Node& ) const throw();

			/// returns whether communication_upper_bound() returns a useful value
			virtual bool exists_communication_upper_bound( void ) const throw();

			/** if exists_communication_upper_bound(), nodes whose euclidean
			*  distance exceeds communication_upper_bound() can never communicate
			*  in any direction
			*/
			virtual double communication_upper_bound( void ) const throw();
			///
			virtual bool is_status_available_on_construction( void ) const throw();

			virtual void set_size_hint(double size_hint)  throw();
			///@}

			///
			virtual std::string name( void ) const throw();
			///
			virtual std::string description( void ) const throw();			
	};

	class PolygonTopologyDistEst : public shawn::NodeDistanceEstimate, public CommonTaggedPolygonBase 
	{
	private:
		std::string name_;
	public:
		PolygonTopologyDistEst(std::string name, const topology::PolygonTopology& topo,double upper, double lower, std::string fn, std::string rssi2distfile/*, bool att*/)
			: CommonTaggedPolygonBase(topo, upper, lower, fn, rssi2distfile),
			  name_(name)
			{}

			virtual ~PolygonTopologyDistEst() {}
	
			///@name NodeDistanceEstimate interface
			///@{
			
			/// Estimates the distance between to given nodes.
			virtual bool estimate_distance( const shawn::Node& source, const shawn::Node& target, double& result ) const throw();
			///
			virtual std::string name( void ) const throw();
			///
			virtual std::string description( void ) const throw();			

			///@}
	};

}


#endif
#endif
