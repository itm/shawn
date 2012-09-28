#ifndef AUTOCAST_TEST_DATAUNIT
#define AUTOCAST_TEST_DATAUNIT

#include "_apps_enable_cmake.h"
#ifdef ENABLE_AUTOCAST
#include "dataunit.h"

namespace autocast{

	class TestDataUnit 
		: public DataUnit
	{
	public:
		TestDataUnit();

		TestDataUnit(int id,
					 const shawn::Node* from_addr,
				     /*double x,
				     double y,
				     double time,*/
				     double max_life_time,
				     int priority,
					 std::string responsible_app,
				     const autocast::DistributionArea* area,
				     int size);

		TestDataUnit(const TestDataUnit& o);
		
		virtual ~TestDataUnit();
	};

}
#endif
#endif
