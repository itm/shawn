/**
 * @file doxygen_architecture_page.h
 * Contains a separate page of the Doxygen documentation about the applicaton's
 * architecture
 *
 * @author Walter Bamberger
 *
 * $Revision: 543 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/vanet_init.h $
 * $Id: vanet_init.h 543 2011-04-14 13:51:12Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_DOXYGEN_ARCHITECTURE_PAGE_H
#define __SHAWN_APPS_VANET_DOXYGEN_ARCHITECTURE_PAGE_H

/**
 * @page vanet-architecture Architecture of the VANET Application
 *
 * With the VANET application, you can test algorithms for the information
 * processing in vehicular networks. The application implements several
 * components related to the information flow in vehicular networks:
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
 * Dividing the information processing algorithms in all these components makes
 * it easy to add and test new algorithms. Finally you can easily compare your
 * algorithms with those of others in the same scenarios.
 *
 * The shawn::Node and the vanet::VehicleProcessor manage the interaction
 * between these components and connect them to the VANET and the
 * simulation core.
 *
 * The following section detail the information flow in the VANET application.
 * After that, I give example on how you could implement and test your
 * algorithms with the help of this application.
 *
 * @section vanet-sec-information-flow Information Flow
 *
 * The VANET application contains algorithms for all parts of the information
 * flow in a vehicular network – for gathering, distributing and organizing
 * information. This section details the interaction of the application's
 * components when processing information. The descriptions and illustrations
 * always take one concrete component (that is, a certain algorithm or model) as
 * an example, although the components can easily be exchanged. This form of
 * description makes it easier to understand the architecture of the application
 * and the interaction between the components.
 *
 * @subsection vanet-sec-from-environment Gathering Information from the Environment
 *
 * In a real vehicular network as well as in the simulated one, all information
 * has its origin in the environment, that is, in components that are not part
 * of the vehicle or the vehicular network. Examples might be the driver, a
 * traffic sign, or simply the situational setting of the environment (like ice
 * on the road). A vehicle has input components to receive this information:
 * the sensors.
 *
 * In Shawn, a reading::Sensor gets information about the environment from a
 * reading::Reading (see
 * <a href="http://www.ibr.cs.tu-bs.de/users/brinkm/ws08/session5.pdf">
 * session 5</a> of the tutorial <a
 * href="http://www.itm.uni-luebeck.de/ShawnWiki/index.php/Shawn_in_3_days">
 * Shawn in 3 days</a>). The reading represents the environment in the
 * simulation. The vanet::ValueTypeIntReading can be filled with environmental
 * events (pieces of information) through a readings file. Vehicles can then
 * talk about these environmental events through the network. So all you have to
 * do, is to create the readings file. This way, you get the information in the network.
 * The Python program makeReadingsFile.py (in the sub-folder
 * <em>environment</em>) helps you to spread random events over a SUMO network
 * and save them in a readings file.
 *
 * The following figure shows the information flow from the readings file into
 * the knowledge base. The task <em>create_environment</em> reads the readings
 * file and saves it in the vanet::ValueTypeIntReading. This must happen before
 * the simulation starts. Then in every simulation round, every
 * vanet::VehicleProcessor polls its vanet::ValueTypeIntSensor once to get the
 * current sensor value. The sensor in turn asks the reading for the current
 * value at the vehicles location. This value is given back to the vehicle,
 * which saves it as an vanet::Observation in the knowledge base.
 *
 * @image html obtain-sensor-values.png "UML sequence diagram that illustrates the information flow from the readings file through the sensor into the knowledge base."
 *
 * You can see, the processor controls this process. It triggers the action
 * while the other components only react on these triggers.
 *
 * Now, some information has found its way in the knowledge base. The next
 * section shows how information from the knowledge base is sent into the
 * vehicular network.
 *
 * @subsection vanet-sec-sending Sending Information in the Vehicular Network
 *
 * A vehicle can only send what it knows. So sending a message invokes an
 * information flow from the knowledge base into the vehicular network. A
 * vehicle must first gather information before it can set something.
 * Information can be gathered @ref vanet-sec-from-environment "from the environment"
 * with the help of sensors (as shown in the previous section) or
 * @ref vanet-sec-from-network "from the vehicular network". The image below
 * illustrates the information flow.
 *
 * @image html send-message.png "UML sequence diagram that illustrates the information flow from the knowledge base into the vehicular network."
 *
 * Again the vanet::VehicleProcessor triggers the process. It invokes a
 * vanet::Disseminator – in the example, the vanet::ObservationDisseminator. A
 * disseminator decides what information to send. Several algorithms have been
 * proposed for this purpose in the literature. For example, you could always
 * send the newest pieces of information, or the most important ones, or a mix
 * of new and old pieces of information. Encapsulating the algorithm in a class
 * makes it possible to exchange it easily per configuration. This way, you
 * can test your vehicular network with different algorithms and even invent
 * your own one.
 *
 * The disseminator can also model an attack on the network. An attacker would
 * like to send certain messages in the network. A disseminator can control
 * this. Thus if you want to implement a certain attack on the network, you
 * must implement the attack as disseminators in the simulation. For example,
 * the vanet::ModifyingDisseminator implements an attacker that sends good
 * information for a while and switches its behaviour after a configurable
 * simulation round starting its attack. The attack is quite simple then:
 * A certain ratio of messages is manipulated.
 *
 * A vanet::VehicleProcessor can have several disseminators at once. They are
 * called one after the other during one simulation round. The disseminators can
 * be for different kinds of messages, or for an attack and for usual behaviour.
 *
 * @subsection vanet-sec-from-network Gathering Information from the Vehicular Network
 *
 * Now a message has entered the vehicular network. What happens in nearby
 * vehicles that receive this message? The figure below illustrates that
 * process. It is quite simple: The vanet::VehicleProcessor receives the message
 * from the simulation core and tranforms it in a vanet::Report. A report is
 * simple that part of a message that should be saved in the knowledge base.
 * Then the vehicle processor passes the report to the knowledge base. In
 * addition, it saves that it has met the other node. (This is kept for
 * statistical output.)
 *
 * @image html receive-message.png "UML sequence diagram that illustrates the information flow from the vehicular network into the knowledge base."
 *
 * So far, this documented introduced the main information flows and the
 * algorithms that process the information. The flow either starts or ends
 * in the knowledge base of the vehicle. This is an important property of the
 * VANET application. You can also see that the processes start at the
 * vanet::VehicleProcessor. This is the gateway to the simulation core. So if
 * you want to gain full control, you simply derive a new processor class from
 * vanet::VehicleProcessor and override the methods as you like.
 *
 * @subsection vanet-sec-integrating Integrating Information in the Knowledge Base
 *
 * This last subsection covers the processing in the knowledge base. It is not
 * another information flow but describes the important algorithm for
 * information integration. That algorithm organises the knowledge base. The
 * figure below shows how it is invoked from the knowledge base.
 *
 * @image html integrate-information.png "UML sequence diagram that illustrates the processing in the knowledge base."
 *
 * The process starts with some incoming information – be it an observation or
 * a report as described in the previous subsections. The knowledge base must
 * now decide whether to keep this new piece of information and where to assign
 * it to. In its internal data structures, the knowledge base relates pieces
 * of information to each other. An observation is associates with a location
 * and a type of information as well as with a time span. In addition, a
 * reported is connected to its sender and its reception time. So an algorithm
 * must decide, how a new piece of information relates to other objects in
 * the knowledge base. The algorithm in the information integrator exactly does
 * this. The algorithm also derives the vehicle's truth or reality from that.
 * This means, for example, if a vehicle receives several message about the same
 * thing just with different observation times, it will assume that the received
 * event was valid over a complete time span, not only at the given points in
 * time. So the vehicle wants to reason about the world with the received
 * observations and reports. This reasoning is done by the information
 * integrator.
 *
 * Again several algorithms exist in the literature to do this. To make the
 * algorithm exchangeable, it has been encapsulated a the separate class.
 *
 * @section vanet-sec-configuration How to Extend the VANET Application
 *
 */

#endif /* __SHAWN_APPS_VANET_DOXYGEN_ARCHITECTURE_PAGE_H */
