# Improved Makefile by Brett Huffman v1.1
# (c)2021 Brett Huffman
# To use, just change the next line to the name of
# the application
#
# Version 1 - creates the bin_adder program
appname := bin_adder
srcfiles := $(shell find . -name "bin_adder*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(appname) $(objects) $(LDLIBS)


depend: .depend_bin_adder

.depend_bin_adder: $(srcfiles)
	rm -f ./.depend_bin_adder
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend_bin_adder;

clean:
	rm -f $(objects)
	rm -f $(appname)

dist-clean: clean
	rm -f *~ .depend_bin_adder

include .depend_bin_adder