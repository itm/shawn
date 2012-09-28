/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/world.h"
#include "sys/misc/degree/degree_histogram.h"

namespace shawn
{

   DegreeHistogram::
   DegreeHistogram()
      : world_        ( NULL ),
        total_weight_ ( -1 ),
        peak_         ( -1 )
   {}
   // ----------------------------------------------------------------------
   DegreeHistogram::
   ~DegreeHistogram()
   {}
   // ----------------------------------------------------------------------
   void
   DegreeHistogram::
   set_world( const World& w )
      throw()
   {
      assert( world_ == NULL );
      world_ = &w;
   }
   // ----------------------------------------------------------------------
   void
   DegreeHistogram::
   init( void )
      throw()
   {
      assert( data_.empty() ); // otherwise dup init()
      for( World::const_node_iterator it = world().begin_nodes(), endit = world().end_nodes(); it != endit; ++it)
		encounter( world().edge_model().nof_adjacent_nodes(*it, EdgeModel::CD_BIDI) );
   }
   // ----------------------------------------------------------------------
   const World&
   DegreeHistogram::
   world( void )
      const throw()
   {
      assert( world_ != NULL );
      return *world_;
   }
   // ----------------------------------------------------------------------
   int
   DegreeHistogram::
   value( int d )
      const throw()
   {
      if( (d<0) || (d>max_degree()) )
         return 0;
      else
         return data_[d];
   }
   // ----------------------------------------------------------------------
   int
   DegreeHistogram::
   range_value( int lo, int hi )
      const throw()
   {
      int sum = 0;
      for( int i=lo; i<=hi; ++i )
         sum += value(i);
      return sum;
   }
   // ----------------------------------------------------------------------
   int
   DegreeHistogram::
   max_degree( void )
      const throw()
   {
      assert( !data_.empty() );
      return int(data_.size()-1);
   }
   // ----------------------------------------------------------------------
   int
   DegreeHistogram::
   total_weight( void )
      const throw()
   {
      return total_weight_;
   }
   // ----------------------------------------------------------------------
   void
   DegreeHistogram::
   encounter( int v )
      throw()
   {
      if( v >= int(data_.size()) )
         for( int cnt=v-data_.size()+1; cnt>0; --cnt )
            data_.push_back(0);

      ++data_[v];
      if( (peak_==-1) || (data_[peak_]<data_[v]) )
         peak_=v;

      total_weight_ += v;
   }
   // ----------------------------------------------------------------------
   std::ostream&
   operator << ( std::ostream& os,
                 const DegreeHistogram& h )
      throw()
   {
      for( int i=0; i<=h.max_degree(); ++i )
         os << i << "\t" << h.value(i) << std::endl;
      return os;
   }
   // ----------------------------------------------------------------------
   int
   DegreeHistogram::
   peak_degree( void )
      const throw()
   {
      assert( peak_ >= 0 );
      return peak_;
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/degree/degree_histogram.cpp,v $
 * Version $Revision: 287 $
 * Date    $Date: 2008-11-16 11:35:44 +0100 (Sun, 16 Nov 2008) $
 *-----------------------------------------------------------------------
 * $Log: degree_histogram.cpp,v $
 *-----------------------------------------------------------------------*/
