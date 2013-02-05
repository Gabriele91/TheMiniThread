#ifndef STREAMING_THREAD_H
#define STREAMING_THREAD_H

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
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

	enum ThreadState{
		SUCCESSFUL=0,
		ERROR_CREATE,
		ERROR_JOIN
	};

	enum ActionOnDelete{
		TERMINATE_NONE=0,
		TERMINATE_JOIN,
		TERMINATE_DELETE
	};
	
	stfn mFunction;
	void * mArg;

	Thread(stfn fn,void * arg,ActionOnDelete cad=TERMINATE_NONE);
	~Thread();
	static void YieldCpu();
	static void SleepThread(unsigned int msec);
	bool Start();
	int Join();
	void Destroy();
	int GetError();

	
	private:
	
	typedef int (*stfn)(void *arg);
	thrd_t thr;
	int th_error;
	bool cad;
	
};

#endif
