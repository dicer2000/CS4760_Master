/********************************************
 * master.h - Master include file
 * 
 * Brett Huffman
 * CMP SCI 4760 - Project 2
 * Due Feb 23, 2021
 * Master .h file for project
 ********************************************/
#include <iostream>

using namespace std;

// Master Function Declarations
int processMaster(int numberOfChildrenAllowed, int timeInSecondsToTerminate, string InputDataFile);
int forkProcess(int nItemStart, int nDepth);
