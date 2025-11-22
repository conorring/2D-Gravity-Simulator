CXX := g++							# cpp compiler
# compiler flags: -Wall and -Wextra for extra compiler warnings
# -g builds executable with debugging symbols
# -O0 makes the compiler not optimize code at all 
CXXFLAGS := -Wall -Wextra -g -O0    
LDFLAGS := -lGLEW -lGL -lglfw		# library flags for linking

SRC_DIR := src
BIN_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.cpp)				# resolves to list of all .cpp files in src/
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)     # matches each src/*.cpp with a bin/*.o
TARGET := $(BIN_DIR)/app							# bin/app

# Default target runs by just running make
all: $(TARGET)

# Link object files together to form executable
# at this step we need to link against the openGL libraries
# $^ is all pre-requisites
# $@ resolves to the target name
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile step to create object files
# link step is above for now we just need object files which is -c flag
# $< refers to first pre-requisite listed - there is only one here but it is good practice
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure bin/ exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)
