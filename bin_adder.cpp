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
#include "sharedStructures.h"



using namespace std;

int main(int argc, char* argv[])
{

    // Argument processing
    // If any error happens, show error statement and
    // return error immediately
    try
    {
        if(argc!=3) throw std::runtime_error("Incorrect Arguements");
        int index = optind;
        int nFirstNumber = atoi(argv[index]);
        int nSecondNumber = atoi(argv[index+1]);
    }
    catch(const std::exception& e)
    {
        errno = EINVAL;
        perror(e.what());
        // General error
        show_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Allocate the shared memory
    // And get ready for read/write
    if ((key = ftok(HostProcess, 100)) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    // Get a reference to the shared memory, if available
    shm_id = shmget(key, 0, 0);
    if (shm_id == -1) {
        perror("shmget1: ");
        exit(EXIT_FAILURE);
    }

    // Read the memory size and calculate the array size
    struct shmid_ds shmid_ds;
    shmctl(shm_id, IPC_STAT, &shmid_ds);
    size_t realSize = shmid_ds.shm_segsz;
    int length = (int) shmid_ds.shm_segsz / sizeof(AddItem);

    // Now we have the size - actually setup with shmget
    shm_id = shmget(key, realSize, 0);
    if (shm_id == -1) {
        perror("shmget2: ");
        exit(EXIT_FAILURE);
    }

    // attach the shared memory segment to our process's address space
    shm_addr = (char*)shmat(shm_id, NULL, 0);
    if (!shm_addr) { /* operation failed. */
        perror("shmat: ");
        exit(EXIT_FAILURE);
    }

    cout << "Items: " << length << endl;

    // Determine the two numbers to add and do it


    return EXIT_SUCCESS;
}

// Handle errors in input arguments by showing usage screen
static void show_usage(std::string name)
{
    std::cerr << std::endl
              << name << " - bin_adder app by Brett Huffman for CMP SCI 4760" << std::endl
              << std::endl
              << "Usage:\t" << name << " xx yy" << std::endl
              << "Parameters:" << std::endl
              << "  xx   The index of the first number to add in shared memory" << std::endl
              << "  yy   The depth of the tree in shared memory" << std::endl
              << "Note that since this program uses shared memory, it will " << std::endl
              << "only work when called by the master program." << std::endl
              << std::endl << std::endl;
}