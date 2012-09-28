/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/node_movements/playback/movement_reset_task.h"
#include "sys/event_scheduler.h"

#include <string>

namespace shawn
{

    MovementResetTask::
        MovementResetTask()
    {}
    // ----------------------------------------------------------------------
    MovementResetTask::
        ~MovementResetTask()
    {}
    // ----------------------------------------------------------------------
    void
        MovementResetTask::
        run( SimulationController& sc )
        throw( std::runtime_error )
    {
       if (sc.environment_w().optional_string_param("reset", "")=="true")
          sc.world_w().reset();
    }
    // ----------------------------------------------------------------------
    std::string
        MovementResetTask::
        name( void )
        const throw()
    {
        return std::string("reset_movement");
    }
    // ----------------------------------------------------------------------
    std::string
        MovementResetTask::
        description( void )
        const throw()
    {
        return std::string("Resets the world");
    }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/playback/movement_reset_task.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: movement_reset_task.cpp,v $
 *-----------------------------------------------------------------------*/
