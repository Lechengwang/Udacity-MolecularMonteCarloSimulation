#.SUFFIXES: .o .cxx

options= -O3

MYDIR=./

CFLAGS =-I$(MYDIR)sprng/include -DMOVECENTROIDTEST $(GSL_CFLAGS)
LDFLAGS=-lm -L$(MYDIR)sprng/lib -llcg -static $(GSL_LDFLAG)
#For MacOS X
#LDFLAGS=-lm -L$(MYDIR)sprng/lib -llcg $(GSL_LDFLAG)
 
#-------------------------------------------------------------------------
#  Compilers
#-------------------------------------------------------------------------

CC=g++ 

#-------------------------------------------------------------------------
# objects for QMC
 
mch2OBJS=mc_clsmc.o mc_estim.o mc_input.o mc_setup.o mc_poten.o mc_randg.o mc_utils.o
 
#----------------------------------------- PIMC --------------------------

mch2:			mc_main.o $(mch2OBJS) 
			$(CC) $(options) -o $@  mc_main.o $(mch2OBJS) $(LDFLAGS) 

mc_main.o:		mc_main.cpp mc_confg.h mc_input.h mc_setup.h mc_randg.h mc_utils.h mc_clsmc.h mc_estim.h 
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cpp
 
mc_input.o:        	mc_input.cpp mc_input.h	mc_confg.h mc_setup.h mc_utils.h 
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cpp

mc_setup.o:        	mc_setup.cpp mc_setup.h mc_confg.h mc_utils.h 
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cpp

mc_randg.o:        	mc_randg.cpp mc_randg.h mc_input.h mc_confg.h mc_utils.h  
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cpp

mc_utils.o:        	mc_utils.cpp mc_utils.h mc_confg.h 
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cpp

mc_poten.o:        	mc_poten.cpp mc_poten.h mc_setup.h mc_confg.h mc_utils.h mc_input.cpp
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cpp

mc_clsmc.o:        	mc_clsmc.cpp mc_clsmc.h mc_poten.h mc_setup.h mc_utils.h mc_estim.h
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cpp

mc_estim.o:        	mc_estim.cpp mc_estim.h mc_setup.h mc_utils.h mc_poten.h mc_input.h mc_confg.h 
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cpp

.PHONY: clean
			
clean:
			rm -f *.o core mch2 

wipe:
			rm -f out mc00* mc00*. *old *.dump ./results/* ./1d/h2*

.f.o:
	f77 -O3 -c  $<
#---------------------------------------------------------------------------
# %.o: %.cxx
# $(CC) $(options) -c  $(CFLAGS) $(CPPFLAGS) $< -o $@
#---------------------------------------------------------------------------
