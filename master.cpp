/********************************************
 * doenv - A env derivation program
 * Brett Huffman
 * CMP SCI 4760 - Project 1
 * Due Feb 9, 2021
 * Main CPP file for project
 ********************************************/
#include <iostream>
#include <string.h>
#include <vector>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "bin_adder.h"

// Forward declarations
static void show_usage(std::string);

// Main - expecting arguments
int main(int argc, char* argv[])
{
    // Argument processing
    int opt;

    first_space::testFunc();

    // Go through each parameter entered and
    // prepare for processing
    while ((opt = getopt(argc, argv, "hst")) != -1) {
        switch (opt) {
            case 'h':
                show_usage(argv[0]);
                return 0;
            case 's':
                break;
            case 't':
                break;
            case '?': // Unknown arguement                
                if (isprint (optopt))
                {
                    errno = EINVAL;
                    perror("Unknown option");
//                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                }
                else
                {
                    errno = EINVAL;
                    perror("Unknown option character");
                }
                return -1;
            default:    // An bad input parameter was entered
                // Show error because a bad option was found
                perror ("master: Error: Illegal option found");
                show_usage(argv[0]);
                return -1;
        }
    }

    return 0;
}


// Handle errors in input arguments by showing usage screen
static void show_usage(std::string name)
{
    std::cerr << std::endl
              << name << " - master app by Brett Huffman for CMP SCI 4760" << std::endl
              << std::endl
              << "Usage:\t" << name << " [-h]" << std::endl
              << "\t" << name << " [-i] [var1=value] [var2=value] [...] {command1 [;command2] [;...]}" << std::endl
              << "Options:" << std::endl
              << "  -h        Describe how the project should be run and then, terminate." << std::endl
              << "  -s x      Indicate the number of children allowed to exist in the system at the same time. (Default 20)" << std::endl
              << "  -t time   The time in seconds after which the process will terminate, even if it has not finished. (Default 100)"
              << "  datafile  Input file containing one integer on each line."
              << std::endl << std::endl;
}