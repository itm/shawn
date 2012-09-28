#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_WISEML
#ifdef ENABLE_READING
#include "apps/wiseml/sensors/wiseml_sensor_factory.h"

namespace wiseml
{
   WisemlSensorFactory::
   WisemlSensorFactory( void )
   :data_reader_(NULL)
   {}
   // ----------------------------------------------------------------------
   WisemlSensorFactory::
   ~WisemlSensorFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   WisemlSensorFactory::
   name( void )
      const throw()
   {
      return "wiseml_sensor";
   }
   // ----------------------------------------------------------------------
   std::string
   WisemlSensorFactory::
   description( void )
      const throw()
   {
      return "Factory base class for creating WiseML sensors";
   }
   // ----------------------------------------------------------------------
   void
   WisemlSensorFactory::
   set_data_reader( WisemlSensorUpdater *data_reader)
   {
      data_reader_ = data_reader;
   }

}
#endif
#endif
