/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/tasks/vis_single_snapshot.h"
#include "apps/vis/writer/vis_writer_factory.h"
#include "apps/vis/writer/vis_writer_keeper.h"

#include <cairo.h>
#include <time.h>

using namespace shawn;

namespace vis
{

   SingleSnapshotTask::
   SingleSnapshotTask()
   {}
   // ----------------------------------------------------------------------
   SingleSnapshotTask::
   ~SingleSnapshotTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   SingleSnapshotTask::
   name( void )
      const throw()
   { return "vis_single_snapshot"; }
   // ----------------------------------------------------------------------
   std::string
   SingleSnapshotTask::
   description( void )
      const throw()
   { return "XXX"; }
   // ----------------------------------------------------------------------
   void
   SingleSnapshotTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);

      double t = sc.environment().optional_double_param("time",0.0);
      int dr = sc.environment().optional_int_param("draft",0);

	   WriterFactoryHandle wfh = sc.keeper_by_name_w<WriterKeeper>("WriterKeeper")
		   ->find_w(sc.environment().optional_string_param("writer", "pdf"));
	   Writer* wr = wfh->create();

      wr->set_draft(dr);
      wr->pre_write( visualization(), sc.environment().optional_string_param("filename", "snapshot"), false );
      wr->write_frame( t );
      wr->post_write();

      delete wr;
   }


}
#endif
