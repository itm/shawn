/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_AUTO_ELEMENTS_H
#define __SHAWN_TUBSAPPS_VIS_AUTO_ELEMENTS_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/visualization.h"
#ifdef HAVE_BOOST_REGEX
#include <boost/regex.hpp>
#endif



namespace shawn
{ class SimulationController; }

namespace vis
{

   /** \brief Visual element iterator.
    *
    * The AutoElements class allows to search in and iterate through visual
    * elements by name, regular expression on names and regular expressions
    * on a node's tag value.
    */
   class AutoElements
   {
   public:
      ///@name Constructor/Destructor
      ///@{
      AutoElements( shawn::SimulationController&,
                    Visualization& )
         throw( std::runtime_error );
      virtual ~AutoElements();
      ///@}

      ///@name Iteration
      ///@{
      /**
       * Returns the current element.
       */
      ElementHandle top( void ) const throw();
      /**
       * Advances to the next element, if there is one.
       */
      void pop( void ) throw();
      ///@}

   protected:
      /**
       * Advances to the next element which fullfills the search constraint. Used
       * by pop method.
       */
      void advance_infeasible( void ) throw();
   private:
      /// List of available modes. Single searches just by name, Regex uses a
      /// regular expression and TagRegex a regular expression on a given tag.
      enum Mode
         { Single, Regex, TagRegex };
      /// The current mode (see Mode enum).
      Mode mode_;
      /// The current element that is returned by top method.
      ElementHandle current_;
      /// The tag name, that is to be used in TagRegex mode.
      std::string tag_;

#ifdef HAVE_BOOST_REGEX
      /// Boost-Regex object used for regular expression searches.
      boost::regex* regex_;
      /// Element iterator (current element).
      Visualization::ElementMap::iterator vis_cur_;
      /// Element iterator (last element).
      Visualization::ElementMap::iterator vis_end_;
#endif
   };


}
#endif
#endif
