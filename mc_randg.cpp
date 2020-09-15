#include <math.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>

#include "sprng.h"	

#include "mc_input.h"
#include "mc_randg.h"
#include "mc_utils.h"
	

RNDGenerator::RNDGenerator(int mpi_id, int max_mpi_procs)  // sprng init
{ 
  std::cout <<"RNDGenerator constructor called." << std::endl;
  int max_streams=MAXGENS*max_mpi_procs;        // number of streams 
           
  srand(time(0));       //seed
  _seed = rand()%1000000000;        //seed for PIMC,seed is (1,1000000000)

  cout<<"The random seed for this work is: "<<_seed<<endl;

  for (int ip=0;ip<MAXGENS;ip++)
  { 
     int streamnum = mpi_id*MAXGENS+ip;
     _stream[ip] = init_sprng(streamnum,max_streams,_seed,SPRNG_DEFAULT); // initialize stream 
//   print_sprng(stream);	
  }
}

RNDGenerator::~RNDGenerator() // free memory used by sprng 
{
  std::cout << "RND Destructor called." << std::endl;
  for (int ip=0;ip<MAXGENS;ip++)
    free_sprng(_stream[ip]);     
}

double RNDGenerator::rnd()
// double random number generator in the range [0,1] 
{
  return sprng(_stream[0]);
}

double RNDGenerator::rnd1()
// double random number generator in the range [0,1] 
{
  return sprng(_stream[1]);
}

double RNDGenerator::rnd2()
// double random number generator in the range [0,1] 
{
  return sprng(_stream[2]);
}

double RNDGenerator::rnd3()
// double random number generator in the range [0,1] 
{
  return sprng(_stream[3]);
}

double RNDGenerator::rnd4()
// double random number generator in the range [0,1] 
{
  return sprng(_stream[4]);
}

double RNDGenerator::rnd5()
// double random number generator in the range [0,1] 
{
  return sprng(_stream[5]);
}

double RNDGenerator::rnd6()
// double random number generator in the range [0,1] 
{
  return sprng(_stream[6]);
}

double RNDGenerator::rnd7()
// double random number generator in the range [0,1] 
{
  return sprng(_stream[7]);
}

double RNDGenerator::gauss(double alpha)
// random numbers according Gaussian distribution exp{-alpha*x^2}
{
    double r1 = sprng(_stream[8]);
    double r2 = sprng(_stream[9]);

    double x1 = sqrt(-log(r1))*cos(2.0*M_PI*r2);

//  double x1 = sqrt(-log(r1))*cos(2.0*M_PI*r2);
//  double x2 = sqrt(-log(r1))*sin(2.0*M_PI*r2);

    return (x1/sqrt(alpha));
}

int RNDGenerator::nrnd1(int n)
// integer random number generator in the range [0,n-1] 
{
  return (int)floor(n*sprng(_stream[10]));
}

int RNDGenerator::nrnd2(int n)
// integer random number generator in the range [0,n-1] 
{
  return (int)floor(n*sprng(_stream[11]));
}

int RNDGenerator::nrnd3(int n)
// integer random number generator in the range [0,n-1] 
{
  return (int)floor(n*sprng(_stream[12]));
}

int RNDGenerator::nrnd4(int n)
// integer random number generator in the range [0,n-1] 
{
  return (int)floor(n*sprng(_stream[13]));
}
