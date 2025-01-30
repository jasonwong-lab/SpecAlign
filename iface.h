/**
 * Class iface
 * Author: Jason Wong
 *
 * Description: Objects of this class interfaces the data and the user interface
 *				by providing methods to manipulate all data objects simultaneously.
 *				Note that more data processing methods are actually individually
 *				implemented in the DATA class and this IFACE class is used to actually
 *				call the processes on all DATA objects.
 */

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
#include "daub.h"

using namespace std;

class GreaterThanPIntn{
public:
	bool operator()(const mass &a, const mass &b)
	{return a.intn > b.intn;}
};

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
	int exportPeaks(char *filename, double cutoff, double win, double height);
	int exportPeakAreas(char *filename, float percent);
	int exportModeledSpectra(char *filename);

	int saveData(char *filename);
	int readData(char *filename);

	int exportSingleFileRot(char *filename, bool save);

	// OBSOLETE
	void readTarget(int, char * filename, int);
	void readClasses(int, char * filename, int);
	void writeResults();

	//Processing methods
	void normalizeInput();								// OBSOLETE
	void extract(vector <int> &vals);					// OBSOLETE
	void normalizeSpec(int start, int end);				// Normalise the total intenisty within a
														// given range for all spectra
	vector <double> sumSpec();							// Sums all spectra
	vector <int> avgPeakList(double cutoff, int window);	// NOT DONE: Outputs a peak list that is 
															// common to all
	void alignPeaks(int margin, double cutoff, int window,double height, double low, double up);
															// Aligns peaks by peak picking method
	void convertToRelative();								// Converts the intensity of all spectra to
															// relative intenisty (should be in DATA.h)
	void pickPeaks(double cutoff, int window, double height, bool clr);		// picks peaks for all spectra
	void iterPeaks(int window);								// OBSOLETE
	void clearPeaks(bool manual);							// Clear all peaks
	void binSpectra(double binsize);						// Bin spectra
	int removeSpec(int i);									// Deletes a given spectrum
	int moveSpec(int oldp, int newp);						// Moves a spectrum put or down a list
	void smoothSpec(int pass);								// Applies SG smoothing
	void mergeAvg();										// Merge the new average spectrum with exisiting
	void scale(double factor);								// Scales spectra
	void subtractBase();									// Subtracts baseline
	void calBase(int subVal);
	void addPeak(double mz, bool manual);								// Adds a peak manually
	void removePeaks(double minmz, double maxmz);			// Remove all peaks in a given range
	void scalePositive();									// Scale datasets so that there are no negative values
	void removeNegative();									// Makes all negative values = 0
	int combineSpec(vector <int> samp);						// Generates average spectrum for a give list of samples
	int subtractSpec(int spec1, int spec2);					// Subtracts two spectrum, this subtracted spectrum is
															// then added to the top of the list
	int subtractAvgSpec(int spec1);							// Subtracts given spectrum to average
	void genCorr(int win);									// OBSOLETE

	vector <double> InterpolXScale(vector <double> &inScale, vector <double> &input,int orgsize);
															// Rescale input data to match x axis values of loaded data
	void convertPrevXScale(vector<double> &inScale);
	vector <double> convertCurXScale(vector <double> &inScale, vector <double> &input);
	vector <double> convertBothXScale(vector <double> &inScale, vector <double> &input);

	void rafftAlign(double low, double up, float scale,int shift,int lookahead, int ref);		// RAFFT alignment
	void fftRecurAlign(int spec, int start, int end, int shift, int lookahead);			// recursive routine for RAFFT
	void preshiftSpec(int win, int segs, double low, double up);						// OBSOLETE
	void pafftAlign(int segs, double low, double up, float scale, int shift, int ref);			// PAFFT alignment
	vector<int> fragmentate(int spec, int segs, int start, int end);					// Fragmentate spectrum for PAFFT

	int findMidMin(int spec, int start, int end);			// finds approximate mid point to split segments for RAFFT recursion
	void doShift(int spec, int start, int end, int lag);	// Preforms the actual shifting for RAFFT
	int fftCorr(int spec1, int spec2, int start, int end, int shift);					// Calculated optimal shift value
	double fftCorr(vector <double> *input1, vector <double> *input2, int shift);
	double corrcoef(vector <double> *input1, vector <double> *input2);

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

	void modelPeaks(int model, int base_val);
	void cropSpec(int xmin, int xmax);
	void waveDenoise(double sens, int recurs);

	int evaluateDataset(char *filename, int xmin, int xmax, int segments);

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

//	Daubechies waveOp;
};
#endif

