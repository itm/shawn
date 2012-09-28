/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef SEGMENT_2D_H_
#define SEGMENT_2D_H_

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "sys/misc/box.h"
#include "sys/vec.h"

using namespace std;
using namespace shawn;

namespace polygon
{

/** This class implements segments.
*
*	It provides functions for polygon topologies.
*
*/
class Segment2D
{
	   
public:

	Segment2D();
	Segment2D(const Vec&, const Vec&);
	~Segment2D();

	/** This method returns the source point of the segment.
	*
	*  \return Vec : returns the source
	*/  	
	const Vec& get_source(void) const throw();
	
	/** This method returns the sink point of the segment.
	*
	*  \return Vec : returns the sink
	*/ 	
	const Vec& get_sink(void) const throw();
	
	
	/** This method returns the smallest bounding box of the segment.
	*
	*  \return Box : returns the bounding box of the segment
	*/	
	Box get_Bbox(void) const throw();
	
	/** This method returns true if the point lies on the segment else returns false.
	*
	*  \param Vec	: the point to check for whether it lies on the segment or not
	*  \return bool 	: true if the point lies on the segment else false
	*/		
	bool point_on_segment(const Vec&) const throw();
	
	/** This method returns true if two segments have a point of intersection else returns false.
	* You have the chance to return the computed point of intersection as a reference.
	*
	*  \param Segment2D*: the second segment
	*  \param Vec*	: return the point of intersection as a reference here
	*  \return bool 	: true if the two segments do have a point of intersection else false
	*/	
	bool check_for_intersections(const Segment2D&, Vec&) const throw();	
	
	/** This method returns true if two segments are equal else returns false.
	*
	*  \param Segment2D	: the second segment
	*  \return bool 	: true if the two segments are equal else false
	*/		
	bool equals(const Segment2D&) const throw();
	
	
private:
 		
	Vec source_;
	Vec sink_;
	

};

}

#endif

#endif /*SEGMENT_2D_H_*/
