/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_VIS_LIVEVIEW_H
#define __SHAWN_VIS_LIVEVIEW_H
#include "../buildfiles/_apps_enable_cmake.h"

#ifdef ENABLE_VIS
#ifdef HAVE_GLUT

#ifdef HAVE_BOOST
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#endif

namespace vis
{
/**
 * Creates an external Window of the provided size an using the provided
 * texture resolution.
 */
void createWindow(int sizex, int sizey, int resx, int resy);

/**
 * Notifies the external window that the texture has changed and needs to
 * be updated.
 */
void updateTexture(unsigned char* textureData);

/**
 * Returns a pointer to the currently used texture data.
 */
unsigned char* getTexture();

#ifdef HAVE_BOOST
/**
 * Returns the mutex that needs to be locked before changing or reading
 * texture data.
 */
boost::mutex* getUpdateMutex();
#endif

}
#endif
#endif
#endif
