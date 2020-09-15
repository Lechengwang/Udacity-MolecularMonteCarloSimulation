#include <memory>

#ifndef _MC_CLSMC_H
#define _MC_CLSMC_H 1

class RNDGenerator;
class Potential;

extern double MCTotal;
extern double MCAccep;

void MCMove(MCSettings, std::shared_ptr<RNDGenerator>, std::shared_ptr<Potential>);
double MCPot(int,double **, std::shared_ptr<Potential>);

#endif
