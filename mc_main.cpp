#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <math.h>

#include "mc_input.h"
#include "mc_estim.h"
#include "mc_confg.h"
#include "mc_setup.h"
#include "mc_utils.h"
#include "mc_poten.h"
#include "mc_clsmc.h"
#include "mc_randg.h"

//#define FNAMETEST 1
//#define POTTEST 1


double avergCount;   // # of calls of get_estim inside a block
double totalCount;   // sum avergCount  

double _bpot;   // kinetic   energy, block average
double _pot_total;  // kinetic   energy, global average
fstream _feng;      // save accumulated energy

#ifdef POTTEST
const double rtest = 2.9654;
#endif
void MCGetAverage(void);
void SaveAcceptRatio(long int,long int);
void SaveSumEnergy(double,double);
void SaveBlockAverages(long int);
void SaveEnergy(const char [],double,int);
void InitTotalAverage(void);

int main()
{
 double vtest;
 MPIsize = 1;            // this is for InitRandom()          
 MPIrank = MPI_MASTER;   // this is for InitRandom() 
 IOReadParams(FINPUT);
 RandomInit(MPIrank,MPIsize);
 MCConfigInit();
 InitPotentials();
#ifdef POTTEST
 vtest=SPot1D(rtest);
 cout<<rtest<<BLANK<<vtest<<endl;
#endif
 MCTotal=0.0;
 MCAccep=0.0;
 InitMCEstims();
 InitTotalAverage();
 Fix_Density();
 long int blockCount=0;
 double sumsCount=0.0;
 while (blockCount<NumberOfMCBlocks)
 {
   blockCount ++;
   avergCount = 0.0;
   _bpot = 0.0;
   long int StepCount = 0;        // BEGIN NEW MC PASS

   while (StepCount++ < NumberOfMCSteps)
   {
     MCMove();

     if (blockCount>NumberOfEQBlocks)        // skip equilibration steps
     {
              // evaluate averages
       if (StepCount % MCSKIP_AVERG == 0)   // skip correlated configurations
         MCGetAverage();

       if (StepCount % MCSKIP_TOTAL == 0)
       {
         sumsCount += 1.0;
         SaveSumEnergy (totalCount,sumsCount);
       }
     }
     if (StepCount % MCSKIP_RATIO == 0)
     SaveAcceptRatio(StepCount,blockCount);
   }//End Steps
   if (blockCount>NumberOfEQBlocks)   // skip equilibration steps
       SaveBlockAverages(blockCount);
   stringstream bc;                // convert block # to string
   bc.width(IO_BLOCKNUMB_WIDTH);
   bc.fill('0');
   bc<<blockCount;

   string fname1 = FNPrefix + bc.str();
   SaveDensities1D    (fname1.c_str(),totalCount);
 }//End Blocks

 
}

void MCGetAverage(void)
{
   avergCount += 1.0;
   totalCount += 1.0;

   double spot = SinglePot_Density(); // pot energy and density distributions

  _bpot += spot;  // block average for pot energy

  _pot_total  += spot;
}

void SaveAcceptRatio(long int step,long int block)
{
   int w = 8;

   cout << "BLOCK:" << setw(w) << block << BLANK;
   cout << "STEP:"  << setw(w) << step  << BLANK;

   double ratio_molec = MCAccep/MCTotal;
   cout<<setw(w)<<MCAtom.type<<BLANK; // atom type
   cout<<setw(w)<<ratio_molec<<BLANK;       // accept ratio for "molecular" move
   cout<<endl;
}


void SaveSumEnergy(double acount,double numb)
{
   const char *_proc_=__func__;    //  SaveSumEnergy()

//   cout<<"SUMTEST"<<endl;
  _feng << setw(IO_WIDTH_BLOCK) << numb << BLANK;
  _feng << setw(IO_WIDTH) << _pot_total/acount << BLANK;
  _feng << endl;

}

void SaveBlockAverages(long int blocknumb)
{
   const char *_proc_=__func__;    //   MCSaveBlockAverages()

   stringstream bc;                // convert block # to string
   bc.width(IO_BLOCKNUMB_WIDTH);
   bc.fill('0');
   bc<<blocknumb;

   string fname = MCFileName + bc.str();
   SaveEnergy         (MCFileName.c_str(),avergCount,blocknumb);
}

void SaveEnergy(const char fname [],double acount,int blocknumb)
{
   const char *_proc_=__func__;    //  SaveEnergy()
   fstream fid;
   string fenergy;

   fenergy  = fname;
   fenergy += IO_EXT_ENG;

   fid.open(fenergy.c_str(),ios::app | ios::out);
   io_setout(fid);
   if (!fid.is_open())
  _io_error(_proc_,IO_ERR_FOPEN,fenergy.c_str());

#ifdef FNAMETEST
 cout<<fenergy<<endl;
#endif

   fid << setw(IO_WIDTH_BLOCK) << blocknumb  << BLANK;    // block number
   fid << setw(IO_WIDTH) << _bpot/avergCount << BLANK;    // potential anerg
   fid <<endl;
   fid.close();
}

void InitTotalAverage(void)  // DUMP
{
   const char *_proc_=__func__;

   totalCount = 0.0;  // need to save in the status file 

  _pot_total = 0.0;

// open files for output
// ENERGY

   string fenergy;

   fenergy  = MCFileName + IO_SUM;
   fenergy += IO_EXT_ENG;

   if (FileExist(fenergy.c_str()))   // backup the output of previous simulations 
   IOFileBackUp(fenergy.c_str());

  _feng.open(fenergy.c_str(), ios::out);
   io_setout(_feng);
   if (!_feng.is_open())
  _io_error(_proc_,IO_ERR_FOPEN,fenergy.c_str());
#ifdef FNAMETEST
  cout<<"_feng: "<<BLANK<<fenergy<<endl;
#endif

}


