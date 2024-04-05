CXX = g++
ROOTCONFIG = root-config
CXXFLAGS = -Wall -Wextra -std=c++11 $(shell $(ROOTCONFIG) --cflags)
LDFLAGS = $(shell $(ROOTCONFIG) --ldflags --libs)

TARGET = splitROOT

SRCS = splitROOT.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) $(TARGET) $(OBJS)
