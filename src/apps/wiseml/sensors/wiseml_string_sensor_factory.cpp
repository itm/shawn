#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_WISEML
#ifdef ENABLE_READING
#include "apps/wiseml/sensors/wiseml_string_sensor_factory.h"

namespace wiseml
{
   WisemlStringSensorFactory::
   WisemlStringSensorFactory( void )
   {}
   // ----------------------------------------------------------------------
   WisemlStringSensorFactory::
   ~WisemlStringSensorFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   WisemlStringSensorFactory::
   name( void )
      const throw()
   {
      return "wiseml_string_sensor";
   }
   // ----------------------------------------------------------------------
   std::string
   WisemlStringSensorFactory::
   description( void )
      const throw()
   {
      return "Base class for creating WiseML string sensors";
   }
   // ----------------------------------------------------------------------
   WisemlStringSensor*
   WisemlStringSensorFactory::
   create(string capability, shawn::Node &node)
   {
      string sensor_id = node.label() + ":" + capability;

      reading::Sensor* sensor = find_sensor(sensor_id);

      WisemlStringSensor* out_sensor = NULL;
      if(sensor == NULL)
      {
         out_sensor = 
            new WisemlStringSensor((*data_reader_)[sensor_id]);
         sensor_map_[sensor_id] = out_sensor;
      }
      else
      {
         out_sensor = dynamic_cast<WisemlStringSensor*>(sensor);
      }

      return out_sensor;
   }
}
#endif
#endif
