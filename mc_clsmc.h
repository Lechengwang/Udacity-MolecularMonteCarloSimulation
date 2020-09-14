#ifndef _MC_CLSMC_H
#define _MC_CLSMC_H 1

extern double MCTotal;
extern double MCAccep;

void MCMove(MCSettings);
double MCPot(int,double **);

#endif
