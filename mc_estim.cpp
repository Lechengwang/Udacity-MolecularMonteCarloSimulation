#include <math.h>
#include <iomanip>

#include "MCSettings.h"
#include "mc_poten.h"
#include "mc_utils.h"
#include "mc_estim.h"
#include "mc_input.h"
#include "mc_confg.h"

// #define fixdebug 1
// #define relaxdensity 1

using namespace mcsimulation;

MCEstim::MCEstim(std::shared_ptr<Potential> pot) : _pot(pot)
{
 std::cout << "Initializing MC Estimators..." << std::endl;
}

MCEstim::~MCEstim() {
 std::cout << "MCEstim destructor called..." << std::endl;
}


double MCEstim::SinglePot_Density(double ** MCCoords, int * IFix, int * IMoving)
{
  const char *_proc_=__func__; //  SinglePot_Density()


  double dr[MCSettings::NDIM];
  double spot = 0.0;

// Pot and density of Moving-Fix pairs
  int numb=NumbMoveAtoms;
  for (int iatom0=0;iatom0<NumbMoveAtoms;iatom0++)
  {
    int atom0=IMoving[iatom0];  
    for (int iatom1=0;iatom1<NumbFixAtoms;iatom1++)
    {
         int atom1=IFix[iatom1];
         double dr2 = 0.0;
         for (int id=0;id<MCSettings::NDIM;id++)
         {
            dr[id]  = (MCCoords[id][atom0] - MCCoords[id][atom1]);
            dr2    += (dr[id]*dr[id]);
         }

         double r = sqrt(dr2);
         spot += _pot -> SPot1D(r);
    }
  }
// Pot and density of Moving-Moving pairs
  for (int iatom0=0;iatom0<NumbMoveAtoms-1;iatom0++)
  {
    int atom0=IMoving[iatom0];
    for (int iatom1=iatom0+1;iatom1<NumbMoveAtoms;iatom1++)
    {
         int atom1=IMoving[iatom1];
         double dr2 = 0.0;
         for (int id=0;id<MCSettings::NDIM;id++)
         {
            dr[id]  = (MCCoords[id][atom0] - MCCoords[id][atom1]);
            dr2    += (dr[id]*dr[id]);
         }

         double r = sqrt(dr2);
         spot += _pot -> SPot1D(r);
    }
  }
  return (spot);
}
