/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_CONTEXT_H
#define __SHAWN_TUBSAPPS_VIS_CONTEXT_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "sys/vec.h"
#include <map>
#include <cstdlib>

namespace shawn
{ class SimulationController; class World; }

namespace vis
{

   /** \brief Graphic context class.
    *
    * Represents a graphical context to draw in.
    */
   class Context
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      /**
       * \param trans Translation vector.
       * \param scale Scaling factor World<->Image.
       * \param w Width of the image.
       * \param h Height of the image.
       * \param draft Draft level (quality settings).
       */
      Context( const shawn::Vec& trans,
               double scale,
               double w, double h,
               int draft );
      ~Context();
      ///@}

      ///@name Getter
      ///@{
      /**
       * Returns the image width.
       */
      inline double image_width( void ) const throw()
      { return width_; }
      /**
       * Returns the image height.
       */
      inline double image_height( void ) const throw()
      { return height_; }

      /**
       * Returns the scaling factor.
       */
      inline double scale( void ) const throw()
      { return scale_; }
      /**
       * Returns the translation vector.
       */
      inline const shawn::Vec& translation( void ) const throw()
      { return trans_; }

      /**
       * Returns the draft level.
       *
       * Levels:
       * 0  high quality
       * 1  no antialias
       * 2  no outlines & similar
       * 3  reduce alpha & stuff
       * 4  replace things by dummies [e.g. graphics]
       */
      inline int draft_level( void ) const throw()
      { return draft_; }
      ///@}

      /**
       * Gets the (absolute) network position of a given image position.
       *
       * abs coords: lower-left=(0,0) upper-right=(image_width(),image_height())
       */
      shawn::Vec abs_screen_to_network( const shawn::Vec& ) const throw();

   private:
      //Context( const Context& ) { abort(); }

      /// Translation vector.
      shawn::Vec trans_;
      /// Scalation factor.
      double scale_;
      /// Image width.
      double width_;
      /// Image height.
      double height_;
      /// Draft level (see draft_level).
      int draft_;
   };


}

#endif
#endif
