#include "mc_input.h"
#include "mc_confg.h"
#include "mc_setup.h"
#include "mc_utils.h"
#include "mc_poten.h"
#include "mc_clsmc.h"
#include "mc_randg.h"
#include <math.h>

// #define coodstest 1

double MCTotal;
double MCAccep;

void MCMove(void)
{
  int numb=NumbMoveAtoms;
  double disp[NDIM];
  for(int atom=0;atom<numb;atom++)
  {
    int gatom=IMoving[atom];
    #ifdef coodstest
    cout<<"Moving : "<<gatom<<endl;
    #endif
    for (int id=0;id<NDIM;id++)   // MOVE
    {
       disp[id] = MCAtom.mcstep*(rnd1()-0.5);
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
    (exp(-deltav/Temperature)>rnd2()) Accepted = true;

    MCTotal+= 1.0;
    if (Accepted)
    {
       MCAccep += 1.0;

       for (int id=0;id<NDIM;id++)       // save accepted configuration
       MCCoords[id][gatom] = newcoords[id][gatom];
    }
  }   // END sum over atoms (fixed atom type)
}

double MCPot(int atom0, double **pos)
{   
   double dr[NDIM];
   double spot =  0.0;

   for (int atom1=0;atom1<NumbAtoms;atom1++)
   if (atom1 != atom0)                      // skip "self-interaction"
   {
       double dr2 = 0.0;
       for (int id=0;id<NDIM;id++)
       {
          dr[id]  = (pos[id][atom0] - MCCoords[id][atom1]);
          dr2    += (dr[id]*dr[id]);
       }
       double r = sqrt(dr2);
       spot += SPot1D(r);    // 1D interaction
    }
    return (spot);
}
