CPPFLAGS=
CCFLAGS=-g -O0
LDFLAGS=-lboost_thread

tptest: ThreadPoolWorker.o ThreadPool.o main.o
	$(LINK.cc) -o $@ $^

clean:
	rm -f *.o tptest core

ThreadPoolWorker.o: ThreadPoolWorker.h ThreadPool.h
ThreadPool.o: ThreadPool.h ThreadPoolWorker.h
main.o: ThreadPool.h
