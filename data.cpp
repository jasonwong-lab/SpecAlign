#include "stdafx.h"

#include "data.h"
#include "ProgressBar.h"

#include "lm.h"
#include <deque>
#include "FastSymmlet8.h"

#define PI	 3.1415926535897932
#define M_2S2LN2 2.354820045030949382
#define DBL_MAX 1.7976931348623158e+308
#define SQ2PI 2.50662827463100050241577
#define sqr(x) ((x)*(x))


/* These are apparently not defined in MSVC */
#if !defined(M_PI)
#define M_PI 3.14159265358979323846
#endif
#if !defined(M_LN2)
#define M_LN2 0.69314718055994530942
#endif

/**
 * data.cpp
 * By Jason Wong
 *
 * The class implements objects that hold the inputs from a file and allows certain
 * processing functions. For description of methods see data.h
 */

static void voigtmodel(double *p, double *x, int m, int n, void *data)
{
	register int i;
	struct mydata *dptr;

	dptr=(struct mydata *)data; 
	register double gauss_dev = 0;
	register double lorenz_v = 0;
	register double gauss_v = 0;
	register double exp_v = 0;
	register int var = 0;
	for(i=0; i<n; i++) {
		var = 0;
		x[i] = 0;
		for (unsigned int j = 0; j < ceil((double)m/4); j++){
			if (dptr->type == 0){ //gauss
				exp_v = 0;
				gauss_dev = p[var+3]/M_2S2LN2;
				gauss_v = (1/(gauss_dev*sqrt(2*M_PI)))*exp(-(sqr(dptr->y[i]-p[var+2])/(2*sqr(gauss_dev))));
			}
			else if (dptr->type == 1){ // lorentz
				exp_v = 1;
				lorenz_v = (p[var+3]/(2*M_PI))/(sqr(dptr->y[i]-p[var+2])+sqr(p[var+3]/2));
			}
			else{				// voigt
				exp_v = exp(-fabs(p[var+1]));
				lorenz_v = (p[var+3]/(2*M_PI))/(sqr(dptr->y[i]-p[var+2])+sqr(p[var+3]/2));
				gauss_dev = p[var+3]/M_2S2LN2;
				gauss_v = (1/(gauss_dev*sqrt(2*M_PI)))*exp(-(sqr(dptr->y[i]-p[var+2])/(2*sqr(gauss_dev))));
			}
			x[i] += p[var]*((exp_v*lorenz_v)+((1-exp_v)*gauss_v));
			var+=4;
		}
		x[i]-=dptr->x[i];
    }
}

void lognormmodel(double *p, double *x, int m, int n, void *data)
{
	register int i;
	struct mydata *dptr;

	dptr=(struct mydata *)data; 

	register int vars = 0;
	register double top = 0;
	register double mid = 0;
	register double bottom = 0;
	register double exp_v = 0;
	for(i=0; i<n; ++i){
		x[i] = 0;
		vars = 0;
		for (unsigned int j = 0; j < m/4; ++j){
			if (dptr->y[i]<=p[vars+1]){
				x[i] += 0;
			}
			else{
				exp_v = exp(-fabs(p[vars+3]));
				top = sqr(log((dptr->y[i]-p[vars+1])/p[vars+2]));
				mid = 2.0*sqr(exp_v);
				bottom = exp_v*(dptr->y[i]-p[vars+1])*SQ2PI;
				x[i]+=p[vars]*(exp(-top/mid)/bottom);
			}
			vars+=4;
		}
		x[i]-=dptr->x[i];
	}
}

static double voigt(double &a, double &r, double &loc, double &w, double &x)
{
	if (a == 0)
		return 0;
	double lorenz_v = 0;
	double gauss_dev = 0;
	double gauss_v = 0;
	if (r != 0)
		lorenz_v = (fabs(w)/(2*M_PI))/(sqr(x-loc)+sqr(w/2));
	if (r != 1){
		gauss_dev = fabs(w)/M_2S2LN2;
		gauss_v = (1/(gauss_dev*sqrt(2*M_PI)))*exp(-(sqr(x-loc)/(2*sqr(gauss_dev))));
	}
	double res = ((r*lorenz_v)+((1-r)*gauss_v));
	return 	a*res;
}

static double lognorm(double &a, double &loc, double &r, double &w, double &x)
{
	if (a == 0)
		return 0;
	if (x <= loc)
		return 0;
	double exp_v = exp(-fabs(r));
	double top = sqr(log((x-loc)/w));
	double mid = 2.0*sqr(exp_v);
	double bottom = exp_v*(x-loc)*SQ2PI;
	double res =a*(exp(-top/mid)/bottom);
	return res;
}

static void expmodel(double *p, double *x, int m, int n, void *data)
{
	register int i;
	struct mydata *dptr;

	dptr=(struct mydata *)data; 
	for(i=0; i<n; ++i){
		x[i] = p[0]*exp(p[1]*dptr->x[i])-dptr->y[i];
	}
}

static double expfunc(double &alpha, double &beta, double &x)
{
	return alpha*exp(beta*x);
}

static double linfunc(double &alpha, double &beta, double &x)
{
	return alpha+beta*x;
}
//
// Standard Constructor
//
data::data(){
	cls = 0;
	smoothed = false;
	avgOf = -1;
	lineType = 0;
	reg.alpha = 25;
	reg.beta = 0.0001;
	percentGPeaks = 1;
	norm_factor = 0;
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
	base = o.base;
	manualPeaks = o.manualPeaks;
	//expected = o.expected;
	//classes = o.classes;
	peaks = o.peaks;
	trough = o.trough;
	mz_val=o.mz_val;
	name = o.name;
	cls = o.cls;
	desc = o.desc;
	pos = o.pos;
	reg = o.reg;
	percentGPeaks = o.percentGPeaks;
	m_cut = o.m_cut;									
	m_win = o.m_win;
	m_height = o.m_height;
	avgOf = o.avgOf;
	lineType = o.lineType;
	smoothed = o.smoothed;
	TIC = o.TIC;
	norm_factor = o.norm_factor;
	return *this;
}

double data::getMaxY(int xmin, int xmax)
{
	double maxy = *max_element(input.begin()+xmin,input.begin()+xmax);
	//for (unsigned int i = xmin; i < xmax; ++i){
	//	if (input.at(i) > maxy){
	//		maxy = input.at(i);
	//	}
	//}
	return maxy;
}

double data::getMinY(int xmin, int xmax)
{
	double miny = *min_element(input.begin()+xmin,input.begin()+xmax);
	//for (unsigned int i = xmin; i < xmax; ++i){
	//	if (input.at(i)< miny){
	//		miny = input.at(i);
	//	}
	//}
	if (miny > 0)
		miny = 0;
	return miny;
}

double data::getAvg(int xmin, int xmax)
{
	double sumval = 0;
	for (unsigned int i = xmin; i < xmax; ++i){
			sumval += input.at(i);
	}
	return sumval/(xmax-xmin);
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

void data::pickPeaks(double cutoff, int window, double height)
{
	if (peaks.size() != 0){
		for (unsigned int i = 0; i < peaks.size(); ++i){
			peaks.at(i).intn = input.at(peaks.at(i).pos);
		}
		return;
	}
	m_cut = cutoff;
	m_win = window;
	m_height = height;
	int r = (int)(mz_val->size()*0.01);
	if (input.empty()){
		return;
	}
	for (unsigned int i = 0; i < input.size(); ++i){
		input.at(i)*=10;
	}
	for (unsigned int i = 0; i < base.size(); ++i){
		base.at(i)*=10;
	}
	bool peak;
	double max = 0;
	double tmax = 0;
	double avg = 0;
	double min = 9999;
	mass m;
	register int pos, j;
	peaks.clear();
	register int left = 0;
	register int right = 0;
	int hwin = (int)floor(window*0.5);
	for (int i = 0; i < input.size();){   // calculates average for spectrum
		pos = i;
		peak = false;
		j = 0;
		double winMin = 0;
		left = 0;
		right = 0;
		while (pos-left-1 > 0 && (input[pos-left-1]) < (input[pos-left])*1.001){
			++left;
		}
		while (pos+right+1 < input.size() && (input[pos+right])*1.001 > (input[pos+right+1])){
			++right;
		}
		if (left > hwin && right > hwin){
			peak = true;
			winMin = min(input[pos-left]-base[pos-left],input[pos+right]-base[pos+right]);
		}
		double a = input[pos];
		double b = mz_val->at(pos-left);
		double c = mz_val->at(pos+right);
		double d = mz_val->at(pos);
		if (peak && input[pos] > (cutoff+1)*(base.at(pos)) && (input[pos]-base[pos]) > (winMin)*height ){
			m.mz = mz_val->at(pos);
			m.intn = input.at(pos)*0.1;
			m.pos = pos;
			m.a = 0;
			m.loc = 0;
			m.r = 0;
			m.w = 0;
			m.modelled = false;
			m.bbase = pos-left;
			m.ebase = pos+right;
			peaks.push_back(m);	
		}
		while (pos+right+1 < input.size() && input[pos+right] < input[pos+right+1])
				right++;
		i = pos+right;
//		d=mz_val->at(i);
		if (right == 0)
			i=i+1;
	}
	for (unsigned int i = 0; i < input.size(); ++i){
		input.at(i)*=0.1;
	}
	for (unsigned int i = 0; i < base.size(); ++i){
		base.at(i)*=0.1;
	}
	//for (int i = 1; i < iters; ++i){
	//	peaksFromPeaks();
	//}
	for (unsigned int i = 0; i < manualPeaks.size(); ++i){
		addPeak(manualPeaks.at(i), false);
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
	vector <double> newBase;
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
	base.clear();
	return newMZ;
}

void data::SGsmooth(int pass, vector<double> &rough)
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
		sInput.push_back(rough.at(i));
	}
	double sum = 0;
	for (unsigned int i = h; i < rough.size()-h; ++i){
		sum = 0;
		for (int j = -h; j < h+1; ++j){
			sum += filter[j+h]*(rough.at(i+j));
		}
		sInput.push_back(sum/norm);
	}
	for (unsigned int i = 0; i < h; ++i){
		sInput.push_back(rough.at(rough.size()-h));
	}
	rough = sInput;		
	smoothed = true;
}

void data::scale(double factor)
{
	if (factor != 0){
		for (unsigned int i = 0; i < input.size(); ++i){
			if (input.at(i) > 0)
				input.at(i) = pow(input.at(i),factor);
		}
		for (unsigned int i = 0; i < base.size(); ++i){
			if (base.at(i) > 0)
				base.at(i) = pow(base.at(i),factor);
		}
		for (unsigned int i = 0; i < peaks.size(); ++i){
			if (peaks.at(i).intn > 0)
				peaks.at(i).intn = pow(peaks.at(i).intn,factor);
			peaks.at(i).modelled = false;
			peaks.at(i).a = 0;
		}
	}
}

void data::calBase(int subVal)
{
	vector <double> tmp_input = input;
	double lastval = input.at(0);
	for (int i = 0; i < input.size();++i){
		bool peaked = false;
		for (unsigned int j =0; j < peaks.size() && !peaked; ++j){
			if (i >= peaks.at(j).bbase && i <= peaks.at(j).ebase){
				peaked = true;
				double m = (input.at(peaks.at(j).bbase)-input.at(peaks.at(j).ebase))/(mz_val->at(peaks.at(j).bbase)-mz_val->at(peaks.at(j).ebase));
				double b = input.at(peaks.at(j).bbase)-(m*mz_val->at(peaks.at(j).bbase));
				double n=(m*mz_val->at(i))+b;
				input.at(i)=n+((input.at(i)-n)*0.5);
			}
		}
		if (!peaked)
			lastval = input.at(i);
	}
	double minmin = getMinY(0,input.size());
	double oldmin = minmin;
	//minmin*=100;
	//if (minmin < 0){
	SGsmooth(13,input);
	for (unsigned int i =0; i < input.size(); ++i){
			input.at(i)-=minmin;
			input.at(i)*=10000;
	}
	base.clear();
	double sub = subVal*0.01;
//	sub=fabs(sub-0.25);
	int r = (int)((mz_val->size()/4)*sub);
	if (r<50)
		r = 50;
	double fullavg = 0;
	double movavg = 0;
	deque <double> avg;
	//for (unsigned int k = 0; k < r; ++k)
	avg.push_back(input.at(0));
	double maxstart = getAvg(0,min(r*2,input.size()-1));
	for (unsigned int k = 1; k < r && k < input.size();++k){
		if (input.at(k) < maxstart*2)
			avg.push_back(input.at(k));
	}
	double sumavg = 0;
	for (unsigned int k = 0; k < avg.size(); ++k){
		sumavg += avg.at(k);
	}
	movavg = sumavg/avg.size();
	double minval = getMinY(0,min(r*2,input.size()-1));
    fullavg = maxstart;
	bool above = false;
	for (int i = 0; i < input.size();++i){
		if (i!= 0){
			if ((i - r) >= 0 && (i+r) < input.size()){
				if (input[i+r] < fullavg*2 && movavg <= input[i]){
					movavg*=avg.size();
					movavg-=avg.front();
					avg.push_back(input[i+r]);
					avg.pop_front();
					movavg+=avg.back();
					movavg/=avg.size();
				}
				else if(movavg > input[i]){
					movavg*=avg.size();
					movavg-=avg.front();
					avg.push_back(input[i]*0.9);
					avg.pop_front();
					movavg+=avg.back();
					movavg/=avg.size();
					above = true;
				}

				fullavg*= (r*2);
				fullavg -= input[i-r];
				fullavg += input[i+r];
				fullavg /= (r*2);
			}
			else if ((i - r) >= 0 && (i + r) >= input.size()){
				if (avg.size() > 1 && movavg <= input[i]){
					movavg*=avg.size();
					movavg-=avg.front();
					avg.pop_front();
					movavg/=avg.size();
				}
				//else if (avg.size() > 1 &&  movavg > input[i]){
				//	deque <double>::iterator maxpos = max_element(avg.begin(),avg.end());
				//	movavg*=avg.size();
				//	movavg-=*maxpos;
				//	avg.erase(maxpos);
				//	movavg/=avg.size();
				//}
				else if (movavg > input[i]){
					movavg = input[i];
					
				}
				fullavg*=((r*2)-(i+r-input.size()));
				fullavg -= input[i-r];
				fullavg /= (((r*2)-(i+r-input.size()))-1);
			}
			else if ((i - r) < 0 && (i+r) < input.size()){
				if (input[i+r] < fullavg*2 &&movavg <= input[i]){
					movavg*=avg.size();
					movavg+=input[i+r];
					avg.push_back(input[i+r]);
					movavg/=avg.size();
				}
				//else if (input[i+r] < fullavg*3){
				//	movavg*=avg.size();
				//	avg.push_back(getMinY(0,i+r));
				//	movavg+=avg.back();
				//	movavg/=avg.size();
				//}
				else if(movavg > input[i]){
					movavg*=avg.size();
					avg.push_back(input[i]);
					movavg+=avg.back();
					movavg/=avg.size();
				}
				fullavg*=(i+r-1);
				fullavg += input[i+r];
				fullavg /= (i+r);
				//if (input[i+r] < minval)
				//	minval = input[i+r];
			}
		}
		if (!above)
			base.push_back(movavg);
		else{
			base.push_back((movavg+input[i]*2)*0.33333);
			above = false;
		}
	}
	//if (minmin < 0){
		for (unsigned int i =0; i < input.size(); ++i){
	//		input.at(i)*=0.0001;
			base.at(i)*=0.0001;
		//	input.at(i)+=minmin;
			base.at(i)+=minmin;
			if (oldmin > 0 && base.at(i) < 0)
				base.at(i)=0;
		}
	//}

	SGsmooth(13,base);
	SGsmooth(13,base);
	input = tmp_input;
	//for (unsigned int i = 0; i < r/2; ++i)
	//	base.push_back(movavg);
	//for (unsigned int i = 0; i < base.size(); ++i){
	//	input.at(i)-=base.at(i);
	//	if (input.at(i) < 0)
	//		input.at(i) = 0.0000001;
	//}
}

void data::subtractBase()
{
	if (base.empty())
		return;
	while (base.size() < input.size())
		base.push_back(input.back());
	while (base.size() > input.size())
		base.pop_back();
	for (unsigned int i = 0; i < input.size(); ++i){
		input.at(i)-=base.at(i);
	}
	base.clear();
}

int data::addPeak(int x, bool manual)
{
	mass m;
	m.a = 0;
	m.r = 0;
	m.loc = 0;
	m.w = 0;
	m.modelled = false;
	int low = x;
	int high = x;
	if (x >= input.size() || x < 0)
		return -1;
	double frac = 0.001;
	if (mz_val->at(x) < 100)
		frac = 0.01;
	while (low > 0 && mz_val->at(low) > mz_val->at(x)*(1-frac))
		low--;
	while (high < mz_val->size() && mz_val->at(high) < mz_val->at(x)*(1+frac))
		high++;
	if (high>=input.size() || low<0){
		m.intn = input.at(x);
		m.mz = mz_val->at(x);
		m.pos = x;
	}
	else{
		double maxR = (double) input.at(x);
		int maxRP = x;
		double maxL = (double) input.at(x);
		int maxLP = x;
		for (unsigned int j = 1; j < high-x /*&& input.at(x+j) >= input.at(x+j-1)*/; ++j){
			if (input.at(x+j)> maxR){
				maxR = input.at(x+j);
				maxRP = x+j;
			}
		}
		for (unsigned int j = 1; j < x-low /*&& input.at(x-j) >= input.at(x-j+1)*/; ++j){
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
//		if (manual && (m.pos == x+29 || m.pos == x-29)){		// no peak found
//			return 0;
//		}
//		else if (!manual && (m.pos == x+29 || m.pos == x-29)){
		if (m.pos == x+high || m.pos == x-low){
			m.intn = input.at(x);
			m.mz = mz_val->at(x);
			m.pos = x;
		}
	}

	if (peaks.empty()){
		peaks.push_back(m);
	}
	else{
		int i = 0;
		while (i < peaks.size() && peaks.at(i).mz <= m.mz){
			++i;
		}
		if (i == 0){
			peaks.insert(peaks.begin()+i,m);
		}
		else if (peaks.at(i-1).mz != m.mz){
			peaks.insert(peaks.begin()+i,m);
		}
		else{
			m.intn = input.at(x);
			m.mz = mz_val->at(x);
			m.pos = x;
			int tpos = peaks.at(i-1).pos;
			if (peaks.at(i-1).pos <= m.pos)
				peaks.insert(peaks.begin()+i,m);
			else/*(peaks.at(i-1).pos > m.pos+1)*/
				peaks.insert(peaks.begin()+i-1,m);
		}

		
	}
	if (manual)
		manualPeaks.push_back(m.pos);
	return 1;
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
		for (unsigned int i = 0; i < base.size(); ++i){
			base.at(i)+=specMin;
		}
		for (unsigned int i = 0; i < peaks.size(); ++i){
			peaks.at(i).intn+=specMin;
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

void data::peaksAt(vector <mass> &pLoc)
{
	peaks.clear();
	for (unsigned int i = 0; i < pLoc.size(); ++i){
		addPeak(pLoc.at(i).pos, false);
	}
}
	
int data::clusterPeaks(bool reclust)
{
	if (peaks.empty())
		return 0;
	cluster.clear();
	pClust curClust;
	int s = 0;
	//while (peaks.at(s).intn < base.at(peaks.at(s).pos) && s < peaks.size())
	//	s++;
	curClust.overlaps.push_back(&peaks.at(0));
	s++;
	for (unsigned int i = s; i < peaks.size(); ++i){		
		double largep = max(curClust.overlaps.back()->intn-base.at(curClust.overlaps.back()->pos),peaks.at(i).intn-base.at(i));
		int startp = curClust.overlaps.back()->pos;
		int endp = peaks.at(i).pos;
		while (startp < endp && input.at(startp)-base.at(startp) > largep*0.10 && mz_val->at(startp) < curClust.overlaps.back()->mz*1.05){
			startp++;
		}
		if (startp != endp || curClust.overlaps.size() == 5 || peaks.at(i).intn < base.at(peaks.at(i).pos)*2){
			cluster.push_back(curClust);
			curClust.overlaps.clear();
		}
		curClust.overlaps.push_back(&peaks.at(i));
	}
	cluster.push_back(curClust);
	return 1;
}

double data::modelPeaks(int model, double ulim, double llim, bool reclust, bool debug)
{
	double leastSq = 0;
	calpeakNums();
	if (base.empty())
		base.assign(mz_val->size(),0);
	if (reclust){
		if (clusterPeaks(true) == 0)
			return -1;
	}
	int ret;
	double p[20],	// max is 5 overlapping gaussians
		   x[5000]; // variables
	double lb[20], ub[20];
	int m, n;
	struct mydata d;

	double opts[LM_OPTS_SZ], info[LM_INFO_SZ];
	opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
	opts[4]=LM_DIFF_DELTA; 

	double lastpos = 0;
	double nextpos = 0;
	unsigned int ibegin = 0;
	//vector <mass> before;
	//vector<mass> after;
	while (cluster.at(ibegin).overlaps.front()->mz<llim)
		ibegin++;
	for (unsigned int i = ibegin; i < cluster.size() && cluster.at(i).overlaps.front()->mz<ulim ; ++i){
		int pfront = cluster.at(i).overlaps.front()->pos;
		int pback = cluster.at(i).overlaps.back()->pos;
		lastpos = cluster.at(i).overlaps.back()->pos;
		pfront = cluster.at(i).overlaps.front()->bbase;
		pback = cluster.at(i).overlaps.back()->ebase;
		int vars = 0;
		n = pback-pfront;
		if (n > 5000){
			int extra = n-5000;
			pback-=extra*0.5;
			pfront+=extra*0.5;
			n=5000;
		}
		m = cluster.at(i).overlaps.size()*4;
		typedef void (*modelFunc)(double*,double*,int,int,void*);
		vector <modelFunc> funcVec;
		vector <int> types;
		double minlsqr = DBL_MAX;
		if (model == 3){
			funcVec.push_back(&lognormmodel);
			types.push_back(3);
		}
		else if (model < 3){
			funcVec.push_back(&voigtmodel);
			types.push_back(model);
		}
		else{
			funcVec.push_back(&voigtmodel);
			types.push_back(0);
			funcVec.push_back(&voigtmodel);
			types.push_back(1);
			funcVec.push_back(&voigtmodel);
			types.push_back(2);
			funcVec.push_back(&lognormmodel);
			types.push_back(3);
		}
		vector <double> old_w;
		for (unsigned int j = 0; j < cluster.at(i).overlaps.size(); ++j){
			if (cluster.at(i).overlaps.size() ==2)
					cluster.at(i).overlaps.at(j)->w*=0.8;
			if (cluster.at(i).overlaps.size() > 2)
					cluster.at(i).overlaps.at(j)->w*=0.9;
			old_w.push_back(cluster.at(i).overlaps.at(j)->w);
		}
		if (n <= 5000 && n > 5){
			for (unsigned int fun = 0; fun < funcVec.size(); ++fun){
				double mu = 1.2;
				if (types.at(fun) == 0)
					mu = 1;
				else if (types.at(fun) == 1)
					mu = 1.6;
				else if (types.at(fun) == 3)
					mu = 0.75;
				d.type = types.at(fun);
				vars = 0;
				int incr = 1;
				if (n > 100){
					incr = (floor)((double)n/(double)100);
				}
				for (unsigned int j = pfront; j < pback; j+=incr){
					d.x[vars] = input.at(j)-base.at(j);
					if (input.at(j) < 0 ||d.x[vars] < 0)
						d.x[vars] = 0;
					d.y[vars] = mz_val->at(j);
					x[vars] = 0.0;
					vars++;
				}
				n = vars;
				vars = 0;
				double minpval = 1;
				for (unsigned int j = 0; j < cluster.at(i).overlaps.size(); ++j){
					if (cluster.at(i).overlaps.at(j)->mz < 300)
						mu = 1.5;
					if (cluster.at(i).overlaps.at(j)->mz < 10)
						mu=0.6;
					//cluster.at(i).overlaps.at(j)->bbase = pfront;
					//cluster.at(i).overlaps.at(j)->ebase = pback;
					//if (cluster.at(i).overlaps.at(j)->mz < 100)
					minpval =old_w.at(j);
					if (minpval < 1 && cluster.at(i).overlaps.at(j)->mz > 100)
						minpval = 1;
					if (types.at(fun) == 3){
						//if (minpval <= cluster.at(i).overlaps.at(j)->mz*0.001 && cluster.at(i).overlaps.at(j)->mz > 10000)
						//	minpval = cluster.at(i).overlaps.at(j)->mz*0.001;
						if (cluster.at(i).overlaps.at(j)->mz < 300)
							mu = 1;
						p[vars]=(cluster.at(i).overlaps.at(j)->intn-base.at(cluster.at(i).overlaps.at(j)->pos))*minpval*mu;
						if (cluster.at(i).overlaps.at(j)->mz < 100)
							p[vars+1]=(cluster.at(i).overlaps.at(j)->mz+mz_val->at(cluster.at(i).overlaps.at(j)->bbase))*0.5;
						else
							p[vars+1]=cluster.at(i).overlaps.at(j)->mz*0.99;
						p[vars+2]=minpval;
						p[vars+3]=1;
						if (p[vars] < 0 || cluster.at(i).overlaps.at(j)->intn < 0)
							p[vars] = 0;
						lb[vars]=p[vars]*0.5;
						if (cluster.at(i).overlaps.at(j)->peak_num != 0)
							lb[vars+1]=min(p[vars+1],peaks.at(cluster.at(i).overlaps.at(j)->peak_num-1).mz);
						else{
							if (p[vars+1] > 0)
								lb[vars+1]=p[vars+1]*0.99;
							else
								lb[vars+1]=p[vars+1]*1.01;
						}
						lb[vars+2]=p[vars+2]*0.1;
						lb[vars+3]=0.6;
						ub[vars]=p[vars]*5;
						if (p[vars+1] > 0)
							ub[vars+1]=min(cluster.at(i).overlaps.at(j)->mz,p[vars+1]*1.01);
						else
							ub[vars+1]=p[vars+1]*0.99;
						ub[vars+2]=p[vars+2]*5;
						ub[vars+3]=6;

						vars+=4;
					}
					else{
						p[vars]=(cluster.at(i).overlaps.at(j)->intn-base.at(cluster.at(i).overlaps.at(j)->pos))*minpval*mu;
						p[vars+1]=0.69314718;
						p[vars+2]=cluster.at(i).overlaps.at(j)->mz;
						p[vars+3]=minpval;
						if (p[vars] < 0|| cluster.at(i).overlaps.at(j)->intn < 0)
							p[vars] = 0;
						lb[vars]=min(p[vars],(cluster.at(i).overlaps.at(j)->intn-base.at(cluster.at(i).overlaps.at(j)->pos)));
						lb[vars+1]=0.01;
						if (p[vars+2] > 0)
							lb[vars+2]=p[vars+2]*0.99995;
						else
							lb[vars+2]=p[vars+2]*1.00005;
						lb[vars+3]=p[vars+3]*0.1;
						ub[vars]=p[vars]*5;
						ub[vars+1]=DBL_MAX;
						if (p[vars+2] > 0)
							ub[vars+2]=p[vars+2]*1.00005;
						else
							ub[vars+2]=p[vars+2]*0.99995;
						ub[vars+3]=p[vars+3]*3;

						vars+=4;
					}
					//mass m;
					//m = *cluster.at(i).overlaps.at(j);
					//before.push_back(m);
				}
				if (p[0] != 0){
					if (vars <= 4){
						ret=dlevmar_dif(funcVec.at(fun),p,x,m,n,200,opts,info,NULL,NULL,(void *)&d);
						if (info[0] <= info[1] || ret == 200 || p[0] > ub[0]*5 || (types.at(fun) ==3 && (p[2] < 0 ||(p[2]>=ub[2]*1.5) || (p[1]<lb[1]) || (p[1]>ub[1])||(fabs(p[3]) <lb[3])))||  (types.at(fun) != 3 &&(p[3] < 0 || p[2] > ub[2]*1.005 || p[2] < lb[2]*0.995))){
							for (unsigned int j = 0; j < cluster.at(i).overlaps.size(); ++j){
								vars = 0;
								if (types.at(fun) == 3){
									p[vars]=(cluster.at(i).overlaps.at(j)->intn-base.at(cluster.at(i).overlaps.at(j)->pos))*minpval*mu;
									if (cluster.at(i).overlaps.at(j)->mz < 100)
										p[vars+1]=(cluster.at(i).overlaps.at(j)->mz+mz_val->at(cluster.at(i).overlaps.at(j)->bbase))*0.5;
									else
										p[vars+1]=cluster.at(i).overlaps.at(j)->mz*0.99;
									p[vars+2]=minpval;
									p[vars+3]=1;
									if (p[vars] < 0|| cluster.at(i).overlaps.at(j)->intn < 0)
										p[vars] = 0;
									vars+=4;
								}
								else{
									p[vars]=(cluster.at(i).overlaps.at(j)->intn-base.at(cluster.at(i).overlaps.at(j)->pos))*minpval*mu;
									p[vars+1]=0.69314718;
									p[vars+2]=cluster.at(i).overlaps.at(j)->mz;
									p[vars+3]=old_w.at(j);
									if (p[vars] < 0|| cluster.at(i).overlaps.at(j)->intn < 0)
										p[vars] = 0;
									vars+=4;
								}
								ret=dlevmar_bc_dif(funcVec.at(fun),p,x,m,n,lb,ub,20,opts,info,NULL,NULL,(void *)&d);
							}
						}
					}
					else
						ret=dlevmar_bc_dif(funcVec.at(fun),p,x,m,n,lb,ub,20,opts,info,NULL,NULL,(void *)&d);
					vars = 0;
				//	if (fabs(info[1])<minlsqr){
						for (unsigned int j = 0; j < cluster.at(i).overlaps.size(); ++j){
								cluster.at(i).overlaps.at(j)->a = p[vars];
					//			cluster.at(i).overlaps.at(j)->fit_err = info[1];
							//	cluster.at(i).overlaps.at(j)->type = 4;
								if (types.at(fun) == 3){
									cluster.at(i).overlaps.at(j)->type = types.at(fun);
									cluster.at(i).overlaps.at(j)->loc = p[vars+1];
									cluster.at(i).overlaps.at(j)->r = fabs(p[vars+3]);
									cluster.at(i).overlaps.at(j)->w = p[vars+2];
								}
								else{
									cluster.at(i).overlaps.at(j)->type = types.at(fun);
									if (types.at(fun) == 0)
										cluster.at(i).overlaps.at(j)->r = 0;
									else if (types.at(fun) == 1)
										cluster.at(i).overlaps.at(j)->r = 1;
									else
										cluster.at(i).overlaps.at(j)->r = exp(-fabs(p[vars+1]));
									cluster.at(i).overlaps.at(j)->loc = p[vars+2];
									cluster.at(i).overlaps.at(j)->w = p[vars+3];
								}
								//mass m;
								//m = *cluster.at(i).overlaps.at(j);
								//after.push_back(m);
								vars+=4;
							//	if (checkModel(types.at(fun),*cluster.at(i).overlaps.at(j))){
									cluster.at(i).overlaps.at(j)->modelled = true;
							//	}
							//	else
							//		cluster.at(i).overlaps.at(j)->a = 0;
								minlsqr = fabs(info[1]);
							}
					}
					leastSq += minlsqr;
			//	}
			}
		}
	}
	return leastSq;
	//if (debug)
	//	this->outputPeakParams("peaks.csv",before,after);
}


bool data::checkModel(int type, mass &peak)
{
	bool res = false;
	if (type == 3){
		double x = peak.loc+peak.w;
		if ((peak.intn-base.at(peak.pos))*1.5 > lognorm(peak.a,peak.loc,peak.r,peak.w,x) && peak.loc < peak.mz)
			res = true;
	}
	else{
		double z = (peak.intn-base.at(peak.pos))*1.5;
		double x = voigt(peak.a,peak.r,peak.loc,peak.w,peak.loc);
		if ((peak.intn-base.at(peak.pos))*1.5 > voigt(peak.a,peak.r,peak.loc,peak.w,peak.loc))
			res = true;
	}
	return res;
}

void data::cropSpec(int xmin, int xmax)
{
		vector <double> inmz;
		vector <double> inbase;
		int a = input.size();
		for (unsigned int j = xmin; j < xmax; ++j){
			inmz.push_back(input.at(j));
			if (!base.empty())
				inbase.push_back(base.at(j));
		}
		while (!peaks.empty() && xmin > peaks.front().pos){
			peaks.erase(peaks.begin());
		}
		while (!peaks.empty() && xmax < peaks.back().pos){
			peaks.pop_back();
		}
		while (!manualPeaks.empty() && xmin > manualPeaks.front()){
			manualPeaks.erase(manualPeaks.begin());
		}
		while (!manualPeaks.empty() && xmax < manualPeaks.back()){
			manualPeaks.pop_back();
		}
		for (unsigned int k = 0; k < peaks.size(); ++k){
			peaks.at(k).pos-=xmin;
		}
		for (unsigned int k = 0; k < manualPeaks.size(); ++k){
			manualPeaks.at(k)-=xmin;
		}
		input = inmz;
		if (!base.empty()){
			base = inbase;
			//while (base.size() < input.size())
			//	base.push_back(base.back());
		}
}


double data::modelOnAvg(data &avg, double ulim, bool debug)
{
	double totLeastSq = 0;
	calpeakNums();
	reg.alpha = avg.reg.alpha;
	reg.beta = avg.reg.beta;
	if (base.empty())
		base.assign(mz_val->size(),0);
	if (clusterPeaks(false) == 0)
		return -1;
	int ret;
	double p[20],	// max is 5 overlapping gaussians
		   x[5000]; // variables
	double lb[20], ub[20];
	int m, n;
	struct mydata d;

	double opts[LM_OPTS_SZ], info[LM_INFO_SZ];
	opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
	opts[4]=LM_DIFF_DELTA; 

	double lastpos = 0;
	double nextpos = 0;
	int avg_peak_count = 0;

	typedef void (*modelFunc)(double*,double*,int,int,void*);
	vector <modelFunc> funcVec;
	for (unsigned int i = 0; i < cluster.size() ; ++i){
		int vars = 0;

		m = cluster.at(i).overlaps.size()*4;

		funcVec.clear();
		//int types;
		double minlsqr = DBL_MAX;
		int model;

		vector <double> old_w;
		vector <double> avg_r;
		vector <double> avg_loc;

		int pfront = avg.peaks.at(cluster.at(i).overlaps.front()->peak_num).bbase;
		
		for (unsigned int j = 0; j < cluster.at(i).overlaps.size(); ++j){
			cluster.at(i).overlaps.at(j)->bbase = avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).bbase;
			cluster.at(i).overlaps.at(j)->ebase = avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).ebase;
			avg_r.push_back(avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).r);
			avg_loc.push_back(avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).loc);
			if (cluster.at(i).overlaps.at(j)->mz>ulim)
				old_w.push_back(fabs(avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).w));
			else{
				double expf = expfunc(reg.alpha,reg.beta,cluster.at(i).overlaps.at(j)->mz);
				if (expf < 10)
					expf = avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).w;
				old_w.push_back(expf);
			}
			avg_peak_count++;
		}
		int pback = avg.peaks.at(cluster.at(i).overlaps.back()->peak_num).ebase;

		n = pback-pfront;
		if (n > 5000){
			int extra = n-5000;
			pback-=extra*0.5;
			pfront+=extra*0.5;
			n=5000;
		}
		double mu = 1.6;
		if (cluster.at(i).overlaps.size() > 1)
			mu /=(double)cluster.at(i).overlaps.size();
		if (n <= 5000 && n > 5){
//			for (unsigned int fun = 0; fun < funcVec.size(); ++fun){
				//d.type = types.at(fun);
				vars = 0;
				int incr = 1;
				if (n > 100){
					incr = (floor)((double)n/(double)100);
				}
				for (unsigned int j = pfront; j < pback; j+=incr){
					d.x[vars] = input.at(j)-base.at(j);
					if (input.at(j) < 0 || d.x[vars] < 0)
						d.x[vars] = 0;
					d.y[vars] = mz_val->at(j);
					x[vars] = 0.0;
					vars++;
				}
				n = vars;
				vars = 0;
				double minpval = 1;
				for (unsigned int j = 0; j < cluster.at(i).overlaps.size(); ++j){
					model = avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).type;
					cluster.at(i).overlaps.at(j)->type = model;
					if (model == 3)
						funcVec.push_back(&lognormmodel);
					else
						funcVec.push_back(&voigtmodel);
					d.type = model;
					//if (cluster.at(i).overlaps.at(j)->mz < 100)
					//	mu = 1.5;
					minpval = old_w.at(j);
					if (minpval < 1 && cluster.at(i).overlaps.at(j)->mz > 100)
						minpval = 1;
					if (model == 3){
						p[vars]=(cluster.at(i).overlaps.at(j)->intn/avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).intn)*avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).a;
					//	p[vars]=(cluster.at(i).overlaps.at(j)->intn-base.at(cluster.at(i).overlaps.at(j)->pos))*(minpval/avg_r.at(j))*0.7;
						p[vars+1]=min(cluster.at(i).overlaps.at(j)->mz,cluster.at(i).overlaps.at(j)->mz-(old_w.at(j)));
					//	p[vars+1]=avg_loc.at(j);
						p[vars+2]=old_w.at(j);
						p[vars+3]=avg_r.at(j);
						if (p[vars] < 0|| cluster.at(i).overlaps.at(j)->intn < 0)
							p[vars] = 0;
						lb[vars]=p[vars]*0.5;
						//if (cluster.at(i).overlaps.at(j)->peak_num != 0)
						//	lb[vars+1]=min(p[vars+1],peaks.at(cluster.at(i).overlaps.at(j)->peak_num-1).mz);
						//else{
							if (p[vars+1] > 0)
								lb[vars+1]=p[vars+1]*0.99;
							else
								lb[vars+1]=p[vars+1]*1.01;
						//}
						lb[vars+2]=p[vars+2]*0.1;
						lb[vars+3]=max(0.6,p[vars+3]*0.9);
						ub[vars]=p[vars]*5;
						if (p[vars+1] > 0)
							ub[vars+1]=min(cluster.at(i).overlaps.at(j)->mz,p[vars+1]*1.01);
						else
							ub[vars+1]=p[vars+1]*0.99;
						ub[vars+2]=p[vars+2]*4;
						ub[vars+3]=p[vars+3]*2;

						//cluster.at(i).overlaps.at(j)->a = p[vars];
						//cluster.at(i).overlaps.at(j)->r = p[vars+3];
						//cluster.at(i).overlaps.at(j)->loc = p[vars+1];
						//cluster.at(i).overlaps.at(j)->w = p[vars+2];

						vars+=4;
					}
					else{
						p[vars]=(cluster.at(i).overlaps.at(j)->intn/avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).intn)*avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).a;
						//p[vars]=(cluster.at(i).overlaps.at(j)->intn-base.at(cluster.at(i).overlaps.at(j)->pos))*minpval*mu;
						p[vars+1]=0.69314718;
						p[vars+2]=cluster.at(i).overlaps.at(j)->mz;
						p[vars+3]=old_w.at(j);
						if (p[vars] < 0|| cluster.at(i).overlaps.at(j)->intn < 0)
							p[vars] = 0;
						lb[vars]=p[vars]*0.5;
						lb[vars+1]=0.001;
						if (p[vars+2] > 0)
							lb[vars+2]=p[vars+2]*0.9995;
						else
							lb[vars+2]=p[vars+2]*1.0005;
						lb[vars+3]=p[vars+3]*0.5;
						ub[vars]=p[vars]*10;
						ub[vars+1]=6;
						if (p[vars+2] < 0)
							ub[vars+2]=p[vars+2]*0.9995;
						else
							ub[vars+2]=p[vars+2]*1.0005;
						if (cluster.at(i).overlaps.size() > 1)
							ub[vars+3]=p[vars+3]*3;
						else
							ub[vars+3]=p[vars+3]*8;

						//cluster.at(i).overlaps.at(j)->a = p[vars];
						//cluster.at(i).overlaps.at(j)->r = 1;
						//cluster.at(i).overlaps.at(j)->loc = p[vars+2];
						//cluster.at(i).overlaps.at(j)->w = p[vars+3];
						//if (p[vars] < lb[vars] || p[vars] > ub[vars])
						//	cluster.at(i).overlaps.at(j)->a = -1;
						//if (p[vars+1] < lb[vars+1] || p[vars+1] > ub[vars+1])
						//	cluster.at(i).overlaps.at(j)->r = -1;
						//if (p[vars+2] < lb[vars+2] || p[vars+2] > ub[vars+2])
						//	cluster.at(i).overlaps.at(j)->loc = -1;
						//if (p[vars+3] < lb[vars+3] || p[vars+3] > ub[vars+3])
						//	cluster.at(i).overlaps.at(j)->w = -1;
						vars+=4;
					}
					//mass m;
					//m = *cluster.at(i).overlaps.at(j);
					//before.push_back(m);
					//out<<m.mz<<","<<m.intn<<","<<m.type<<","<<m.bbase<<","<<m.ebase<<endl;
					//out<<m.a<<","<<m.loc<<","<<m.w<<","<<m.r<<endl;
					//out.flush();
				}
				if (p[0] != 0){
					if (vars <= 4){
						ret=dlevmar_dif(funcVec.at(0),p,x,m,n,200,opts,info,NULL,NULL,(void *)&d);
						if (info[0] <= info[1] || ret == 200 || p[0] > ub[0]*5 || (model ==3 && ((p[2]>=ub[2]*1.5) || (p[1]<lb[1]) || (p[1]>ub[1])||(fabs(p[3]) <lb[3])))|| ( model != 3 &&(p[2] > ub[2] || p[2] < lb[2]))){
							for (unsigned int j = 0; j < cluster.at(i).overlaps.size(); ++j){
								vars = 0;
								if (model == 3){
									p[vars]=(cluster.at(i).overlaps.at(j)->intn/avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).intn)*avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).a;
									p[vars+1]=min(cluster.at(i).overlaps.at(j)->mz,cluster.at(i).overlaps.at(j)->mz-(old_w.at(j)));
									p[vars+2]=old_w.at(j);
									p[vars+3]=avg_r.at(j);
									if (p[vars] < 0|| cluster.at(i).overlaps.at(j)->intn < 0)
										p[vars] = 0;
									vars+=4;
								}
								else{
									p[vars]=(cluster.at(i).overlaps.at(j)->intn/avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).intn)*avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).a;
									p[vars+1]=0.69314718;
									p[vars+2]=cluster.at(i).overlaps.at(j)->mz;
									p[vars+3]=old_w.at(j);
									if (p[vars] < 0|| cluster.at(i).overlaps.at(j)->intn < 0)
										p[vars] = 0;
									vars+=4;
								}
								ret=dlevmar_bc_dif(funcVec.at(0),p,x,m,n,lb,ub,20,opts,info,NULL,NULL,(void *)&d);
							}
						}
					}
					else
						ret=dlevmar_bc_dif(funcVec.at(0),p,x,m,n,lb,ub,20,opts,info,NULL,NULL,(void *)&d);
					vars = 0;
					//if (info[1]<minlsqr){
					//	minlsqr = info[1];
					totLeastSq+=info[1];
					for (unsigned int j = 0; j < cluster.at(i).overlaps.size(); ++j){
								cluster.at(i).overlaps.at(j)->a = p[vars];
								cluster.at(i).overlaps.at(j)->fit_err = info[1];
								cluster.at(i).overlaps.at(j)->type =avg.peaks.at(cluster.at(i).overlaps.at(j)->peak_num).type;
								if (cluster.at(i).overlaps.at(j)->type == 3){
									//cluster.at(i).overlaps.at(j)->type =types.at(fun);
									cluster.at(i).overlaps.at(j)->loc = p[vars+1];
									cluster.at(i).overlaps.at(j)->r = fabs(p[vars+3]);
									cluster.at(i).overlaps.at(j)->w = p[vars+2];
								}
								else{
									//cluster.at(i).overlaps.at(j)->type = types.at(fun);
									if (cluster.at(i).overlaps.at(j)->type == 0)
										cluster.at(i).overlaps.at(j)->r = 0;
									else if (cluster.at(i).overlaps.at(j)->type == 1)
										cluster.at(i).overlaps.at(j)->r = 1;
									else
										cluster.at(i).overlaps.at(j)->r = exp(-fabs(p[vars+1]));
									cluster.at(i).overlaps.at(j)->loc = p[vars+2];
									cluster.at(i).overlaps.at(j)->w = p[vars+3];
								}
								//mass m;
								//m = *cluster.at(i).overlaps.at(j);
								//after.push_back(m);

								//out<<cluster.at(i).overlaps.at(j)->a<<","<<cluster.at(i).overlaps.at(j)->loc<<","<<cluster.at(i).overlaps.at(j)->w<<","<<cluster.at(i).overlaps.at(j)->r<<","<<model<<endl;
								//out.flush();

								if (checkModel(model,*cluster.at(i).overlaps.at(j)) && cluster.at(i).overlaps.at(j)->a != 0)
									cluster.at(i).overlaps.at(j)->modelled = true;
								else{
									cluster.at(i).overlaps.at(j)->modelled = false;
									cluster.at(i).overlaps.at(j)->a=0;
								}


								vars+=4;
					//}
					//}
				}
			}
		}
	}
	return totLeastSq;
//	out.close();
//	if (debug)
//		this->outputPeakParams("peaks.csv",before,after);
}

bool data::regressParams(char * filename1, char * filename2)
{
	if (peaks.size() == 0)
		return false;
	double avg_err = 0;
	int gpeaks = 0;
	for (unsigned int i = 0; i < peaks.size(); ++i){
		if (peaks.at(i).modelled){
			gpeaks++;
			avg_err+=peaks.at(i).fit_err;
		}
	}
	avg_err /= gpeaks;
	ofstream out;
	out.open(filename1,ios::trunc);
	out<<"MZ,Intn,Type,Fit,Area,Loc,Width,R"<<endl;
	for (unsigned int i = 0; i < cluster.size(); ++i){
		if (cluster.at(i).overlaps.size() == 1 && cluster.at(i).overlaps.front()->modelled && cluster.at(i).overlaps.front()->fit_err< avg_err){	
			out <<cluster.at(i).overlaps.front()->mz<<","<<cluster.at(i).overlaps.front()->intn<<","<<cluster.at(i).overlaps.front()->type<<","<<cluster.at(i).overlaps.front()->fit_err<<","<<cluster.at(i).overlaps.front()->a<<","<<cluster.at(i).overlaps.front()->loc<<","<<cluster.at(i).overlaps.front()->w<<","<<cluster.at(i).overlaps.front()->r<<endl;
		}
	}
	out << endl;
	out<<"alpha, "<<reg.alpha<<endl;
	out<<"beta, "<<reg.beta<<endl;
	out.close();
	out.open(filename2,ios::trunc);
	out<<"MZ,Intn,Type,Area,Loc,Width,R"<<endl;
	for (unsigned int i = 0; i < cluster.size(); ++i){
		//if (cluster.at(i).overlaps.size() == 1 && cluster.at(i).overlaps.front()->modelled){	
			out <<cluster.at(i).overlaps.front()->mz<<","<<cluster.at(i).overlaps.front()->intn<<","<<cluster.at(i).overlaps.front()->type<<","<<cluster.at(i).overlaps.front()->fit_err<<","<<cluster.at(i).overlaps.front()->a<<","<<cluster.at(i).overlaps.front()->loc<<","<<cluster.at(i).overlaps.front()->w<<","<<cluster.at(i).overlaps.front()->r<<endl;
		//}
	}
	out.close();

	return true;
}

bool data::analyticRegressParams(bool over)
{
	if (peaks.size() == 0)
		return false;
	long double a_t1 = 0;
	long double a_t2 = 0;
	long double a_t3 = 0;
	long double a_t4 = 0;
	long double a_t5 = 0;
	long double a_t6 = 0;
	long double b_t1 = 0;
	long double b_t2 = 0;
	long double b_t3 = 0;
	long double b_t4 = 0;
	long double b_t5 = 0;
	long double b_t6 = 0;
	long double sum_xy2 = 0;
	long double sum_x2 = 0;
	
	double avg_err = 0;
	int gpeaks = 0;
	double max_w = 0;
	for (unsigned int i = 0; i < peaks.size(); ++i){
		if (peaks.at(i).modelled){
			gpeaks++;
			avg_err+=peaks.at(i).fit_err;
			if (peaks.at(i).w > max_w)
				max_w = peaks.at(i).w;
		}
	}
	avg_err /= gpeaks;
	mass *p;
	for (unsigned int i = 0; i < cluster.size(); ++i){
		p = cluster.at(i).overlaps.front();
		bool overlap = false;
		if (over || cluster.at(i).overlaps.size() == 1)
			overlap = true;
		if (overlap &&p->modelled  && p->w > 0 && p->mz < 30000 && p->a > 0){
			a_t1+=log(p->w);
			a_t2+=sqr(p->mz);
			a_t3+=p->mz;
			a_t4+=(p->mz*log(p->w));
			a_t5+=1;
			a_t6+=sqr(p->mz);
			b_t1+=1;
			b_t2+=p->mz*log(p->w);
			b_t3+=p->mz;
			b_t4+=log(p->w);
			b_t5+=1;
			b_t6+=sqr(p->mz);
			sum_x2 += p->mz;
		}
	}
	if (a_t5 < 5)
		return false;
	sum_x2=sqr(sum_x2);
	double c = ((a_t1*a_t2)-(a_t3*a_t4))/((a_t5*a_t6)-sum_x2);
	double d = ((b_t1*b_t2)-(b_t3*b_t4))/((a_t5*a_t6)-sum_x2);
	//for (unsigned int i = 0; i < cluster.size(); ++i){
	//	p = cluster.at(i).overlaps.front();
	//	if (cluster.at(i).overlaps.size() == 1 && p->modelled){
	//		sum_xy2 += p->mz*p->w;
	//	}
	//}
	//sum_xy2 = sqr(sum_xy2);
	//a_t1 = 0;
	//a_t2 = 0;
	//a_t3 = 0;
	//a_t4 = 0;
	//a_t5 = 0;
	//a_t6 = 0;
	//b_t1 = 0;
	//b_t2 = 0;
	//b_t3 = 0;
	//b_t4 = 0;
	//b_t5 = 0;
	//b_t6 = 0;
	//for (unsigned int i = 0; i < cluster.size(); ++i){
	//	p = cluster.at(i).overlaps.front();
	//	bool overlap = false;
	//	if (over || cluster.at(i).overlaps.size() == 1)
	//		overlap = true;
	//	if (overlap &&p->modelled &&p->w > 0 && p->mz < 20000 && (i > 0 && cluster.at(i).overlaps.front()->w < cluster.at(i-1).overlaps.front()->w*5)){
	//		a_t1+=(sqr(p->mz)*p->w);
	//		a_t2+=(p->w*log(p->w));
	//		a_t3+=(p->mz*p->w);
	//		a_t4+=(p->mz*p->w*log(p->w));
	//		a_t5+=p->w;
	//		a_t6+=(sqr(p->mz)*p->w);
	//		b_t1+=p->w;
	//		b_t2+=(p->mz*p->w*log(p->w));
	//		b_t3+=(p->mz*p->w);
	//		b_t4+=(p->w*log(p->w));
	//		b_t5+=p->w;
	//		b_t6+=(sqr(p->mz)*p->w);
	//		sum_xy2 += p->mz*p->w;
	//	}
	//}
	//sum_xy2=sqr(sum_xy2);
	//double a = ((a_t1*a_t2)-(a_t3*a_t4))/((a_t5*a_t6)-sum_xy2);
	//double b = ((b_t1*b_t2)-(b_t3*b_t4))/((b_t5*b_t6)-sum_xy2);


	if (d < 0)
		d = 0.00005;
	reg.alpha = exp(c);
	reg.beta = d;

	return true;
}

void data::outputPeakParams(char * filename, vector <mass> &before, vector <mass> &after)
{
	if (peaks.size() == 0)
		return ;
	ofstream out;
	out.open(filename,ios::app);
	out<<"MZ,Intn,Start,End,Type,B_Area,B_Loc,B_Width,B_R,A_Area,A_Loc,A_Width,A_R"<<endl<<flush;
	for (unsigned int i = 0; i < before.size(); ++i){
		out <<before.at(i).mz<<","<<before.at(i).intn<<","<<mz_val->at(before.at(i).bbase)<<","<<mz_val->at(before.at(i).ebase)<<","<<after.at(i).type<<","<<before.at(i).a<<","<<before.at(i).loc<<","<<before.at(i).w<<","<<before.at(i).r<<","<<after.at(i).a<<","<<after.at(i).loc<<","<<after.at(i).w<<","<<after.at(i).r<<endl<<flush;
	}
	out<<endl<<flush;
	out.close();
}

void data::calpeakNums()
{
	for (unsigned int i = 0; i < peaks.size(); ++i){
		peaks.at(i).peak_num = i;
	}
}

void data::findPeakTroughs()
{
	vector <double> tmp_input = input;
	double thres = 0.2;
	waveletDenoise(thres, 1);
	SGsmooth(5,tmp_input);
	bool hasBase = !base.empty();
	if (!hasBase)
		base.assign(input.size(),0);
	for (unsigned int i = 0; i < peaks.size(); ++i){
		int pfront = peaks.at(i).pos;
		int pback = peaks.at(i).pos;
		int lastpos = 0;
		int nextpos = input.size()-1;
		int same = 0;
		while (pfront-1 >= lastpos &&(input.at(peaks.at(i).pos))*0.9<input.at(pfront-1)&& input.at(peaks.at(i).pos)*1.1>input.at(pfront-1))
			pfront--;
		while (pfront-1 >= lastpos && input.at(pfront)*1.001>input.at(pfront-1) && input.at(pfront-1)-base.at(pfront-1)> 0 && same < 10){
			if (input.at(pfront) > input.at(pfront-1)*0.995 && input.at(pfront) < input.at(pfront-1)*0.105)
				same++;
			else
				same = 0;
			pfront--;
		}
		same = 0;
		while (pback+1 < nextpos && (input.at(peaks.at(i).pos))*0.9<input.at(pback+1)&& input.at(peaks.at(i).pos)*1.1>input.at(pback+1) )
			pback++;
		while (pback+1 < nextpos && input.at(pback)*1.001>input.at(pback+1) && input.at(pback+1)-base.at(pback+1)> 0 && same < 10){
			if (input.at(pback) > input.at(pback-1)*0.995 && input.at(pback) < input.at(pback-1)*0.105)
				same++;
			else
				same = 0;
			pback++;
		}
		if (peaks.at(i).pos-pfront < 5)
			pfront = max(0,peaks.at(i).pos-5);
		if (pback-peaks.at(i).pos < 5)
			pback = min(peaks.at(i).pos+5,input.size()-1);
		peaks.at(i).bbase = pfront;
		peaks.at(i).ebase = pback;
		if (i > 1 && ((peaks.at(i).ebase-peaks.at(i).bbase) > (peaks.at(i-1).ebase-peaks.at(i-1).bbase)*3 || (peaks.at(i).ebase-peaks.at(i).bbase) > (peaks.at(i-2).ebase-peaks.at(i-2).bbase)*3)){
			peaks.at(i).bbase = peaks.at(i).pos-((peaks.at(i-1).ebase-peaks.at(i-1).bbase)*0.5);
			peaks.at(i).ebase = peaks.at(i).pos+((peaks.at(i-1).ebase-peaks.at(i-1).bbase)*0.5);
		}
		if (peaks.at(i).bbase < 0)
			peaks.at(i).bbase = 0;
		if (peaks.at(i).ebase >= mz_val->size())
			peaks.at(i).ebase = mz_val->size()-1;
		peaks.at(i).peak_min = min(input.at(pfront),input.at(pback));
		int phf = max(0,peaks.at(i).pos-2);
		int phb = min(mz_val->size(),peaks.at(i).pos+2);
		double hh = (peaks.at(i).intn-base.at(peaks.at(i).pos))*0.5;
		while (phf-1 >= lastpos && input.at(phf)-base.at(phf) > hh && input.at(phf) > peaks.at(i).intn*0.8 && input.at(phf) < peaks.at(i).intn*1.1)
			phf--;
		while (phf-1 >= lastpos && input.at(phf)-base.at(phf) > hh && input.at(phf-1) < input.at(phf)*1.01 && input.at(phf) < peaks.at(i).intn*1.1)
			phf--;
		while (phb+1 <nextpos && input.at(phb)-base.at(phb) > hh && input.at(phb) > peaks.at(i).intn*0.8 && input.at(phb) < peaks.at(i).intn*1.1)
			phb++;
		while (phb+1 <nextpos && input.at(phb)-base.at(phb) > hh && input.at(phb+1) < input.at(phb)*1.01 && input.at(phb) < peaks.at(i).intn*1.1)
			phb++;
		double dif;
		if (peaks.at(i).pos-phf > peaks.at(i).pos+phb)
			dif = peaks.at(i).mz-mz_val->at(phf);
		else
			dif = mz_val->at(phb)-peaks.at(i).mz;
		peaks.at(i).w = dif*2;
		if (peaks.at(i).w > mz_val->at(peaks.at(i).ebase)-mz_val->at(peaks.at(i).bbase)){
			peaks.at(i).w = mz_val->at(peaks.at(i).ebase)-mz_val->at(peaks.at(i).bbase);
		}
	}
	//if (mz_val->back() > 100)
	//	checkPeakWidths();
	input = tmp_input;
	if (!hasBase)
		base.clear();
}

void data::checkPeakWidths()
{
	double mean_w = 0;
	int p = peaks.size()-2;
	for (int i = 0; i < p; ++i){
		mean_w = (peaks.at(i).w+peaks.at(i+2).w)*0.5;
	}
	for (int i = 0; i < p; ++i){
		if (peaks.at(i+1).w > peaks.at(i).w*10 || peaks.at(i+1).w==0)
			peaks.at(i+1).w = peaks.at(i).w;
	}
}

//void data::waveletDenoise(Daubechies &waveOp, double &sens, int recurs)
//{
//	int orgsize = input.size();
//	for (unsigned int i = 0; i < recurs; ++i){
//		if (i != 0)
//			input.insert(input.begin(),(int)((double)orgsize/(double)i),input.front());
//		int Esize = input.size();
//		int k = 0;
//		while (Esize > pow(2,k)){
//			++k;
//		}
//		Esize = (int)pow(2,k);
//		int diff = Esize-input.size();
//		input.insert(input.end(),diff,0);
//		waveOp.daubTrans(input,Esize);
//		waveOp.threshold(input,diff,k, sens);
//		waveOp.invDaubTrans(input,Esize);
//		input.resize(Esize-diff);
//		if (i != 0)
//			input.erase(input.begin(),input.begin()+(int)((double)orgsize/(double)i));
//	}
//}

void data::waveletDenoise(double &sens, int recurs)
{
	int orgsize = input.size();
	for (unsigned int i = 0; i < recurs; ++i){
		if (i != 0)
			input.insert(input.begin(),(int)((double)orgsize/(double)i),input.front());
		int Esize = input.size();
		int k = 0;
		while (Esize > pow((double)2,(double)k)){
			++k;
		}
		Esize = (int)pow((double)2,(double)k);
		int diff = Esize-input.size();
		input.insert(input.end(),diff,0);
		FastSymmlet8::transform(input,Esize);
		FastSymmlet8::threshold(input,diff,k, sens);
		FastSymmlet8::invTransform(input,Esize);
		input.resize(Esize-diff);
		if (i != 0)
			input.erase(input.begin(),input.begin()+(int)((double)orgsize/(double)i));
	}
}

void data::genModelSpectrum()
{
	if (peaks.empty()){
		return;
	}
	if (peaks.at(0).modelled == false){
		return;
	}
	if (!input_model.empty())
		input_model.clear();
	double cur_val;
	for (unsigned int i = 0; i < mz_val->size(); ++i){
		cur_val = 0;
		if (!base.empty() && i < base.size())
			cur_val += base.at(i);
		for (unsigned int j = 0; j < peaks.size(); ++j){
			if (peaks.at(j).intn > base.at(peaks.at(j).pos)){
				if (peaks.at(j).type == 3){  //lognormal
					cur_val += lognorm(peaks.at(j).a,peaks.at(j).loc,peaks.at(j).r,peaks.at(j).w,mz_val->at(i));
				}
				else{
					cur_val += voigt(peaks.at(j).a,peaks.at(j).r,peaks.at(j).loc, peaks.at(j).w, mz_val->at(i));
				}
			}
		}
		input_model.push_back(cur_val);
	}
}

int data::simplifySpectrum()
{
	input_simp.clear();
	if (input.size() < 20){
		input_simp = input;
		return -1;
	}
	int N = (int)floor((double)input.size()/20);
	int tenth = (int)ceil((double)N/20);
	vector <double> tmp_seg;
	double seg_lim[50];
	for (unsigned int i = 0; i < 20; ++i){
		tmp_seg.clear();
		for (unsigned int j = i*N; j < (i+1)*N; ++j){
			tmp_seg.push_back(input.at(j));
		}
		sort(tmp_seg.begin(),tmp_seg.end(),GreaterThanInput());
		seg_lim[i]=tmp_seg.at(tenth);
	}
	for (unsigned int i = 0; i < 20; ++i){
		for (unsigned int j = i*N; j < (i+1)*N ;++j){
			if (input.at(j) >= seg_lim[i])
				input_simp.push_back(input.at(j));
			else
				input_simp.push_back(0);
		}
	}
	return 1;
}
