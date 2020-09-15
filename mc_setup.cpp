#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "mc_setup.h"
#include "mc_utils.h"
#include "mc_confg.h"
#include "mc_input.h"
#include "mc_poten.h"

#include <math.h>

//#define POSTEST 1

long int NumberOfMCSteps;  // number of steps within a block
long int NumberOfMCBlocks;  // number of blocks
long int NumberOfEQBlocks;  // number of equilibr blocks

int MCSKIP_RATIO = 5000;    //  to save information regarding the accept ratio
int MCSKIP_TOTAL = 5000;    //  to save accumulated average
int MCSKIP_AVERG = 10;      //  to evaluate averages

TParticle MCAtom;
int NumbAtoms;
int NumbMoveAtoms;
int NumbFixAtoms;

int * IMoving;        // the id of moving atoms
int * IFix;
int * Moving;
double ** MCCoords;   // translational degrees of freedom
double ** iniMCCoords;
double ** optMCCoords;
double minieng;
double ** newcoords;  // buffer for new coordinates

const char IO_RELAX [] = "RELAX";
const char IO_FIX   [] = "FIX";

int MPIsize;
int MPIrank;

void MCSettings::setTemperature(double temperature) {
  _temperature = temperature;
}

double MCSettings::getTemperature() {
 return _temperature;
}

void MCMemAlloc(void)  // allocate  memmory 
{
  MCCoords    = doubleMatrix (MCSettings::NDIM,NumbAtoms);
  iniMCCoords = doubleMatrix (MCSettings::NDIM,NumbAtoms);
  optMCCoords = doubleMatrix (MCSettings::NDIM,NumbAtoms);
  newcoords   = doubleMatrix (MCSettings::NDIM,NumbAtoms);
  Moving      = new int [NumbAtoms]; // Moving[i]=0->fixed,other->relaxed
  IMoving     = new int [NumbAtoms]; // The ID of moving H2 in MCCoords
  IFix        = new int [NumbAtoms];
//  cout<<"mem alloc done"<<BLANK<<MCSettings::NDIM<<endl;
}


void MCMemFree(void)  //  free memory
{
   free_doubleMatrix(MCCoords);
   free_doubleMatrix(newcoords);
   free_doubleMatrix(optMCCoords);
   free_doubleMatrix(iniMCCoords);

   delete Moving;
   delete IMoving;
}

void MCConfigInit(void)
{
   int * id; //for dumping only
   string relaxflag;
   ifstream fid(MCAtom.fpos,ios::in);
   int na;
   na=get_filesize(MCAtom.fpos);
   MCAtom.numb=na;
   NumbAtoms=na;
   cout<<"Total number of H2 is: "<<NumbAtoms<<endl;
   MCMemAlloc();
   id = new int [NumbAtoms];
   NumbMoveAtoms=0;
   NumbFixAtoms =0;
   for(int i=0;i<NumbAtoms;i++)
   {
      fid>>id[i]>>iniMCCoords[0][i]>>iniMCCoords[1][i]>>iniMCCoords[2][i]>>relaxflag;
      if(relaxflag==IO_RELAX) 
      {
         Moving[i]=1;
         IMoving[NumbMoveAtoms]=i;
         NumbMoveAtoms++;
      }
      else if(relaxflag==IO_FIX)
      { 
         Moving[i]=0;
         IFix[NumbFixAtoms]=i;
         NumbFixAtoms++;
      }
      for(int j=0;j<MCSettings::NDIM;j++)      
      MCCoords[j][i]=iniMCCoords[j][i];
   }
   //for dumping
   #ifdef POSTEST
   for(int j=0;j<NumbAtoms;j++)
   {
     cout<<setw(8)<<iniMCCoords[0][j]<<BLANK;
     cout<<setw(8)<<iniMCCoords[1][j]<<BLANK;
     cout<<setw(8)<<iniMCCoords[2][j]<<BLANK;
     cout<<setw(6)<<Moving[j]<<endl;
   }
   for(int j=0;j<NumbMoveAtoms;j++)
     cout<<setw(6)<<IMoving[j]<<endl;
   for(int j=0;j<NumbFixAtoms;j++)
     cout<<setw(6)<<IFix[j]<<endl;
   #endif
   cout<<"Total number of relaxing H2 is: "<<setw(6)<<NumbMoveAtoms<<endl;
   cout<<"Total number of fixed H2 is: "<<setw(6)<<NumbFixAtoms<<endl;
}
