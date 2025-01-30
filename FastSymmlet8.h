#ifndef FASTSYMMLET8_H
#define FASTSYMMLET8_H

#include <vector>

using namespace std;

/************************************************
* An efficient implementation of
* the Fast Wavelet Transform using
* Symmlet8 wavelets (FBI fingerprints standard)
*************************************************/
class FastSymmlet8 {
	public:
	
		static void transform (vector <double> &input,int length);
		
		static void invTransform (vector <double> &wav, int length);	
		static void expand (vector <double> &wav, int length);
		
		//class NotAPowerOf2Exception{};
		//class NotDivisibleBy2Exception{};
		
		static void invTransform(vector <double> &wav, int length, int last);
		static void transform(vector <double> &input,int length, int last);
		static void threshold(vector <double> &wav,int diff,int bits, double &sens);
		

	private:


};

#endif;