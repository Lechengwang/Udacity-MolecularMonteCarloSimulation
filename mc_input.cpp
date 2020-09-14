#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "mc_confg.h"
#include "mc_input.h"
#include "mc_utils.h"
#include "mc_poten.h"
#include "mc_setup.h"

const char IO_OUTPUTDIR[]      = "OUTPUTDIR";
const char IO_FILENAMEPREFIX[] = "FILENAMEPREFIX";
const char IO_TEMPERATURE[]    = "TEMPERATURE";
const char IO_ATOM[]           = "ATOM";
const char IO_NUMBEROFSTEPS[]  = "NUMBEROFSTEPS";
const char IO_NUMBEROFBLOCKS[] = "NUMBEROFBLOCKS";
const char IO_MCSKIP_RATIO[]   = "MCSKIP_RATIO";
const char IO_MCSKIP_TOTAL[]   = "MCSKIP_TOTAL";
const char IO_MCSKIP_AVERG[]   = "MCSKIP_AVERG";

string OutputDir;
string FNPrefix;

string MCFileName;     // mc output file name (no extension)

void IOReadParams(const char in_file[], MCSettings settings)
{
   const char *_proc_=__func__;    // "MCReadInput()"; 

   NDIM=3;
   string params;
   ifstream inf(in_file,ios::in);
   if (!inf.good())
   _io_error(_proc_,IO_ERR_FOPEN,in_file);
   while (inf>>params)
   {
     if (params==IO_OUTPUTDIR)
     {
        inf>>OutputDir;
     }
     else
     if (params==IO_FILENAMEPREFIX)
     {
        inf>>FNPrefix;
     }
     else
     if (params==IO_TEMPERATURE)
     {
        double temperature;
        inf>>temperature;
        settings.setTemperature(temperature);
     }
     else
     if (params==IO_ATOM)
     {
         inf>>MCAtom.type;     //[1]
         inf>>MCAtom.mcstep;   //[2]
         inf>>MCAtom.fpot;     //[3]
         inf>>MCAtom.fpos;     //[4]        
     }
     else
     if (params==IO_NUMBEROFBLOCKS)
     {
         inf>>NumberOfMCBlocks>>NumberOfEQBlocks;
     }
     else
     if (params==IO_NUMBEROFSTEPS)
     {
         inf>>NumberOfMCSteps;
     }
     else
     if (params==IO_MCSKIP_RATIO)
     {
        inf >> MCSKIP_RATIO;
     }
     else
     if (params==IO_MCSKIP_TOTAL)
     {
        inf >> MCSKIP_TOTAL;
     }
     else
     if (params==IO_MCSKIP_AVERG)
     {
        inf >> MCSKIP_AVERG;
     }
     else
     {}
     getline(inf,params,'\n');  // skip comments at the end of the line 
   }
   inf.close();
   MCFileName = (OutputDir+FNPrefix);
// begin DUMP
   cout << "OutPut   "          <<OutputDir<<endl;
   cout << "File Name Prefix   "<<FNPrefix<<endl;
   cout << "Temperature "<<settings.getTemperature()<<endl;
   int w=6;
   cout << setw(w) << MCAtom.type << BLANK;
   cout << setw(w) << MCAtom.mcstep << BLANK;
   cout << setw(w) << MCAtom.fpot   << BLANK;
   cout << setw(w) << MCAtom.fpos   << BLANK;

   cout << endl;
   cout << "Number of Steps in block = " << NumberOfMCSteps << endl;
   cout << "Number of Blocks = " << NumberOfMCBlocks << BLANK << NumberOfEQBlocks << endl;

   cout << endl;

   cout << "Number of steps to skip to save ACCEPT RATIO" << BLANK << MCSKIP_RATIO << endl;
   cout << "Number of steps to skip to save ACCUML AVERG" << BLANK << MCSKIP_TOTAL << endl;
   cout << "Number of steps to skip to evaluate AVERAGES" << BLANK << MCSKIP_AVERG << endl;

}

int FileExist (const char fileName [])
{
   FILE *infile = fopen (fileName, "r");
// ifstream  infile(fileName,ios::in); 

   int ret_code=0;
   if (infile == NULL)
       ret_code = 0;    // file doesn't exist
   else
   {
       ret_code = 1;     // file exists
       fclose (infile);
   }
   return (ret_code);
}

void io_setout(fstream &out, int set_precision)
{
   out<<setprecision(set_precision);
   out<<setiosflags (ios::scientific);
// out<<setiosflags (ios::right);
}

void IOFileBackUp(const char file_name[])
{
#ifdef DEBUG_PIMC
   const char *_proc_=__func__;
#endif

   string cmd="cp";

   if (FileExist(file_name))
   {
      cmd += " " + (string)file_name;
      cmd += " " + (string)file_name+(string)FBACKUP_EXTENSION;
      int status = system(cmd.c_str());

#ifdef DEBUG_PIMC
      if (status < 0)
      nrerror(_proc_,IO_ERR_SCALL);
#endif
   }
}

             
