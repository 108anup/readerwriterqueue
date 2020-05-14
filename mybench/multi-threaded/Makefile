all: main provided

main: main.cc ../../readerwriterqueue.h ../../atomicops.h
	g++ -DNDEBUG -O3 main.cc -o main.out -pthread

provided: provided.cc ../../readerwriterqueue.h ../../atomicops.h ../../tests/common/simplethread.h ../../tests/common/simplethread.cpp systemtime.h systemtime.cc
	g++ -std=c++11 -Wpedantic -Wall -DNDEBUG -O3 -g provided.cc ../../tests/common/simplethread.cpp systemtime.cc -o provided.out -pthread -Wl,--no-as-needed -lrt

# g++ -pthread -O3 provided.cc systemtime.cc ../../tests/common/simplethread.cpp \
	-o provided.out

clean:
	rm main.out
