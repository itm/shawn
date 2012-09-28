/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/writer/vis_pdf_writer_factory.h"
#include "apps/vis/writer/vis_writer_keeper.h"
#include "apps/vis/writer/vis_pdf_writer.h"

namespace vis
{
   PdfWriterFactory::
   PdfWriterFactory( void )
   {}

   PdfWriterFactory::~PdfWriterFactory()
   {}

   // ----------------------------------------------------------------------
   void
   PdfWriterFactory::
   register_factory( shawn::SimulationController& sc )
      throw()
   {
	   WriterKeeper* wkeeper = sc.keeper_by_name_w<WriterKeeper>( "WriterKeeper" );
	   if(wkeeper != NULL)
	   {
		   wkeeper->add(new PdfWriterFactory());
	   }
   }

   // ----------------------------------------------------------------------
   std::string
   PdfWriterFactory::
   name( void )
      const throw()
   { 
	   return "pdf"; 
   }
   
   // ----------------------------------------------------------------------
   std::string 
   PdfWriterFactory::
   description( void )
      const throw()
   {
      return "Vis filewriter objects";
   }

   // ----------------------------------------------------------------------
   Writer* 
   PdfWriterFactory::
   create( void )
	throw( std::runtime_error )
   {
	   return new PDFWriter();
   }

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/processors/processor_factory.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: processor_factory.cpp,v $
 *-----------------------------------------------------------------------*/
