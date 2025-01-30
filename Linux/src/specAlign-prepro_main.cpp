/***************************************************************************************************
File:		specAlign-prepro_main.cpp
Created:	2008/03/10
Author:		Jason W H Wong <jwong@physchem.ox.ac.uk>
	
Purpose:	Implements the interface to preprocessing methods of specAlign.

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

#include <sys/types.h>
#include <dirent.h>
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
  char *out_avg_file = NULL;
  int sflag = 0;
  int tflag = 0;
  int nflag = 0;
  int oflag = 0;
  int pflag = 0;
  int iflag = 0;
  int uflag = 0;
  int bflag = 0;
  int eflag = 0;
  int rflag = 0;
  int cflag = 0;
  int xflag = 0;
  int aflag = 0;
  int kflag = 0;
  float bin_size = 1.0;
  int smooth = 8;
  float scale = 1.0;
  int baseline = 50;
  float begin = 0;
  float end = 0;
  string peakdata;

  char buf[256]="";
  
  if (argc == 1) usage();

  while ((c = getopt(argc,argv,"hxirps:t:b:e:n:o:c:u:a:k:")) != -1) {
    switch(c){
    case 's': //source file
      sflag = 1;
      in_filename = optarg;
      break;
    case 't': // destination dir
      tflag = 1;
      out_folder = optarg;
      break;
    case 'b': // begin
      bflag = 1;
      begin = atof(optarg);
      break;
    case 'e': // end
      eflag = 1;
      end = atof(optarg);
      break;
    case 'n': // bin spectra
	nflag = 1;
	bin_size = atof(optarg);
	break;
    case 'o': // smooth
      oflag = 1;
      smooth = atoi(optarg);
      break;
    case 'c': // scale
      cflag = 1;
      scale = atof(optarg);
      break;
    case 'u': // baseline subtraction
      uflag = 1;
      baseline = atoi(optarg);
      break;
    case 'i': // normalise TIC
      iflag = 1;
      break;
    case 'r':  // relative intensities
      rflag = 1;
      break;
    case 'p': // rescale to positive
      pflag = 1;
      break;
    case 'x': // rescale to positive
      xflag = 1;
      break;
    case 'a':
      aflag = 1;
      out_avg_file = optarg;
      break;
    case 'k':
      kflag = 1;
      peakdata = optarg;
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

  cout << "specAlign-prepro 1.3 (10-Mar-08) \nby Jason W. H. Wong, Cartwright Lab, PTCL, University of Oxford, 2005-08"<<endl<<endl;
 iface fHandle;
 int s;
 string tmp = in_filename;
 if (opendir(in_filename) == NULL){
	cout << "Reading file: " << in_filename<<endl;
	s = fHandle.readFileInput(0,in_filename,true,false);
 }
 else{
	cout << "Reading folder: " << in_filename<<endl;
	s = fHandle.readFolderInput(in_filename);
 }

  if (bflag == 0)
	begin = fHandle.mz_val.front();
  if (eflag == 0)
	end = fHandle.mz_val.back();

 tmp = out_folder;
 if (s!= 0){
    cout << "\nSpectra to be analyzed: "<< fHandle.train.size()<<endl;
    cout.flush();

    fHandle.sumSpec();
    if (xflag == 1){
	cout << "Cropping from "<< begin <<" to "<<end <<"..."<<endl;
	fHandle.cropSpectra(fHandle.getXIndex(begin),fHandle.getXIndex(end));
    }
    if (iflag == 1){
	cout << "Normalizing from "<< begin <<" to "<<end <<"..."<<endl;
	fHandle.normalizeSpec(fHandle.getXIndex(begin),fHandle.getXIndex(end));
    }
    if (pflag == 1){
	cout << "Scaling to positive..."<<endl;
	fHandle.scalePositive();
    }
    if (cflag == 1){
	cout << "Scaling spectra by "<<scale<<"..."<<endl;
	fHandle.scale(scale);
    }
    if (nflag == 1){
	cout << "Bining spectra m/z into "<<bin_size << " bins..."<<endl;
	fHandle.binSpectra(bin_size);
    }
    if (uflag == 1){
	cout << "Subtracting baseline with sensitivity "<< baseline<<"..."<<endl;
	fHandle.subtractBase(baseline);
    }
    if (oflag == 1){
	cout << "Smoothing by S-G filter of size "<<smooth<<"..."<<endl;
	fHandle.smoothSpec(smooth);
    }
    if (rflag == 1){
	cout << "Converting to relative intensities..." << endl;
	fHandle.convertToRelative();
    }
    if (kflag == 1){
        double cutoff = 0;
        int window = 21;
        int iter = 1;
        string::size_type last = 0;
        string::size_type space = peakdata.find(":",last+1);
        cutoff = atof(peakdata.substr(last,space-last).c_str());
        last = space+1;
        space = peakdata.find(":",last+1);
        window = atoi(peakdata.substr(last,space-last).c_str());
        last = space+1;
        space = peakdata.find(":",last+1);
        iter = atoi(peakdata.substr(last,space-last).c_str());
        last = space+1;
        space = peakdata.find(":",last+1);
        char* peak_filename = (char*)(peakdata.substr(last,space-last)).c_str();
        cout << "Picking peak (Cutoff: " << cutoff << " Window: " << window << " Iterations: " << iter << " Peak file: " << peak_filename <<")" << endl;
        fHandle.exportPeaks(peak_filename, cutoff, window, iter);
    }
    if (aflag == 1){
        cout << "Writing average spectrum file to: " << out_avg_file << endl;
        fHandle.exportAvg(out_avg_file,false);
    }
    if (tmp.find(".",1) != string::npos)
    {
	cout << "\nWriting to file: " << out_folder<<endl;
    	fHandle.exportSingleFile(out_folder,true);
    }
    else{
	cout << "\nWriting to folder: " << out_folder<<endl;
	fHandle.exportMultiFile(out_folder);
    }
 }
}

int usage() {
	printf("\nspecAlign-prepro 1.3 (10-Mar-08) \nby Jason W. H. Wong, Cartwright Lab, PTCL, University of Oxford, 2005-08\n");
	printf("\nUsage: specAlign-prepro <-s source_file> <-t target_dir>\n");
	printf("       	                [-i] [-b begin] [-e end]               \n");
	printf("       	            	[-n bin_size] [-o smooth_filter_size] [-c scale]\n");
	printf("       	            	[-u baseline] [-r] [-p]\n");
	printf("       	            	[-a avg_file] [-k cutoff:window:iterations:peak_file]\n");
	printf("	----------------------------------------------------------------\n");
	printf("       	(source)   path of source file/directory\n");
	printf("       	(target)   path of target file/directory (must exist)\n");
	printf("	-i   specifies that spectra are to be normalized by total ion\n"); 
	printf("	     current(TIC)\n");
	printf("	-b   where \"begin\" is a float specifying start point for TIC \n");
	printf("	     normalization - used with -i (default: minimum m/z value)\n");
	printf("	-e   where \"end\" is a float specifying end point for TIC \n");
	printf("	     normalization - used with -i (default: maximum m/z value)\n");
	printf("	-n   where \"bin_size\" is an integer specifying the size to bin\n"); 
        printf("             m/z values into (default: 1)\n");
	printf("	-o   where \"smooth_filter_size\" is an integer specifying the filter \n");
        printf("             size to use for S-G smoothing of spectra (default: 8 (max=15))\n");
	printf("	-c   where \"scale\" is a float specifying the amount to scale \n");
        printf("             all spectra before alignment (default: 1 (e.g. 0.5 = sqrt))\n");
	printf("	-u   where \"baseline\" is an integer specifying baseline picking \n");
        printf("             sensitivity to subtract (default: 50 (max=100) [greater = more])\n");
        printf("       	-r   specifies that spectra are to converted to relative intensities\n");
        printf("       	-p   specifies that spectra will be scaled to remove negative \n");
	printf("             intenisities\n");
        printf("       	-a   outputs average spectrum to file\n");
        printf("       	-k   pick peaks using specified parameters \"cutoff\", \"window\",\n");
        printf("             \"iterations\" to be outputted to \"peak_file\".\n");
        printf("             NOTE: paramerters to be separated by \":\" without space\n");
	printf("\nFriendly example: \n");
	printf("specAlign-prepro -s /home/raw_data/example -t /home/processed/example \n");
	printf("           	 -i -b 200.0 -e 10000.0 -n 1 -r -p\n\n");


	exit(1);
}
