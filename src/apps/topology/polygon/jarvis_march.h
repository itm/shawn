/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef JARVIS_MARCH_H_
#define JARVIS_MARCH_H_

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/polygon/polygon.h"
#include <vector>
#include "sys/vec.h"

using namespace std;
using std::vector;

namespace polygon
{

   class JarvisMarch
   {
	   
   public:

	  JarvisMarch();      
      ~JarvisMarch();
 
      vector<Vec> compute_convex_hull(const vector<Vec>&) throw();
    
      
   private:
	   
	  vector<Vec> polygon_;
      int size_of_polygon_;			// size of polygon_
      int size_of_convex_hull_;		// number of nodes of the convex hull of a polygon
      
      
      
      
      
      void jm(void) throw();
      int index_of_lowest_point(void) throw();
      int index_of_rightmost_point_from(const Vec&) throw();
      void swap(int i, int j) throw();

  	  Vec relTo(const Vec&, const Vec&) throw();					// creates a new point relatively to point p as the origin
  	  bool isLess(const Vec&, const Vec&) throw();				// checks if the angle between the position vector of the new point and the origin
  	  												// is smaller than the angle between the position vector of p and the origin
  	  double cross(const Vec&, const Vec&) throw();			// cross product
  	  bool isFurther(const Vec&, const Vec&) throw();				// checks if the distance between the new point and the origin 
  	  												// is shorter than the distance between p and the origin
  	  double mdist(const Vec&) throw();   				// Manhattan distance        
   };

}

#endif /*ENABLE_TOPOLOGY*/

#endif /*JARVIS_MARCH_H_*/
