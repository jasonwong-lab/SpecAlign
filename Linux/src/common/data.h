/******************************************************************************************************
File:		data.h
Created:	2005/09/18
Author:		Jason W H Wong <jason.wong@chem.ox.ac.uk>
	
Purpose:	Stores individual spectrum and contains various preprocessing methods
			for manipulating spectra.

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
*****************************************************************************************************/

#ifndef DATA_H
#define DATA_H

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

struct mass {				// used to store information about peaks.
	double mz;				// mz is the mz value or can be time in chromatographs
	double intn;
	int pos;
};

class data
{

public:

	data ();
	virtual ~data ();
	data& operator=(const data &o );

	double getMaxY(int xmin, int xmax);				// Gets max Y axis value in specified range
	double getMinY(int xmin, int xmax);				// Gets min Y axis value
	void pickPeaks(double cutoff, int window, int iters);		// Automated pick peaks
	void peaksFromPeaks();						// Picks peaks from peak list
	void troughFromTrough();					// OBSOLETE
	vector <double> binData(double binsize);			// Bins data and outputs new X axis range
	void SGsmooth(int pass);					// Performs Savitsky-Golay smoothing
	void scale(double factor);					// Scales data
	void subtractBase(int subVal);					// Subtracts baseline
	void addPeak(int x);						// Adds peak manually at position x
	void removePeaks(double minx, double maxx);			// Remove peaks in a given range
	void addTrough(int start, int end);				// OBSOLETE
	int findTrough(int start, int end);				// OBSOLETE
	double getAvgIntensity(int start, int end);			// Get average intensity in a given region
	int getMinYpos(int xmin, int xmax);				// Get position of min Y axis value.
	void peaksAt(vector<mass> *pLoc);

	void scalePositive();						// Scales data so that all x >= 0
	void removeNegative();						// Makes all negative values = 0

	void reverseInput();						// OBSOLETE

	vector <double> input;						// spectral data
	vector <double> *mz_val;					// pointer to common x axis values

	vector <int> manualPeaks;					// Stores a list of manually selected peaks
	vector <mass> peaks;						// Stores a list of auto detected peaks
	vector <mass> trough;						// OBSOLETE

	string name;							// name of data
	string desc;							// description of data
	int cls;										
	int pos;
	double m_cut;							// stores default peak picking values
	int m_win;
	int m_iter;
	int avgOf;							// If data is average spectrum, stores
									// that this is an average of how many samples
	int lineType;							// stores lineType to be plotted

private:
	bool smoothed;							// Whether SGsmooth has already been aligned
	static const int l_norm = 21;					// constants used for SG smoothing
	static const int m_norm = 323;
	static const int h_norm = 5175;


};

#endif

