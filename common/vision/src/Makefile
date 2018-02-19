CC=g++ --std=c++1y -pthread 
C_FLAGS=`pkg-config --cflags opencv` -g
L_FLAGS=`pkg-config --libs opencv`
OBJS=obj/main.o obj/capture.o obj/process.o obj/display.o obj/transmit.o
OBJ_NAME=vision_exec

all: $(OBJS)
	$(CC) $(L_FLAGS) $(OBJS) -o $(OBJ_NAME)

clean:
	rm -rf obj/*

obj/main.o: main.cpp
	$(CC) $(C_FLAGS) -c main.cpp -o obj/main.o

obj/capture.o: capture.cpp
	$(CC) $(C_FLAGS) -c capture.cpp -o obj/capture.o

obj/process.o: process.cpp
	$(CC) $(C_FLAGS) -c process.cpp -o obj/process.o

obj/display.o: display.cpp
	$(CC) $(C_FLAGS) -c display.cpp -o obj/display.o

obj/transmit.o: transmit.cpp
	$(CC) $(C_FLAGS) -c transmit.cpp -o obj/transmit.o
