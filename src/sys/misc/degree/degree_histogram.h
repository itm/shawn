/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_DEGREE_HISTOGRAM_H
#define __SHAWN_SYS_DEGREE_HISTOGRAM_H

#include <vector>
#include <iostream>

namespace shawn
{
   class World;


   class DegreeHistogram
   {
   public:
      
      ///@name Setup and Init
      ///@{
      ///
      DegreeHistogram();
      ///
      virtual ~DegreeHistogram();
      /// call *before* init()!
      virtual void set_world( const World& ) throw();
      /// initializes histogram, not to be called twice
      virtual void init( void ) throw();
      ///@}

      ///@name Access
      ///@{
      ///
      const World& world( void ) const throw();
      /// value for the given degree
      int value( int ) const throw();
      /// sum_{i=lo}^{hi} value(i)   (bounds inclusive!)
      int range_value( int lo, int hi ) const throw();
      /// largest value for which value() is nonzero
      int max_degree( void ) const throw();
      /// equals range_value(0,infinity) resp. degree sum resp. double edge count
      int total_weight( void ) const throw();
      /// a degree for which value(peak_degree()) is maximal
      int peak_degree( void ) const throw();
      ///@}


      friend 
      std::ostream& operator << ( std::ostream&,
                                  const DegreeHistogram& ) 
         throw();

   protected:

      ///
      virtual void encounter( int ) throw();

   private:
      const World* world_;
      int total_weight_;
      int peak_;
      std::vector<int> data_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/degree/degree_histogram.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: degree_histogram.h,v $
 *-----------------------------------------------------------------------*/
