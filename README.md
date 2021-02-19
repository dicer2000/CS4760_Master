# master Application

master application by Brett Huffman for CMP SCI 4760 - Project 2 - V1.0

This program implements the master program...

A log was kept of each day's activity.  It is found at the end of this README file.

A Git repository was maintained with a public remote found here: https://github.com/dicer2000/CS4760_Master.git

## Program Switches
The program can be invoked as:

```
master [-h]
master [-h] [-s i] [-t time] datafile
Options:
  -h        Describe how the project should be run and then, terminate.
  -s x      Indicate the number of children allowed to exist in the system at the same time. (Default 20)
  -t time   The time in seconds after which the process will terminate, even if it has not finished. (Default 100)
  datafile  Input file containing one integer on each line.
  
```

## Install
To install this program, clone it with git to the folder to which you want 
it saved.
```
git clone https://github.com/dicer2000/CS4760_Master.git
```
## Compile
To compile the master application, simply run the make command:
```
make
```
## Run
To run the program, use the master command.  You can use any of the attributes listed in program switches area.

## Problems / Issues

My initial problem was in understanding how the fork really worked.  I think its interesting that the forked process takes off from the spot it's forked.  I thought it would be from the very start of the program.  Once I go that, I was able to understand at least how the processing was going to happen.

My biggest problem, so far, has been in determining the size of the shared memory in the bin_adder program. Once you figure out the formula, through a series of calls to shmctl, shmget, & shmadd, you are able to get the size of the array, cast it to the correct type,  and finally do the addition.  It was no small feat as the documentation is lacking.

The textbook finally helped me solve the problem with this example on pg 619:
```
if ((id = shmget(IPC_PRIVATE, sizeof(int), PERM)) == -1) {
 perror("Failed to create shared memory segment");
 return 1;
 }
 if ((sharedtotal = (int *)shmat(id, NULL, 0)) == (void *)-1) {
 perror("Failed to attach shared memory segment");
 if (shmctl(id, IPC_RMID, NULL) == -1)
 perror("Failed to remove memory segment");
 return 1;
 }
 if ((childpid = fork()) == -1) {
 perror("Failed to create child process");
 if (detachandremove(id, sharedtotal) == -1)
 perror("Failed to destroy shared memory segment");
 return 1;
 }
```
I also wish I understood better how to do the critical section (through Solution 4).  If I use an extern, I can't dynamically allocate size for the flag array.  So, I guess I must use some sort of shared memory for that so I can dynamically allocate that when I know how processing nodes I'll be needing.

I think I will put that into my main array struct.  Since it's already in Shared Memory, and there is already one for each array node, that would work perfectly (I think).  I'd like to know from Professor Bhatia if that's okay.

On 2/18 I identified a large problem with my algorithm for tree traversal.  To do it, I loop through all available nodes looking for a target that are powers of 2^depth of that tree node AND they must have both the two nodes to be added open and ready to process.  That worked well when processing was speedy, but interestingly broke when I added the delay in the bin_adder Critical Section.  The problem ended up being that I was not always comparing nodes that were at the same depth of processing.  Once I added that check, that they were in the same depth, everything started working again.

## Work Log

- 2/10/2021 - Created project, Makefile, this readme file and got it all to compile
- 2/11/2021 - Created outline of project, started work on main, master processes
- 2/13/2021 - Finished rough master, memory allocation with shm functions
- 2/14/2021 - Finished logic to send requests to bin_adder(), added bin_adder code to determine size of shared memory.
- 2/15/2021 - Debugging functionality
- 2/16/2021 - Fixes to functionality; Setting up Max Time To Process and Max Children
- 2/17/2021 - Added signaling functionality to handle Ctrl-C and timeouts; debugging; Started work on critical section
- 2/18/2021 - Fixed issues with addition; Fixed issues with critical section; Placed critical section in its own function handler
- 2/10/2021 - Added file saving to adder; Debugging
- 2/19/2021 - Started check-off list to make sure all items are covered

*©2021 Brett W. Huffman*