#include "Mutex.h"

/**
* Class Mutex 
*/

Mutex::Mutex(){
#if defined(__DEF_MUTEX_WIN32)
	mutex= CreateMutex(NULL, FALSE, NULL);
#else  
	pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex,&attr);
#endif
}

Mutex::~Mutex(){
#if defined(__DEF_MUTEX_WIN32)
	if(mutex)
      CloseHandle(mutex);
#else
	pthread_mutex_destroy(&mutex);
#endif
}

bool Mutex::lock(bool block){
#if defined(__DEF_MUTEX_WIN32)
	if(mutex)
		return ((bool)WaitForSingleObject(mutex, block ? INFINITE : 0) == WAIT_OBJECT_0);
#else
	if(mutex){
	   if(block)
		   return pthread_mutex_lock(&mutex) == 0;
	   else
		   return pthread_mutex_trylock(&mutex) == 0;	
	}
#endif
	return false;	
}

void Mutex::unlock(){
#if defined(__DEF_MUTEX_WIN32)
	if(mutex)
		ReleaseMutex(mutex);
#else	
	if(mutex)
		pthread_mutex_unlock(&mutex);
#endif
} 