libindiprovclient.so: indiprovclient.o provutils.o
	gcc -std=c++11 -shared -Wl,-soname,libindiprovclient.so.1 -o libindiprovclient.so.1.0  *.o -lzmq -ljson -lboost_thread -lboost_system  -lboost_filesystem -lboost_serialization -lboost_program_options -lboost_regex
	ln -sf ./libindiprovclient.so.1.0 ./libindiprovclient.so.1
	ln -sf ./libindiprovclient.so.1   ./libindiprovclient.so
provutils.o: provutils.cpp
	gcc -std=c++11 -fPIC -c $<

indiprovclient.o: indiprovclient.cpp
	gcc -std=c++11 -fPIC -c $<

main:
	gcc  -std=c++11 -o main main.cpp -L. -lindiprovclient -L/usr/lib64 -lstdc++
.PHONY: clean
clean:
	rm -f *.so* *.o main
