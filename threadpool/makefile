cc=g++ -std=c++11
objs=thread.o mutex.o condition.o fileutil.o threadpool.o

all : $(objs)
.PHONY : all

thread.o : thread.h nocopyable.h
	$(cc) -c thread.cpp

mutex.o : mutex.h nocopyable.h
	$(cc) -c mutex.cpp

condition.o : mutex.h nocopyable.h
	$(cc) -c condition.cpp

fileutil.o : fileutil.h
	$(cc) -c fileutil.cpp

threadpool.o : threadpool.h thread.h mutex.h
	$(cc) -c threadpool.cpp
	
.PHONY : clean
clean :
	rm $(objs)