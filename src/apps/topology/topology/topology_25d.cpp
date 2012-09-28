/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/topology/topology_25d.h"

#include <limits>
#include <algorithm>

using namespace shawn;
using namespace reading;

namespace topology
{

   Topology25D::
   Topology25D()
      : sensitivity_ ( EPSILON ),
        initialized_( false )
   {}
   // ----------------------------------------------------------------------
   Topology25D::
   ~Topology25D()
   {}
   // ----------------------------------------------------------------------
   void
   Topology25D::
   set_base_topology( const reading::BoolReadingHandle& br )
      throw()
   {
      assert( !initialized_ );
      assert( br.is_not_null() );
      assert( base_topo_.is_null() );
      base_topo_=br;
   }
   // ----------------------------------------------------------------------
   void
   Topology25D::
   set_elevation( const reading::DoubleReadingHandle& dr )
      throw()
   {
      assert( !initialized_ );
      assert( dr.is_not_null() );
      assert( elevation_.is_null() );
      elevation_=dr;
   }
   // ----------------------------------------------------------------------
   void
   Topology25D::
   set_upper_elevation( const reading::DoubleReadingHandle& dr )
      throw()
   {
      assert( !initialized_ );
      assert( upper_.is_null() );
      // dr==NULL is okay!
      upper_=dr;
   }
   // ----------------------------------------------------------------------
   void
   Topology25D::
   set_name( const std::string& n )
      throw()
   {
      assert( !initialized_ );
      assert( name_.empty() );
      name_=n;
   }
   // ----------------------------------------------------------------------
   void
   Topology25D::
   set_sensitivity( double s )
      throw()
   {
      assert( !initialized_ );
      assert( s>=0.0 );
      sensitivity_=s;
   }
   // ----------------------------------------------------------------------
   void
   Topology25D::
   init( void )
      throw()
   {
      assert( base_topo_.is_not_null() );
      assert( elevation_.is_not_null() );

      box_ = cut_box_2d(base_topo_->domain(),elevation_->domain());
      if( upper_.is_not_null() )
         box_=cut_box_2d(box_,upper_->domain());


      description_ = std::string("2.5D-topology with area '") +
         base_topo_->name();
      if( upper_.is_not_null() )
         description_ +=  std::string("', elevation '") +
            elevation_->name() +
            std::string("', and upper elevation '") +
            upper_->name() +
            std::string("'");
      else
         description_ +=  std::string("' and elevation '") +
            elevation_->name() +
            std::string("'");

      initialized_=true;
   }
   // ----------------------------------------------------------------------
   Box
   Topology25D::
   cut_box_2d( const Box& a, const Box& b )
      throw()
   {
      return Box( Vec( std::max(a.lower().x(),b.lower().x()),
                       std::max(a.lower().y(),b.lower().y()),
                       -std::numeric_limits<double>::max() ),
                  Vec( std::min(a.upper().x(),b.upper().x()),
                       std::min(a.upper().y(),b.upper().y()),
                       std::numeric_limits<double>::max()) );
   }
   // ----------------------------------------------------------------------
   BoolReading&
   Topology25D::
   base_topology_w( void )
      throw()
   {
      assert( base_topo_.is_not_null() );
      return *base_topo_;
   }
   // ----------------------------------------------------------------------
   const BoolReading&
   Topology25D::
   base_topology( void )
      const throw()
   {
      assert( base_topo_.is_not_null() );
      return *base_topo_;
   }
   // ----------------------------------------------------------------------
   DoubleReading&
   Topology25D::
   elevation_w( void )
      throw()
   {
      assert( elevation_.is_not_null() );
      return *elevation_;
   }
   // ----------------------------------------------------------------------
   const DoubleReading&
   Topology25D::
   elevation( void )
      const throw()
   {
      assert( elevation_.is_not_null() );
      return *elevation_;
   }
   // ----------------------------------------------------------------------
   bool
   Topology25D::
   has_upper_elevation( void )
      const throw()
   { return upper_.is_not_null(); }
   // ----------------------------------------------------------------------
   DoubleReading&
   Topology25D::
   upper_elevation_w( void )
      throw()
   {
      return upper_.is_not_null()
         ? *upper_
         : *elevation_;
   }
   // ----------------------------------------------------------------------
   const DoubleReading&
   Topology25D::
   upper_elevation( void )
      const throw()
   {
      return upper_.is_not_null()
         ? *upper_
         : *elevation_;
   }
   // ----------------------------------------------------------------------
   double
   Topology25D::
   sensitivity( void )
      const throw()
   { return sensitivity_; }
   // ----------------------------------------------------------------------
   bool
   Topology25D::
   value( const shawn::Vec& v )
      const throw()
   {
      assert( initialized_ );
      if( !base_topo_->value(Vec(v.x(),v.y(),0.0)) ) return false;
      return
         (v.z() >= elevation_->value(v)-sensitivity_) &&
         (v.z() <= upper_elevation().value(v)+sensitivity_);
   }
   // ----------------------------------------------------------------------
   shawn::Box
   Topology25D::
   domain( void )
      const throw()
   { return box_; }
   // ----------------------------------------------------------------------
   std::string
   Topology25D::
   name( void )
      const throw()
   { return name_; }
   // ----------------------------------------------------------------------
   std::string
   Topology25D::
   description( void )
      const throw()
   {
      assert( initialized_ );
      return description_;
   }

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/topology_25d.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: topology_25d.cpp,v $
 *-----------------------------------------------------------------------*/
