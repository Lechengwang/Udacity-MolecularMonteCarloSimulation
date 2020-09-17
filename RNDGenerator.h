#ifndef _MC_RANDG_H
#define _MC_RANDG_H 1

class RNDGenerator { 
public:
  RNDGenerator(int, int);
  ~RNDGenerator();
  double rnd (void);  // double random number generators in the range (0,1)
  double rnd1(void); 
  double rnd2(void); 
  double rnd3(void); 
  double rnd4(void); 
  double rnd5(void); 
  double rnd6(void); 
  double rnd7(void); 
 
  int nrnd1(int n);   // int random number generators in the range (0,n-1)
  int nrnd2(int n);   
  int nrnd3(int n);   
  int nrnd4(int n);   

  double gauss(double);

  void RandomInit(int,int);          //  sprng init 
  void RandomFree(void);             //  free memory used by sprng

private:
  static const int MAXGENS = 15; // max number of rnd generators
  // This is the raw data returned by sprng random library. 
  // Will be heavily used in simulation thus no wrapper like smart points or vectors
  int *_stream[MAXGENS]; // rnd number streams
  int _seed; // seed for rnd number
};
#endif
