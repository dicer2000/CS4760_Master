# Improved Makefile by Brett Huffman v1.1
# (c)2021 Brett Huffman
# To use, just change the next line to the name of
# the application
appname := master bin_adder

CXX := g++
CXXFLAGS := -std=c++11

srcfiles := $(shell find . -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(appname) $(objects) $(LDLIBS)

depend: .depend

.depend: $(srcfiles)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	rm -f $(objects)
	rm -f $(appname)

dist-clean: clean
	rm -f *~ .depend

include .depend