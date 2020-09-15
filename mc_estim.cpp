#include <math.h>
#include <iomanip>

#include "mc_setup.h"
#include "mc_poten.h"
#include "mc_utils.h"
#include "mc_estim.h"
#include "mc_input.h"
#include "mc_confg.h"

// #define fixdebug 1
// #define relaxdensity 1

using namespace mcsimulation;

const int MC_BINSR=10000;
const double MIN_RADIUS=0.0;
const double MAX_RADIUS=100.0;

double _delta_radius;

double * _gr1D;
double * _fix1D;

MCEstim::MCEstim()
{
 std::cout << "Initializing MC Estimators..." << std::endl;
 _delta_radius = (MAX_RADIUS - MIN_RADIUS)/(double)MC_BINSR;
 _gr1D=new double [MC_BINSR];
 _fix1D = new double [MC_BINSR];
 for(int ir=0;ir<MC_BINSR;ir++)
 {_gr1D[ir]= 0.0;_fix1D[ir]=0.0;}
}

MCEstim::~MCEstim() {
 std::cout << "MCEstim destructor called..." << std::endl;
 delete _gr1D;
 delete _fix1D;
}

void MCEstim::Fix_Density()
//Calcualte the 1D Density of H2 fixed-fixed, without the moving-moving and moving-fixed
//Should be called only once before MC sampling
//Normalized to Nfix*(Nfix-1)/2
{
   const char *_proc_=__func__; //  SinglePot_Density()


   double dr[MCSettings::NDIM];
   double spot = 0.0;

   for (int iatom0=0;iatom0<(NumbFixAtoms-1);iatom0++)
   {
     int atom0=IFix[iatom0];
     for (int iatom1=(iatom0+1);iatom1<NumbFixAtoms;iatom1++)
     {
         int atom1=IFix[iatom1];
         double dr2 = 0.0;
         for (int id=0;id<MCSettings::NDIM;id++)
         {
            dr[id]  = (iniMCCoords[id][atom0] - iniMCCoords[id][atom1]);
            dr2    += (dr[id]*dr[id]);
         }

         double r = sqrt(dr2);
         int bin_r = (int)floor((r-MIN_RADIUS)/_delta_radius);

         if ((bin_r<MC_BINSR) && (bin_r>=0))
         _fix1D[bin_r] += 1.0;

     }
   }
   #ifdef fixdebug
   double count=0;
   double r;
   for(int i=0;i<MC_BINSR;i++)
   {
     cout<<i<<BLANK;
     r   =  (double)i*_delta_radius;
     r  +=  (0.5*_delta_radius);
     r  +=  MIN_RADIUS;
     cout<<r<<BLANK<<_fix1D[i]<<BLANK;
     count=count+_fix1D[i];
     cout<<"Toal: "<<BLANK<<count<<endl;
   }
   cout<<"test1"<<endl;
   cout<<"Total pairs: "<<count<<endl;
   #endif
   
}

double MCEstim::SinglePot_Density(std::shared_ptr<Potential> pot)
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
         bin_1Ddensity (r);
         spot += pot -> SPot1D(r);
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
         bin_1Ddensity (r);
         spot += pot -> SPot1D(r);
    }
  }
  return (spot);
}

void MCEstim::SaveDensities1D(const char fname [], double acount)
{
  fstream fid;
  string fdens;

  fdens = "./1d/";
  fdens += fname;
  fdens += IO_EXT_GRA;
  fid.open(fdens.c_str(),ios::out); io_setout(fid);

  #ifdef relaxdensity
  cout<<fdens<<endl;
  #endif
  int norm=acount;
  double r;
  double gtotal;

  #ifdef relaxdensity
  double relaxpairs=0.0;
  double totalpairs=0.0;
  #endif
  for (int ir=0;ir<MC_BINSR;ir++) // normalization
  {
     r   =  (double)ir*_delta_radius;
     r  +=  (0.5*_delta_radius);
     r  +=  MIN_RADIUS;  
     gtotal=_gr1D[ir]/norm+_fix1D[ir];
     #ifdef relaxdensity
     relaxpairs=relaxpairs+_gr1D[ir]/norm;
     totalpairs=totalpairs+gtotal;
     #endif
     fid << fixed << setprecision(4) << r <<BLANK;
     fid << fixed << setprecision(9) << gtotal<<endl;
  }
  #ifdef relaxdensity
  cout<<"relax-fix and relax-relax pairs: "<<relaxpairs<<endl;
  cout<<"total paris: "<<totalpairs<<endl;
  #endif
  fid.close();
}

void MCEstim::bin_1Ddensity(double r)
//  r         ==   radius
//  dtype     ==   density type
{
   int bin_r = (int)floor((r-MIN_RADIUS)/_delta_radius);

   if ((bin_r<MC_BINSR) && (bin_r>=0))
  _gr1D[bin_r] += 1.0;
}
