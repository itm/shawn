/**
 * @file vanet_init.h
 * Declares the app's initialisation functions.
 *
 * @author Walter Bamberger
 *
 * $Revision: 548 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/vanet_init.h $
 * $Id: vanet_init.h 548 2011-04-15 10:46:50Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_INIT_H
#define __SHAWN_APPS_VANET_INIT_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

namespace shawn
{
class SimulationController;
}

/**
 * Initialises the VANET application.
 *
 * This method registers the processor factories and the tasks
 * (at present, vanet::InitVanetTask and vanet::CreateEnvironmentTask).
 *
 * @param sc the Shawn simulation controller to register the tasks
 *
 */
extern "C" void
init_vanet( shawn::SimulationController& sc);

/**
 * @namespace vanet
 * Namespace of the Shawn application for vehicular ad-hoc networks.
 *
 * The VANET application simulates the information processing in a vehicular
 * network. The focus is on the information exchanged on the application level.
 * Lower protocol layers are only considered to extend of their effects on the
 * application layer.
 *
 * The application collects several algorithms for the information processing
 * encapsulated in components. Currently the following components exist:
 *
 * <dl>
 * <dt>Sensor</dt>
 *     <dd>Perceives the environment and implements an error model. These
 *     components reside in the folder <em>vehicle</em> at present. See also
 *     vanet::ValueTypeIntSensor for an exemplary implementation.</dd>
 * <dt>Knowledge base</dt>
 *     <dd>Organizes and stores various kinds of information. This component
 *     resides in the folder <em>knowledge_base</em>.
 *     See also vanet::KnowledgeBase.</dd>
 * <dt>Information integration</dt>
 *     <dd>Relates received pieces of information to each other and infers the
 *     vehicle's truth or reality of the world. All pieces of information are
 *     organised in the knowledge base. The component may include trust
 *     and reputation. These components reside in the folder
 *     <em>information_integration</em>. See also vanet::EvidenceIntegrator for
 *     an exemplary implementation.</dd>
 * <dt>Trust</dt>
 *     <dd>Computes trust from own experiences. These components reside in the
 *     folder <em>trust</em>. See also vanet::TrustComputer, the interface of
 *     these components.</dd>
 * <dt>Reputation</dt>
 *     <dd>Computes reputation from the recommendations of others. These
 *     components are highly related to the trust components. Some related
 *     algorithms are sometimes trust and reputation algorithms together. Thus
 *     reputation components also reside in the folder <em>trust</em>. See also
 *     vanet::ReputationComputer, the interface of these components.</dd>
 * <dt>Dissemination</dt>
 *     <dd>Decides what information is sent to which node in the current
 *     situation. This is the right place if you want to implement and attack on
 *     the network. These components reside in the folder <em>dissemination</em>.
 *     See also vanet::Disseminator, the interface of these components.</dd>
 * <dt>Processor</dt>
 *     <dd>The processor controls the interaction between the components and
 *     represents the gateway to the simulation core. Currently there is a
 *     processor for a vehicle in the folder <em>vehicle</em>: the
 *     vanet::VehicleProcessor. A processor for a roadside unit is planed
 *     but not implemented yet.</dd>
 * </dl>
 *
 * You can find an introduction to the development with this application on the
 * page @ref vanet-architecture.
 */

#endif
#endif
