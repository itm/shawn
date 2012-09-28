/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/vis_base_init.h"
#include "apps/vis/base/visualization_keeper.h"
#include "apps/vis/writer/vis_writer_keeper.h"
#include "apps/vis/writer/vis_png_writer_factory.h"
#include "apps/vis/writer/vis_pdf_writer_factory.h"
#include "apps/vis/writer/vis_ps_writer_factory.h"
#include "apps/vis/elements/vis_drawable_node_default_factory.h"
#include "apps/vis/elements/vis_drawable_node_graphics_factory.h"
#include "apps/vis/elements/vis_drawable_node_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/vis/examples/processors/vis_energy_processor.h"
#include "apps/vis/examples/processors/vis_energy_processor_factory.h"

namespace vis
{

   void init_vis_base( shawn::SimulationController& sc )
   {
      sc.add_keeper( new VisualizationKeeper );
      sc.add_keeper( new WriterKeeper );
      sc.add_keeper( new DrawableNodeKeeper );
      PdfWriterFactory::register_factory(sc);
      PngWriterFactory::register_factory(sc);
      PsWriterFactory::register_factory(sc);
      DrawableNodeDefaultFactory::register_factory( sc );
      DrawableNodeGraphicsFactory::register_factory( sc );

#ifdef ENABLE_EXAMPLES
      VisEnergyProcessorFactory::register_factory(sc);
#endif
   }

}

#endif
