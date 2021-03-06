TheMiniThread
=============

A tiny thread header only library

tested under windows, linux and android.

Examples
-------

Get return value:

	int thread_main(void *args){
		printf("%d",(int)(*args));
		return (int)(*args);
	}
	
	int maint(){
		Thread th1(thread_main,10);
		th1.Start();
		return th1.Join();
	}



Set destructor type:

	int thread_main(void *args){
		printf("%d",(int)(*args));
		return 0;
	}
	
	int maint(){
		Thread th1(thread_main,10,Thread::TERMINATE_JOIN);
		th1.Start();
		return 0
	}

OOPS thread:
	
	class MyThread:public Thread{
	
		public:
		MyThread():Thread(TERMINATE_DELETE){ start(); }
		virtual int run(){
			while(true){
				printf("hello from thread 1\n");
			}
			return 1;
		}
	};
	
	int main(){
		MyThread myth1;
		#ifdef	_WIN32
			Sleep(2000);
		#else
			sleep(2);
		#endif
		return 0;
	}


The MIT License (MIT)
-------

Copyright (c) 2013 Gabriele Di Bari

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.