/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include <limits>
#include "apps/localization/modules/distance/localization_test_module.h"
#include "apps/localization/util/localization_defutils.h"
#include "sys/node.h"
#include "sys/world.h"
#include <fstream>

using namespace shawn;

namespace localization
{

   LocalizationTestModule::
   LocalizationTestModule()
      : finished_(false)
   {}
   // ----------------------------------------------------------------------
   LocalizationTestModule::
   ~LocalizationTestModule()
   {
	   delete values_;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationTestModule::
   boot( void )
      throw()
   {
       values_ = new LocalizationStatistic();
	   val_ = new int[100];
	   for(int i =0; i< 100; i++)
		   val_[i]=0;
       }
   // ----------------------------------------------------------------------
   bool
   LocalizationTestModule::
   process_message( const ConstMessageHandle& mh )
      throw()
   {
      return LocalizationModule::process_message( mh );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationTestModule::
   work( void )
      throw()
   {
	 estimate();
	 dumpValue();
   }

   // ----------------------------------------------------------------------
   bool
   LocalizationTestModule::
   finished( void )
      throw()
   {
      return finished_;
   }
   // ----------------------------------------------------------------------
	void
   LocalizationTestModule::
   rollback( void )
      throw()
   {
	}
    void 
    LocalizationTestModule::
    estimate( void ) 
        throw()
    {
        for(int i=0; i< 1000; i++ ){
			shawn::World::node_iterator iter = owner_w().owner_w().world_w().begin_nodes_w();
				shawn::World::node_iterator end = owner_w().owner_w().world_w().end_nodes_w();
			for(; iter!= end; iter++)
			{	
				if(owner().owner().id() == (*iter).id())
					continue;
				double value = owner().estimate_distance(node(),*iter);
				double real = (node().real_position()- iter->real_position()).euclidean_norm();
				value = value / real;
				values_->add(value);
				val_[getIndex(value)]++;
			}
		}
    }
    void 
    LocalizationTestModule::
    dumpValue( void ) 
    throw(){
		std::ofstream ofs( "result.csv",std::ios::app );
		ofs<< "Distance from Node" <<node().id() << " with calculated std_dev: " << values_->std_dev()
			<<std::endl<< "Values:" << std::endl;
		for(int i=0; i< 100; i++){
			ofs<< val_[i] <<" ; " ;
		}
		ofs<< std::endl;
    }
   
	int 
	LocalizationTestModule::
	getIndex(double value)
	throw(){	
		if ( value <= 0 ) return 0;
		else if ( value >= 2 ) return 99;
		else return (int) ((value) / 0.02);
	
	}
}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/distance/localization_dv_hop_module.cpp,v $
 * Version $Revision: 56 $
 * Date    $Date: 2007-07-12 11:04:27 +0200 (Do, 12 Jul 2007) $
 *-----------------------------------------------------------------------
 * $Log: localization_dv_hop_module.cpp,v $
 *-----------------------------------------------------------------------*/
