/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/misc/box.h"
#include <limits>
#include <string>

namespace shawn
{
   const Box Box::INFINITE_3D_SPACE; // default ctor is infty!
   // ----------------------------------------------------------------------
   const Box Box::EMPTY_BOX( Vec(0,0,0), Vec(-1,-1,-1) );
   // ----------------------------------------------------------------------
   std::ostream&
   operator << ( std::ostream& os, const Box& v )
      throw()
   {
        if( v == Box::INFINITE_3D_SPACE )
            return os << "INFINITE_3D_SPACE";

       return os << std::string("(") << 
           v.lower() << 
           std::string(",") << 
           v.upper() << 
           std::string(")");
   }
   // ----------------------------------------------------------------------
   Box::
   Box()
      : p1( -std::numeric_limits<double>::max(),
            -std::numeric_limits<double>::max(),
            -std::numeric_limits<double>::max() ),
        p2( std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max() )
   {}
   // ----------------------------------------------------------------------
   Box::
   Box( const Vec& a, const Vec& b)
      : p1( a ),
        p2( b )
   {}
   // ----------------------------------------------------------------------
   Box::
   Box( const Box& b )
      : p1 ( b.p1 ),
        p2 ( b.p2 )
   {}
   // ----------------------------------------------------------------------
   Box::
   Box( const Vec& p )
      : p1( p ),
        p2( p )
   {}
   // ----------------------------------------------------------------------
   Box::
   ~Box()
   {}
   // ----------------------------------------------------------------------
   Box& 
   Box::operator = ( const Box& b )
      throw()
   {
      p1 = b.p1;
      p2 = b.p2;
      return *this;
   }
   // ----------------------------------------------------------------------
   bool
   Box::
   contains( const Vec& v )
      const throw()
   {
   	return 
        (p1.x() <= v.x()) && (v.x() <= p2.x()) &&
        (p1.y() <= v.y()) && (v.y() <= p2.y()) &&
        (p1.z() <= v.z()) && (v.z() <= p2.z());
   }
   // ----------------------------------------------------------------------
   bool Box::overlaps( const double& this_p1, const double& this_p2, const double& b1, const double& b2 ) const throw() 
   {
   	bool overlap = false;
	double this_min, this_max, b_min, b_max  = 0;
 	//sorting values
	if( this_p1 <= this_p2 ) 
	{ 
		this_min = this_p1; 
		this_max = this_p2; 
	}
	else {
		this_min = this_p2; 
		this_max = this_p1; 
	}
	if( b1 <= b2 )
	{ 
		b_min = b1; 
		b_max = b2; 
	}
	else {
		b_min = b2;
		b_max = b1;
	}
	//check if coordinates overlap
	if( (this_min <= b_min && b_min <= this_max) | (this_min <= b_max && b_max <= this_max) )
	{
		overlap = true;
	}
	return overlap;
   }
   // ----------------------------------------------------------------------
   bool Box::intersects( const Box& b ) const throw()
   {
   	bool intersection = false;
	
	double this_p1, this_p2, b1, b2 = 0;
	
	this_p1 = p1.x();
	this_p2 = p2.x();
	b1 	= b.p1.x();
	b2  	= b.p2.x();
	
	if( overlaps( this_p1, this_p2, b1, b2 ) )
	{
		this_p1	= p1.y();
		this_p2 = p2.y();
		b1	= b.p1.y();
		b2 	= b.p2.y();
		
		if( overlaps( this_p1, this_p2, b1, b2 ) )
		{
			this_p1	= p1.z();
			this_p2 = p2.z();
			b1	= b.p1.z();
			b2 	= b.p2.z();
			
			if( overlaps( this_p1, this_p2, b1, b2 ) )
			{
				intersection = true;
			}
		}
	}
	return intersection;
   }	
		
}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/box.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: box.cpp,v $
 *-----------------------------------------------------------------------*/
