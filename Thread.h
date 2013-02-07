#ifndef THREAD_H
#define THREAD_H

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) || defined(WIN32)
	#include "ThreadWin32.inl"
#else
	#include "ThreadPOSIX.inl"
#endif


#endif
