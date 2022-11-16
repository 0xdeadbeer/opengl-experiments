LIBS=-lGLEW -lGLU -lglfw -lglut -lGL
COMPILER_FLAGS=-Wall
DEBUG_COMPILER_FLAGS=-g -Wall -Wextra -ggdb3 -D DEBUG -O0
BUILD_DIR=./build
OUTPUT_LOCATION=$(BUILD_DIR)/main

all:
	g++ $(COMPILER_FLAGS) *.cpp -o $(OUTPUT_LOCATION) $(LIBS)
	make run 

debug: 
	g++ $(DEBUG_COMPILER_FLAGS) *.cpp -o $(OUTPUT_LOCATION) $(LIBS)
	make run

clean:
	rm $(BUILD_DIR)/*

run: 
	$(OUTPUT_LOCATION)
