CXX := g++
CXXFLAGS := -std=c++1y -Wall -Werror

SOURCES := $(wildcard *.cpp)
EXECUTABLES := $(patsubst %.cpp,%,$(SOURCES))

.PHONY: all clean

all: $(EXECUTABLES)

% : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(EXECUTABLES)
