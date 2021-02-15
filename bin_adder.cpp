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
/*
int shmid = shmget(key, 0, ...);
shmctl(shmid, IPC_STAT, &shm_id);
size_t realSize = shm_id.shm_segsz;
shmid = shmget(key, realSize, ...);
void * p = shmat(shmid);
*/

    // Allocate the shared memory
    // And get ready for read/write
    if ((key = ftok(HostProcess, 100)) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    // First get the size of the memory instantiated (hopefully)
//    struct shmid_ds buf;
//    shmctl(int shmid, int cmd, struct shmid_ds *buf);
//    shmctl(buf, IPC_STAT, &buf);
//    int length = (int) buf.shm_segsz / sizeof(int);

    // allocate a shared memory segment with size of struct array
//    int memSize = sizeof(AddItem) * arrItemCount;
    // Get the size of the array
    shm_id = shmget(key, 0, 0);
    if (shm_id == -1) {
        perror("shmget1: ");
        exit(EXIT_FAILURE);
    }

    struct shmid_ds shmid_ds;
    shmctl(shm_id, IPC_STAT, &shmid_ds);
    size_t realSize = shmid_ds.shm_segsz;
    int length = (int) shmid_ds.shm_segsz / sizeof(AddItem);

    // Now we have the size - actually allocate
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
    testFunc();
    return EXIT_SUCCESS;
}

void testFunc()
{
    cout << "Some test Func" << endl;
    sleep(2);
}
