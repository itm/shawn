/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "sys/misc/os/system_time.h"
#include <iostream>

#ifdef SYS_MISC_OS_SYSTEM_TIME_WINDOWS
	#include <windows.h>
#endif

namespace shawn
{

	// ----------------------------------------------------------------------
	SystemTime::
		SystemTime()
	{
		touch();
		#ifdef SYS_MISC_OS_SYSTEM_TIME_WINDOWS
			LARGE_INTEGER f;
			if (!QueryPerformanceFrequency((LARGE_INTEGER*)&f))
				std::cout << "Performance Counter not applicable" << std::endl;
			else
				g_Frequency = f.QuadPart;
		#endif
	}

	// ----------------------------------------------------------------------
	void
		SystemTime::
		touch()
		throw()
	{
		#ifdef SYS_MISC_OS_SYSTEM_TIME_UNIX
			gettimeofday(&last_touch_, NULL);
		#else
			#ifdef SYS_MISC_OS_SYSTEM_TIME_WINDOWS
				LARGE_INTEGER t;
				QueryPerformanceCounter((LARGE_INTEGER*)&t);
				g_LastTouch = t.QuadPart;
			#else
				last_touch_ = GetTickCount();
			#endif
		#endif
	}

	#ifdef SYS_MISC_OS_SYSTEM_TIME_UNIX
	// ----------------------------------------------------------------------
	int
		SystemTime::
		diff(struct timeval* result, struct timeval* x, struct timeval* y)
	{
	   // Perform the carry for the later subtraction by updating y
	   if (x->tv_usec < y->tv_usec)
	   {
		 int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
		 y->tv_usec -= 1000000 * nsec;
		 y->tv_sec += nsec;
	   }

	   if (x->tv_usec - y->tv_usec > 1000000)
	   {
		   int nsec = (x->tv_usec - y->tv_usec) / 1000000;
	       y->tv_usec += 1000000 * nsec;
	       y->tv_sec -= nsec;
	   }

	   //Compute the time remaining to wait tv_usec is certainly positive
	   result->tv_sec = x->tv_sec - y->tv_sec;
	   result->tv_usec = x->tv_usec - y->tv_usec;

	   return x->tv_sec < y->tv_sec;
	}
	#endif

	// ----------------------------------------------------------------------
	unsigned long
		SystemTime::
		ms_since_last_touch()
	{
		#ifdef SYS_MISC_OS_SYSTEM_TIME_UNIX
			struct timeval	now, res;
			gettimeofday(&now, NULL);
			diff(&res, &now, &last_touch_ );
			return res.tv_sec * 1000 + (res.tv_usec / 1000);
		#else
			#ifdef SYS_MISC_OS_SYSTEM_TIME_WINDOWS
				long long now;
				QueryPerformanceCounter((LARGE_INTEGER*)&now);
				return (unsigned long)((1000.0*(double)(now-g_LastTouch))/((double)g_Frequency));
			#else
				return GetTickCount() - last_touch_;
			#endif
		#endif
	}
	// ----------------------------------------------------------------------
	unsigned long
		SystemTime::
		us_since_last_touch()
	{
		#ifdef SYS_MISC_OS_SYSTEM_TIME_UNIX
			struct timeval	now, res;
			gettimeofday(&now, NULL);
			diff(&res, &now, &last_touch_ );
			return res.tv_sec * 1000000 + (res.tv_usec);
		#else
			#ifdef SYS_MISC_OS_SYSTEM_TIME_WINDOWS
				LARGE_INTEGER now;
				QueryPerformanceCounter((LARGE_INTEGER*)&now);
				return (unsigned long)((1000000.0*(double)((long long)(now.QuadPart)-g_LastTouch))/((double)g_Frequency));
			#else
				return (GetTickCount() - last_touch_)/1000;
			#endif
		#endif
	}

	// ----------------------------------------------------------------------
	void
		SystemTime::
		sleep(unsigned long ms)
		throw()
	{
		#ifdef SYS_MISC_OS_SYSTEM_TIME_UNIX
			usleep( 1000 * ms );
		#else
			#ifdef SYS_MISC_OS_SYSTEM_TIME_WINDOWS
				Sleep( ms );
			#else
				Sleep( ms );
			#endif
		#endif
	}

	// ----------------------------------------------------------------------
	void
		SystemTime::
		sleep_us(unsigned long us)
		throw()
	{
		#ifdef SYS_MISC_OS_SYSTEM_TIME_UNIX
			usleep( us );
		#else
			#ifdef SYS_MISC_OS_SYSTEM_TIME_WINDOWS
				LONGLONG start, now;
				QueryPerformanceCounter((LARGE_INTEGER*)&start);
				//int count = 0;
				if (us > 10000)
					sleep( (us / 10000) *10);
				while (1)
				{
					//count++;
					QueryPerformanceCounter((LARGE_INTEGER*)&now);
					if ( ((1000000.0*(double)(now-start))/((double)g_Frequency)) >= (double)us)
						break;
				}
				//std::cout << count << " done " << ((1000000.0*(double)(now-start))/((double)g_Frequency)) << " should be: " << us  << std::endl << std::flush;
			#else
				Sleep( us /1000 );
			#endif
		#endif
	}


}
