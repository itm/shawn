/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef POLYGON_H_
#define POLYGON_H_

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "sys/misc/box.h"
#include "apps/topology/polygon/segment_2d.h"
#include "apps/topology/polygon/jarvis_march.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>


//using namespace std;
using std::vector;
using std::pair;

namespace polygon
{

/** This class implements polygons.
*
*	It provides functions for polygon topologies.
*
*/

   class Polygon
   {
	   
   public:

      Polygon();
	  /** This constructor creates a new polygon. 
	  * Note that the parameter hands over the corners of the polygon in the right order!
	  *  
	  *  \param vector<Vec>	: the corners of the polygon to construct in the right order
	  */      
      Polygon(const vector<Vec>&);      
      virtual ~Polygon();

      virtual void set_name( const std::string& ) throw();
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
 
	  /** This method adds a corner to the polygon.
	  * Note that the points of a polygon have to be in the right order!
	  *
	  *  \param Vec : the corner to add
	  */
      virtual void push_back(const Vec&) throw(std::runtime_error);
      
	  /** This method deletes a corner of the polygon.
	  *
	  *  \param Vec : the corner to delete
	  */      
	  virtual void delete_point(const Vec&) throw();				
	  
	  /** This method returns the number of corners of the polygon.
	  *
	  *  \return int	: the size of the vector polygon_
	  */ 	  
	  virtual int get_numberofpoints(void) const throw();				
	  
	  /** This method determines whether a point is already contained in the polygon or not.
	  *  If the point is not part of the vector, the method returns false. 	
	  *
	  *  \param Vec	: the corner to search for
	  *  \return bool	: true if contained else false
	  */ 	  
	  virtual bool contains_point(const Vec&) const throw();		
	  
	  /** This method determines whether a point is already contained in a vector of points or not.
	  *  If the point is not part of the vector, the method returns false. 	
	  *
	  *  \param Vec	: the point to search for
	  *  \param vector<Vec> : the vector where to search for the point
	  *  \return bool	: true if contained else false
	  */ 	  
	  virtual bool contains_point(const Vec&, const vector<Vec>&) const throw();
	  
	  /** This method returns the polygon.	
	  *
	  *  \return vector<Vec> 	: returns the member variable polygon_
	  */ 	  
	  virtual const vector<Vec>& get_poly_vector(void) const throw();	
	   
	  /** This method computes the segments of a polygon consisting of ordered corners and returns them in a vector.	
	  *
	  *  \return vector<Segment2D> 	: returns the computed segments of the polygon
	  */ 	  	  
	  virtual vector<Segment2D> get_segments(void) const throw();		

	  /** This method computes and returns the bounding box of a polygon.	
	  *
	  *  \return Box 	: the smallest bounding box of a polygon
	  */	  
	  virtual Box getBoundingBox(void) const throw();	
	  
	  /** This method returns true if a point lies inside a polygon's area and 
	  * returns false if the point lies outside.	
	  *
	  *  \param Vec	: the point to check for
	  *  \return bool 	: true if the point lies on the bounded side of the polygon, else false
	  */		  
	  virtual bool bounded_side(const Vec&) const throw();	
	  
	  /** This method returns true if a point lies on a polygon's boundary else returns false.
	  *
	  *  \param Vec	: the point to check for
	  *  \return bool 	: true if the point lies on the boundary of the polygon, else false
	  */	  
	  virtual bool on_boundary(const Vec&) const throw();
	  
	  /** This method returns true if the polygon is simple else returns false.
	  * A polygon is simple if the boundary of the polygon does not cross itself
	  * (all nodes of the graph induced by its segments have degree two).
	  *
	  *  \return bool 	: true if the polygon is simple else false
	  */	  
	  virtual bool is_simple() const throw();							
	  
	  /** This method returns true if the polygon is empty (contains no corners) else returns false.
	  *
	  *  \return bool 	: true if the polygon is empty else false
	  */	  
	  virtual bool is_empty() const throw();							
	  
	  /** This method computes and returns a polygon's convex hull.
	  *
	  *  \return Polygon : the polygon's convex hull
	  */	  
	  virtual Polygon compute_convex_hull(void) throw();			

	  /** An iterator for the corners of the polygon.
	  */
	  typedef vector<Vec>::iterator iterator; 	
	  /** A const iterator for the corners of the polygon.
	  */	  
	  typedef vector<Vec>::const_iterator const_iterator; 	

	  iterator begin() { return polygon_.begin(); } 
	  iterator find(Vec v) {return std::find(polygon_.begin(), polygon_.end(), v);}
	  iterator end()   { return polygon_.end(); } 
	  const_iterator begin() const { return polygon_.begin(); } 
	  const_iterator end() const   { return polygon_.end(); } 

	  /** An iterator for the segments of the polygon.
	  */	  
	  typedef vector<Segment2D>::iterator Edge_iterator; 
	  /** A const iterator for the segments of the polygon.
	  */		  
	  typedef vector<Segment2D>::const_iterator Edge_const_iterator; 	

	  Edge_iterator edges_begin() { return segments_.begin(); } 
	  Edge_iterator edges_end()   { return segments_.end(); } 
	  Edge_const_iterator edges_begin() const { return segments_.begin(); } 
	  Edge_const_iterator edges_end() const   { return segments_.end(); } 	  

	  /** To write the polygon's elements.
	  */	  	  
	  friend std::ostream& operator << ( std::ostream&, const Polygon& )
         throw();

   private:
	  std::string name_;
	    	  
      vector<Vec> polygon_;									// vector of vertices
      vector<Segment2D> segments_;								// vector of edges between the vertices
  	 
   };

}

#endif /*ENABLE_TOPOLOGY*/
#endif /*POLYGON_H_*/
