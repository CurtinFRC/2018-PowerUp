CC=g++ --std=c++17 -pthread 
C_FLAGS=`pkg-config --cflags opencv` -g
L_FLAGS=`pkg-config --libs opencv`
OBJS=obj/main.o obj/capture.o obj/processing.o obj/display.o
OBJ_NAME=vision_exec

all: $(OBJS)
	$(CC) $(L_FLAGS) $(OBJS) -o $(OBJ_NAME)

clean:
	rm -rf obj/*

obj/main.o: main.cpp
	$(CC) $(C_FLAGS) -c main.cpp -o obj/main.o

obj/capture.o: capture.cpp
	$(CC) $(C_FLAGS) -c capture.cpp -o obj/capture.o

obj/processing.o: processing.cpp
	$(CC) $(C_FLAGS) -c processing.cpp -o obj/processing.o

obj/display.o: display.cpp
	$(CC) $(C_FLAGS) -c display.cpp -o obj/display.o
