
EXECUTABLE	:= engine

RELEASE		:= -O3
DEBUG2		:= -gdwarf-4 -g3 -Og
DEBUG		:= -ggdb3 -Og
STATIC		:= -Bstatic -static-libgcc -static-libstdc++
SHARED		:=

#BUILD		:= $(DEBUG)
#BUILD		:= $(DEBUG2)
BUILD		:= $(RELEASE)

#LINKTYPE	:= $(STATIC)
LINKTYPE	:= $(SHARED)

CXX_FLAGS	:= -Wall -Wextra -std=c++11 $(BUILD) -fpermissive -Wtype-limits $(LINKTYPE)
# CXX			:= clang
CXX			:= g++
INC_FLAG	:= -Iinc
LIB_FLAG	:= -Llib

BIN			:= bin
SRC			:= src
INC			:= inc
LIB			:= lib
OBJ     	:= obj
RES			:= res

LIBRARIES	:= -lGL -lGLEW -lSDL2 -lassimp -lSOIL

SOURCES		:= $(shell find $(SRC) -type f -name *.cpp)
OBJECTS		:= $(patsubst $(SRC)/%,$(OBJ)/%,$(SOURCES:.cpp=.o))

DEPSRC		:= $(shell find $(INC) -type f -name *.hpp)
DEPENDENCIES:= $(DEPSRC:.hpp)

# if you want to find out the value of a makefile variable
# make print-VARIABLE  <--- VARIABLE is one defined here, like CXX_FLAGS, so type make print-CXX_FLAGS
print-%  : ; @echo $* = $($*)

.PHONY: depend clean all

all: $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

clean:
	-rm $(BIN)/engine $(OBJ)/*.o

# Compile only

$(OBJ)/%.o : $(SRC)/%.cpp $(DEPENDENCIES)
	$(CXX) $(CXX_FLAGS) $(INC_FLAG) -c -o $@ $<

# Link the object files and libraries
$(BIN)/$(EXECUTABLE) : $(OBJECTS)
	$(CXX) $(CXX_FLAGS) -o $(BIN)/$(EXECUTABLE) $^ $(LIBRARIES) $(LIB_FLAG)

