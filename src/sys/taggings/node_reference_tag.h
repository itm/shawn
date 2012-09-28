/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TAGGINGS_NODE_REFERENCE_TAG_H
#define __SHAWN_SYS_TAGGINGS_NODE_REFERENCE_TAG_H

#include "sys/tag.h"

namespace shawn
{
   class Node;
   class World;

   DECLARE_HANDLES(NodeReferenceTag);
   class NodeReferenceTag
      : public Tag//<Node*,node_reference_tag_type>
   {
   public:
      static const std::string TAG_TYPE;
      ///
      enum State
         {
            Consistent,
            Dangling,
            Unitialized
         };
      NodeReferenceTag( const std::string& n,
                        Node* v,
                        bool lock = false );
      NodeReferenceTag( const std::string& n,
                        const std::string& val_base,
                        World&,
                        bool lock = false );
      virtual ~NodeReferenceTag();
      virtual std::string encoded_content(void) const throw();
      virtual void set_value( Node* ) throw();
      virtual Node* value( void ) const throw();

      virtual const std::string& type_identifier( void ) const throw();

   private:
      mutable std::string* label_str_;
      mutable World* world_;
      mutable Node* n_val_;
      mutable State state_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/taggings/node_reference_tag.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: node_reference_tag.h,v $
 *-----------------------------------------------------------------------*/
