#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#define EPS .00001

#include "apps/topology/polygon/segment_2d.h"
#include <math.h>
#include <iostream>
#include <vector>
#include "sys/util/defutils.h"

namespace polygon
{

	Segment2D::
		Segment2D()
	{}

	// ----------------------------------------------------------------------	
	
	Segment2D::
		Segment2D(const Vec& p1, const Vec& p2)
	{
		source_ = p1;
		sink_ = p2;
	} 
	
	// ----------------------------------------------------------------------	
	
	Segment2D::
		~Segment2D()
	{}
	
	// ----------------------------------------------------------------------	

	const Vec& 
		Segment2D::
		get_source(void) const 
		throw()
	{
		return source_;
	}

	// ----------------------------------------------------------------------	
	

	const Vec& 
		Segment2D::
		get_sink(void) const 
		throw()
	{
		return sink_;
	}

	// ----------------------------------------------------------------------	
	

	Box 
		Segment2D::
		get_Bbox(void) const 
		throw()
	{
		double min_x = min(source_.x(), sink_.x());
		double max_x = max(source_.x(), sink_.x());
		double min_y = min(source_.y(), sink_.y());
		double max_y = max(source_.y(), sink_.y());
		
		Vec lower = Vec(min_x, min_y, 0.0);
		Vec upper = Vec(max_x, max_y, 0.0);
		
		return Box(lower, upper);
	}	

	// ----------------------------------------------------------------------		
	
	bool
		Segment2D::
		point_on_segment(const Vec& p) const 
		throw()
	{	
		Box bbox = get_Bbox();
		Vec lower = bbox.lower();
		Vec upper = bbox.upper();
		double bbox_min_x = lower.x();
		double bbox_max_x = upper.x();
		double bbox_min_y = lower.y();
		double bbox_max_y = upper.y();
		
		double p_x = p.x();
		double p_y = p.y();
		if ( ((bbox_min_x-p_x)<= EPS) && ((bbox_max_x-p_x)>= -EPS) && ((bbox_min_y-p_y)<= EPS) && ((bbox_max_y-p_y)>= -EPS) )
		{
			
			//compute normal vector of seg
			Vec dv = sink_ - source_;// direction vector
			Vec n; // normal vector
			
			if(fabs(dv.x())<EPS)
			{ // dv is a vertical vector
				n = Vec(dv.y(), dv.x(), 0.0);
			}
			else{		
				n = Vec((- (dv.y() / dv.x())),1.0, 0.0);
			}
			double length_of_n = n.euclidean_norm();
			Vec p_minus_a = p - source_; // a = point on seg (=source of seg)
			double help = fabs(n * p_minus_a);
			double distance_point_segment = help / length_of_n; // |n*(P-A)| / |n|
			
			if (fabs(distance_point_segment)<EPS)
			{
				return true;
			}
		}
		return false;		
	}
	
	// ----------------------------------------------------------------------	
	
	bool 
		Segment2D::
		check_for_intersections(const Segment2D& p_seg, Vec& ipoint) const 
		throw()	
	{
		if (equals(p_seg)) 
		{
			return true;
		}
		
		// compute point of intersection:
		
		//xA = x1 + lambda*(x2-x1)
		//yA = y1 + lambda*(y2-y1)
		double x1 = source_.x(); 
		double x2 = sink_.x();
		double y1 = source_.y();
		double y2 =	sink_.y();
			
		//xB = x3 + mue*(x4-x3)
		//yB = y3 + mue*(y4-y3)	
		double x3 = p_seg.get_source().x();
		double x4 = p_seg.get_sink().x();
		double y3 = p_seg.get_source().y();
		double y4 =	p_seg.get_sink().y();			
	
		double mue = (x1*y2 + x2*y3 + x3*y1 - x2*y1 - x3*y2 - x1*y3) /
					 (x1*y4 + x4*y2 + x2*y3 + x3*y1 - x4*y1 - x1*y3 -x3*y2 - x2*y4);
		double lambda =	(x3 + mue*x4 - mue*x3 -x1) / (x2 - x1);
		
		// s1: point of intersection located on this segment (owner)
		double x_s1 = x1 + lambda*(x2-x1);
		double y_s1 = y1 + lambda*(y2-y1);
		
		// s2: point of intersection located on p_seg
		double x_s2 = x3 + mue*(x4-x3);
		double y_s2 = y3 + mue*(y4-y3);
				
		//----------------------------------------------------------
		
		// (s1==s2?) 
		if(EQDOUBLE(x_s1, x_s2) && EQDOUBLE(y_s1, y_s2))
		{
			//the point of intersection can not be an end point of a segment!
			if( !((EQDOUBLE(x_s1, source_.x()) && EQDOUBLE(y_s1, source_.y()) ) || (EQDOUBLE(x_s1, sink_.x()) && EQDOUBLE(y_s1, sink_.y())) ))
			{ 			
				// does the point of intersection s lie on both segments? (segments are no lines!)
				Vec s;
				s = Vec(x_s1, y_s1, 0.0);	
			
				if(point_on_segment(s) && p_seg.point_on_segment(s))
				{
					ipoint=s;
					return true;
				}
			}
		}
		
		return false;	// returns false if there does not exist any point of intersection between seg1 and seg2
		
	}

	// ----------------------------------------------------------------------
	
	bool 
		Segment2D::
		equals(const Segment2D& s) const 
		throw()	
	{
		if(	(source_==s.get_source() && sink_==s.get_sink()) || 
			(source_==s.get_sink()   && sink_==s.get_source()) )
		{
			return true;
		}
		
		return false;
	}
	

}

#endif	
