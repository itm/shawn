/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/elevation/xyz_file.h"

#include <fstream>
#include <algorithm>
#include <limits>
#include <cstdio>
#include <sstream>


using namespace std;

namespace topology
{

   XYZFile::
   XYZFile()
      : current_row_index_ ( -1 ),
        x_dim_             ( -1 ),
        y_dim_             ( -1 ),
        min_val_           ( std::numeric_limits<double>::max() ),
        max_val_           ( -std::numeric_limits<double>::max() )
   {}
   // ----------------------------------------------------------------------
   XYZFile::
   ~XYZFile()
   {}
   // ----------------------------------------------------------------------
   void
   XYZFile::
   read( const std::string& fn )
      throw( std::runtime_error )
   {
      int x,y;
      double val;
      ifstream in(fn.c_str());

      if( !in )
         throw std::runtime_error(string("cannot load xyz file ")+fn);
      try {
         char linebuf[100];
         while( in.getline(linebuf,100) )
            {
               linebuf[99]=0;
               if( sscanf(linebuf,"%i %i %lf",&x,&y,&val) != 3 )
                  throw runtime_error(string("illegal line: ")+string(linebuf));
               encounter(x,y,val);
            }
         flush_current_row();
         y_dim_=matrix_.size();
      }
      catch( std::runtime_error& re ) {
         in.close();
         throw std::runtime_error(string("in XYZ file ") + fn + string(": ") + re.what());
      }
      in.close();
   }
   // ----------------------------------------------------------------------
   void
   XYZFile::
   flush_current_row( void )
      throw( std::runtime_error )
   {
      if( matrix_.empty() )
         x_dim_=current_row_.size();

      if( current_row_index_ != int(matrix_.size()) )
         {
            ostringstream oss;
            oss << "expected row #" << matrix_.size()
                << ", but found #" << current_row_index_;
            throw runtime_error(oss.str());
         }

      if( int(current_row_.size()) != x_dim_ )
         {
            ostringstream oss;
            oss << "row #" << matrix_.size()
                << " has " << current_row_.size()
                << " elements, but " << x_dim_
                << " are required";
            throw runtime_error(oss.str());
         }

      matrix_.push_back(current_row_);
      current_row_.clear();
   }
   // ----------------------------------------------------------------------
   void
   XYZFile::
   encounter( int x, int y, double val )
      throw( std::runtime_error )
   {
      if( (!current_row_.empty()) && (y!=current_row_index_) )
         flush_current_row();
      
      if( current_row_.empty() )
         current_row_index_=y;
      
      if( x!=int(current_row_.size()) )
         {
            ostringstream oss;
            oss << "row #" << current_row_index_
                << " has elements in wrong order -- #"
                << x
                << " comes unexpected";
            throw runtime_error(oss.str());
         }
         
      current_row_.push_back(val);
      if( val>max_val_ ) max_val_=val;
      if( val<min_val_ ) min_val_=val;
   }

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/elevation/xyz_file.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: xyz_file.cpp,v $
 *-----------------------------------------------------------------------*/
