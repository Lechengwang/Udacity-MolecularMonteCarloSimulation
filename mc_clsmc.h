#include <memory>

#ifndef _MC_CLSMC_H
#define _MC_CLSMC_H 1

class RNDGenerator;

extern double MCTotal;
extern double MCAccep;

void MCMove(MCSettings, std::shared_ptr<RNDGenerator>);
double MCPot(int,double **);

#endif
