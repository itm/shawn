/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/properties/int/vis_property_constant_int.h"

using namespace shawn;

namespace vis
{

   PropertyConstantIntTask::PropertyConstantInt::
   PropertyConstantInt( int v )
      : val_(v)
   {}
   // ----------------------------------------------------------------------
   PropertyConstantIntTask::PropertyConstantInt::
   ~PropertyConstantInt()
   {}
   // ----------------------------------------------------------------------
   int
   PropertyConstantIntTask::PropertyConstantInt::
   value( double,
          const PropertyStack<int>&,
          const Element& ) 
      const throw()
   { return val_; }
   // ----------------------------------------------------------------------
   PropertyConstantIntTask::
   PropertyConstantIntTask()
   {}
   // ----------------------------------------------------------------------
   PropertyConstantIntTask::
   ~PropertyConstantIntTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   PropertyConstantIntTask::
   name( void )
      const throw()
   {
      return "vis_constant_int";
   }
   // ----------------------------------------------------------------------
   std::string
   PropertyConstantIntTask::
   description( void )
      const throw()
   {
      return "create a constant value property";
   }
   // ----------------------------------------------------------------------
   ConstPropertyHandle
   PropertyConstantIntTask::
   create_property( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      int val = sc.environment().required_int_param("value");
      PropertyConstantInt* pc = new PropertyConstantInt(val);
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
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/int/vis_property_constant_int.cpp,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/01/31 12:44:00 $
 *-----------------------------------------------------------------------
 * $Log: vis_property_constant_int.cpp,v $
 * Revision 1.1  2006/01/31 12:44:00  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
