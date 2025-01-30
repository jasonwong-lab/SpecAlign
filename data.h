/**
 * Class Data
 * Author: Jason Wong
 *
 * Description: Objects of this class stores the actually input data and also
 *				provides certain manipulation tools on each data file (spectrum
 *				or chromatorgraph).
 */

#ifndef DATA_H
#define DATA_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>

#include "daub.h"

using namespace std;

class GreaterThanInput{
public:
	bool operator()(const double &a, const double &b)
	{return a > b;}
};

struct mass {				// used to store information about peaks.
	double mz;				// mz is the mz value or can be time in chromatographs
	double intn;
	int pos;
	int type;				// 0 = Gaussian, 1 = Lorentzian, 2 = Voigt, 3 = Lognormal
	bool modelled;
	double a;
	double loc;
	double w;
	double r;
	double fit_err;
	int bbase;
	int ebase;
	int peak_num;
	double peak_min;
	double peak_corr;
};

struct pClust{
	vector <mass*> overlaps;
};

struct regModel{
	double alpha;
	double beta;
};

struct mydata{
   double x[5000];
   double y[5000];
   int overlaps;
   int type;
};

class data
{

public:

	data ();
	virtual ~data ();
	data& operator=(const data &o );

	double getMaxY(int xmin, int xmax);				// Gets max Y axis value in specified range
	double getMinY(int xmin, int xmax);				// Gets min Y axis value
	double getAvg(int xmin, int xmax);
	void pickPeaks(double cutoff, int window, double height);		// Automated pick peaks
	void peaksFromPeaks();							// Picks peaks from peak list
	void troughFromTrough();						// OBSOLETE
	vector <double> binData(double binsize);		// Bins data and outputs new X axis range
	void SGsmooth(int pass, vector<double> &rough);						// Performs Savitsky-Golay smoothing
	void scale(double factor);						// Scales data
	void calBase(int subVal);					// Subtracts baseline
	void subtractBase();
	int addPeak(int x, bool manual);							// Adds peak manually at position x
	void removePeaks(double minx, double maxx);		// Remove peaks in a given range
	void addTrough(int start, int end);				// OBSOLETE
	int findTrough(int start, int end);			// For alignment
	double getAvgIntensity(int start, int end);		// Get average intensity in a given region
	int getMinYpos(int xmin, int xmax);				// Get position of min Y axis value.
	void peaksAt(vector<mass> &pLoc);
	void scalePositive();							// Scales data so that all x >= 0
	void removeNegative();							// Makes all negative values = 0

	void cropSpec(int xmin, int xmax);

	bool checkModel(int type, mass &peak);
	double modelPeaks(int model, double ulim, double llim, bool reclust, bool debug=false);

	double modelOnAvg(data &avg, double ulim, bool debug=false);
	bool regressParams(char * filename1, char * filename2);
	bool analyticRegressParams(bool over);
	void findPeakTroughs();
	void checkPeakWidths();
	void calpeakNums();

	void genModelSpectrum();

	int simplifySpectrum();

	//void waveletDenoise(Daubechies &waveOp, double &sens, int recurs);
	void waveletDenoise(double &sens, int recurs);

	void reverseInput();							// OBSOLETE

	vector <double> input;							// inputs to train or test network
	vector <double> base;
	vector <double> *mz_val;						// pointer to common x axis values
	vector <double> input_model;
	vector <double> input_simp;
	

	vector <int> manualPeaks;						// Stores a list of manually selected peaks
	vector <mass> peaks;							// Stores a list of auto detected peaks
	vector <mass> trough;							// OBSOLETE

	CString name;									// name of data
	CString desc;									// description of data
	int cls;										
	int pos;
	double m_cut;									// stores default peak picking values
	int m_win;
	double m_height;
	int avgOf;										// If data is average spectrum, stores
													// that this is an average of how many samples
	int lineType;									// stores lineType to be plotted
	regModel reg;
	double TIC;

	double percentGPeaks;
	vector <double> full_corr;
	double avg_full_corr;
	double zero_corr;

	double norm_factor;

private:
	
	int clusterPeaks(bool reclust);



	void outputPeakParams(char * filename, vector <mass> &before, vector <mass> &after);

	//void gaussmodel(double *p, double *x, int m, int n, void *data);

	vector <pClust> cluster;
	vector <double> tmpInput;

	bool smoothed;									// Whether SGsmooth has already been aligned
	static const int l_norm = 21;					// constants used for SG smoothing
	static const int m_norm = 323;
	static const int h_norm = 5175;


};

#endif

