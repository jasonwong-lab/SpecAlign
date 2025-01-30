#ifndef _DAUB_H_
#define _DAUB_H_

#include <vector>
#include <algorithm>

using namespace std;

#define INVERF 0.6744897414216

class GreaterThan{
public:
	bool operator()(const double &a, const double &b)
	{return a > b;}
};

class Daubechies {
   private:
   /** forward transform scaling coefficients */
   double h0, h1, h2, h3;
   /** forward transform wave coefficients */
   double g0, g1, g2, g3;

   double Ih0, Ih1, Ih2, Ih3;
   double Ig0, Ig1, Ig2, Ig3;

   /**
     Forward Daubechies D4 transform
    */
   void transform( vector <double> &a, const int n )
   {
      if (n >= 4) {
         int i, j;
         const int half = n >> 1;
         
       vector <double> tmp(n,0);

         for (i = 0, j = 0; j < n-3; j += 2, i++) {
            tmp[i]      = a[j]*h0 + a[j+1]*h1 + a[j+2]*h2 + a[j+3]*h3;
            tmp[i+half] = a[j]*g0 + a[j+1]*g1 + a[j+2]*g2 + a[j+3]*g3;
         }

         tmp[i]      = a[n-2]*h0 + a[n-1]*h1 + a[0]*h2 + a[1]*h3;
         tmp[i+half] = a[n-2]*g0 + a[n-1]*g1 + a[0]*g2 + a[1]*g3;

	   for (i = 0; i < n; ++i)
		   a[i] = tmp[i];
      }
   }

   /**
     Inverse Daubechies D4 transform
    */
   void invTransform( vector <double> &a, const int n )
   {
     if (n >= 4) {
       int i, j;
       const int half = n >> 1;
       const int halfPls1 = half + 1;

       vector <double> tmp(n,0);
       //      last smooth val  last coef.  first smooth  first coef
       tmp[0] = a[half-1]*Ih0 + a[n-1]*Ih1 + a[0]*Ih2 + a[half]*Ih3;
       tmp[1] = a[half-1]*Ig0 + a[n-1]*Ig1 + a[0]*Ig2 + a[half]*Ig3;
       for (i = 0, j = 2; i < half-1; i++) {
	 //     smooth val     coef. val       smooth val    coef. val
			tmp[j++] = a[i]*Ih0 + a[i+half]*Ih1 + a[i+1]*Ih2 + a[i+halfPls1]*Ih3;
			tmp[j++] = a[i]*Ig0 + a[i+half]*Ig1 + a[i+1]*Ig2 + a[i+halfPls1]*Ig3;
       }

	   for (i = 0; i < n; ++i)
		   a[i] = tmp[i];
     }
   }

   public:

   Daubechies() 
   {
    //  const double sqrt_3 = 1.7320508075688772935274463415059;
    //  const double denom = 5.6568542494923801952067548968388;

      //
      // forward transform scaling (smoothing) coefficients
      //
      h0 = 0.482962913144533;
      h1 = 0.836516303737807;
      h2 = 0.224143868042015;
      h3 = -0.129409522551259;

      //h0 = (1 + sqrt_3)/denom;
      //h1 = (3 + sqrt_3)/denom;
      //h2 = (3 - sqrt_3)/denom;
      //h3 = (1 - sqrt_3)/denom;

      //
      // forward transform wavelet coefficients
      //
      g0 =  h3;
      g1 = -h2;
      g2 =  h1;
      g3 = -h0;

      Ih0 = h2;
      Ih1 = g2;  // h1
      Ih2 = h0;
      Ih3 = g0;  // h3

      Ig0 = h3;
      Ig1 = g3;  // -h0
      Ig2 = h1;
      Ig3 = g1;  // -h2
   }

   void daubTrans( vector <double> &spec, int N )
   {
      int n;
      for (n = N; n >= 4; n >>= 1) {
         transform( spec, n );
      }
   }


   void invDaubTrans( vector <double> &wav, int N )
   {
      int n;
      for (n = 4; n <= N; n <<= 1) {
         invTransform( wav, n );
      }
   }

   void threshold(vector <double> &wav,int diff,int bits, double &sens)
   {
	   int n = wav.size();
	   vector<double> tmp;
	   vector<double> thres;
	   for (unsigned int i = 0; i < n-floor(diff*0.5); ++i){
			tmp.push_back(fabs(wav.at(i)));
	   }
	   sort(tmp.begin(), tmp.end(), GreaterThan());
	   while (tmp.back() == 0)
		   tmp.pop_back();
	   //int totdiffs = 0;
	   //for (unsigned int i = 1; i < bits; ++i){
		  // totdiffs += diff/(pow(2,i+1));
	   //}
	   //tmp.resize(tmp.size()-totdiffs);
	   double median = tmp.at((int)((tmp.size()-1)*sens)-1);
	   double t = sqrt(2*log((double)n))*median;
	   for (unsigned int i = 0; i < n; ++i){
		   double res = fabs(wav[i])-t;
		   res = (res + fabs(res))*0.5;
		   if (wav[i] > 0) wav[i] = 1;
		   else if (wav[i] < 0) wav[i] = -1;
		   wav[i] *= res;
	   }

	 //  for (unsigned int j = 0; j < 3; ++j){
		//   tmp.clear();
		//   double sfrac_last = 1/pow(2,j);
		//   double sfrac = 1/pow(2,j+1);
		//	for (unsigned int i = n*sfrac; i < (n*sfrac_last)-floor(diff*sfrac); ++i){
		//		tmp.push_back(wav.at(i));
		//	}
		//	sort(tmp.begin(), tmp.end());
		//	variance = tmp.at((int)(tmp.size()*sens));
		//	thres.push_back((fabs(variance)*sqrt(2*log(tmp.size()))/sqrt(tmp.size()));
	 //  }
		//double tval = thres.at(2);
		//for (unsigned int i = 2; i < n-2; ++i)
		//{
		//	if (fabs(wav[i-2]) <= tval&&fabs(wav[i-1]) <= tval && fabs(wav[i]) <= tval && fabs(wav[i+1]) <= tval&& fabs(wav[i+2]) <= tval)
		//		wav[i] = 0;
		//	if (i >= n>>1)
		//		tval = thres.at(0);
		//	else if (i >= n*0.25)
		//		tval = thres.at(1);
		//}
   }

}; // Daubechies

#endif /* _DAUB_H_ */
