cc=g++ -std=c++11
objs=thread.o mutex.o condition.o

all : $(objs)
.PHONY : all

thread.o : thread.h nocopyable.h
	$(cc) -c thread.cpp

mutex.o : mutex.h nocopyable.h
	$(cc) -c mutex.cpp

condition.o : mutex.h nocopyable.h
	$(cc) -c condition.cpp

.PHONY : clean
clean :
	rm $(objs)