#ifndef _MM_SETUP_H
#define _MM_SETUP_H 1

#include "mc_confg.h"
#include <string>

class MCSettings {
public:
  // Setters
  void setTemperature(double);

  // Getters
  double getTemperature();

  // Some constants
  // Dimensions. We are living in 3D space so it is 3
  static const int NDIM = 3;
private:
  double _temperature;
};

extern long int NumberOfMCSteps;
extern long int NumberOfMCBlocks;
extern long int NumberOfEQBlocks;

extern int MCSKIP_RATIO;     //  to save information regarding the accept ratio
extern int MCSKIP_TOTAL;     //  to save accumulated average
extern int MCSKIP_AVERG;     //  to evaluate averages

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

extern double ** MCCoords;   // translational degrees of freedom
extern double ** iniMCCoords;// initial configuration of beads
extern double ** optMCCoords;// translational degrees of freedom of lowest energy during annealing
extern double minieng;       // minimal energy during annealing

extern double ** newcoords;  // buffer for new coordinates
extern int    * Moving;
extern int    * IMoving;
extern int    * IFix;

extern int   MPIsize;    // MPI
extern int   MPIrank;    // MPI

void MCMemAlloc(void); // allocate memory
void MCMemFree(void);  // free memory
void MCConfigInit(void);

#endif
