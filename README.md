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



## Work Log

- 2/10/2021 - Created project, Makefile, this readme file and got it all to compile
- 2/11/2021 - Created outline of project, started work on main, master processes

*©2021 Brett W. Huffman*