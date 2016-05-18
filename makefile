#.SUFFIXES: .o .cxx

options= -O3

MYDIR=./

CFLAGS =-I$(MYDIR)sprng/include -DMOVECENTROIDTEST $(GSL_CFLAGS)
LDFLAGS=-lm -L$(MYDIR)sprng/lib -llcg -static $(GSL_LDFLAG)
 
#-------------------------------------------------------------------------
#  Compilers
#-------------------------------------------------------------------------

CC=g++ 
FC=gfortran

#-------------------------------------------------------------------------
# objects for QMC
 
mch2OBJS=mc_clsmc.o mc_estim.o mc_input.o mc_setup.o mc_poten.o mc_randg.o mc_utils.o
 
#----------------------------------------- PIMC --------------------------

mch2:			mc_main.o $(mch2OBJS) 
			$(CC) $(options) -o $@  mc_main.o $(mch2OBJS) $(LDFLAGS) 

mc_main.o:		mc_main.cc mc_confg.h mc_input.h mc_setup.h mc_randg.h mc_utils.h mc_clsmc.h mc_estim.h 
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cc
 
mc_input.o:        	mc_input.cc mc_input.h	mc_confg.h mc_setup.h mc_utils.h 
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cc

mc_setup.o:        	mc_setup.cc mc_setup.h mc_confg.h mc_utils.h 
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cc

mc_randg.o:        	mc_randg.cc mc_randg.h mc_input.h mc_confg.h mc_utils.h  
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cc

mc_utils.o:        	mc_utils.cc mc_utils.h mc_confg.h 
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cc

mc_poten.o:        	mc_poten.cc mc_poten.h mc_setup.h mc_confg.h mc_utils.h mc_input.cc
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cc

mc_clsmc.o:        	mc_clsmc.cc mc_clsmc.h mc_poten.h mc_setup.h mc_utils.h mc_estim.h
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cc

mc_estim.o:        	mc_estim.cc mc_estim.h mc_setup.h mc_utils.h mc_poten.h mc_input.h mc_confg.h 
			$(CC) $(options) -c  $(CFLAGS) -o $@ $*.cc

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
