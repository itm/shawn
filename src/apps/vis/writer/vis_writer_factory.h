/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __VIS_WRITER_FACTORY_H
#define __VIS_WRITER_FACTORY_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include <string>

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "apps/vis/writer/vis_writer.h"
#include "sys/simulation/simulation_controller.h"


namespace vis
{
   DECLARE_HANDLES(WriterFactory);

   /** \brief Base class for writer factories.
    * This class is the base class for all vis writer factories, which create 
    * writers of a given type. See vis::PdfWriterFactory for an example.
    * 
    */
   class WriterFactory
	   : public shawn::KeeperManaged
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      WriterFactory( void );
      virtual ~WriterFactory();
      ///@}

      ///@name Getter
      ///@{
      /**
       * Returns the name of the factory.
       */
	   virtual std::string name( void ) const throw();
      /**
       * Returns a short description of the factory.
       */
      virtual std::string description( void ) const throw();
      ///@}

      /**
       * Abstract method to create a visualization writer.
       */
	   virtual Writer* create( void ) throw(std::runtime_error) = 0;
   };
}

#endif
#endif
