/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_TOPOLOGY_ELEVATION_XYZ_FILE_H
#define __SHAWN_APPS_TOPOLOGY_ELEVATION_XYZ_FILE_H
#include <shawn_config.h>
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include <vector>
#include <string>
#include <stdexcept>
#include <cassert>

namespace topology
{

   class XYZFile
   {
   public:
      typedef std::vector<double> DoubleVec;
      typedef std::vector<DoubleVec> DoubleMatrix;

      XYZFile();
      ~XYZFile();

      void read( const std::string& ) throw( std::runtime_error );

      inline double value( int x, int y ) const throw()
      {
         assert( x>=0 ); assert( x<x_dim_ );
         assert( y>=0 ); assert( y<y_dim_ );
         return matrix_[y][x];
      }
      inline int x_dimension( void ) const throw()
      { return x_dim_; }
      inline int y_dimension( void ) const throw()
      { return y_dim_; }
      inline double max_value( void ) const throw()
      { return max_val_; }
      inline double min_value( void ) const throw()
      { return min_val_; }

   protected:
      void flush_current_row( void ) throw( std::runtime_error );
      void encounter( int x, int y, double val ) throw( std::runtime_error );

   private:
      DoubleMatrix matrix_;
      DoubleVec    current_row_;
      int          current_row_index_;
      int          x_dim_;
      int          y_dim_;
      double       min_val_;
      double       max_val_;
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/elevation/xyz_file.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: xyz_file.h,v $
 *-----------------------------------------------------------------------*/
