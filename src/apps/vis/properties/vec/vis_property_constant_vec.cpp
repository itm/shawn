/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/properties/vec/vis_property_constant_vec.h"

using namespace shawn;

namespace vis
{

   PropertyConstantVecTask::PropertyConstantVec::
   PropertyConstantVec( shawn::Vec v )
      : val_(v)
   {}
   // ----------------------------------------------------------------------
   PropertyConstantVecTask::PropertyConstantVec::
   ~PropertyConstantVec()
   {}
   // ----------------------------------------------------------------------
   shawn::Vec
   PropertyConstantVecTask::PropertyConstantVec::
   value( double,
          const PropertyStack<shawn::Vec>&,
          const Element& ) 
      const throw()
   { return val_; }
   // ----------------------------------------------------------------------
   PropertyConstantVecTask::
   PropertyConstantVecTask()
   {}
   // ----------------------------------------------------------------------
   PropertyConstantVecTask::
   ~PropertyConstantVecTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   PropertyConstantVecTask::
   name( void )
      const throw()
   {
      return "vis_constant_vec";
   }
   // ----------------------------------------------------------------------
   std::string
   PropertyConstantVecTask::
   description( void )
      const throw()
   {
      return "create a constant value property";
   }
   // ----------------------------------------------------------------------
   ConstPropertyHandle
   PropertyConstantVecTask::
   create_property( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      double x = sc.environment().required_double_param("x");
      double y = sc.environment().required_double_param("y");
      double z = sc.environment().optional_double_param("z",0.0);

      PropertyConstantVec* pc = new PropertyConstantVec(shawn::Vec(x,y,z));
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
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vec/vis_property_constant_vec.cpp,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2006/02/01 17:07:29 $
 *-----------------------------------------------------------------------
 * $Log: vis_property_constant_vec.cpp,v $
 * Revision 1.2  2006/02/01 17:07:29  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/31 12:44:00  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
