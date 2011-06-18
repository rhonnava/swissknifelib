CFLAGS = -g3 -fpic 
SHARED_FLAG = -shared
INCLUDES = -I../utils/
shared : Watcher.o
	gcc -o libwatcher.so Watcher.o -lstdc++ $(CFLAGS) $(SHARED_FLAG) $(INCLUDES)

Watcher.o : Watcher.cpp 
	gcc -c -o Watcher.o Watcher.cpp $(INCLUDES)
clean: 
	rm -f *.o *.a *.so
