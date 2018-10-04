COMPILER = g++
COMPILER_FLAGS = -ggdb -std=c++0x -Wall -pedantic -I/usr/local/include#-O3 for release #-ggdb for debug
LINKER_FLAGS = -ggdb -std=c++0x -L/usr/local/lib -lgsl -lgslcblas -lm -lSDL2 #-O3 for release #-ggdb for debug #-lSDL2_image -pthread -o
SOURCES = src/*.cpp
OBJECTS := ${subst src/,obj/,$(SOURCES:.cpp=.o)}

all: bin/simulation

bin/simulation: $(OBJECTS)
	$(COMPILER) $(OBJECTS) $(LINKER_FLAGS) -o bin/simulation

$(OBJECTS): $(SOURCES) $(SOURCES:.cpp=h)
	$(COMPILER) -c $(SOURCES) $(COMPILER_FLAGS)
	mv ./*.o ./obj
