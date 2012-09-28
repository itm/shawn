/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_apps_WISEML_COMMUNICATION_MODEL_H
#define __SHAWN_apps_WISEML_COMMUNICATION_MODEL_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISEML
#include "sys/communication_model.h"
#include "sys/node.h"
using namespace std;
using namespace shawn;

namespace wiseml
{
   /**
    * Simple communication model which uses the wiseml informations
    * to gather currently active links.
    */
   class WiseMlCommModel:
      public shawn::CommunicationModel
   {
   public:
      WiseMlCommModel();
      virtual ~WiseMlCommModel();
      virtual bool can_communicate_uni( const shawn::Node&, const Node& ) const throw();
      virtual bool can_communicate_bidi( const Node&, const Node& ) const throw();
      virtual bool exists_communication_upper_bound( ) const throw();
      virtual double communication_upper_bound( void ) const throw();
      virtual bool is_status_available_on_construction( void ) const throw();

      virtual void add_link(const Node&, const Node& );
      //virtual void remove_link(const Node&, const Node& );
      //virtual void enable_link(const Node&, const Node& );
      //virtual void disable_link(const Node&, const Node& );
      //virtual LinkInfo link_info(const Node&, const Node& );
   };
}

#endif
#endif
