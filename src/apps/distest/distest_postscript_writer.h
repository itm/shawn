/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_DISTEST_POSTSCRIPT_WRITER_H
#define __SHAWN_APPS_DISTEST_POSTSCRIPT_WRITER_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_DISTEST

#include "sys/simulation/simulation_controller.h"
#include "sys/processor.h"
#include "sys/node.h"
#include "sys/world.h"

using namespace shawn;

namespace distest {

	//----------------------------------------------------------------
	class DIPostscriptWriter
	{
	protected:
		std::ostream& psfile_;
		int pageNo_;

		bool eps_;
		double x0,x1,y0,y1;
		Vec* pageSize;

		//void paint(std::vector<shawn::Node*>& nvec);
	public:
		Vec* addvec;		
		double fac;
		double boundary;
		Vec offset;
		Vec border;
		double scale;
enum Color { grey, red, green, blue, yellow, orange, black, pink};
		DIPostscriptWriter(std::ostream& psfile, SimulationController& sc );
		~DIPostscriptWriter(void);

		void calcOffset(const shawn::World&);

		void setcolor(enum Color color);
		void setcolor(double r, double g, double b);
		
		void line(double x1, double y1, double x2, double y2);
		void lineto(double x, double y);
		void stroke();

		void circle(double x, double y, double radius, bool filled = false, int start_angle = 0, int end_angle = 360);

		void cross(double x, double y);
		void silly_triangle(double x, double y);
		void moveto(double x, double y);

		void setlinewidth2(double width = 0.0 );
		void showpage();
		void setcolor2(double X, double Y, double Z);
		void line2(double x1, double y1, double x2, double y2);
		void proline2(Vec from, Vec to);
		void mystroke();
		void circle3(double x, double y, double radius, bool filled);
		void procircle3(Vec pos, double radius, bool filled);
		void text(Vec pos, std::string s);
		void newpage();
		void ps_header();

	};

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/distest/distest_postscript_writer.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: distest_postscript_writer.h,v $
 *-----------------------------------------------------------------------*/
