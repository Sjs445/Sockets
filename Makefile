all:
	g++ -std=c++11 -pthread -o file sockets.cpp
	./file
clean:
	rm file
