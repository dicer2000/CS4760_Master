/********************************************
 * master - Processes and shared memory
 * This file is for the master functionality of the
 * application.  It kicks off from the main file.
 * 
 * Brett Huffman
 * CMP SCI 4760 - Project 1
 * Due Feb 9, 2021
 * Master CPP file for project
 ********************************************/
#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "master.h"

// Static process counter => Never > 20
const int MAX_PROCESSES = 20;
static int ProcessCount = 0;

using namespace std;

struct addItem {
    int AddItemIndex;
    int ItemValue;
};

// Item Variables
vector<addItem> vecItemArray;

// ProcessMaster
// The processMaster function to process data from the given input file.
int processMaster(int numberOfChildrenAllowed, int timeInSecondsToTerminate, string InputDataFile)
{
    // Read in data file
    string line;
    ifstream processFile(InputDataFile);
    // Open the file and read it in
    if (processFile.is_open())
    {
        // Read each line
        while (getline(processFile,line))
        {
            // Error check
            try
            {
                int i = std::stoi(line);
                addItem ai;
                ai.ItemValue = i;
                ai.AddItemIndex = -1;
                // We've got a good value
                // Push it to the vector array
                vecItemArray.push_back(ai);
            }
            catch (std::invalid_argument const &e)
            {
                processFile.close();
                perror("Bad data file value");
                return -1;
            }
            catch (std::out_of_range const &e)
            {
                processFile.close();
                perror("Bad data file value: Integer overflow");
                return -1;
            }

        }
        processFile.close();
    }
    else
    {
        // Error - cant open file
        errno = ENOENT;
        perror(InputDataFile.c_str());
        return -1;
    }

    // Check that the input file is a power of 2.  Kind of a
    // wizzy way of doing it... but I like it.  When it's done
    // it should tell me what power of 2 it is
    int index = vecItemArray.size();
    int targetlevel = 0;
    // Could do this with log(), but then I've got
    // to use floats. :(
    while (index >>= 1) ++targetlevel;

    // While there is less than a full power of 2
    // Fill in zeros
    if(pow(2, targetlevel) < vecItemArray.size())
    {
        targetlevel++;
        do
        {
            addItem ai;
            ai.ItemValue = 0;   // Per directions, add 0
            ai.AddItemIndex = -1;
            // We've got a good value
            // Push it to the vector array
            vecItemArray.push_back(ai);
        }
        while(pow(2, targetlevel) < vecItemArray.size());
    }

    // If this was successful, we are finally ready to process
    // the file adds with our child processes.
    // At this point, I know the array size needed to handle
    // processing this entire problem: 2^(targetlevel+1)-1
    // For instance, 2^3 items is: 2^4-1 = 15 array items
    /*
    const int arrItemCount = pow(2, targetlevel+1) - 1;
    int arrItemCount[arrItemCount] = { 0 };

    // Fill array
    for(int i=0;i< vecItemArray.size(); i++)
    {
//        arrItemCount[i]    vecItemArray
    }
    */
    // Success!
    return 0;
}