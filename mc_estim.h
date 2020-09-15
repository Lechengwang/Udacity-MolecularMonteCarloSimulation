#ifndef _MC_ESTIM_H
#define _MC_ESTIM_H 1

#include <fstream>
#include <memory>

class Potential;

void InitMCEstims(void);
void Fix_Density(void);
double SinglePot_Density(std::shared_ptr<Potential>);
void SaveDensities1D(const char [], double);

#endif
