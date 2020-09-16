#ifndef _MC_ESTIM_H
#define _MC_ESTIM_H 1

#include <fstream>
#include <memory>

class Potential;

class MCEstim {
public:
  MCEstim(std::shared_ptr<Potential>);
  ~MCEstim();
  double SinglePot_Density();
private:
  std::shared_ptr<Potential> _pot;
};

#endif
