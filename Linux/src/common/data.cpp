/******************************************************************************************************
File:		data.cpp
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

#include "data.h"
#include <stdlib.h>
#include <math.h>

//
// Standard Constructor
//
data::data(){
	cls = 0;
	smoothed = false;
	avgOf = -1;
	lineType = 0;
}

//
// Destructor
//
data::~data(){

}

//
// Standard copy constructor
//
data& data::operator=(const data &o )
{
	if (this == &o)
		return *this;
	input = o.input;
	peaks = o.peaks;
	trough = o.trough;
	mz_val=o.mz_val;
	name = o.name;
	cls = o.cls;
	desc = o.desc;
	pos = o.pos;
	return *this;
}

double data::getMaxY(int xmin, int xmax)
{
	double maxy = 0;
	for (unsigned int i = xmin; i < xmax; ++i){
		if (input.at(i) > maxy){
			maxy = input.at(i);
		}
	}
	return maxy;
}

double data::getMinY(int xmin, int xmax)
{
	double miny = 99999999;
	for (unsigned int i = xmin; i < xmax; ++i){
		if (input.at(i)< miny){
			miny = input.at(i);
		}
	}
	if (miny > 0)
		miny = 0;
	return miny;
}

int data::getMinYpos(int xmin, int xmax)
{
	if (xmin > xmax)
		return -1;
	double miny = 99999999;
	int pos = -1;
	for (unsigned int i = xmin; i < xmax; ++i){
		if (input.at(i)< miny){
			miny = input.at(i);
			pos = i;
		}
	}
	return pos;
}

void data::pickPeaks(double cutoff, int window, int iters)
{
	if (peaks.size() != 0){
		for (unsigned int i = 0; i < peaks.size(); ++i){
			peaks.at(i).intn = input.at(peaks.at(i).pos);
		}
		return;
	}
	m_cut = cutoff;
	m_win = window;
	m_iter = iters;;
	int r = (int)(mz_val->size()*0.01);
	if (input.empty()){
		return;
	}
	bool peak, tro;
	double max = 0;
	double tmax = 0;
	double avg = 0;
	double min = 9999;
	mass m;
	register unsigned int pos, j;
	peaks.clear();
	double relCut=cutoff*getMaxY(0,input.size());
	for (int i = 0; i < input.size()-(int)floor((double)(window)/2);++i){   // calculates average for spectrum
		pos = i+(int)floor((double)(window)/2);
		peak = true;
		//tro = true;
		j = 0;
		//double start = input[pos];
		while (j <=(int)floor((double)(window)/2)-1 && peak){
			if (input[pos-j-1] >= input[pos-j] || input[pos+j] <= input[pos+j+1]){
				peak = false;
			}
			++j;
		}
		if (peak && input[pos] > relCut /*&& input[pos] > avg*/){
			m.mz = mz_val->at(pos);
			m.intn = input.at(pos);
			m.pos = pos;
			peaks.push_back(m);	
		}
	}
	for (int i = 1; i < iters; ++i){
		peaksFromPeaks();
	}
	for (unsigned int i = 0; i < manualPeaks.size(); ++i){
		addPeak(manualPeaks.at(i));
	}
}

void data::peaksFromPeaks()
{
	vector <mass> newPeaks;
	if (peaks.size() > 1){
		for (unsigned int i = 0 ; i < peaks.size(); ++i){
			if (i == 0){
				if (peaks.at(0).intn > peaks.at(1).intn){
					newPeaks.push_back(peaks.at(0));
				}
			}
			else if (i == peaks.size()-1){
				if (peaks.at(i).intn > peaks.at(i-1).intn){
					newPeaks.push_back(peaks.at(i));
				}
			}
			else{
				if (peaks.at(i).intn > peaks.at(i-1).intn || peaks.at(i).intn > peaks.at(i+1).intn){
					newPeaks.push_back(peaks.at(i));
				}
			}
		}
	}
	peaks = newPeaks;
}

void data::troughFromTrough()
{
	vector <mass> newtrough;
	for (unsigned int i = 0 ; i < trough.size(); ++i){
		if (i == 0){
			if (trough.at(0).intn > trough.at(1).intn){
				newtrough.push_back(trough.at(0));
			}
		}
		else if (i == trough.size()-1){
			if (trough.at(i).intn > trough.at(i-1).intn){
				newtrough.push_back(trough.at(i));
			}
		}
		else{
			if (trough.at(i).intn > trough.at(i-1).intn && trough.at(i).intn > trough.at(i+1).intn){
				newtrough.push_back(trough.at(i));
			}
		}
	}
	trough = newtrough;
}

void data::reverseInput()
{
	reverse(input.begin(), input.end());
}

vector <double> data::binData(double binsize)
{
	int j = 0;
	vector <double> newInput;
	vector <double> newMZ;
	double curVal = 0;
	double cur = mz_val->front();
	double end = mz_val->back();
	for (unsigned int i =0; i < mz_val->size();){
		j = 0;
		curVal = 0;
		while (i+j < mz_val->size() && mz_val->at(i+j) <= mz_val->at(i)+binsize){
			curVal+=input.at(i+j);
			++j;
		}
		newMZ.push_back(mz_val->at(i));
		i = i+j;
		newInput.push_back(curVal);
	}
	input = newInput;
	return newMZ;
}

void data::SGsmooth(int pass)
{
	//static const int l_filter[] = {-2,3,6,7,6,3,-2};
	//static const int m_filter[] = {-21,-6,7,18,27,34,39,42,43,42,39,34,27,18,7,-6,-21};
	//static const int h_filter[] = {-253,-138,-33,62,147,222,287,322,387,422,447,462,467,
	//				         462,447,422,387,322,287,222,147,62,-33,-138-253};
	int filter[50];
	int h;
	int norm;
	if (pass <= 5){
		const int l_filter[] = {-2,3,6,7,6,3,-2};
		h = 3;
		norm = l_norm;
		for (unsigned int i = 0; i < h*2+1; ++i)
			filter[i] = l_filter[i];
	}
	else if (pass > 5 && pass <= 10){
		const int m_filter[] = {-21,-6,7,18,27,34,39,42,43,42,39,34,27,18,7,-6,-21};
		h = 8;
		norm = m_norm;
		for (unsigned int i = 0; i < h*2+1; ++i)
			filter[i] = m_filter[i];
	}
	else{
		const int h_filter[] = {-253,-138,-33,62,147,222,287,322,387,422,447,462,467,
					         462,447,422,387,322,287,222,147,62,-33,-138,-253};
		h = 12;
		norm = h_norm;
		for (int i = 0; i < h*2+1; ++i)
			filter[i] = h_filter[i];
	}
	vector <double> sInput;
	for (unsigned int i = 0; i < h; ++i){
		sInput.push_back(input.at(i));
	}
	double sum = 0;
	for (unsigned int i = h; i < input.size()-h; ++i){
		sum = 0;
		for (int j = -h; j < h+1; ++j){
			sum += filter[j+h]*(input.at(i+j));
		}
		sInput.push_back(sum/norm);
	}
	for (unsigned int i = 0; i < h; ++i){
		sInput.push_back(input.at(input.size()-h));
	}
	input = sInput;		
	smoothed = true;
}

void data::scale(double factor)
{
	if (factor != 0){
		for (unsigned int i = 0; i < input.size(); ++i){
			if (input.at(i) > 0)
				input.at(i) = pow(input.at(i),factor);
		}
	}
}

void data::subtractBase(int subVal)
{
	vector <double> base;
	double sub = subVal*0.01*0.25;
	sub=fabs(sub-0.25);
	int r = (int)(mz_val->size()*sub);
	if (r<10)
		r = 10;
	double avg = 0;
	for (unsigned int k = 0; k < r && k < input.size();++k){
		avg += input[k];
	}
	avg /= r;
	for (int i = 0; i < input.size();++i){
		if (i!= 0){
			if ((i - r) >= 0 && (i+r) < input.size()){
				avg*= (r*2);
				avg -= input[i-r];
				avg += input[i+r];
				avg /= (r*2);
			}
			else if ((i - r) >= 0 && (i + r) >= input.size()){
				avg*=((r*2)-(i+r-input.size()));
				avg -= input[i-r];
				avg /= (((r*2)-(i+r-input.size()))-1);
			}
			else if ((i - r) < 0 && (i+r) < input.size()){
				avg*=(i+r-1);
				avg += input[i+r];
				avg /= (i+r);
			}
		}
		base.push_back(avg);
		//if (avg > 0.5)
		//	int a = 0;
	}
	for (unsigned int i = 0; i < base.size(); ++i){
		input.at(i)-=base.at(i);
		if (input.at(i) < 0)
			input.at(i) = 0.0000001;
	}
}

void data::addPeak(int x)
{
	mass m;
	if (x+25>=input.size() || x-25<0){
		m.intn = input.at(x);
		m.mz = mz_val->at(x);
		m.pos = x;
	}
	else{
		double maxR = (double) input.at(x);
		int maxRP = x;
		double maxL = (double) input.at(x);
		int maxLP = x;
		for (unsigned int j = 1; j < 26 /*&& input.at(x+j) >= input.at(x+j-1)*/; ++j){
			if (input.at(x+j)> maxR){
				maxR = input.at(x+j);
				maxRP = x+j;
			}
		}
		for (unsigned int j = 1; j < 26 /*&& input.at(x-j) >= input.at(x-j+1)*/; ++j){
			if (input.at(x-j)> maxL){
				maxL = input.at(x-j);
				maxLP = x-j;
			}
		}
		if (maxR >= maxL){
			m.intn = input.at(maxRP);
			m.mz = mz_val->at(maxRP);
			m.pos = maxRP;
		}
		else{
			m.intn = input.at(maxLP);
			m.mz = mz_val->at(maxLP);
			m.pos = maxLP;
		}
		if (m.pos == x+25 || m.pos == x-25)		// no peak found
			m.intn = 0;	
	}
	if (peaks.empty()){
		peaks.push_back(m);
	}
	else{
		int i = 0;
		while (i < peaks.size() && peaks.at(i).mz < m.mz){
			++i;
		}
		if (i == 0){
			peaks.insert(peaks.begin()+i,m);
		}
		else if (peaks.at(i-1).mz != m.mz){
			peaks.insert(peaks.begin()+i,m);
		}
		
	}
}

void data::removePeaks(double minx, double maxx)
{
	if (peaks.size() == 0)
		return;
	int minpos = 0;
	while (minpos < peaks.size()&& peaks.at(minpos).mz < minx){
		++minpos;
	}
	int maxpos = minpos;
	while (maxpos < peaks.size()&& peaks.at(maxpos).mz < maxx){
		++maxpos;
	}
	for (int i = minpos; i < maxpos; ++i){
		peaks.erase(peaks.begin()+minpos);
	}
	for (unsigned int i = 0; i < manualPeaks.size(); ++i){
		if (manualPeaks.at(i) >= minpos && manualPeaks.at(i) <= maxpos){
			manualPeaks.erase(manualPeaks.begin()+i);
		}
	}
}

void data::addTrough(int start, int end)
{
	mass res;
	if (input.size() == 0 || start < 0 || end > input.size())
		return;
	double min = 9999999;
	int minpos = 0;
	for (int i = start; i < end; ++i){
		if (input.at(i) < min){
			min = input.at(i);
			minpos = i;
		}
	}
	res.intn = min;
	res.mz = mz_val->at(minpos);
	res.pos = minpos;
	if (trough.size() ==  0){
		trough.push_back(res);
	}
	else{
		int j = 0;
		while (j < trough.size() && res.mz < trough.at(j).mz){
			++j;
		}
		if (j == 0)
			trough.insert(trough.begin()+j,res);
		else if (trough.at(j-1).mz != res.mz)
			trough.insert(trough.begin()+j,res);
	}
}

int data::findTrough(int start, int end)
{
	if (input.size() == 0 || start < 0 || end > input.size())
		return -1;
	double min = 9999999;
	int minpos = 0;
	for (int i = start; i < end; ++i){
		if (input.at(i) <= min){
			min = input.at(i);
			minpos = i;
		}
	}
	return minpos;
}

void data::scalePositive()
{
	double specMin = 9999999;
	for (unsigned int i = 0; i < input.size(); ++i){
		if (input.at(i) < specMin){
			specMin = input.at(i);
		}
	}
	if (specMin < 0){
		specMin = fabs(specMin);
		for (unsigned int i = 0; i < input.size(); ++i){
			input.at(i)+=specMin;
		}
	}
}

void data::removeNegative()
{
	for (unsigned int i = 0; i < input.size(); ++i){
		if (input.at(i) < 0){
			input.at(i) = 0.00000000001;			// not set to 0 due to drawing problem
		}
	}
}

double data::getAvgIntensity(int start, int end)
{
	if (start < 0 || end > input.size() || end <= start)
		return -1;
	double avg = 0;
	for (unsigned int i = start; i < end; ++i){
		avg+=input.at(i);
	}
	return avg/(end-start);
}

void data::peaksAt(vector <mass> *pLoc)
{
	peaks.clear();
	for (unsigned int i = 0; i < pLoc->size(); ++i){
		addPeak(pLoc->at(i).pos);
	}
}

