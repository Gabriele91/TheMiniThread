#include <windows.h>
#include <process.h>
typedef HANDLE thrd_t;

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

	virtual ~Thread(){
		if(thr){
			switch(cad){
				case TERMINATE_JOIN: join(); break;
				case TERMINATE_DELETE: destroy(); break;
				default: break;
			}
		}
	}
	bool start(){
		thr =  (HANDLE)_beginthreadex(NULL,0,
									  _thrd_wrapper_function,
									  (void*)this,
									   0,NULL);

		if(!thr)
			th_error=ERROR_CREATE;

		th_error=SUCCESSFUL;

		return thr!=0;
	}
	void Thread::yield(){
		  Sleep(0);
	}
	void Thread::sleepThread(unsigned int msec){
		Sleep(msec);
	}
	void Thread::destroy(){
		TerminateThread((HANDLE)thr, 0);		
		thr=NULL;
	}
	int Thread::join(){
	  if (WaitForSingleObject(thr, INFINITE) == WAIT_FAILED){
		this->th_error=ERROR_JOIN;
		return -1;
	  }
	  DWORD dwRes;
	  GetExitCodeThread(thr, &dwRes);
	  thr=NULL;
	  return (int)dwRes;
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

	static unsigned WINAPI _thrd_wrapper_function(void * aArg){
	  Thread *_this = (Thread *) aArg;
	  return _this->run();
	}

};