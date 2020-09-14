#include "mc_utils.h"
#include "mc_poten.h"
#include "mc_setup.h"

#include <math.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

// ----------- POTENTIALS ----------------------------------
// 1D
double * _poten1D; // tabulated 1D potentials
double * _pgrid1D; // grids for 1D potentials
double * _pderiv2; // second derives for spline

int      _psize1D; // number of grid points for 1D potential
double   drgrid;

double   _alpha;    // parameters for u0e^(-alpha*r) extrapolation
double   _unode;    // of the potential
double   _c6;    // -C6/r^6

void read_datafile(const char [],double *,double *);

void InitPotentials(void)
{
   string fextn;
   fextn=EXT_POT;
   string fname=(MCAtom.fpot + fextn);
   int size        =  get_filesize(fname.c_str());
   _psize1D  =  size;
   _pgrid1D  =  new double [size];  // grid
   _poten1D  =  new double [size];  // data
   _pderiv2  =  new double [size];  // second derivatives for spline  

    read_datafile(fname.c_str(),_pgrid1D,_poten1D);

    init_spline(_pgrid1D,_poten1D,_pderiv2,_psize1D);

// -------- extrapolation for short distances ------------

//  model pot=U0*exp(-alpha*r) - small distance extrapolation  

    double fr = _poten1D[0]/_poten1D[1];
    double dr = _pgrid1D[1]-_pgrid1D[0];
    drgrid=dr;

    _alpha =  log(fr)/dr;
    _unode = _poten1D[0]*exp(_alpha*_pgrid1D[0]);

// -------- extrapolation for long distances -------------

//  model pot=-C6/r^6  - small distance  asymptotic

    double r0 = pow(_pgrid1D[size-2],6.0);
    double r1 = pow(_pgrid1D[size-1],6.0);

    fr = _poten1D[size-1]-_poten1D[size-2];

   _c6 =  fr/(1.0/r0-1.0/r1);
}

double SPot1D(double r)
{
   int size = _psize1D;
 
   if (r >= _pgrid1D[size-1])                //  large distances
   return (-_c6/pow(r,6.0));
// return 0.0;

   if (r <= _pgrid1D[0])
   return (_unode*exp(-_alpha*r));    //  small distances

   double spot;

   splint(_pgrid1D,_poten1D,_pderiv2,size,r,spot);
//   cout<<"r: "<<r<<" pot= "<<spot<<endl;
   return spot;
}

int get_filesize(const char fname [])
// count lines with data in input file
{	
   const char *_proc_=__func__;    // "file_size"; 

   ifstream fid(fname,ios::in);   

//  if (!fid.good())
//   _io_error(_proc_,IO_ERR_FOPEN,fname);

   string stmp;

   int count=0;
   while (fid>>stmp)
   {
      if ((stmp != "") && (stmp != COMMENTS)) count++;
      getline(fid,stmp,'\n');
   } 

   fid.close();
   return (count);

// fid.clear();             // can be used to reset input file 
// fid.seekg(0,ios::beg);
}

void read_datafile(const char fname[],double *grid,double *data)
//  fname:      first   column grid points 
//	        second  column data points
{
   const char *_proc_=__func__;    // "read_DataFile"; 
   
   ifstream fid(fname,ios::in);   

//   if (!fid.good())
//   _io_error(_proc_,IO_ERR_FOPEN,fname);

   string sgrid;  
   string sdata;
  
   int count=0;
   while (fid>>sgrid)
   {
       if ((sgrid != "") && (sgrid != COMMENTS))  // skip comments and empty lines
       {                                          // should be compatible with get_filesize()
           grid [count] = strtod(sgrid.c_str(),NULL);

           fid>>sdata;
           data [count] = strtod(sdata.c_str(),NULL);

           count++;
       }  
       getline(fid,sdata,'\n');              // skip the rest of the line 
   }
// if (count!= maxsize) 
// nrerror(_proc_,"Wrong size of data file");
   fid.close();
}

void read_datafile(const char fname[],double *grid,double *data0,double *data1)
//  fname:      first   column grid points 
//	        second  column data points
//	        third   column data points
{
   const char *_proc_=__func__;    // "read_DataFile"; 
   
   ifstream fid(fname,ios::in);   

//   if (!fid.good())
//   _io_error(_proc_,IO_ERR_FOPEN,fname);

   string sgrid;  
   string sdata;
  
   int count=0;
   while (fid>>sgrid)
   {
       if ((sgrid != "") && (sgrid != COMMENTS))  // skip comments and empty lines
       {                                          // should be compatible with get_filesize()
           grid  [count] = strtod(sgrid.c_str(),NULL);

           fid>>sdata;
           data0 [count] = strtod(sdata.c_str(),NULL);

           fid>>sdata;
           data1 [count] = strtod(sdata.c_str(),NULL);

           count++;
       }  
       getline(fid,sdata,'\n');              // skip the rest of the line 
   }
// if (count!= maxsize) 
// nrerror(_proc_,"Wrong size of data file");
   fid.close();
}
