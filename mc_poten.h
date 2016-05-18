#ifndef _MC_POTEN_H
#define _MC_POTEN_H 1

#include "mc_confg.h"
#include "mc_setup.h"
#include "mc_utils.h"

void     InitPotentials(void);
double   SPot1D(double);
int  get_filesize (const char []);

const char EXT_POT[] = ".pot";

#endif  // mc_poten.h
