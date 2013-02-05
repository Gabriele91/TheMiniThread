#include "StreamingThread.h"

#include <stdio.h>
#include <stdlib.h>
#if defined(_TTHREAD_POSIX_)
  #include <signal.h>
  #include <sched.h>
  #include <unistd.h>
  #include <sys/time.h>
  #include <errno.h>
#elif defined(_TTHREAD_WIN32_)
  #include <process.h>
  #include <sys/timeb.h>
#endif


#if defined(_TTHREAD_WIN32_)
static unsigned WINAPI _thrd_wrapper_function(void * aArg){
  Thread *_this = (Thread *) aArg;
  return _this->mFunction(_this->mArg);
}
#elif defined(_TTHREAD_POSIX_)
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
  res=_this->mFunction(_this->mArg);

  pres = malloc(sizeof(int));
  if (pres != NULL)
  {
    *(int*)pres = res;
  }

  return pres;

}
#endif

Thread::Thread(stfn fn,void * arg)
				:mFunction(fn)
				,mArg(arg){}

bool Thread::Start(){

    #if defined(_TTHREAD_WIN32_)
        thr =  (HANDLE)_beginthreadex(NULL,0,
                                      _thrd_wrapper_function,
                                      (void*)this,
                                      0,NULL);
    #elif defined(_TTHREAD_POSIX_)
        if(pthread_create(&thr,NULL,
                          _thrd_wrapper_function,
                          (void *)this) != 0)
                          thr = 0;
    #endif

    if(!thr)
        th_error=ST_ERROR_CREATE;

    th_error=ST_ERROR_CREATE;

    return thr!=0;
}

void Thread::YieldCpu(){
    #if defined(_TTHREAD_WIN32_)
      Sleep(0);
    #else
      sched_yield();
    #endif
}

void Thread::SleepThread(unsigned int msec){
#ifdef _TTHREAD_WIN32_
    Sleep(msec);
    #else
    usleep(msec * 1000);
#endif
}

void Thread::Destroy(){
#ifdef _TTHREAD_WIN32_
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
}

int Thread::Join(){
#if defined(_TTHREAD_WIN32_)
  if (WaitForSingleObject(thr, INFINITE) == WAIT_FAILED){
    this->th_error=ST_ERROR_JOIN;
    return -1;
  }
  DWORD dwRes;
  GetExitCodeThread(thr, &dwRes);
  return (int)dwRes;
#elif defined(_TTHREAD_POSIX_)
  void *pres;
  int ires = 0;
  if (pthread_join(thr, &pres) != 0){
    this->th_error=ST_ERROR_JOIN;
    return -1;
  }
  if (pres != NULL){
    ires = *(int*)pres;
    free(pres);
  }
  return (int)ires;
#endif
}

