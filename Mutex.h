#ifndef MUTEX_H
#define MUTEX_H

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) || defined(WIN32)
	#include "MutexWin32.inl"
#else
	#include "MutexPOSIX.inl"
#endif

#endif