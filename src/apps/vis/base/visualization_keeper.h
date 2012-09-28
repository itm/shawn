/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_VISUALIZATION_KEEPER_H
#define __SHAWN_TUBSAPPS_VIS_VISUALIZATION_KEEPER_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "sys/util/handle_keeper.h"
#include "apps/vis/base/visualization.h"
#include <string>


namespace vis
{
   /**
    * Visualization keeper class.
    */
   class VisualizationKeeper
      : public shawn::HandleKeeper<Visualization>
   {
   public:
      /// Name of the keeper.
      static const std::string KEEPER_NAME;

      ///@name Contructor/Destructor
      ///@{
      VisualizationKeeper();
      virtual ~VisualizationKeeper();
      ///@}
   };


}

#endif
#endif
