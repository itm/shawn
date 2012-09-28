/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_UTIL_PS_WRITER_H
#define __SHAWN_APPS_LOCALIZATION_UTIL_PS_WRITER_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "sys/node.h"
#include "sys/world.h"


namespace localization
{

   class LocalizationPsWriter
   {

      public:

         enum Color { grey, red, green, blue, yellow, orange, black, pink};

         LocalizationPsWriter( std::ostream&, bool );
         virtual ~LocalizationPsWriter();

         virtual void ps_header();
         virtual void eps_header();

         virtual void paint_color( const shawn::World&, const std::string&, bool );


      protected:

         virtual void write_info( const std::string& );
         virtual void paint( const shawn::World& world, bool );
         virtual void paint_think( const shawn::World& world, bool );
         virtual void paint_real( const shawn::World& world, bool );
         virtual void paint_anchors( const shawn::World& world );

         inline void showpage();
         void newpage();
         void color_header();


         inline void calcOffset( const shawn::World& );

         inline void setcolor(enum Color color);
         inline void setcolor(double r, double g, double b);

         void commRange( const shawn::World& );
         inline void line(double x1, double y1, double x2, double y2);
         inline void lineto(double x, double y);
         inline void stroke();

         inline void circle(double x, double y, double radius, bool filled = false, int start_angle = 0, int end_angle = 360);
         inline void cross(double x, double y);
         inline void silly_triangle(double x, double y);
         inline void moveto(double x, double y);

         inline void setlinewidth(double width = 0.0 );

         std::ostream& psfile_;
         int pageNo_;
         shawn::Vec* addvec;
         double fac;
         bool eps_;
         double x0,x1,y0,y1;
         shawn::Vec* pageSize;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/util/localization_ps_writer.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_ps_writer.h,v $
 *-----------------------------------------------------------------------*/
