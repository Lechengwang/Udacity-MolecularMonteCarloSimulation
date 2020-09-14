#ifndef _MC_INPUT_H
#define _MC_INOUT_H 1

const char FINPUT[]="qmc.input";   // input params file

#include "mc_confg.h"
#include <stdlib.h>
#include <stdio.h>

#include <string>
class MCSettings;

const int  IO_PRECISION       =  6;
const int  IO_BLOCKNUMB_WIDTH =  3;
const int  IO_WIDTH           = 14;  // width > precision + 4 + 3    
const int  IO_WIDTH_BLOCK     =  4;  // io width to save counters    
//----IO errors----------------------------------------------
const char IO_ERR_FOPEN[] = "Can't open input file";
const char IO_ERR_FEXST[] = "File already exists:";
const char IO_ERR_WMODE[] = "Wrong IO mode";
const char IO_ERR_SCALL[] = "Error of the system call";


inline void _io_error(const char proc [],const char error[], const char file_name[])
{
// cerr 
   cout << endl;
   cout << proc << " : " <<error<<"  ["<<file_name<<"]" << endl;
   cout << endl;
   exit(1);
}
extern string OutputDir;
extern string FNPrefix;

extern string MCFileName;                // OutputDir+FNPrefix 

const char IO_EXT_GRA []    = ".gra.txt";   // radial  pair distribution
const char IO_EXT_ENG []    = ".eng.txt";   // energy
const char IO_SUM [] = "_sum";          // file name postfix for accum averages
const char IO_EXT_POS []    = ".pos.txt";   // atomic position

void IOReadParams(const char [], MCSettings &);
void io_setout(fstream &, int=IO_PRECISION);
void IOFileBackUp(const char []);
int FileExist (const char []);
#endif
