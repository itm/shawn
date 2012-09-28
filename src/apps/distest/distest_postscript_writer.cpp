/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "_apps_enable_cmake.h"
#ifdef ENABLE_DISTEST

#include <limits>
#include <set>
#include <sstream>
#include "apps/distest/distest_postscript_writer.h"
#include "sys/world.h"
#include "sys/communication_model.h"
#include "sys/simulation/simulation_controller.h"


using namespace std;

namespace distest {

	// ----------------------------------------------------------------------
	DIPostscriptWriter::
		DIPostscriptWriter(std::ostream& psfile, SimulationController& sc) : psfile_(psfile), pageNo_(1), eps_(false)
	{

//		eps ? eps_header() : ps_header();
		ps_header();
		calcOffset(sc.world());
		//newpage();
		setlinewidth2(0.1);


	}

	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		ps_header() 
	{
		pageSize = new Vec(594, 842);
//		pageSize = new Vec(496, 742);


		psfile_ << "%!PS-Adobe-2.0" << std::endl
			<< "%%BoundingBox: 0 0 "<< pageSize->x() <<" " << pageSize->y() << std::endl
			<< "%%Pages: (atend)" << std::endl
			<< "%%PageOrder: Ascend" << std::endl
			<< "%%EndComments" << endl;

	}


	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		calcOffset(const shawn::World& world)
	{
		x0 = y0 = std::numeric_limits<double>::max();
		x1 = y1 = std::numeric_limits<double>::min();

      for ( shawn::World::const_node_iterator
               it = world.begin_nodes();
               it != world.end_nodes();
               ++it )
		{
			Vec p = (*it).real_position();

			if( p.x() < x0 ) 
				x0=p.x();

			if( p.x() > x1 ) 
				x1=p.x();

			if( p.y() < y0 ) 
				y0=p.y();

			if( p.y() > y1 ) 
				y1=p.y();

		}
		border = Vec(20.0, 20.0, 0);
		double boundary = 10.0;

		double xfac = (pageSize->x()-boundary) / (x1-x0);
		double yfac = (pageSize->y()-boundary) / (y1-y0);
		fac  = (xfac < yfac) ? xfac : yfac;
		addvec = new Vec( boundary - (fac*x0), boundary - (fac*y0) );

		xfac = (pageSize->x()-2*border.x()) / (x1-x0);
		yfac = (pageSize->y()-2*border.y()) / (y1-y0);
		scale = (xfac < yfac) ? xfac : yfac;
		offset = Vec(x0, y0, 0);

		//cout << border << "x0="<< x0 << " x1=" << x1 << "y0="<< y0 << " y1=" << y1 << " xfac=" << xfac << " yfac=" << yfac << " scale: " << scale << " offset: " << offset << endl;


	}



	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		showpage() 
	{

		if(eps_)
			return;

		psfile_ << "showpage" << std::endl; 
		psfile_.flush();
	}
	// ----------------------------------------------------------------------

	void DIPostscriptWriter::
		setcolor2(double X, double Y,double Z) 
	{

		psfile_ << X << " " << Y << " " <<  Z << " setrgbcolor" << std::endl;
	}


	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		line2(double x1, double y1, double x2, double y2) 
	{

//		psfile_ << X << " " << Y << " " <<  Z << " setrgbcolor" << std::endl;
		psfile_ << x1 << " " << y1 << " moveto" << endl <<
			x2 << " " << y2 << " lineto" << endl;
	}
	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		proline2(Vec from, Vec to) 
	{
		Vec newfrom = border + (from - offset) * scale;
		Vec newto = border + (to - offset) * scale;

//		psfile_ << X << " " << Y << " " <<  Z << " setrgbcolor" << std::endl;
		psfile_ << newfrom.x() << " " << newfrom.y() << " moveto" << endl <<
			newto.x() << " " << newto.y() << " lineto" << endl;
		mystroke();
	}

	void DIPostscriptWriter::
		mystroke() 
	{		psfile_ << "stroke" << endl;
	}

	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		circle3(double x, double y, double radius, bool filled) 
	{

//		psfile_ << X << " " << Y << " " <<  Z << " setrgbcolor" << std::endl;
//		psfile_ << x1 << " " << y1 << " moveto" << endl <<
//			x2 << " " << y2 << " lineto" << endl;
		psfile_ << x << " " << y << " " << radius << " 0 360 arc " <<
			(filled ? "fill" : "stroke") << std::endl;

	}

	void DIPostscriptWriter::
		procircle3(Vec pos, double radius, bool filled) 
	{

		

		Vec newpos;
		newpos = border + (pos - offset) * scale;
		//radius *= scale;

		//cout << newpos << "=" << border << "+(" << pos << "-" << offset << ")*" << scale<<endl;

//		psfile_ << X << " " << Y << " " <<  Z << " setrgbcolor" << std::endl;
//		psfile_ << x1 << " " << y1 << " moveto" << endl <<
//			x2 << " " << y2 << " lineto" << endl;
		psfile_ << newpos.x() << " " << newpos.y() << " " << radius * scale << " 0 360 arc " <<
			(filled ? "fill" : "stroke") << std::endl;

	}


	void DIPostscriptWriter::
		text(Vec pos, std::string s) 
	{
		Vec newpos;
		newpos = border + (pos - offset) * scale;
		psfile_ << "/Times-Italic findfont 10 scalefont setfont" << endl
			<< newpos.x() << " " << newpos.y() << " moveto" << endl
			<< "(" << s << ") show" << endl;
	}


	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		setcolor(enum Color color)  
	{
		switch(color) {
			case black	: setcolor(0, 0, 0);			break;
			case grey	: setcolor(0.67, 0.67, 0.67);	break;
			case red	: setcolor(1, 0, 0);			break;
			case green	: setcolor(0, 1, 0);			break;
			case blue	: setcolor(0, 0, 1);			break;
			case yellow	: setcolor(1, 1, 0);			break;
			case orange : setcolor(1, 0.5, 0);			break;
			case pink	: setcolor(1, 0.5, 1);			break;
			default		: setcolor(0, 0, 0);			break;
		}
	}

	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		setcolor(double r, double g, double b)  
	{
		psfile_ << r << " " << g << " " <<  b << " setrgbcolor" << std::endl;
	}

	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		moveto(double x, double y)  
	{
		psfile_ << x << " " << y << " moveto" << std::endl;
	}

	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		setlinewidth2(double width /* = 0.0 */)
	{
		psfile_ << width << " setlinewidth" << std::endl;
	}

	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		newpage()
	{
		if( eps_ && pageNo_ > 1 )
			return;

		if( pageNo_ > 1 )
			showpage();

		psfile_ << "%%Page: " << pageNo_ <<" " << pageNo_ << std::endl;

		pageNo_++;
	}

	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		circle(double x, double y, double radius, bool filled /* = false */, int start_angle /* = 0 */, int end_angle /*= 360 */)  
	{
		psfile_ << x << " " << y << " " << radius << " " << start_angle << " " << end_angle << " arc " <<
			(filled ? "fill" : "stroke") << std::endl;
	}

	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		line(double x1, double y1, double x2, double y2)
	{
		psfile_ << x1 << " " << y1 << " moveto" << endl <<
			x2 << " " << y2 << " lineto" << endl;
	}

	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		stroke()
	{
		psfile_ << "stroke" << endl;
	}

	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		lineto(double x, double y)
	{
		psfile_ << x << " " << y << " lineto" << endl;
	}

	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		cross(double x, double y) 
	{
		psfile_ << x << " 2.0 sub " << y << " moveto" << endl <<
			x << "4.0 add " << y << " lineto stroke" << endl <<

			x << " " << y << " 2.0 sub moveto" << endl <<
			x << " " << y << " 4.0 add lineto stroke" << endl;
	}

	// ----------------------------------------------------------------------
	void DIPostscriptWriter::
		silly_triangle(double x, double y) 
	{
		psfile_ << x << " 2.0 sub " << y << " 2.0 add moveto" << endl <<
			x << " " << y << " 2.0 sub lineto" << endl <<
			x << " 2.0 add " << y << " 2.0 add lineto" << endl <<
			"closepath fill" << endl;
	}

	// ----------------------------------------------------------------------
	DIPostscriptWriter::
		~DIPostscriptWriter(void)
	{
		showpage();
		if(!eps_)
			psfile_ << "%%Pages: " << pageNo_ << endl << "%%EOF" << endl;

      if ( pageSize != NULL ) delete pageSize;
      pageSize = NULL;

      if ( addvec != NULL ) delete addvec;
      addvec = NULL;
	}


} //namespace

#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/distest/distest_postscript_writer.cpp,v $
* Version $Revision: 197 $
* Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: distest_postscript_writer.cpp,v $
 *-----------------------------------------------------------------------*/
