/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <cstdlib>
#include "sys/taggings/node_reference_tag.h"
#include "sys/node.h"
#include "sys/world.h"


namespace shawn
{
   const std::string NodeReferenceTag::TAG_TYPE ( "node_reference" );
   // ----------------------------------------------------------------------
   NodeReferenceTag::
   NodeReferenceTag( const std::string& n,
                     Node* v,
                     bool lock )
      : Tag(n,lock),
        label_str_ ( NULL ),
        world_     ( NULL ),
        n_val_     ( v ),
        state_     ( Consistent )
   {}
   // ----------------------------------------------------------------------
   NodeReferenceTag::
   NodeReferenceTag( const std::string& n,
                     const std::string& val_base,
                     World& w,
                     bool lock )
      : Tag(n,lock),
        label_str_ ( new std::string(val_base) ),
        world_     ( &w ),
        n_val_     ( NULL ),
        state_     ( Unitialized )
   {}
   // ----------------------------------------------------------------------
   NodeReferenceTag::
   ~NodeReferenceTag()
   {
      if( label_str_ != NULL )
         delete label_str_;
   }
   // ----------------------------------------------------------------------
   std::string
   NodeReferenceTag::
   encoded_content(void)
      const throw()
   {
      if( state_ == Consistent )
         {
            if( value() == NULL )
               return std::string("");
            else
               return value()->label();
         }
      else
         {
            assert( label_str_ != NULL );
            return *label_str_;
         }
   }
   // ----------------------------------------------------------------------
   const std::string&
   NodeReferenceTag::
   type_identifier(void)
      const throw()
   {
      return TAG_TYPE;
   }
   // ----------------------------------------------------------------------
   void
   NodeReferenceTag::
   set_value( Node* v )
      throw()
   {
      n_val_=v;
      if( label_str_ != NULL ) delete label_str_;
      label_str_ = NULL;
      world_= NULL;
      state_ = Consistent;
   }
   // ----------------------------------------------------------------------
   Node*
   NodeReferenceTag::
   value( void ) 
      const throw()
   {
      switch( state_ )
         {
         case Consistent:
            return n_val_;
         case Dangling:
            return NULL;
         case Unitialized:
            assert( label_str_ != NULL );
            assert( world_ != NULL );

            bool okay;
            if( label_str_->empty() )
               {
                  n_val_ = NULL;
                  okay = true;
               }
            else
               {
                  n_val_ = world_->find_node_by_label_w(*label_str_);
                  okay = (n_val_!=NULL);
               }
            if( okay )
               {
                  state_=Consistent;
                  delete label_str_;
                  world_=NULL;
                  label_str_=NULL;
                  return n_val_;
               }
            else
               {
                  std::cerr << "WARNING: Ignoring dangling node reference '"
                            << (*label_str_)
                            << "'." << std::endl;
                  state_=Dangling;
                  return NULL;
               }
         }
      abort();//impossible to reach
      return NULL;
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/taggings/node_reference_tag.cpp,v $
 * Version $Revision: 182 $
 * Date    $Date: 2008-04-07 15:14:38 +0200 (Mon, 07 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: node_reference_tag.cpp,v $
 *-----------------------------------------------------------------------*/
