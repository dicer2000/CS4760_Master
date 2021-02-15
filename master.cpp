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
#include <sys/types.h>
#include "master.h"
#include "sharedStructures.h"

// Static process counter => Never > 20
const int MAX_PROCESSES = 20;
static int ProcessCount = 0;
const char* ChildProcess = "./bin_adder";

using namespace std;

// Item Variables
vector<int> vecItemArray;
int* addItem_num;
struct AddItem* addItems;
char* shm_addr;

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

    // Allocate the shared memory
    // And get ready for read/write
    if ((key = ftok("./master", 100)) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    // allocate a shared memory segment with size of struct array
    int memSize = sizeof(AddItem) * arrItemCount;
    shm_id = shmget(key, memSize, IPC_CREAT | IPC_EXCL | 0600);
    if (shm_id == -1) {
        perror("shmget: ");
        exit(EXIT_FAILURE);
    }
    // attach the shared memory segment to our process's address space
    shm_addr = (char*)shmat(shm_id, NULL, 0);
    if (!shm_addr) { /* operation failed. */
        perror("shmat: ");
        exit(EXIT_FAILURE);
    }
    addItem_num = (int*) shm_addr;
    *addItem_num = 0;
    addItems = (struct AddItem*) (shm_addr+sizeof(int));

    // Fill struct
    for(int i=0; i < arrItemCount; i++)
    {
        addItems[i].itemValue = vecItemArray[i];
        addItems[i].pidAssigned = 0;
        addItems[i].complete = false;
        // Setup every other item as 
        // ready-to-process (Leaf items of tree)
        addItems[i].readyToProcess = (i%2==0);
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


    // Set a variable to keep track of target level
    int nDepth = targetlevel;
    // Now, start looping until the entire calculation is complete
    // This is in a giant loop that both forks items that need
    // to be calculated and responds to PID's that are returning/erroring.
    // Once everything is done, cleanup and exit
    while(!bComplete)
    {

        // Search for an "Ready" node first by depth
        // then by every node in the array
        for(int i=0;i<nDepth;i++)
        {
            for(int j=0;j<arrItemCount;j+=pow(2, i+1))
            {
                // j will always be the nodes we need to
                // check.  If it is "Ready", check it's 
                // partner.  If it's "Ready" too => send to bin_adder
                int nCheck1 = j;
                int nCheck2 = pow(2, i) + j;

                if(addItems[nCheck1].readyToProcess && addItems[nCheck2].readyToProcess)
                {
                    // Set as processing
                    addItems[nCheck1].readyToProcess = addItems[nCheck2].readyToProcess = false;
                    // Set the depth of it's last process run
                    addItems[nCheck1].nodeDepth = addItems[nCheck1].nodeDepth = nDepth-i;
                    // Fork and store pid in each node
                    int pid = forkProcess(addItems[nCheck1].itemValue, nDepth-3);
                    addItems[nCheck1].pidAssigned = addItems[nCheck2].pidAssigned = pid;
                }
            }
        }

        // Wait for any PID
        do {
            w = waitpid(-1, &wstatus, WUNTRACED | WCONTINUED);
            if (w == -1) {
                perror("waitpid");
                bComplete = true;
                break;
//                exit(EXIT_FAILURE);
            }

            if (WIFEXITED(wstatus)) {
                printf("exited, status=%d\n", WEXITSTATUS(wstatus));
                // Success!  We are done processing
                if(addItems[0].pidAssigned == w && addItems[0].nodeDepth==nDepth)
                {
                    bComplete = true;
                    break;
                }
                else
                {
                    // Find the item in the array based on PID
                    for(int i=0;i<arrItemCount;i++)
                    {
                        if(addItems[i].pidAssigned == w)
                        {
                            // Set this node as ready to process and continue
                            addItems[i].readyToProcess = true;
                            break;
                        }
                    }
                }
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

    // Dedetach shared memory segment from process's address space
    cout << "De-allocating shared memory" << endl;
    if (shmdt(shm_addr) == -1) {
        perror("main: shmdt: ");
    }

    // De-allocate the shared memory segment.
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("main: shmctl: ");
    }

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


