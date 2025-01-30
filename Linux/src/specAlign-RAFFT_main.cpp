/*************************************************************************************************
File:		specAlign-RAFFT_main.cpp
Created:	2005/09/18
Author:		Jason W H Wong <jason.wong@chem.ox.ac.uk>
	
Purpose:	Implements the interface to the RAFFT method of specAlign.

License:	The stand-alone specAlign application and its source code is free to academic, 
		government and non-profit users for non-commercial use. Use of the specAlign
		application and source code in part or in whole for commercial use by non-profit 
		and for-profit entities requires a license agreement. Parties interested in 
		commercial should contact jason.wong@chem.ox.ac.uk for permission to use
		the specAlign application or its source code.

		The use of specAlign in any research must cite as appropriate:
		Wong, J.W.H., Cagney, G. and Cartwright, H.M. (2005) SpecAlign - processing and 
			alignment of mass spectra datasets. Bioinformatics. 21(9):2088-2090.
		Wong, J.W.H., Durante, C and Cartwright, H.M. (2005) Application of Fast Fourier 
			Transform Cross-Correlation for the Alignment of Large Chromatographic and 
			Spectral Datasets. Anal. Chem. 77 (17), 5655 -5661		

Disclaimer:	specAlign and all associated documents ARE PROVIDED "AS IS" WITHOUT ANY WARRANTY 
		OF ANY KIND, EITHER EXPRESS, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, 
		ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
		FREEDOM FROM INFRINGEMENT, OR THAT specAlign and all associated documents WILL 
		BE ERROR FREE. The authur MAKES NO REPRESENTATIONS THAT THE USE OF specAlign or 
		any associated documents WILL NOT INFRINGE ANY PATENT OR PROPRIETARY RIGHTS OF 
		THIRD PARTIES. In no event will the authors be liable for any damages, including 
		but not limited to direct, indirect, special or consequential damages, arising out 
		of, resulting from, or in any way connected with the use of specAlign or any associated 
		documents.
**************************************************************************************************/

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fstream>
#include "common/iface.h"

using namespace std;

int usage();

int main(int argc, char *argv[])
{
  int c;

  char *in_filename = NULL;
  char *out_folder = NULL;
  char *in_avg_file = NULL;
  int sflag = 0;
  int tflag = 0;
  int rflag = 0;
  int bflag = 0;
  int eflag = 0;
  int lflag = 0;
  int mflag = 0;
  int cflag = 0;
  float begin = 0;
  float end = 0;
  int mshift = 20;
  int lookahead = 1;
  float scale = 1;

  char buf[256]="";
  
  if (argc == 1) usage();

  while ((c = getopt(argc,argv,"s:t:r:b:e:l:m:c:h")) != -1) {
    switch(c){
    case 's': //source file
      sflag = 1;
      in_filename = optarg;
      break;
    case 't': // destination dir
      tflag = 1;
      out_folder = optarg;
      break;
    case 'r': // average file
	rflag = 1;
	in_avg_file = optarg;
	break;
    case 'b': // begin
      bflag = 1;
      begin = atof(optarg);
      break;
    case 'e': // end
      eflag = 1;
      end = atof(optarg);
      break;
    case 'l': // lookahead
      lflag = 1;
      lookahead = atoi(optarg);
      break;
    case 'm': // max shift
      mflag = 1;
      mshift = atoi(optarg);
      break;
    case 'c': // scale
      cflag = 1;
      scale = atof(optarg);
      break;
    case 'h':  //help
      usage();
    default: 
      usage();
    }
  }
  
  if (sflag == 0){
  	cout << "Please enter input file/folder name (-s)."<<endl;
  	exit(0);
  }
  if (tflag == 0){
  	cout << "Please enter destination file/folder name for dta files (-t)."<<endl;
  	exit(0);
  }  

  cout << "specAlign-RAFFT 1.21 (18-Sept-05) \nby Jason W. H. Wong, Cartwright Lab, PTCL, University of Oxford, 2005"<<endl<<endl;
 iface fHandle;
 int s;
 string tmp = in_filename;
 if (tmp.find(".",1) != string::npos){
	cout << "Reading file: " << in_filename<<endl;
	s = fHandle.readFileInput(0,in_filename,true,false);
 }
 else{
	cout << "Reading folder: " << in_filename<<endl;
	s = fHandle.readFolderInput(in_filename);
 }

 if (rflag == 1){
	s = fHandle.readAvg(in_avg_file, false);
 }

 if (bflag == 0)
	begin = fHandle.mz_val.front();
 if (eflag == 0)
	end = fHandle.mz_val.back();
 if (mflag == 0)
	mshift = 20;
 if (lflag == 0)
	lookahead = 1;
 if (cflag == 0)
	scale = 1;

 tmp = out_folder;
 if (s!= 0){
    cout << "\nSpectra to be analyzed: "<< fHandle.train.size()<<endl<<endl;
    cout.flush();

    if (rflag == 0)
	  fHandle.sumSpec();

    cout << "Aligning spectra by RAFFT...";
    fHandle.rafftAlign(begin,end,scale,mshift,lookahead);
    cout << "Done"<<endl<<endl;

    if (tmp.find(".",1) != string::npos)
    {
	cout << "Writing to file: " << out_folder<<endl;
    	fHandle.exportSingleFile(out_folder,true);
    }
    else{
	cout << "Writing to folder: " << out_folder<<endl;
	fHandle.exportMultiFile(out_folder);
    }
 }
}

int usage() {
	printf("\nspecAlign-RAFFT 1.21 (18-Sept-05) \nby Jason W. H. Wong, Cartwright Lab, PTCL, Univeristy of Oxford, 2005\n");
	printf("\nUsage: specAlign-RAFFT <-s source> <-t target>\n");
	printf("       	               [-r reference_spec] [-b begin]                     \n");
	printf("       	               [-e end] [-m max_shift] [-c scale]\n");
	printf("       	               [-l lookahead][h]\n");
	printf("	----------------------------------------------------------------\n");
	printf("       	(source)   path of source file/directory\n");
	printf("       	(target)   path of target file/directory (must exist)\n");
	printf("	-r   where \"reference_spec\" is the path for the reference spectrum\n"); 
        printf("             (default: average spectrum from source dataset)\n");
	printf("	-b   where \"begin\" is a float specifying the begin m/z value\n"); 
        printf("             to align (default: minimum m/z value)\n");
	printf("	-e   where \"end\" is a float specifying the end m/z value\n");
        printf("             to align (default: maximum m/z value)\n");
	printf("	-m   where \"max_shift\" is an integer specifying maximum shift points\n");
        printf("             allowed for each segment to be aligned (default: 20)\n");
	printf("	-c   where \"scale\" is a float specifying the amount to scale \n");
        printf("             all spectra before alignment (default: 1 (e.g. 0.5 = sqrt))\n");
        printf("       	-l   where \"lookahead\" is an integer specifying the number of\n");
        printf("             recursions to look ahead (default: 1)\n");
        printf("       	-h   displays this message\n");
	printf("\nFriendly example: \n");
	printf("specAlign-RAFFT -s /home/raw_data/example -t /home/processed/example \n");
	printf("           	-b 200.0 -e 50000.0 -m 50\n\n");


	exit(1);
}
