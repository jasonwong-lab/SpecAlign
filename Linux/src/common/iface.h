/***************************************************************************************************
File:		iface.h
Created:	2005/09/18
Author:		Jason W H Wong <jason.wong@chem.ox.ac.uk>
	
Purpose:	Implements the interface to access and process specAlign data objects.

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

#ifndef IFACE_H
#define IFACE_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <ctype.h>
#include <vector>
#include "data.h"
#include "FFTReal.h"

using namespace std;

class iface
{
public:
	iface();
	virtual ~iface();

	// Methods for data importing and exporting
	void readData(int, char * filename, int);
	int readFileInput(int, char * filename, bool first, bool open);
	int readFolderInput(char * foldername);
	int readAvg(char *filename, bool open);
	int exportSingleFile(char *filename, bool save);
	int exportMultiFile(char * foldername);
	int exportAvg(char *filename, bool save);
	int exportPeaks(char *filename, double cutoff, double win, double iter);
	int saveData(char *filename);
	int readData(char *filename);

	// OBSOLETE
	void readTarget(int, char * filename, int);
	void readClasses(int, char * filename, int);
	void writeResults();

	//Processing methods
	void normalizeInput();								// OBSOLETE
	void extract(vector <int> vals);					// OBSOLETE
	void normalizeSpec(int start, int end);				// Normalise the total intenisty within a
														// given range for all spectra
	vector <double> sumSpec();							// Sums all spectra
	vector <int> avgPeakList(double cutoff, int window);	// NOT DONE: Outputs a peak list that is 
															// common to all
	void alignPeaks(int margin, double cutoff, double window,int peakIter, double low, double up);
															// Aligns peaks by peak picking method
	void convertToRelative();								// Converts the intensity of all spectra to
															// relative intenisty (should be in DATA.h)
	void pickPeaks(double cutoff, int window, int iters, bool clr);		// picks peaks for all spectra
	void iterPeaks(int window);								// OBSOLETE
	void clearPeaks(bool manual);							// Clear all peaks
	void binSpectra(double binsize);						// Bin spectra
	int removeSpec(int i);									// Deletes a given spectrum
	int moveSpec(int oldp, int newp);						// Moves a spectrum put or down a list
	void smoothSpec(int pass);								// Applies SG smoothing
	void mergeAvg();										// Merge the new average spectrum with exisiting
	void scale(double factor);								// Scales spectra
	void subtractBase(int subVal);							// Subtracts baseline
	void addPeak(double mz);								// Adds a peak manually
	void removePeaks(double minmz, double maxmz);			// Remove all peaks in a given range
	void scalePositive();									// Scale datasets so that there are no negative values
	void removeNegative();									// Makes all negative values = 0
	int combineSpec(vector <int> samp);						// Generates average spectrum for a give list of samples
	int subtractSpec(int spec1, int spec2);					// Subtracts two spectrum, this subtracted spectrum is
															// then added to the top of the list
	int subtractAvgSpec(int spec1);							// Subtracts given spectrum to average
	void genCorr(int win);									// OBSOLETE
	void cropSpectra(int startPos, int endPos);

	vector <double> InterpolXScale(vector <double> *inScale, vector <double> *input,int orgsize);
															// Rescale input data to match x axis values of loaded data
	void convertPrevXScale(vector<double> inScale);
	vector <double> convertCurXScale(vector <double> *inScale, vector <double> *input);
	vector <double> convertBothXScale(vector <double> *inScale, vector <double> *input);

	void rafftAlign(double low, double up, float scale,int shift,int lookahead);		// RAFFT alignment
	void fftRecurAlign(int spec, int start, int end, int shift, int lookahead);			// recursive routine for RAFFT
	void preshiftSpec(int win, int segs, double low, double up);						// OBSOLETE
	void pafftAlign(int segs, double low, double up, float scale, int shift);			// PAFFT alignment
	vector<int> fragmentate(int spec, int segs, int start, int end);					// Fragmentate spectrum for PAFFT

	int findMidMin(int spec, int start, int end);			// finds approximate mid point to split segments for RAFFT recursion
	void doShift(int spec, int start, int end, int lag);	// Preforms the actual shifting for RAFFT
	int fftCorr(int spec1, int spec2, int start, int end, int shift);					// Calculated optimal shift value

	vector <double> calInsert(int inspos, int lastp, int nextp, int numIns, int samp);	// Determine points to insert for alignment

	void writeToBackup();									// Stores dataset for revert
	void copyFromBackup();									// Reverts to initial unprocessed data

	void writeToLast(int type);								// Stores dataset for undo
	int copyFromLast();										// Actually copies data back to perform the undo

	void printSpec(vector <double> spec);					// OBSOLETE
	void printSpec(vector <int> spec);						// OBSOLETE

	int getXIndex(double x);								// Get x axis index of value at x
	void reverseSpec();										// OBSOLETE

	void smooth(int start, int end, int in);				// OBSOLETE

	// OBSOLETE
	double cal_dist(int spec1, int spec2);							
	double cal_sca(int spec1, int spec2);
	double cal_partition_sca(int spec1, int spec2, int psize);
	double cal_si(int spec1, int spec2);
	double cal_R(int spec1, int spec2);

	double cal_sca(int spec, int start, int end, int shift);
	double cal_dist(int spec, int start, int end, int shift);
	double cal_R(int spec1,int spec2, int start, int end, int shift);
	double cal_cov(int spec1, int spec2, int start, int end);

	vector <data> train;									// holds dataset
	vector <data> test;										// OBSOLETE

	vector <data> last;										// holds dataset before last process (for undo)
	vector <data> backup;									// holds initial dataset (for revert)

	data avg;												// holds average/reference spectrum
	data avg_last;											// holds last average spectrum (for undo)
	data avg_backup;										// holds initial average spectrum (for revert)
	vector <double> mz_val;									// holds x axis values
	vector <double> last_mz_val;							// like as above
	vector <double> backup_mz_val;							// like as above

	// predefined objects to perform FFT. Needed to be global due to size.
	FFTReal::flt_t in1[131072];
	FFTReal::flt_t in2[131072];
	FFTReal::flt_t out1[131072];
	FFTReal::flt_t out2[131072];

	int lastType; // 0 = normaliseTIC, 1 = Relative, 2 = Peak Align, 3 = Crop, 4 = remove, 5 = move, 6 = bin

	char* itoa(int val, int base);
};
#endif

