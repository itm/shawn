/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_TOPOLOGY_TOPOLOGY_25D_H
#define __SHAWN_APPS_TOPOLOGY_TOPOLOGY_25D_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/reading/readings/simple_reading.h"

namespace topology
{

   class Topology25D
      : public reading::BoolReading
   {
   public:
      Topology25D();
      virtual ~Topology25D();

	  virtual void set_base_topology( const reading::BoolReadingHandle& ) throw();
      virtual void set_elevation( const reading::DoubleReadingHandle& ) throw();
      virtual void set_upper_elevation( const reading::DoubleReadingHandle& ) throw();
      virtual void set_name( const std::string& ) throw();
      virtual void set_sensitivity( double ) throw();
      virtual void init( void ) throw();



      virtual reading::BoolReading& base_topology_w( void ) throw();
      virtual const reading::BoolReading& base_topology( void ) const throw();

      virtual reading::DoubleReading& elevation_w( void ) throw();
      virtual const reading::DoubleReading& elevation( void ) const throw();

      virtual bool has_upper_elevation( void ) const throw();
      virtual reading::DoubleReading& upper_elevation_w( void ) throw();
      virtual const reading::DoubleReading& upper_elevation( void ) const throw();

      virtual double sensitivity( void ) const throw();



      virtual bool value( const shawn::Vec& ) const throw();
      virtual shawn::Box domain( void ) const throw();


      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   protected:
      shawn::Box cut_box_2d( const shawn::Box&, const shawn::Box& ) throw();

   private:
      reading::BoolReadingHandle   base_topo_;
      reading::DoubleReadingHandle elevation_;
      reading::DoubleReadingHandle upper_;
      shawn::Box                 box_;
      std::string                name_;
      std::string                description_;
      double                     sensitivity_;
      bool                       initialized_;
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/topology_25d.h,v $
 * Version $Revision: 348 $
 * Date    $Date: 2009-05-07 10:15:59 +0200 (Thu, 07 May 2009) $
 *-----------------------------------------------------------------------
 * $Log: topology_25d.h,v $
 *-----------------------------------------------------------------------*/
