/**
 * @file doxygen_main_page.h
 * Contains main page of the Doxygen documentation. This main page is
 * independent of any code and any Shawn application. So it resides in a
 * separate file in the main source directory.
 *
 * @author Walter Bamberger
 *
 * $Revision: 549 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/doxygen_main_page.h $
 * $Id: doxygen_main_page.h 549 2011-04-15 10:48:57Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_DOXYGEN_MAIN_PAGE_H
#define __SHAWN_DOXYGEN_MAIN_PAGE_H

/**
 * @mainpage Shawn Code Documentation
 *
 * This manual documents the source code of the Shawn simulation core and of
 * some Shawn applications. It addresses developers who want to use the Shawn
 * API or extend Shawn itself. The main documentation resides in the
 * <a href="https://www.itm.uni-luebeck.de/ShawnWiki/index.php/Main_Page">Shawn wiki</a>.
 * Especially read the page
 * <a href="https://www.itm.uni-luebeck.de/ShawnWiki/index.php/Extending_Shawn">Extending Shawn</a>
 * there.
 *
 * Every Shawn application has its own C++ namespace: the name of the
 * application in lower case. The documentation of the namespace gives an
 * overview of the application's source code. In addition, an application
 * may provide some additional information in the <em>Related Pages</em>
 * section.
 *
 * Shawn has quite many applications for different purposes. Only the source
 * code of the following applications is documented here.
 *
 * @section sec-sys The Shawn Core
 *
 * The simulation core of Shawn can be found in the <em>sys</em> folder. Its
 * namespace is @ref shawn.
 *
 * @section sec-sumo The SUMO Application
 *
 * The SUMO Application offers access to the files of the network simulator
 * SUMO. Currently it support SUMO network files and SUMO dump files.
 * Its source code is documented in the namespace @ref sumo. There you can also
 * find configuration file examples for normal usage and testing.
 *
 * @section sec-vanet The VANET Application
 *
 * With the VANET application you can simulate the information flow in a
 * vehicular network and test your information processing algorithms. Its
 * source code is documented in the namespace @ref vanet. The additional page
 * @ref vanet-architecture describes the concept of the application.
 */

#endif /* __SHAWN_DOXYGEN_MAIN_PAGE_H */
