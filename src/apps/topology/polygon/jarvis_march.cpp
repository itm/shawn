#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY


#include "apps/topology/polygon/jarvis_march.h"
#include <math.h>
#include "sys/util/defutils.h"

using namespace std;

namespace polygon
{

	JarvisMarch::
	JarvisMarch()
	{

	}

	// ----------------------------------------------------------------------

	JarvisMarch::
		~JarvisMarch()
	{}

	// ----------------------------------------------------------------------

	vector<Vec>
		JarvisMarch::
		compute_convex_hull(const vector<Vec>& poly)
		throw()
	{
		polygon_=poly;
		size_of_polygon_=polygon_.size();
		size_of_convex_hull_=0;
		jm();

		// size_of_convex_hull_ now contains the number of the first points in polygon_ (they are in the right order) which belong to the convex hull
		vector<Vec> convex_hull;
		for(int i=0;i<size_of_convex_hull_;i++)
		{
			convex_hull.push_back(polygon_[i]);
		}
    	return convex_hull;
	}

	// ----------------------------------------------------------------------

	void
		JarvisMarch::
		jm()
		throw()
	{
		int i=index_of_lowest_point();
		do
		{
			swap(size_of_convex_hull_, i);
			i=index_of_rightmost_point_from(polygon_[size_of_convex_hull_]);
			size_of_convex_hull_++;
		}
		while (i>0);
	}

	// ----------------------------------------------------------------------

	int
		JarvisMarch::
		index_of_lowest_point()
		throw()
	{
		int i, min=0;
		for (i=1; i<size_of_polygon_; i++){
			if (polygon_[i].y()<polygon_[min].y() || (EQDOUBLE(polygon_[i].y(),polygon_[min].y()) && polygon_[i].x()<polygon_[min].x()))
			{
				min=i;
			}
		}
		return min;
	}

	// ----------------------------------------------------------------------

	int
		JarvisMarch::
		index_of_rightmost_point_from(const Vec& q)
		throw()
	{
		int i=0, j;
		for (j=1; j<size_of_polygon_; j++){
			Vec reltoj = relTo(polygon_[j],q);
			Vec reltoi = relTo(polygon_[i],q);
			if (isLess(reltoj, reltoi))
			{
				i=j;
			}
		}
		return i;
	}

	// ----------------------------------------------------------------------

	void
		JarvisMarch::
		swap(int i, int j)
		throw()
	{
		Vec t=polygon_[i];
		polygon_[i]=polygon_[j];
		polygon_[j]=t;
	}

	// ----------------------------------------------------------------------
	// ----------------------------------------------------------------------

	Vec
	JarvisMarch::
		relTo(const Vec& p1, const Vec& p2)
		throw()
	{
		return Vec(p1.x()-p2.x(), p1.y()-p2.y(), 0.0);
	}

	// ----------------------------------------------------------------------

	bool
	JarvisMarch::
		isLess(const Vec& p1, const Vec& p2)
		throw()
	{
		double f=cross(p1,p2);
		return f>0 || (EQDOUBLE(f,0) && isFurther(p1,p2));
	}

	// ----------------------------------------------------------------------

	double
	JarvisMarch::
		cross(const Vec& p1, const Vec& p2)
		throw()
	{
		return p1.x()*p2.y()-p2.x()*p1.y();
	}

	// ----------------------------------------------------------------------

	bool
	JarvisMarch::
		isFurther(const Vec& p1, const Vec& p2)
		throw()
	{
		return mdist(p1)>mdist(p2);
	}

	// ----------------------------------------------------------------------

	double
	JarvisMarch::
		mdist(const Vec& p)
		throw()
	{
		return fabs(p.x())+fabs(p.y());
	}

}

#endif
