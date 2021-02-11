 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
 
# ****************************************************
# The build target 
TARGET = master

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)