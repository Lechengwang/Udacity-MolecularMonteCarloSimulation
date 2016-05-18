#ifndef _MC_ESTIM_H
#define _MC_ESTIM_H 1

#include <fstream>

void InitMCEstims(void);
void Fix_Density(void);
double SinglePot_Density(void);
void SaveDensities1D(const char [], double);

#endif
