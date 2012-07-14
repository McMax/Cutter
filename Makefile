CC      = g++
LD      = g++

CCFLAGS = -g -O0 `root-config --cflags` -Wall -I./inc
LDFLAGS = -g -O0 `root-config --libs` -Wall -L./lib

TOPDIR = .
SRC_DIR = $(TOPDIR)/src
OBJ_DIR = $(TOPDIR)/lib
INC_DIR = $(TOPDIR)/inc

PROGRAM = cutter

SOURCES := $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
INCLUDES := inc/Particle.h inc/Event.h inc/ParticleTree.h inc/linkdef.h

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS) Dict.o
	$(LD) $(LDFLAGS) $^ -o $@ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) -c $(CCFLAGS) $< -o $@ 

Dict.o: Dict.cpp
	$(CC) -c $(CCFLAGS) Dict.cpp -o Dict.o

Dict.cpp: $(INCLUDES)
	@echo "Generating dictionary..."
	@rootcint -f Dict.cpp -c -P -I$(ROOTSYS) -I/usr/local/include $(INCLUDES)

clean:
	@rm -rf $(PROGRAM) ./lib        

realclean: clean
	@rm -f Dict.*
