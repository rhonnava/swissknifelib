CFLAGS = 

#Specify debug level
ifneq (, $(debug))
CFLAGS += -g$(debug)
endif

OBJS = Watcher.o
SHARED_FLAG = -shared
INCLUDES = -I../utils/
LIBS = -lstdc++

shared : Watcher.o
	gcc -o libwatcher.so $(OBJS) $(LIBS) $(CFLAGS) $(SHARED_FLAG) $(INCLUDES)

static : Watcher.o
	ar rcs libwatcher.a $(OBJS)

Watcher.o : Watcher.cpp 
	gcc -c -o Watcher.o Watcher.cpp $(INCLUDES)
clean: 
	rm -f *.o *.a *.so
