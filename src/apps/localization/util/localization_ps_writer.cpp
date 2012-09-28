/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include <limits>
#include <set>
#include <sstream>
#include "apps/localization/util/localization_ps_writer.h"
#include "sys/misc/localization/localization_processor_type.h"
#include "sys/world.h"
#include "sys/communication_model.h"


using namespace std;
using namespace shawn;

namespace localization
{
   #define NORMAL_NODE_RADIUS (2.5)
   #define ANCHOR_NODE_RADIUS (3)
   #define LINE_WIDTH         (1.5)
   // ----------------------------------------------------------------------
   LocalizationPsWriter::
   LocalizationPsWriter( std::ostream& psfile, bool eps )
      : psfile_(psfile), pageNo_(1), eps_(eps)
   {
      eps ? eps_header() : ps_header();
   }
   // ----------------------------------------------------------------------
   LocalizationPsWriter::
      ~LocalizationPsWriter(void)
   {
      if ( pageSize != NULL ) delete pageSize;
      pageSize = NULL;

      if ( addvec != NULL ) delete addvec;
      addvec = NULL;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationPsWriter::
   eps_header()
   {
      pageSize = new Vec(500, 500);

      psfile_ << "%!PS-Adobe EPSF-3.0" << std::endl
         << "%%BoundingBox: 0 0 "<< pageSize->x() <<" " << pageSize->y() << std::endl
         << "%%Pages: 1" << std::endl
         << "%%EndComments" << endl;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationPsWriter::
   ps_header()
   {
    // 43 = 1.5 cm = RAND
    // 595 = 21 cm = DINA4 Breite
    // 842 = 29 cm = DINA4 H�he
    // pageSize = new Vec(842, 150);
    pageSize = new Vec(596, 842);
      psfile_ << "%!PS-Adobe-2.0" << std::endl
         << "%%BoundingBox: 0 0 "<< pageSize->x() <<" " << pageSize->y() << std::endl
         << "%%Pages: 3" << std::endl
         << "%%PageOrder: Ascend" << std::endl
         << "%%EndComments" << endl;
   }
   // ----------------------------------------------------------------------
	void LocalizationPsWriter::
		color_header()
	{
		psfile_ << "/drawtopologynode { 0 0 0   setrgbcolor .5 0 360 arc fill   } bind def" << endl
				<< "/drawnormalnode   { 0 0 0   setrgbcolor .2 0 360 arc stroke } bind def" << endl
			    << "/drawconfignode   { 0 0 0   setrgbcolor .2 0 360 arc fill   } bind def" << endl
			    << "/draworangenode   { 1 0.5 0 setrgbcolor 1  0 360 arc fill   } bind def" << endl
			    << "/drawyellownode   { 1 1 0   setrgbcolor 1  0 360 arc fill   } bind def" << endl
			    << "/drawwhitenode    { 0 1 0   setrgbcolor 1  0 360 arc fill   } bind def" << endl
			    << "/drawprewhitenode { 1 0 0   setrgbcolor 1  0 360 arc fill   } bind def" << endl
				<< "/drawdotnode   { 0 0 0   setrgbcolor .2 0 360 arc stroke } bind def" << endl
			    << "/drawfatnode { 2.5 0 360 arc fill   } bind def" << endl;

	}
   // ----------------------------------------------------------------------
	inline void LocalizationPsWriter::
		showpage()
	{
		if(eps_)
			return;

		psfile_ << "showpage" << std::endl;
		psfile_.flush();
	}
   // ----------------------------------------------------------------------
	inline void LocalizationPsWriter::
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
   void
   LocalizationPsWriter::
   paint_color( const shawn::World& world, const std::string& topo_info, bool comm_range )
   {
      color_header();
      calcOffset(world);
      newpage();
      setlinewidth(0.0);
      //write_info( topo_info );
      paint(world,comm_range);

      if ( !eps_ )
      {
         newpage();
         //write_info( topo_info );
         paint_think(world,comm_range);

         newpage();
         //write_info( topo_info );
         paint_real(world,comm_range);
      }

      showpage();
   }
   // ----------------------------------------------------------------------
   void
   LocalizationPsWriter::
   write_info( const std::string& topo_info )
   {
      psfile_ << "/Times findfont 10 scalefont setfont" << endl
         << 20 << " " << 700
         << " moveto" << endl
         << "(" << topo_info << ") show" << endl;
      stroke();
   }
   // ----------------------------------------------------------------------
   void
   LocalizationPsWriter::
   paint( const shawn::World& world, bool comm_range )
   {
      for ( shawn::World::const_node_iterator
               it = world.begin_nodes();
               it != world.end_nodes();
               ++it )
      {
         Vec pos = ( (*it).real_position() * fac ) + *addvec;
         const LocalizationProcessorType* lproc = (*it).get_processor_of_type<LocalizationProcessorType>();

         if( NULL == lproc )
            continue;

         if ( !lproc->is_anchor() )
         {
            if ( (*it).has_est_position() )
            {
               setcolor( grey );
               //circle( pos.x(), pos.y(), NORMAL_NODE_RADIUS, true );
               circle( pos.x(),  pageSize->y() - pos.y(), NORMAL_NODE_RADIUS, true );
               setlinewidth(LINE_WIDTH);
               Vec est_pos = ( (*it).est_position() * fac ) + *addvec;
               // line( pos.x(),pos.y(), est_pos.x(), est_pos.y() );
               line( pos.x(), pageSize->y() - pos.y(), est_pos.x(), pageSize->y() - est_pos.y() );

               stroke();
               //setcolor( green );
               //circle( est_pos.x(), est_pos.y(), NORMAL_NODE_RADIUS, true );
            }
            else
            {
               setcolor( red );
               //circle( pos.x(), pos.y(), NORMAL_NODE_RADIUS, true );
               circle( pos.x(),  pageSize->y() - pos.y(), NORMAL_NODE_RADIUS, true );
            }
         }

         //setcolor( black );
         //psfile_ << "/Times findfont 5 scalefont setfont" << endl
         //   << pos.x() + 2 << " " << pos.y()
         //   << " moveto" << endl
         //   << "(" << lproc->owner().label() << ") show" << endl;
         //stroke();

      }

      paint_anchors( world );

      if ( comm_range )
         commRange(world);
   }
   // ----------------------------------------------------------------------
   void
   LocalizationPsWriter::
   paint_think( const shawn::World& world, bool comm_range )
   {
      for ( shawn::World::const_node_iterator
               it = world.begin_nodes();
               it != world.end_nodes();
               ++it )
      {
         Vec pos = ( (*it).real_position() * fac ) + *addvec;
         const LocalizationProcessorType* lproc = (*it).get_processor_of_type<LocalizationProcessorType>();

         if( NULL == lproc )
            continue;

         if ( !lproc->is_anchor() )
         {
            if ( (*it).has_est_position() )
            {
               setcolor( grey );
               Vec est_pos = ( (*it).est_position() * fac ) + *addvec;
               //circle( est_pos.x(), est_pos.y(), NORMAL_NODE_RADIUS, true );
					circle( est_pos.x(), pageSize->y() - est_pos.y(), NORMAL_NODE_RADIUS, true );
               stroke();
            }
         }

      }

      paint_anchors( world );

      if ( comm_range )
         commRange(world);
   }
   // ----------------------------------------------------------------------
   void
   LocalizationPsWriter::
   paint_real( const shawn::World& world, bool comm_range )
   {
      for ( shawn::World::const_node_iterator
               it = world.begin_nodes();
               it != world.end_nodes();
               ++it )
      {
         Vec pos = ( (*it).real_position() * fac ) + *addvec;
         const LocalizationProcessorType* lproc = (*it).get_processor_of_type<LocalizationProcessorType>();

         if( NULL == lproc )
            continue;

         if ( !lproc->is_anchor() )
         {
            setcolor( grey );
            Vec real_pos = ( (*it).real_position() * fac ) + *addvec;
         //   circle( real_pos.x(), real_pos.y(), NORMAL_NODE_RADIUS, true );
				circle( real_pos.x(), pageSize->y() - real_pos.y(), NORMAL_NODE_RADIUS, true );
            stroke();
         }

      }

      paint_anchors( world );

      if ( comm_range )
         commRange(world);
   }
   // ----------------------------------------------------------------------
   void
   LocalizationPsWriter::
   paint_anchors( const shawn::World& world )
   {
      for ( shawn::World::const_node_iterator
               it = world.begin_nodes();
               it != world.end_nodes();
               ++it )
      {
         Vec pos = ( (*it).real_position() * fac ) + *addvec;
         const LocalizationProcessorType* lproc = (*it).get_processor_of_type<LocalizationProcessorType>();

         if( NULL == lproc )
            continue;

         if ( lproc->is_anchor() )
         {
            setcolor( black );
            //circle( pos.x(), pos.y(), ANCHOR_NODE_RADIUS, true );
				circle( pos.x(), pageSize->y() - pos.y(), ANCHOR_NODE_RADIUS, true );
         }
      }
   }
   // ----------------------------------------------------------------------
	inline void LocalizationPsWriter::
		calcOffset(  const shawn::World& world )
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

		double boundary = 0.0;

		double xfac = (pageSize->x()-boundary) / (x1-x0);
		double yfac = (pageSize->y()-boundary) / (y1-y0);
		fac  = (xfac < yfac) ? xfac : yfac;
		addvec = new Vec( boundary - (fac*x0), boundary - (fac*y0) );
	}


	// ----------------------------------------------------------------------
	inline void LocalizationPsWriter::
		commRange( const shawn::World& world )
	{
		double r = 1.0;

      if ( world.communication_model().exists_communication_upper_bound() )
         r = world.communication_model().communication_upper_bound();

		r *= fac;

		Vec desPos( x0 , y0 );
		Vec diff(r,r);
		Vec pos = ( desPos * fac ) + *addvec + diff;

		//circle(pos.x(), pos.y(), r, false, 0, 90);
		circle(pos.x(), pos.y(), r);
		circle(pos.x(), pos.y(), 1, true);

		line(pos.x(), pos.y(), pos.x() + r * 0.7071, pos.y() + r * 0.7071);
		stroke();

		psfile_ << "/Times-Italic findfont 5 scalefont setfont" << endl
			<< pos.x() + 2 + r * 0.7071 / 2.0 << " " << pos.y() - 5 + r * 0.7071 / 2.0
			<< " moveto" << endl
			<< "(R) show" << endl;
	}
   // ----------------------------------------------------------------------
	inline void LocalizationPsWriter::
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
	inline void LocalizationPsWriter::
		setcolor(double r, double g, double b)
	{
		psfile_ << r << " " << g << " " <<  b << " setrgbcolor" << std::endl;
	}

	// ----------------------------------------------------------------------
	inline void LocalizationPsWriter::
		moveto(double x, double y)
	{
		psfile_ << x << " " << y << " moveto" << std::endl;
	}

	// ----------------------------------------------------------------------
	inline void LocalizationPsWriter::
		setlinewidth(double width /* = 0.0 */)
	{
		psfile_ << width << " setlinewidth" << std::endl;
	}
	// ----------------------------------------------------------------------
	inline void LocalizationPsWriter::
		circle(double x, double y, double radius, bool filled /* = false */, int start_angle /* = 0 */, int end_angle /*= 360 */)
	{
		psfile_ << x << " " << y << " " << radius << " " << start_angle << " " << end_angle << " arc " <<
			(filled ? "fill" : "stroke") << std::endl;
	}

	// ----------------------------------------------------------------------
	inline void LocalizationPsWriter::
		line(double x1, double y1, double x2, double y2)
	{
		psfile_ << x1 << " " << y1 << " moveto" << endl <<
			x2 << " " << y2 << " lineto" << endl;
	}

	// ----------------------------------------------------------------------
	inline void LocalizationPsWriter::
		stroke()
	{
		psfile_ << "stroke" << endl;
	}

	// ----------------------------------------------------------------------
	inline void LocalizationPsWriter::
		lineto(double x, double y)
	{
		psfile_ << x << " " << y << " lineto" << endl;
	}

	// ----------------------------------------------------------------------
	inline void LocalizationPsWriter::
		cross(double x, double y)
	{
		psfile_ << x << " 2.0 sub " << y << " moveto" << endl <<
			x << "4.0 add " << y << " lineto stroke" << endl <<

			x << " " << y << " 2.0 sub moveto" << endl <<
			x << " " << y << " 4.0 add lineto stroke" << endl;
	}

	// ----------------------------------------------------------------------
	inline void LocalizationPsWriter::
		silly_triangle(double x, double y)
	{
		psfile_ << x << " 2.0 sub " << y << " 2.0 add moveto" << endl <<
			x << " " << y << " 2.0 sub lineto" << endl <<
			x << " 2.0 add " << y << " 2.0 add lineto" << endl <<
			"closepath fill" << endl;
	}

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/util/localization_ps_writer.cpp,v $
 * Version $Revision: 347 $
 * Date    $Date: 2009-04-24 18:37:30 +0200 (Fri, 24 Apr 2009) $
 *-----------------------------------------------------------------------
 * $Log: localization_ps_writer.cpp,v $
 *-----------------------------------------------------------------------*/
