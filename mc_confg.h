#ifndef _MC_CONFG_H
#define _MC_CONFG_H 1

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
const int MAX_STRING_LENGTH = 80;

//--------- FILES  ----------------------
const char FBACKUP_EXTENSION[]=".old";
const char COMMENTS[] = "#";        // comment line in output files
const char BLANK[]    = "   ";      // spacing between columns

const int MPI_MASTER = 0;  // MPI id for the root process 

#endif  // mc_confg.h
