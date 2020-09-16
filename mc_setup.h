#ifndef _MM_SETUP_H
#define _MM_SETUP_H 1

#include "mc_confg.h"
#include <string>

namespace mcsimulation {
// Those data structures are heavily read (won't change) in simulation. Organize them into namespace
// Grant direct access to those data structures to avoid overhead of getter/setters
// This is a balance between OOD encapsulation and efficiency
  typedef struct TParticle
  {
   int    numb;            //  numb of atoms in 1D
   double mcstep;          //  simulation step length
   char   type[MAX_STRING_LENGTH];
   char   fpot[MAX_STRING_LENGTH]; // the file with the tabulated potential
   char   fpos[MAX_STRING_LENGTH]; // the file with h2 positions and relax lable
  };

  extern TParticle MCAtom;
  extern int NumbAtoms;
  extern int NumbMoveAtoms;
  extern int NumbFixAtoms;
}

class MCSettings {
public:
  MCSettings();
  ~MCSettings();
  // Setters
  void setTemperature(double);
  void setNumberOfMCSteps(long int);
  void setNumberOfMCBlocks(long int);
  void setNumberOfEQBlocks(long int);
  void setMCSkipRatio(int);
  void setMCSkipTotal(int);
  void setMCSkipAverg(int);

  // Getters
  double getTemperature();
  long getNumberOfMCSteps();
  long getNumberOfMCBlocks();
  long getNumberOfEQBlocks();
  int getMCSkipRatio();
  int getMCSkipTotal();
  int getMCSkipAverg();
  double ** getIniMCCoords();
  int * getIMoving();
  int * getIFix();

  // Some constants
  // Dimensions. We are living in 3D space so it is 3
  static const int NDIM = 3;

  // initiators:
  void MCConfigInit(void);
  void IOReadParams(const char []); // This method defined in mc_input.cpp since it is related to IO

private:
  double _temperature;
  long _numberOfMCSteps;
  long _numberOfMCBlocks;
  long _numberOfEQBlocks;
  int _mcSkipRatio;     //  to save information regarding the accept ratio
  int _mcSkipTotal;     //  to save accumulated average
  int _mcSkipAverg;     //  to evaluate averages

  double ** _iniMCCoords;
  int * _IMoving;
  int * _IFix;
};
#endif
