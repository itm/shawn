/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_auto_elements.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/vis/elements/vis_drawable_edge.h"
#include "apps/vis/elements/vis_drawable_node.h"
#include "sys/taggings/basic_tags.h"
#include "sys/tag.h"

namespace vis
{

   AutoElements::
   AutoElements( shawn::SimulationController& sc,
                 Visualization& vis )
      throw( std::runtime_error )
      : current_ ( NULL )
#ifdef HAVE_BOOST_REGEX
      , regex_   ( NULL )
#endif
   {
      std::string parm;

      tag_ = sc.environment().optional_string_param("tag", "");

      parm=sc.environment().optional_string_param("elem","");
      if( !parm.empty()  )
         {
            mode_=Single;
            current_ = vis.element_w(parm);
            if( current_.is_null() )
               throw std::runtime_error(std::string("no such element: ") + parm );
            return;
         }

      parm=sc.environment().optional_string_param("elem_regex","");
      std::string tagregex = sc.environment().optional_string_param("tag_regex","");
      if( !parm.empty() || (!tagregex.empty() && !tag_.empty()))
         {
#ifdef HAVE_BOOST_REGEX
            if(tag_.empty() || tagregex.empty())
               mode_=Regex;
            else
               mode_=TagRegex;

            try {
               if(mode_==Regex)
                  regex_= new boost::regex(parm);
               else
                  regex_= new boost::regex(tagregex);
            } 
            catch(...) {
               throw std::runtime_error(std::string("regular expression is bad"));
            }

            vis_cur_=vis.elements().begin();
            vis_end_=vis.elements().end();
            advance_infeasible();

            return;
#else
            throw std::runtime_error(std::string("no regexp support compiled in"));
#endif
         }
       

      throw std::runtime_error(std::string("specify elements (either $elem or $elem_regex)"));
   }
   // ----------------------------------------------------------------------
   AutoElements::
   ~AutoElements()
   {
#ifdef HAVE_BOOST_REGEX
      if( regex_ != NULL ) delete regex_;
#endif
   }
   // ----------------------------------------------------------------------
   ElementHandle
   AutoElements::
   top( void )
      const throw()
   {
      return current_;
   }
   // ----------------------------------------------------------------------
   void 
   AutoElements::
   pop( void )
      throw()
   {
      switch( mode_ )
         {
         case Single:
            current_ = NULL;
            break;
#ifdef HAVE_BOOST_REGEX
         case Regex:
         case TagRegex:
            if( vis_cur_==vis_end_ )
               current_ = NULL;
            else {
               ++vis_cur_;
               advance_infeasible();
            }
            break;
#endif
         }
   }
   // ----------------------------------------------------------------------
   void
   AutoElements::
   advance_infeasible( void )
      throw()
   {
#ifdef HAVE_BOOST_REGEX

      if( mode_==Regex )
         {
            assert( regex_ != NULL );

            while( (vis_cur_ != vis_end_) &&
                   (!boost::regex_search(vis_cur_->first,*regex_)) )
               {
                  ++vis_cur_;
               }

            if( vis_cur_ == vis_end_ )
               current_ = NULL;
            else
               current_ = vis_cur_->second;
         }

      if( mode_==TagRegex )
         {
            assert( regex_ != NULL );

            while( (vis_cur_ != vis_end_) )
               {
                  DrawableNode* dn = dynamic_cast<DrawableNode*>(vis_cur_->second.get());

                  if(dn!=NULL)
                  {
                     const shawn::Tag *tag = NULL;
                     try
                     {
                         tag = dn->node().find_tag(tag_).get();
                     }
                     catch(...)
                     {}

                     const shawn::StringTag *stag = NULL;
                     if(tag)
                     {
                        stag = dynamic_cast<const shawn::StringTag*>(tag);
                     }

                     if(stag && boost::regex_search(stag->value(),*regex_))
                     {
                        break;
                     }
                  }
                  
                  ++vis_cur_;
               }

            if( vis_cur_ == vis_end_ )
               current_ = NULL;
            else
               current_ = vis_cur_->second;

         }
#endif
   }

}
#endif
