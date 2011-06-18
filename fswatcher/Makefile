CFLAGS = -g3 -fpic 
SHARED_FLAG = -shared

shared : Watcher.o
	gcc -o libwatcher.so Watcher.o -lstdc++ $(CFLAGS) $(SHARED_FLAG)

Watcher.o : Watcher.cpp

clean: 
	rm -f *.o *.a *.so
