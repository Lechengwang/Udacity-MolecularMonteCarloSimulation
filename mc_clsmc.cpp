#include "mc_input.h"
#include "mc_confg.h"
#include "mc_setup.h"
#include "mc_utils.h"
#include "mc_poten.h"
#include "mc_clsmc.h"
#include "mc_randg.h"
#include <math.h>

// #define coodstest 1

using namespace mcsimulation;

MCMover::MCMover(MCSettings mcSettings, std::shared_ptr<RNDGenerator> rnd, std::shared_ptr<Potential> pot) {
  _MCTotal = 0;
  _MCAccep = 0;
  _mcSettings = mcSettings;
  _rnd = rnd;
  _pot = pot;
}

void MCMover::MCMove()
{
  int numb=NumbMoveAtoms;
  double disp[MCSettings::NDIM];
  for(int atom=0;atom<numb;atom++)
  {
    int gatom=IMoving[atom];
    #ifdef coodstest
    cout<<"Moving : "<<gatom<<endl;
    #endif
    for (int id=0;id<MCSettings::NDIM;id++)   // MOVE
    {
       disp[id] = MCAtom.mcstep*(_rnd->rnd1()-0.5);
       #ifdef coodstest
       cout<<"Offset : "<<disp[id]<<endl;
       #endif


       newcoords[id][gatom]  =  MCCoords[id][gatom];
       newcoords[id][gatom] +=  disp[id];
       #ifdef coodstest
       cout<<MCCoords[id][gatom]<<BLANK<<newcoords[id][gatom]<<endl;
       #endif
    }

    double deltav = 0.0;         // ACCEPT/REJECT
    deltav += (MCPot(gatom,newcoords)-MCPot(gatom,MCCoords));
    bool Accepted = false;

    if (deltav<0.0)             Accepted = true;
    else if
    (exp(-deltav/_mcSettings.getTemperature())>_rnd->rnd2()) Accepted = true;

    _MCTotal+= 1.0;
    if (Accepted)
    {
       _MCAccep += 1.0;

       for (int id=0;id<MCSettings::NDIM;id++)       // save accepted configuration
       MCCoords[id][gatom] = newcoords[id][gatom];
    }
  }   // END sum over atoms (fixed atom type)
}

double MCMover::MCPot(int atom0, double **pos)
{   
   double dr[MCSettings::NDIM];
   double spot =  0.0;

   for (int atom1=0;atom1<NumbAtoms;atom1++)
   if (atom1 != atom0)                      // skip "self-interaction"
   {
       double dr2 = 0.0;
       for (int id=0;id<MCSettings::NDIM;id++)
       {
          dr[id]  = (pos[id][atom0] - MCCoords[id][atom1]);
          dr2    += (dr[id]*dr[id]);
       }
       double r = sqrt(dr2);
       spot += _pot -> SPot1D(r);    // 1D interaction
    }
    return (spot);
}

double MCMover::getAccRatio() {
  return _MCAccep/_MCTotal;
}
