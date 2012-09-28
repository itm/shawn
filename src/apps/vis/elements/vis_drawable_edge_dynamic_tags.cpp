/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_drawable_edge_dynamic_tags.h"
#include "sys/taggings/basic_tags.h"
#include "sys/taggings/node_reference_tag.h"

#ifdef HAVE_BOOST_REGEX
#include <boost/regex.hpp>


namespace vis
{

   const std::string DrawableEdgeDynamicTags::PREFIX("dynamic_bytags");
   // ----------------------------------------------------------------------
   DrawableEdgeDynamicTags::
   DrawableEdgeDynamicTags( const shawn::Node& v1,
                        const shawn::Node& v2,
                        const std::string& p,
                        const std::string& np,
                        const std::string tagname_regex)
      : DrawableEdge( std::string("edge.") + p, v1, v2 ),
        props_ ( NULL ),
        node_prefix (np),
        tagname_regex_(tagname_regex)
   {}
   // ----------------------------------------------------------------------
   DrawableEdgeDynamicTags::
   ~DrawableEdgeDynamicTags()
   {}
   // ----------------------------------------------------------------------
   void
   DrawableEdgeDynamicTags::
   init( void )
      throw()
   {
      props_ = new EdgePropertySet;
      props_->init(*this);
      DrawableEdge::init();
   }
   // ----------------------------------------------------------------------
   void
   DrawableEdgeDynamicTags::
   draw( cairo_t* cr, double t, const Context& C )
      const throw(std::runtime_error)
   {
      Drawable::draw(cr,t,C);
      if( visible() )
         {
            double lw       = edge_properties().line_width(t);
            shawn::Vec col  = edge_properties().color(t);
            double blend   = edge_properties().blend(t);

            cairo_save(cr);
            cairo_set_line_width( cr, lw );

            cairo_set_source_rgba(cr,col.x(),col.y(),col.z(),1.0-blend);

            for( shawn::World::const_node_iterator
                  it    = visualization().world().begin_nodes(),
                  endit = visualization().world().end_nodes();
                  it != endit; ++it )
            {
               const shawn::Node& _srcNode = *it;
               for(shawn::TagContainer::tag_iterator
            	  tit = _srcNode.begin_tags(),
            	  tendit = _srcNode.end_tags();
                  tit != tendit; ++tit)
               {
            	   std::string curTagName = (*tit).first;
            	   boost::regex targets(tagname_regex_);
            	   if(boost::regex_search(curTagName, targets))
            	   {
            		   shawn::NodeReferenceTag *tag =
            			   dynamic_cast<shawn::NodeReferenceTag *>((*tit).second.get());
            		   if(tag != NULL)
            		   {
            			   shawn::Node *ntgt = tag->value();
            			   const DrawableNode* dsrc = drawable_node( *it, DrawableNodeDefault::PREFIX );
            			   const DrawableNode* dtgt = drawable_node( *ntgt, DrawableNodeDefault::PREFIX );
            			   shawn::Vec pos1 = dsrc->position(t);
						   shawn::Vec pos2 = dtgt->position(t);
						   cairo_move_to(cr,pos1.x(),pos1.y());
						   cairo_line_to(cr,pos2.x(),pos2.y());
						   cairo_stroke(cr);
            		   }
            		   else
            		   {
							  // TODO: WARN needs a logger object
							  //WARN("DrawableEdgeDynamicTags", "Tag is not of type NodeReferenceTag as expected");
							  std::cerr << "DrawableEdgeDynamicTags: Tag is not of type NodeReferenceTag as expected" << std::endl;
            		   }
            	   }
               }
            }

            cairo_restore(cr);
         }
   }
   // ----------------------------------------------------------------------
   const PropertySet&
   DrawableEdgeDynamicTags::
   properties( void )
      const throw()
   {
      assert( props_.is_not_null() );
      return *props_;
   }
   // ----------------------------------------------------------------------
   PropertySet&
   DrawableEdgeDynamicTags::
   properties_w( void )
      throw()
   {
      assert( props_.is_not_null() );
      return *props_;
   }
   // ----------------------------------------------------------------------
   const DrawableNode*
   DrawableEdgeDynamicTags::
   drawable_node( const shawn::Node& v,
                  const std::string& nprefix )
      const throw( std::runtime_error )
   {
      std::string n = nprefix+std::string(".")+v.label();
      ConstElementHandle eh =
         visualization().element( n );
      if( eh.is_null() )
         throw std::runtime_error(std::string("no such element: ")+n);
      const DrawableNode* dn = dynamic_cast<const DrawableNode*>(eh.get());
      if( dn == NULL )
         throw std::runtime_error(std::string("element is no DrawableNode: ")+n);
      return dn;
   }
   // ----------------------------------------------------------------------
      std::string
      DrawableEdgeDynamicTags::
      read_predecessor( const shawn::Node& node, const std::string& taglabel )
         const throw()
      {
         shawn::ConstTagHandle htag = node.find_tag( taglabel );
         if( htag != NULL )
         {
            const shawn::StringTag *strt = NULL;
            strt = dynamic_cast<const shawn::StringTag*>(htag.get());
            if ( strt )
               return strt->value();
         }

         return std::string("");
      }

}
#endif
#endif
