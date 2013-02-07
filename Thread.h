#ifndef THREAD_H
#define THREAD_H

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) || defined(WIN32)
    #define __DEF_THREAD_WIN32
    #include <windows.h>
    typedef HANDLE thrd_t;
#else
    #define __DEF_THREAD_POSIX
    #include <pthread.h>
    typedef pthread_t thrd_t;
#endif

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

	Thread(actionOnDelete cad=TERMINATE_NONE);
	Thread(threadFunction fn,void* args=NULL,actionOnDelete cad=TERMINATE_NONE);
	virtual ~Thread();
	void yield();
	void sleepThread(unsigned int msec);
	bool start();
	int join();
	void destroy();
	int getError();
	virtual int run();

    protected:

    threadFunction thisThFun;
    void * thisThFunArgs;

	private:

	actionOnDelete cad;
	thrd_t thr;
	int th_error;

};

#endif
