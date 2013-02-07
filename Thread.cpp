#include "Thread.h"

#include <stdio.h>
#include <stdlib.h>
#if defined(__DEF_THREAD_POSIX)
  #include <signal.h>
  #include <sched.h>
  #include <unistd.h>
  #include <sys/time.h>
  #include <errno.h>
#elif defined(__DEF_THREAD_WIN32)
  #include <process.h>
  #include <sys/timeb.h>
#endif


/**
* Class Thread 
*/

#if defined(__DEF_THREAD_WIN32)
static unsigned WINAPI _thrd_wrapper_function(void * aArg){
  Thread *_this = (Thread *) aArg;
  return _this->run();
}
#elif defined(__DEF_THREAD_POSIX)
void thread_exit_handler(int sig){
	pthread_exit(0);
}
static void * _thrd_wrapper_function(void * aArg){

  int  res;
  void *pres;
  ///SET OUT SIGNAL
  struct sigaction actions;
  memset(&actions, 0, sizeof(actions));
  sigemptyset(&actions.sa_mask);
  actions.sa_flags = 0;
  actions.sa_handler = thread_exit_handler;
  int rc = sigaction(SIGUSR1,&actions,NULL);
  ///
  Thread *_this = (Thread *) aArg;
  res=_this->run();

  pres = malloc(sizeof(int));
  if (pres != NULL)
  {
    *(int*)pres = res;
  }

  return pres;

}
#endif

Thread::Thread(actionOnDelete cad)
        :thisThFun(NULL)
        ,thisThFunArgs(NULL)
        ,cad(cad)
        ,thr(NULL)
        ,th_error(SUCCESSFUL){}

Thread::Thread(threadFunction fn,
               void* args,
               actionOnDelete cad)
               :thisThFun(fn)
               ,thisThFunArgs(args)
               ,cad(cad)
               ,thr(NULL)
               ,th_error(SUCCESSFUL){}

Thread::~Thread(){
	if(thr){
		switch(cad){
			case TERMINATE_JOIN: join(); break;
			case TERMINATE_DELETE: destroy(); break;
			default: break;
		}
	}
}

bool Thread::start(){

    #if defined(__DEF_THREAD_WIN32)
        thr =  (HANDLE)_beginthreadex(NULL,0,
                                      _thrd_wrapper_function,
                                      (void*)this,
                                      0,NULL);
    #elif defined(__DEF_THREAD_POSIX)
        if(pthread_create(&thr,NULL,
                          _thrd_wrapper_function,
                          (void *)this) != 0)
                          thr = 0;
    #endif

    if(!thr)
        th_error=ERROR_CREATE;

    th_error=SUCCESSFUL;

    return thr!=0;
}

void Thread::yield(){
    #if defined(__DEF_THREAD_WIN32)
      Sleep(0);
    #else
      sched_yield();
    #endif
}

void Thread::sleepThread(unsigned int msec){
#ifdef __DEF_THREAD_WIN32
    Sleep(msec);
    #else
    usleep(msec * 1000);
#endif
}

void Thread::destroy(){
#ifdef __DEF_THREAD_WIN32
    TerminateThread((HANDLE)thr, 0);
    #else
	int status=0;
	if ( (status = pthread_kill(thr, SIGUSR1)) != 0){
		//error
		#ifndef __ANDROID__
			 pthread_cancel(thr);
		#endif
	}
    pthread_join(thr, 0);
#endif
	//reset
	thr=0;
}

int Thread::join(){
#if defined(__DEF_THREAD_WIN32)
  if (WaitForSingleObject(thr, INFINITE) == WAIT_FAILED){
    this->th_error=ERROR_JOIN;
    return -1;
  }
  DWORD dwRes;
  GetExitCodeThread(thr, &dwRes);
  return (int)dwRes;
#elif defined(__DEF_THREAD_POSIX)
  void *pres;
  int ires = 0;
  if (pthread_join(thr, &pres) != 0){
    this->th_error=ERROR_JOIN;
    return -1;
  }
  if (pres != NULL){
    ires = *(int*)pres;
    free(pres);
  }
  return (int)ires;
#endif
  //reset
  thr=0;
}

int Thread::getError(){
	return th_error;
}

int Thread::run(){
    if(thisThFun)
        return thisThFun(thisThFunArgs);
    else
        return 0;
}
