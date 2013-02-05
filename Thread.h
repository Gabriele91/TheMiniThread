#ifndef STREAMING_THREAD_H
#define STREAMING_THREAD_H

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    #define _TTHREAD_WIN32_
    #include <windows.h>
    typedef HANDLE thrd_t;
#else
    #define _TTHREAD_POSIX_
    #include <pthread.h>
    typedef pthread_t thrd_t;
#endif

class Thread{

	typedef int (*stfn)(void *arg);
	thrd_t thr;
	int th_error;

	public:

	enum {
		ST_SUCCESSFUL,
		ST_ERROR_CREATE,
		ST_ERROR_JOIN
	};


	stfn mFunction;
	void * mArg;

	Thread(stfn fn,void * arg);
	static void YieldCpu();
	static void SleepThread(unsigned int msec);
	bool Start();
	int Join();
	void Destroy();
	int GetError(){ 
		return th_error;
	}

};

#endif
