#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_WISEML
#ifdef ENABLE_READING
#include "apps/wiseml/sensors/wiseml_double_sensor_factory.h"

namespace wiseml
{
   WisemlDoubleSensorFactory::
   WisemlDoubleSensorFactory( void )
   {}
   // ----------------------------------------------------------------------
   WisemlDoubleSensorFactory::
   ~WisemlDoubleSensorFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   WisemlDoubleSensorFactory::
   name( void )
      const throw()
   {
      return "wiseml_double_sensor";
   }
   // ----------------------------------------------------------------------
   std::string
   WisemlDoubleSensorFactory::
   description( void )
      const throw()
   {
      return "Base class for creating WiseML double sensors";
   }
   // ----------------------------------------------------------------------
   WisemlDoubleSensor*
   WisemlDoubleSensorFactory::
   create(string capability, shawn::Node &node)
   {
      string sensor_id = node.label() + ":" + capability;

      reading::Sensor* sensor = find_sensor(sensor_id);

      WisemlDoubleSensor* out_sensor = NULL;
      if(sensor == NULL)
      {
         out_sensor = 
            new WisemlDoubleSensor((*data_reader_)[sensor_id]);
         sensor_map_[sensor_id] = out_sensor;
      }
      else
      {
         out_sensor = dynamic_cast<WisemlDoubleSensor*>(sensor);
      }

      return out_sensor;
   }
}
#endif
#endif
