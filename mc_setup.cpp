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

using namespace mcsimulation;

const char IO_RELAX [] = "RELAX";
const char IO_FIX   [] = "FIX";

// namespace variables
namespace mcsimulation {
  TParticle MCAtom;
  int NumbAtoms;
  int NumbMoveAtoms;
  int NumbFixAtoms;
  double ** iniMCCoords;
  int * IMoving;
  int * IFix;
}

// Member methods: setters

void MCSettings::setTemperature(double temperature) {
  _temperature = temperature;
}

void MCSettings::setNumberOfMCSteps(long steps) {
  _numberOfMCSteps = steps;
}

void MCSettings::setNumberOfMCBlocks(long blocks) {
  _numberOfMCBlocks = blocks;
}

void MCSettings::setNumberOfEQBlocks(long blocks) {
  _numberOfEQBlocks = blocks;
}

void MCSettings::setMCSkipRatio(int ratio) {
  _mcSkipRatio = ratio;
}

void MCSettings::setMCSkipTotal(int total) {
  _mcSkipTotal = total;
}

void MCSettings::setMCSkipAverg(int averg) {
  _mcSkipAverg = averg;
}

// Member methods: getters

double MCSettings::getTemperature() {
  return _temperature;
}

long MCSettings::getNumberOfMCSteps() {
  return _numberOfMCSteps;
}

long MCSettings::getNumberOfMCBlocks() {
  return _numberOfMCBlocks;
}

long MCSettings::getNumberOfEQBlocks() {
  return _numberOfEQBlocks;
}

int MCSettings::getMCSkipRatio() {
  return _mcSkipRatio;
}

int MCSettings::getMCSkipTotal() {
  return _mcSkipTotal;
}

int MCSettings::getMCSkipAverg() {
  return _mcSkipAverg;
}

void MCSettings::MCMemAlloc(void)  // allocate  memmory 
{
  iniMCCoords = doubleMatrix (MCSettings::NDIM,NumbAtoms);
  IMoving     = new int [NumbAtoms]; // The ID of moving H2 in MCCoords
  IFix        = new int [NumbAtoms];
//  cout<<"mem alloc done"<<BLANK<<MCSettings::NDIM<<endl;
}


void MCSettings::MCMemFree(void)  //  free memory
{
   std::cout <<"Calling destructor" << std::endl;
   free_doubleMatrix(iniMCCoords);

   delete IMoving;
   delete IFix;
}

void MCSettings::MCConfigInit(void)
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
         IMoving[NumbMoveAtoms]=i;
         NumbMoveAtoms++;
      }
      else if(relaxflag==IO_FIX)
      { 
         IFix[NumbFixAtoms]=i;
         NumbFixAtoms++;
      }
   }
   //for dumping
   #ifdef POSTEST
   for(int j=0;j<NumbAtoms;j++)
   {
     cout<<setw(8)<<iniMCCoords[0][j]<<BLANK;
     cout<<setw(8)<<iniMCCoords[1][j]<<BLANK;
     cout<<setw(8)<<iniMCCoords[2][j]<<BLANK;
   }
   for(int j=0;j<NumbMoveAtoms;j++)
     cout<<setw(6)<<IMoving[j]<<endl;
   for(int j=0;j<NumbFixAtoms;j++)
     cout<<setw(6)<<IFix[j]<<endl;
   #endif
   cout<<"Total number of relaxing H2 is: "<<setw(6)<<NumbMoveAtoms<<endl;
   cout<<"Total number of fixed H2 is: "<<setw(6)<<NumbFixAtoms<<endl;
}
