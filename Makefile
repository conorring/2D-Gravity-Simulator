CXX := g++							# cpp compiler
# compiler flags: -Wall and -Wextra for extra compiler warnings
# -g builds executable with debugging symbols
# -O tells compiler how much to optimize 0-3 
CXXFLAGS := -Wall -Wextra -Werror -Wpedantic -std=c++20 -O0 -g -Iinclude	   
LDFLAGS := -lGLEW -lGL -lglfw 	# library flags for linking

SRC_DIR := src
BIN_DIR := build/bin
OBJ_DIR := build/obj

SRCS := $(wildcard $(SRC_DIR)/*.cpp)				# resolves to list of all .cpp files in src/
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)     # matches each src/*.cpp with a bin/*.o
TARGET := $(BIN_DIR)/app							# bin/app

# Default target runs by just running make
all: $(TARGET)

# Link object files together to form executable
# at this step we need to link against the openGL libraries
# $^ is all pre-requisites
# $@ resolves to the target name
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) 	

# Compile step to create object files
# link step is above for now we just need object files which is -c flag
# $< refers to first pre-requisite listed - there is only one here but it is good practice
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure bin/ exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

test: $(TARGET)
	g++ test/test.cpp -o test/test $(LDFLAGS)

clean:
	rm -rf build
