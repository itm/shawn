/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_OS_SYSTEM_TIME_H__
#define __SHAWN_SYS_MISC_OS_SYSTEM_TIME_H__

/* This does not work, because you can have a GNU compiler under windows, too.
#ifdef __GNUC__
	#define SYS_MISC_OS_SYSTEM_TIME_UNIX
#elif defined WIN32
	//Nothing to do, currently only one of both OSs is supported
#else
	#error No operating system support defined
#endif
*/

// Enable none or one of the two following defines:
//#define SYS_MISC_OS_SYSTEM_TIME_WINDOWS
//#define SYS_MISC_OS_SYSTEM_TIME_UNIX

/** Should work this way */

#ifdef _WINDOWS					//Windows case
#ifndef CYGWIN					//Not using Cygwin
#define SYS_MISC_OS_SYSTEM_TIME_WINDOWS
#elif defined CYGWIN			//Using Cygwin
#define SYS_MISC_OS_SYSTEM_TIME_UNIX
#endif
#endif

#ifndef _WINDOWS				//Unix case
#define SYS_MISC_OS_SYSTEM_TIME_UNIX
#endif

#ifdef SYS_MISC_OS_SYSTEM_TIME_UNIX
	#include <unistd.h>
	#include <sys/time.h>
#else
	#include <windows.h>
	#include <winbase.h>
#endif


namespace shawn
{

	// ----------------------------------------------------------------------
	/**
	 *
	 */
	class SystemTime
	{
	public:
		/**
		 *
		 */
		SystemTime();

		/**
		 *
		 */
		void touch() throw();

		/**
		 * @return
		 */
		unsigned long ms_since_last_touch();
		unsigned long us_since_last_touch();

		/**
		 * @param ms
		 */
		void sleep(unsigned long ms) throw();
		void sleep_us(unsigned long us) throw();

	private:
		#ifdef SYS_MISC_OS_SYSTEM_TIME_UNIX
			/**
			 * @param result
			 * @param x
			 * @param y
			 * @return
			 */
			int diff(struct timeval* result, struct timeval* x, struct timeval* y);
		#endif

		#ifdef SYS_MISC_OS_SYSTEM_TIME_UNIX
			/// The reference time for all calls
			struct timeval last_touch_;
		#else
			#ifdef SYS_MISC_OS_SYSTEM_TIME_WINDOWS
				long long g_Frequency, g_LastTouch;
				//unsigned long last_touch_;
			#else
				/// The reference time for all calls
				unsigned long last_touch_;
			#endif
		#endif
	};

}
#endif
