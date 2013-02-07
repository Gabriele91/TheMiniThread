#include <pthread.h>
typedef pthread_t thrd_t;

class Thread{

	public:

	enum threadState{
		SUCCESSFUL=0,
		ERROR_CREATE,
		ERROR_JOIN
	};
	enum actionOnDelete{
		TERMINATE_NONE=0,
		TERMINATE_JOIN,
		TERMINATE_DELETE
	};
    typedef int (*threadFunction)(void*);


	Thread(actionOnDelete cad=TERMINATE_NONE)
		   :thisThFun(NULL)
		   ,thisThFunArgs(NULL)
		   ,cad(cad)
		   ,thr(NULL)
		   ,th_error(SUCCESSFUL){}
	Thread(threadFunction fn,
		   void* args=NULL,
		   actionOnDelete cad=TERMINATE_NONE)
		   :thisThFun(fn)
		   ,thisThFunArgs(args)
		   ,cad(cad)
		   ,thr(NULL)
		   ,th_error(SUCCESSFUL){}

	bool Thread::start(){
		if(pthread_create(&thr,NULL,
						  _thrd_wrapper_function,
						  (void *)this) != 0)
						  thr = NULL;
		if(!thr)
			th_error=ERROR_CREATE;

		th_error=SUCCESSFUL;

		return thr!=0;
	}

	void Thread::yield(){
		  sched_yield();
	}

	void Thread::sleepThread(unsigned int msec){
		usleep(msec * 1000);
	}

	void Thread::destroy(){
		int status=0;
		if ( (status = pthread_kill(thr, SIGUSR1)) != 0){
			//error
			#ifndef __ANDROID__
				 pthread_cancel(thr);
			#endif
		}
		pthread_join(thr, 0);

		thr=NULL;
	}

	int Thread::join(){
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
	  thr=NULL;
	  return (int)ires;
	}

	int Thread::getError(){
		return th_error;
	}

	virtual int Thread::run(){
		if(thisThFun)
			return thisThFun(thisThFunArgs);
		else
			return 0;
	}


    protected:

    threadFunction thisThFun;
    void * thisThFunArgs;

	private:

	actionOnDelete cad;
	thrd_t thr;
	int th_error;
	static void thread_exit_handler(int sig){
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
};