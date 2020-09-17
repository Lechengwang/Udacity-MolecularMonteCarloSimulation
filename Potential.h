#ifndef _MC_POTEN_H
#define _MC_POTEN_H 1

#include "mc_confg.h"
#include "mc_utils.h"

class Potential {
public:
  Potential();
  ~Potential();
  // Getting potential at any position by spline
  double SPot1D(double);
private:
  void read_datafile(const char [],double *,double *);

// ---- POTENTIALS, interaction energy between 2 atoms/molecules----
// 1D
  double * _poten1D; // tabulated 1D potentials
  double * _pgrid1D; // grids for 1D potentials
  double * _pderiv2; // second derives for spline

  int      _psize1D; // number of grid points for 1D potential
  double   drgrid;

  double   _alpha;    // parameters for u0e^(-alpha*r) extrapolation
  double   _unode;    // of the potential
  double   _c6;    // -C6/r^6

  const std::string EXT_POT = ".pot";
};

#endif  //
