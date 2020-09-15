#ifndef _MC_ESTIM_H
#define _MC_ESTIM_H 1

#include <fstream>
#include <memory>

class Potential;

class MCEstim {
public:
  MCEstim();
  ~MCEstim();
  void Fix_Density(void);
  double SinglePot_Density(std::shared_ptr<Potential>);
  void SaveDensities1D(const char [], double);
private:
  double _delta_radius;
  double * _gr1D;
  double * _fix1D;
  void bin_1Ddensity(double);
};

#endif
