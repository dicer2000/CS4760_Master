# Improved Makefile by Brett Huffman v1.1
# (c)2021 Brett Huffman
# To use, just change the next line to the name of
# the application

# Version 1 - creates the bin_adder program
appname := master
srcfiles := $(shell find . -name "master*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(appname) $(objects) $(LDLIBS)

depend: .depend_master

.depend_master: $(srcfiles)
	rm -f ./.depend_master
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend_master;

clean:
	rm -f $(objects)
	rm -f $(appname)

dist-clean: clean
	rm -f *~ .depend_master

include .depend_master