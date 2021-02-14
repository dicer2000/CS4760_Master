/********************************************
 * bin_adder - Processes and shared memory
 * This file is for the bin_adder functionality of the
 * application.  It kicks off from the main file.
 * 
 * Brett Huffman
 * CMP SCI 4760 - Project 1
 * Due Feb 9, 2021
 * bin_adder CPP file for project
 ********************************************/
#include <iostream>
#include "bin_adder.h"
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{
    cout << "got here" << endl;
    testFunc();
    return EXIT_SUCCESS;
}

void testFunc()
{
    cout << "Some test Func" << endl;
    sleep(2);
}
