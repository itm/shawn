/**
 * @file sumo_init.h
 * Initialisation of the SUMO app.
 *
 * @author Josef Schlittenlacher
 * @author Walter Bamberger
 *
 * $Revision: 546 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/sumo/sumo_init.h $
 * $Id: sumo_init.h 546 2011-04-15 10:40:27Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010 Josef Schlittenlacher<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_SUMO_SUMO_INIT_H
#define __SHAWN_APPS_SUMO_SUMO_INIT_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_SUMO

namespace shawn
{
class SimulationController;
}

/**
 * Initialises the SUMO app. (At present, only a message is printed out.)
 *
 * @param sc the Shawn simulation controller
 */
extern "C" void
init_sumo( shawn::SimulationController& sc );

/**
 * @namespace sumo
 * The namespace of the SUMO application.
 *
 * The SUMO Application offers access to the files of the network simulator
 * <a href="http://sumo.sourceforge.net">SUMO</a>.
 *
 * sumo::SumoNet parses a SUMO network file and offers its content to the
 * simulation. It can also translate between world coordinates and edge-based
 * coordinates.
 *
 * sumo::SumoNodeMovementCreator parses a SUMO dump file, which contains the
 * node movements. According to the dump file, it creates, moves and deletes the
 * nodes in the simulation. To realise this, it implements
 * a shawn::NodeMovementCreator.
 *
 * For test purposes, it contains the sumo::LogMovementProcessor. This
 * processor puts out the position of every node in a log file. This helps to
 * verify that the application works correctly. The sumo::LogMovementProcessor
 * resides in the folder <em>log_movement_processor</em>.
 *
 * Usage in the Shawn configuration file:
 *
 * @code
 * node_movement mode=sumo net_file=NET dump_file=DUMP processors=PROCESSOR
 * @endcode
 *
 * <dl>
 * <dt>mode=sumo<dd>Activates this application.
 * <dt>NET<dd>The SUMO net file on which the SUMO dump file is based.
 * <dt>DUMP<dd>The SUMO dump file containing the node movements.
 * <dt>PROCESSORS<dd>A list of processors that should be added
 * to every created node.
 * </dl>
 *
 * You can test the application with the following Shawn configuration file:
 *
 * @code
 * prepare_world edge_model=simple comm_model=disk_graph range=100
 * node_movement mode=sumo net_file=NET dump_file=DUMP processors=log_movement
 * simulation
 * @endcode
 */

#endif /* ENABLE_SUMO */
#endif /* __SHAWN_APPS_SUMO_SUMO_INIT_H */
