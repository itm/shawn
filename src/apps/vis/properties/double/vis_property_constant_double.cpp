/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/properties/double/vis_property_constant_double.h"

using namespace shawn;

namespace vis
{

   PropertyConstantDoubleTask::PropertyConstantDouble::
   PropertyConstantDouble( double v )
      : val_(v)
   {}
   // ----------------------------------------------------------------------
   PropertyConstantDoubleTask::PropertyConstantDouble::
   ~PropertyConstantDouble()
   {}
   // ----------------------------------------------------------------------
   double
   PropertyConstantDoubleTask::PropertyConstantDouble::
   value( double,
          const PropertyStack<double>&,
          const Element& ) 
      const throw()
   { return val_; }
   // ----------------------------------------------------------------------
   PropertyConstantDoubleTask::
   PropertyConstantDoubleTask()
   {}
   // ----------------------------------------------------------------------
   PropertyConstantDoubleTask::
   ~PropertyConstantDoubleTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   PropertyConstantDoubleTask::
   name( void )
      const throw()
   {
      return "vis_constant_double";
   }
   // ----------------------------------------------------------------------
   std::string
   PropertyConstantDoubleTask::
   description( void )
      const throw()
   {
      return "create a constant double property";
   }
   // ----------------------------------------------------------------------
   ConstPropertyHandle
   PropertyConstantDoubleTask::
   create_property( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      double val = sc.environment().required_double_param("value");
      PropertyConstantDouble* pc = new PropertyConstantDouble(val);
      try {
         fill_default_params(*pc,sc);
      }
      catch( std::runtime_error& ) {
         delete pc;
         throw;
      }
      return pc;
   }


}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/double/vis_property_constant_double.cpp,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/02/01 17:07:29 $
 *-----------------------------------------------------------------------
 * $Log: vis_property_constant_double.cpp,v $
 * Revision 1.1  2006/02/01 17:07:29  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
