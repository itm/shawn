#include "sys/taggings/taggings_tasks.h"
#include "sys/taggings/basic_tags.h"

using namespace std;

namespace shawn{

   	// ----------------------------------------------------------------------
	SimulationTaskSetGlobalTagInteger::
		SimulationTaskSetGlobalTagInteger()
	{}

	// ----------------------------------------------------------------------
	SimulationTaskSetGlobalTagInteger::
		~SimulationTaskSetGlobalTagInteger()
	{}

	// ----------------------------------------------------------------------
	void
		SimulationTaskSetGlobalTagInteger::
		run( SimulationController& sc )
		throw( std::runtime_error )
	{
		World& w = sc.world_w();
		
		string tagname = sc.environment().required_string_param("tagname");
		int tagvalue = sc.environment().required_int_param("value");
		
		for(World::node_iterator it = w.begin_nodes_w(); it != w.end_nodes_w(); it++)
		{
			it->write_simple_tag<int>(tagname, tagvalue);
		}
	}

	// ----------------------------------------------------------------------
	std::string
		SimulationTaskSetGlobalTagInteger::
		name( void )
		const throw()
	{
		return std::string("set_global_tag_int");
	}

	// ----------------------------------------------------------------------
	std::string
		SimulationTaskSetGlobalTagInteger::
		description( void )
		const throw()
	{
		return std::string("Sets the value of an integer tag globally for all nodes.");
	}
	
	
	
   	// ----------------------------------------------------------------------
	SimulationTaskSetGlobalTagDouble::
		SimulationTaskSetGlobalTagDouble()
	{}

	// ----------------------------------------------------------------------
	SimulationTaskSetGlobalTagDouble::
		~SimulationTaskSetGlobalTagDouble()
	{}

	// ----------------------------------------------------------------------
	void
		SimulationTaskSetGlobalTagDouble::
		run( SimulationController& sc )
		throw( std::runtime_error )
	{
		World& w = sc.world_w();
		
		string tagname = sc.environment().required_string_param("tagname");
		double tagvalue = sc.environment().required_double_param("value");
		
		for(World::node_iterator it = w.begin_nodes_w(); it != w.end_nodes_w(); it++)
		{
			it->write_simple_tag<double>(tagname, tagvalue);
		}
	}

	// ----------------------------------------------------------------------
	std::string
		SimulationTaskSetGlobalTagDouble::
		name( void )
		const throw()
	{
		return std::string("set_global_tag_double");
	}

	// ----------------------------------------------------------------------
	std::string
		SimulationTaskSetGlobalTagDouble::
		description( void )
		const throw()
	{
		return std::string("Sets the value of a double tag globally for all nodes.");
	}
	
	
   	// ----------------------------------------------------------------------
	SimulationTaskSetGlobalTagString::
		SimulationTaskSetGlobalTagString()
	{}

	// ----------------------------------------------------------------------
	SimulationTaskSetGlobalTagString::
		~SimulationTaskSetGlobalTagString()
	{}

	// ----------------------------------------------------------------------
	void
		SimulationTaskSetGlobalTagString::
		run( SimulationController& sc )
		throw( std::runtime_error )
	{
		World& w = sc.world_w();
		
		string tagname = sc.environment().required_string_param("tagname");
		string tagvalue = sc.environment().required_string_param("value");
		
		for(World::node_iterator it = w.begin_nodes_w(); it != w.end_nodes_w(); it++)
		{
			it->write_simple_tag<string>(tagname, tagvalue);
		}
	}

	// ----------------------------------------------------------------------
	std::string
		SimulationTaskSetGlobalTagString::
		name( void )
		const throw()
	{
		return std::string("set_global_tag_string");
	}

	// ----------------------------------------------------------------------
	std::string
		SimulationTaskSetGlobalTagString::
		description( void )
		const throw()
	{
		return std::string("Sets the value of a string tag globally for all nodes.");
	}
	
	
	   	// ----------------------------------------------------------------------
	SimulationTaskSetGlobalTagBool::
		SimulationTaskSetGlobalTagBool()
	{}

	// ----------------------------------------------------------------------
	SimulationTaskSetGlobalTagBool::
		~SimulationTaskSetGlobalTagBool()
	{}

	// ----------------------------------------------------------------------
	void
		SimulationTaskSetGlobalTagBool::
		run( SimulationController& sc )
		throw( std::runtime_error )
	{
		World& w = sc.world_w();
		
		string tagname = sc.environment().required_string_param("tagname");
		bool tagvalue = sc.environment().required_bool_param("value");
		
		for(World::node_iterator it = w.begin_nodes_w(); it != w.end_nodes_w(); it++)
		{
			it->write_simple_tag<bool>(tagname, tagvalue);
		}
	}

	// ----------------------------------------------------------------------
	std::string
		SimulationTaskSetGlobalTagBool::
		name( void )
		const throw()
	{
		return std::string("set_global_tag_bool");
	}

	// ----------------------------------------------------------------------
	std::string
		SimulationTaskSetGlobalTagBool::
		description( void )
		const throw()
	{
		return std::string("Sets the value of a boolean tag globally for all nodes.");
	}
}
