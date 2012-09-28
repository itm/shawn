/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/communication/wiseml_comm_model.h"
#ifdef ENABLE_WISEML
#include "sys/taggings/node_reference_tag.h"
#include "sys/taggings/group_tag.h"
#include "sys/taggings/basic_tags.h"
using namespace shawn;
namespace wiseml
{
   WiseMlCommModel::WiseMlCommModel() {};
   // *********************************************************************
   WiseMlCommModel::~WiseMlCommModel() {};
   // *********************************************************************
   bool WiseMlCommModel::exists_communication_upper_bound() const throw()
   {
      return false;
   }
   // *********************************************************************
   double WiseMlCommModel::communication_upper_bound() const throw()
   {
      return 0.0;
   }
   // *********************************************************************
   bool WiseMlCommModel::is_status_available_on_construction() const throw()
   {
      return true;
   }
   // *********************************************************************
   bool WiseMlCommModel::
   can_communicate_bidi(const shawn::Node &node1,
      const shawn::Node &node2)
      const throw()
   {
      return can_communicate_uni(node1,node2)
         && can_communicate_uni(node2,node1);
   }
   // *********************************************************************
   bool WiseMlCommModel::
   can_communicate_uni(const shawn::Node &source,
      const shawn::Node &target)
      const throw()
   {
      // Check if both nodes are enabled
      bool src_enabled = (dynamic_cast<const BoolTag *>(
         source.find_tag("WISEML_ENABLED").get()))->value();
      bool tgt_enabled = (dynamic_cast<const BoolTag *>(
         target.find_tag("WISEML_ENABLED").get()))->value();
      if(!src_enabled || !tgt_enabled)
      {
         return false;
      }

      // Check if nodes are connected
      const GroupTag *links = dynamic_cast<const GroupTag*>(
         source.find_tag("WISEML_LINKS").get());
      if(links==NULL)
         return false;
      const NodeReferenceTag *link = dynamic_cast<const NodeReferenceTag*>
         (links->find_tag(target.label()).get());
      if(link != NULL && link->value() == &target)
      {
         return true;
      }
      return false;
   }
   // *********************************************************************
   void WiseMlCommModel::
   add_link(const shawn::Node &source, const shawn::Node &target)
   {
   }
}

#endif
