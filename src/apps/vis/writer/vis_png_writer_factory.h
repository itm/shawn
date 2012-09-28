/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __VIS_PNG_WRITER_FACTORY_H
#define __VIS_PNG_WRITER_FACTORY_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include <string>

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "apps/vis/writer/vis_writer.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/vis/writer/vis_writer_factory.h"


namespace vis
{
   DECLARE_HANDLES(PngWriterFactory);

   /** \brief A factory for creating PNGWriter instances.
    * This WriterFactory creates instances of PNGWriter, which are used to
    * output the visualization to PNG image files.
    */
   class PngWriterFactory
	   : public WriterFactory
   {
   public:
      ///@name Constructor/Destructor
      ///@{
      PngWriterFactory( void );
      virtual ~PngWriterFactory();
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
      * Creates and returns a PNGWriter.
      */
      virtual Writer* create( void ) throw(std::runtime_error);

      /**
      * Static method to register the factory to the WriterKeeper.
      */
      static void register_factory( shawn::SimulationController& ) throw();
   };
}

#endif
#endif
