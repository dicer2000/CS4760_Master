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
const char* ChildProcess = "./bin_adder";

using namespace std;

struct AddItem {
    pid_t pidAssigned;
    int itemValue;
    bool complete;
};

// Item Variables
vector<int> vecItemArray;
int* addItem_num;
struct AddItem* addItems;

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
                // We've got a good value
                // Push it to the vector array
                vecItemArray.push_back(i);
            }
            catch (std::invalid_argument const &e)
            {
                processFile.close();
                perror("Bad data file value");
                return EXIT_FAILURE;
            }
            catch (std::out_of_range const &e)
            {
                processFile.close();
                perror("Bad data file value: Integer overflow");
                return EXIT_FAILURE;
            }

        }
        processFile.close();
    }
    else
    {
        // Error - cant open file
        errno = ENOENT;
        perror(InputDataFile.c_str());
        return EXIT_FAILURE;
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
            // Pad Array with zeros
            vecItemArray.push_back(0);
        }
        while(pow(2, targetlevel) < vecItemArray.size());
    }

    // If this was successful, we are finally ready to process
    // the file adds with our child processes.
    int arrItemCount = vecItemArray.size();
//    AddItem arrItem[arrItemCount];


    countries_num = (int*) shm_addr;
    *countries_num = 0;
    countries = (struct country*) ((void*)shm_addr+sizeof(int));


    // Fill array
    for(int i=0;i< vecItemArray.size(); i++)
    {
        arrItem[i].itemValue = vecItemArray[i];
        arrItem[i].pidAssigned = 0;
        arrItem[i].complete = false;
    }
    printf("parent process, pid = %u\n",getpid());  

    // Start Processing with bin_adder xx yy
    // Where xx is start of processing index and yy is depth.
    // And the result should just go in the yy spot
    bool bComplete = false;
    int status;
    int arrayIndex = 0;
    pid_t cpid, w;
    int wstatus;

    // Start the process by sending out the first
    // round of commands to every other item in the
    // array.
    for(int i = 0; i < arrItemCount; i+=2)
    {
        // Fork and store pid for later use
        int pid = forkProcess(arrItem[i].itemValue, targetlevel);
    }

    // Now, start listening for them to complete
    while(!bComplete)
    {
        // Wait for any PID
        do {
            w = waitpid(-1, &wstatus, WUNTRACED | WCONTINUED);
            if (w == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }

            if (WIFEXITED(wstatus)) {
                printf("exited, status=%d\n", WEXITSTATUS(wstatus));
            } else if (WIFSIGNALED(wstatus)) {
                printf("killed by signal %d\n", WTERMSIG(wstatus));
            } else if (WIFSTOPPED(wstatus)) {
                printf("stopped by signal %d\n", WSTOPSIG(wstatus));
            } else if (WIFCONTINUED(wstatus)) {
                printf("continued\n");
            }
        } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
        

sleep(2);
    }
/*

    // Calculated with 2^(Depth(t) - Depth(c))
    int forkCount = 2^()

    // Loop until final answer or error
    while(!bComplete)
    {

        int pid = fork();
        // No child made - exit with failure
        if(pid < 0)
        {
            // Signal to any child process to exit

            perror("Could not fork process");
            return EXIT_FAILURE;
        }
        // Child process here - Assign out it's work
        if(pid == 0)
        {
            cout << "I am child" << endl;

            char* argv[3];
            argv[0] = "2";
            argv[1] = "4";
            argv[2] = NULL;
     

            execv(ChildProcess, argv);
            fflush(stdout);
            exit(EXIT_SUCCESS);    // Exit from forked process successfully
        }
        // The parent process - do the housekeeping
        if (waitpid(pid, &status, 0) > 0) { 
              
            if (WIFEXITED(status) && !WEXITSTATUS(status))  
              printf("program execution successful\n");

            else if (WIFEXITED(status) && WEXITSTATUS(status)) { 
                if (WEXITSTATUS(status) == 127) { 
  
                    // execv failed 
                    printf("execv failed\n"); 
                } 
                else 
                    printf("program terminated normally,"
                       " but returned a non-zero status\n");                 
            }
            else 
               printf("program didn't terminate normally\n");
        }

//        sleep(2);
        break;
    }
*/

    // Success!
    return EXIT_SUCCESS;
}


int forkProcess(int nItemStart, int nDepth)
{
        int pid = fork();
        // No child made - exit with failure
        if(pid < 0)
        {
            // Signal to any child process to exit

            perror("Could not fork process");
            return EXIT_FAILURE;
        }
        // Child process here - Assign out it's work
        if(pid == 0)
        {
            cout << "I am child" << endl;
            // Get strings from the int params
            string strItemStart = format("%d", nItemStart);
            string strDepth = format("%d", nDepth);
            // Execute child process
            execl(ChildProcess, strItemStart.c_str(), strDepth.c_str(), NULL);

            fflush(stdout); // Mostly for debugging -> tty wasn't flushing
            exit(EXIT_SUCCESS);    // Exit from forked process successfully
        }
        else
            return pid; // Returns the Parent PID
}


