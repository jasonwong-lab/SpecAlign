/***************************************************************************************************
File:		specAlign-peakalign_main.cpp
Created:	2005/09/18
Author:		Jason W H Wong <jason.wong@chem.ox.ac.uk>
	
Purpose:	Implements the interface to the peakAlign method of specAlign.

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
***************************************************************************************************/

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
  int gflag = 0;
  int mflag = 0;
  int cflag = 0;
  int wflag = 0;
  int iflag = 0;
  float begin = 0;
  float end = 0;
  int iter = 1;
  int win = 21;
  int mshift = 20;
  float cutoff = 0.01;

  char buf[256]="";
  
  if (argc == 1) usage();

  while ((c = getopt(argc,argv,"s:t:r:b:e:w:i:m:c:h")) != -1) {
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
    case 'w': // window size
      wflag = 1;
      win = atoi(optarg);
      break;
    case 'i': // iterations
      iflag = 1;
      iter = atoi(optarg);
      break;
    case 'm': // segment size
      mflag = 1;
      mshift = atoi(optarg);
      break;
    case 'c': // cutoff
      cflag = 1;
      cutoff = atof(optarg);
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

  cout << "specAlign-peakalign 1.21 (18-Sept-05) \nby Jason W. H. Wong, Cartwright Lab, PTCL, University of Oxford, 2005"<<endl<<endl;
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
 if (iflag == 0)
	iter = 1;
 if (wflag == 0)
	win = 21;
 if (cflag == 0)
	cutoff = 0.01;

 tmp = out_folder;
 if (s!= 0){
    cout << "Spectra to be analyzed: "<< fHandle.train.size()<<endl<<endl;
    cout.flush();

    if (rflag == 0)
	  fHandle.sumSpec();

    cout << "Aligning spectra by peak alignment...";
    fHandle.alignPeaks(mshift,cutoff,win,iter,begin,end);
    cout << "Done" <<endl<<endl;

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
	printf("\nspecAlign-peakalign 1.21 (18-Sept-05) \nby Jason W. H. Wong, Cartwright Lab, PTCL, Univeristy of Oxford, 2005\n");
	printf("\nUsage: specAlign-peakalign <-s source> <-t target>\n");
	printf("                           [-r reference_spec] [-b begin]\n");
	printf("                           [-e end] [-m max_shift] [-w window]\n");
	printf("                           [-i iterations][-c cutoff][h]\n");
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
	printf("	-w   where \"window\" is an integer specifying the window size for\n");
        printf("             a peak to be found (default: 21)\n");
        printf("       	-i   where \"iterations\" is an integer specifying the number of\n");
        printf("             iterations to look for peaks (default: 1)\n");
        printf("       	-c   where \"cutoff\" is a float specifying the minimum\n");
        printf("             relative intenisty required for a peak to be picked (default: 0.01)\n");
        printf("       	-h   displays this message\n");
	printf("\nFriendly example: \n");
	printf("specAlign-peakalign -s /home/raw_data/example -t /home/processed/example\n");
	printf("           	    -b 200.0 -e 50000.0 -w 9 -c 0.1\n\n");


	exit(1);
}
