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

double ** MCSettings::getIniMCCoords() {
  return _iniMCCoords;
}

int * MCSettings::getIMoving() {
  return _IMoving;
}

int * MCSettings::getIFix() {
  return _IFix;
}

MCSettings::MCSettings()  // allocate  memmory 
{
  std::cout << "MCSettings: allocating memory" << std::endl;
  _iniMCCoords = doubleMatrix (MCSettings::NDIM,NumbAtoms);
  _IMoving     = new int [NumbAtoms]; // The ID of moving H2 in MCCoords
  _IFix        = new int [NumbAtoms];
}


MCSettings::~MCSettings()  //  free memory
{
   std::cout <<"MCSettings: Calling destructor" << std::endl;
   free_doubleMatrix(_iniMCCoords);

   delete _IMoving;
   delete _IFix;
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
   id = new int [NumbAtoms];
   NumbMoveAtoms=0;
   NumbFixAtoms =0;
   for(int i=0;i<NumbAtoms;i++)
   {
      fid>>id[i]>>_iniMCCoords[0][i]>>_iniMCCoords[1][i]>>_iniMCCoords[2][i]>>relaxflag;
      if(relaxflag==IO_RELAX) 
      {
         _IMoving[NumbMoveAtoms]=i;
         NumbMoveAtoms++;
      }
      else if(relaxflag==IO_FIX)
      { 
         _IFix[NumbFixAtoms]=i;
         NumbFixAtoms++;
      }
   }
   //for dumping
   #ifdef POSTEST
   for(int j=0;j<NumbAtoms;j++)
   {
     cout<<setw(8)<<_iniMCCoords[0][j]<<BLANK;
     cout<<setw(8)<<_iniMCCoords[1][j]<<BLANK;
     cout<<setw(8)<<_iniMCCoords[2][j]<<BLANK;
   }
   for(int j=0;j<NumbMoveAtoms;j++)
     cout<<setw(6)<<_IMoving[j]<<endl;
   for(int j=0;j<NumbFixAtoms;j++)
     cout<<setw(6)<<_IFix[j]<<endl;
   #endif
   cout<<"Total number of relaxing H2 is: "<<setw(6)<<NumbMoveAtoms<<endl;
   cout<<"Total number of fixed H2 is: "<<setw(6)<<NumbFixAtoms<<endl;
}
