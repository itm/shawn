/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/writer/vis_ps_writer_factory.h"
#include "apps/vis/writer/vis_writer_keeper.h"
#include "apps/vis/writer/vis_ps_writer.h"

namespace vis
{
   PsWriterFactory::
   PsWriterFactory( void )
   {}

   PsWriterFactory::~PsWriterFactory()
   {}

   // ----------------------------------------------------------------------
   void
   PsWriterFactory::
   register_factory( shawn::SimulationController& sc )
      throw()
   {
	   WriterKeeper* wkeeper = sc.keeper_by_name_w<WriterKeeper>( "WriterKeeper" );
	   if(wkeeper != NULL)
	   {
		   wkeeper->add(new PsWriterFactory());
	   }
   }

   // ----------------------------------------------------------------------
   std::string
   PsWriterFactory::
   name( void )
      const throw()
   { 
	   return "ps"; 
   }
   
   // ----------------------------------------------------------------------
   std::string 
   PsWriterFactory::
   description( void )
      const throw()
   {
      return "Vis filewriter objects";
   }

   // ----------------------------------------------------------------------
   Writer* 
   PsWriterFactory::
   create( void )
	throw( std::runtime_error )
   {
	   return new PsWriter();
   }

}

#endif
