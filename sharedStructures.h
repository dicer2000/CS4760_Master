/********************************************
 * sharedStructures - This is a file that
 * contains all the structures shared between
 * the master + bin_adder processes.  It
 * contains library calls, the main structure
 * containing data, and semephores.
 * 
 * Brett Huffman
 * CMP SCI 4760 - Project 1
 * Due Feb 9, 2021
 * Master CPP file for project
 ********************************************/

#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

// Semiphore union
union semun {
    int                 val;
    struct semid_ds *   buf;
    unsigned short *    array;
#if defined(__linux__)
    struct seminfo *    __buf;
#endif
};

// Critical Section Processing
enum state { idle, want_in, in_cs };

// Shared Memory structure
struct AddItem {
    bool readyToProcess;    // Ready to Process
    pid_t pidAssigned;      // In-Process
    bool complete;          // Completed Process
    int nodeDepth;          // Depth that this node is processing
    int itemValue;          // The actual value
    state itemState;        // The Critical Secion Flag
};

key_t key = 0;  // Shared key
int shm_id; // Shared Mem ident
char* shm_addr;

const char* HostProcess = "./master";
const char* ChildProcess = "./bin_adder";