/**************************************************
*	Fichier : FastSymmlet8.C
*	Format : ANSI C++
*
* (c) 1998-2002 Daniel Lemire, Ph.D.
* http://www.ondelette.com/ - wavelet forum - forum sur les ondelettes
*
* Last modified on March 6th 1998
* Dernière modification le 6 mars 1998
*
* Free for any use, but this copyright notice must remain.
* Provide as is. Use at your risks.
*
* Gratuit pour toute utilisation, mais préservez cette mention.
* Sans garantie aucune, utilisez ?vos risques.
*
* C++ implementation of the Symmlet8 wavelet.
* See end of the file for usage.
*
* Cette classe utilise les ondelettes Symmlet8. Son
* utilisation est très simple (voir la fin du fichier
* pour un exemple). Elle suppose des conditions périodiques
* aux bords.
*
***************************************************/
#include "stdafx.h"

#include "FastSymmlet8.h"
#include <algorithm>

class GreaterThanS{
public:
	bool operator()(const double &a, const double &b)
	{return a > b;}
};

	  static const float scale[8] = {0.0322231006040782f,
	                                -0.0126039672622638f,
	                                -0.0992195435769564f,
	                                0.297857795605605f,
	                                0.803738751805386f,
	                                0.497618667632563f,
	                                -0.0296355276459604f,
	                                -0.0757657147893567f};

	  static const float wavelet[8] = {0.0757657147893567f,
	  				-0.0296355276459604f,
	  				-0.497618667632563f,
	  				0.803738751805386f,
	  				-0.297857795605605f,
	  				-0.0992195435769564f,
	  				0.0126039672622638f,
	  				0.0322231006040782f};

void FastSymmlet8::transform(vector <double> &input,int length, int last) {
    vector <double> ans;
	ans.assign(last,0);
    int half=last/2;
    for(int k=0;k<half-3;k++) {
          ans[k+half]=input[(2*k+0)]*wavelet[0]+input[(2*k+1)]*wavelet[1]+input[(2*k+2)]*wavelet[2]+input[(2*k+3)]*wavelet[3]+input[(2*k+4)]*wavelet[4]+input[(2*k+5)]*wavelet[5]+input[(2*k+6)]*wavelet[6]+input[(2*k+7)]*wavelet[7];
          ans[k]=input[(2*k+0)]*scale[0]+input[(2*k+1)]*scale[1]+input[(2*k+2)]*scale[2]+input[(2*k+3)]*scale[3]+input[(2*k+4)]*scale[4]+input[(2*k+5)]*scale[5]+input[(2*k+6)]*scale[6]+input[(2*k+7)]*scale[7];
	}
    ans[last-3]=input[last-6]*wavelet[0]+input[last-5]*wavelet[1]+input[last-4]*wavelet[2]+input[last-3]*wavelet[3]+input[last-2]*wavelet[4]+input[last-1]*wavelet[5]+input[0]*wavelet[6]+input[1]*wavelet[7];
    ans[half-3]=input[last-6]*scale[0]+input[last-5]*scale[1]+input[last-4]*scale[2]+input[last-3]*scale[3]+input[last-2]*scale[4]+input[last-1]*scale[5]+input[0]*scale[6]+input[1]*scale[7];
    ans[last-2]=input[last-4]*wavelet[0]+input[last-3]*wavelet[1]+input[last-2]*wavelet[2]+input[last-1]*wavelet[3]+input[0]*wavelet[4]+input[1]*wavelet[5]+input[2]*wavelet[6]+input[3]*wavelet[7];
    ans[half-2]=input[last-4]*scale[0]+input[last-3]*scale[1]+input[last-2]*scale[2]+input[last-1]*scale[3]+input[0]*scale[4]+input[1]*scale[5]+input[2]*scale[6]+input[3]*scale[7];
    ans[last-1]=input[last-2]*wavelet[0]+input[last-1]*wavelet[1]+input[0]*wavelet[2]+input[1]*wavelet[3]+input[2]*wavelet[4]+input[3]*wavelet[5]+input[4]*wavelet[6]+input[5]*wavelet[7];
    ans[half-1]=input[last-2]*scale[0]+input[last-1]*scale[1]+input[0]*scale[2]+input[1]*scale[3]+input[2]*scale[4]+input[3]*scale[5]+input[4]*scale[6]+input[5]*scale[7];
	for(int k=0;k<last;k++) {
		input[k]=ans[k];
	}
}

void FastSymmlet8::transform (vector <double> &input,int length) {
	int last;
    for (last=length;last>8;last/=2) {
      transform(input,length,last);
    }
}

void FastSymmlet8::invTransform(vector <double> &wav, int length, int last) {
    int ResultingLength=2*last;
    vector <double> ans;
	ans.assign(ResultingLength,0);
    for(int k=0;2*k+7<ResultingLength;k++) {
          ans[(2*k+7)]+=scale[7]*wav[k]+wavelet[7]*wav[k+last] ;
          ans[(2*k+6)]+=scale[6]*wav[k]+wavelet[6]*wav[k+last] ;
          ans[(2*k+5)]+=scale[5]*wav[k]+wavelet[5]*wav[k+last] ;
          ans[(2*k+4)]+=scale[4]*wav[k]+wavelet[4]*wav[k+last] ;
          ans[(2*k+3)]+=scale[3]*wav[k]+wavelet[3]*wav[k+last] ;
          ans[(2*k+2)]+=scale[2]*wav[k]+wavelet[2]*wav[k+last] ;
          ans[(2*k+1)]+=scale[1]*wav[k]+wavelet[1]*wav[k+last] ;
          ans[(2*k+0)]+=scale[0]*wav[k]+wavelet[0]*wav[k+last] ;
    }
    ans[ResultingLength-6]+=scale[0]*wav[last-3]+wavelet[0]*wav[ResultingLength-3] ;
    ans[ResultingLength-5]+=scale[1]*wav[last-3]+wavelet[1]*wav[ResultingLength-3] ;
    ans[ResultingLength-4]+=scale[2]*wav[last-3]+wavelet[2]*wav[ResultingLength-3] ;
    ans[ResultingLength-3]+=scale[3]*wav[last-3]+wavelet[3]*wav[ResultingLength-3] ;
    ans[ResultingLength-2]+=scale[4]*wav[last-3]+wavelet[4]*wav[ResultingLength-3] ;
    ans[ResultingLength-1]+=scale[5]*wav[last-3]+wavelet[5]*wav[ResultingLength-3] ;
    ans[0]+=scale[6]*wav[last-3]+wavelet[6]*wav[ResultingLength-3] ;
    ans[1]+=scale[7]*wav[last-3]+wavelet[7]*wav[ResultingLength-3] ;
    ans[ResultingLength-4]+=scale[0]*wav[last-2]+wavelet[0]*wav[ResultingLength-2] ;
    ans[ResultingLength-3]+=scale[1]*wav[last-2]+wavelet[1]*wav[ResultingLength-2] ;
    ans[ResultingLength-2]+=scale[2]*wav[last-2]+wavelet[2]*wav[ResultingLength-2] ;
    ans[ResultingLength-1]+=scale[3]*wav[last-2]+wavelet[3]*wav[ResultingLength-2] ;
    ans[0]+=scale[4]*wav[last-2]+wavelet[4]*wav[ResultingLength-2] ;
    ans[1]+=scale[5]*wav[last-2]+wavelet[5]*wav[ResultingLength-2] ;
    ans[2]+=scale[6]*wav[last-2]+wavelet[6]*wav[ResultingLength-2] ;
    ans[3]+=scale[7]*wav[last-2]+wavelet[7]*wav[ResultingLength-2] ;
    ans[ResultingLength-2]+=scale[0]*wav[last-1]+wavelet[0]*wav[ResultingLength-1] ;
    ans[ResultingLength-1]+=scale[1]*wav[last-1]+wavelet[1]*wav[ResultingLength-1] ;
    ans[0]+=scale[2]*wav[last-1]+wavelet[2]*wav[ResultingLength-1] ;
    ans[1]+=scale[3]*wav[last-1]+wavelet[3]*wav[ResultingLength-1] ;
    ans[2]+=scale[4]*wav[last-1]+wavelet[4]*wav[ResultingLength-1] ;
    ans[3]+=scale[5]*wav[last-1]+wavelet[5]*wav[ResultingLength-1] ;
    ans[4]+=scale[6]*wav[last-1]+wavelet[6]*wav[ResultingLength-1] ;
    ans[5]+=scale[7]*wav[last-1]+wavelet[7]*wav[ResultingLength-1] ;
	for(int k=0;k<ResultingLength;k++) {
		wav[k]=ans[k];
	}
}

void FastSymmlet8::invTransform (vector <double> &wav, int length) {
    int last;
    for (last=8;2*last<=length;last*=2) {
      invTransform(wav, length,last);
    }
}

void FastSymmlet8::expand(vector <double> &wav, int length)
{
	int last=length/2;
	int ResultingLength=2*last;
    vector <double> ans;
	ans.assign(ResultingLength,0);
    for(int k=0;2*k+7<ResultingLength;k++) {
          ans[(2*k+7)]+=scale[7]*wav[k];
          ans[(2*k+6)]+=scale[6]*wav[k];
          ans[(2*k+5)]+=scale[5]*wav[k];
          ans[(2*k+4)]+=scale[4]*wav[k];
          ans[(2*k+3)]+=scale[3]*wav[k];
          ans[(2*k+2)]+=scale[2]*wav[k];
          ans[(2*k+1)]+=scale[1]*wav[k];
          ans[(2*k+0)]+=scale[0]*wav[k];
    }
    ans[ResultingLength-6]+=scale[0]*wav[last-3];
    ans[ResultingLength-5]+=scale[1]*wav[last-3];
    ans[ResultingLength-4]+=scale[2]*wav[last-3];
    ans[ResultingLength-3]+=scale[3]*wav[last-3];
    ans[ResultingLength-2]+=scale[4]*wav[last-3];
    ans[ResultingLength-1]+=scale[5]*wav[last-3];
    ans[0]+=scale[6]*wav[last-3];
    ans[1]+=scale[7]*wav[last-3];
    ans[ResultingLength-4]+=scale[0]*wav[last-2];
    ans[ResultingLength-3]+=scale[1]*wav[last-2];
    ans[ResultingLength-2]+=scale[2]*wav[last-2];
    ans[ResultingLength-1]+=scale[3]*wav[last-2];
    ans[0]+=scale[4]*wav[last-2];
    ans[1]+=scale[5]*wav[last-2];
    ans[2]+=scale[6]*wav[last-2];
    ans[3]+=scale[7]*wav[last-2];
    ans[ResultingLength-2]+=scale[0]*wav[last-1];
    ans[ResultingLength-1]+=scale[1]*wav[last-1];
    ans[0]+=scale[2]*wav[last-1];
    ans[1]+=scale[3]*wav[last-1];
    ans[2]+=scale[4]*wav[last-1];
    ans[3]+=scale[5]*wav[last-1];
    ans[4]+=scale[6]*wav[last-1];
    ans[5]+=scale[7]*wav[last-1];
	for(int k=0;k<length;k++) {
		wav[k]=ans[k];
	}
}

void FastSymmlet8::threshold(vector <double> &wav,int diff,int bits, double &sens)
{
	int n = wav.size();
	vector<double> tmp;
//	double variance;
	vector<double> thres;
	for (unsigned int i = 0; i < n-floor(diff*0.5); ++i){
		tmp.push_back(fabs(wav.at(i)));
	}
	sort(tmp.begin(), tmp.end(), GreaterThanS());
	while (tmp.back() == 0)
		tmp.pop_back();
	
	if (tmp.size() < 2)
		return;
	double median = tmp.at((int)((tmp.size())*sens)-1);
	double t = sqrt(2*log((double)n))*median;
	for (int i = 0; i < n; ++i){
		double res = fabs(wav[i])-t;
		res = (res + fabs(res))*0.5;
		if (wav[i] > 0) wav[i] = 1;
		else if (wav[i] < 0) wav[i] = -1;
		wav[i] *= res;
	}
}



