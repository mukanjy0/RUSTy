CXX := g++                     # Use g++ as the compiler
CXXFLAGS := -std=c++20 -Wall -g   # Compiler flags: C++17 standard + enable warnings and debugging

SRCS := $(shell find . -name "*.cpp")  # Find all .cpp files recursively
OBJS := $(SRCS:.cpp=.o)               # Convert .cpp filenames to .o (object files)
TARGET := rusty

# Default target (`make`)
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# `make clean` to clean up
clean:
	rm -f $(OBJS) $(TARGET)
