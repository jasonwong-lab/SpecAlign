/////////////////////////////////////////////////////////////////////////////////
// 
//  Demonstration driver program for the Levenberg - Marquardt minimization
//  algorithm
//  Copyright (C) 2004-05  Manolis Lourakis (lourakis@ics.forth.gr)
//  Institute of Computer Science, Foundation for Research & Technology - Hellas
//  Heraklion, Crete, Greece.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
/////////////////////////////////////////////////////////////////////////////////

/******************************************************************************** 
 * Levenberg-Marquardt minimization demo driver. Only the double precision versions
 * are tested here. See the Meyer case for an example of verifying the jacobian 
 ********************************************************************************/

#include <stdio.h>

#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "lm.h"

struct mydata{
   double x[200];
   double y[200];
   double amp1;
   double amp2;
};

/* Sample functions to be minimized with LM and their jacobians.
 * More test functions at http://www.csit.fsu.edu/~burkardt/f_src/test_nls/test_nls.html
 * Check also the CUTE problems collection at ftp://ftp.numerical.rl.ac.uk/pub/cute/;
 * CUTE is searchable through http://numawww.mathematik.tu-darmstadt.de:8081/opti/select.html
 * CUTE problems can also be solved through the AMPL web interface at http://www.ampl.com/TRYAMPL/startup.html
 */

#define ROSD 105.0

void gauss(double *p, double *x, int m, int n, void *data)
{
	register int i;
	struct mydata *dptr;

	dptr=(struct mydata *)data; // adata is passed as void *

	for(i=0; i<n; ++i)
		x[i]=p[0]*exp(-pow(((dptr->y[i]-p[1])/p[2]),2)/2)-dptr->x[i];
}

void gauss2(double *p, double *x, int m, int n, void *data)
{
	register int i;
	struct mydata *dptr;

	dptr=(struct mydata *)data; // adata is passed as void *

	for(i=0; i<n; ++i)
		x[i]=(p[0]*exp(-pow(((dptr->y[i]-p[1])/p[2]),2)/2)+p[3]*exp(-pow(((dptr->y[i]-p[4])/p[5]),2)/2))-dptr->x[i];
}

void gauss3(double *p, double *x, int m, int n, void *data)
{
	register int i;
	struct mydata *dptr;

	dptr=(struct mydata *)data; // adata is passed as void *

	for(i=0; i<n; ++i)
		x[i]=(p[0]*exp(-pow(((dptr->y[i]-p[1])/p[2]),2)/2)+p[3]*exp(-pow(((dptr->y[i]-p[4])/p[5]),2)/2)+p[6]*exp(-pow(((dptr->y[i]-p[7])/p[8]),2)/2))-dptr->x[i];
}

/* Rosenbrock function, global minimum at (1, 1) */
void ros(double *p, double *x, int m, int n, void *data)
{
register int i;

  for(i=0; i<n; ++i)
    x[i]=((1.0-p[0])*(1.0-p[0]) + ROSD*(p[1]-p[0]*p[0])*(p[1]-p[0]*p[0]));
}

void jacros(double *p, double *jac, int m, int n, void *data)
{
register int i, j;

  for(i=j=0; i<n; ++i){
    jac[j++]=(-2 + 2*p[0]-4*ROSD*(p[1]-p[0]*p[0])*p[0]);
    jac[j++]=(2*ROSD*(p[1]-p[0]*p[0]));
  }
}


#define MODROSLAM 1E02
/* Modified Rosenbrock problem, global minimum at (1, 1) */
void modros(double *p, double *x, int m, int n, void *data)
{
register int i;

  for(i=0; i<n; i+=3){
    x[i]=10*(p[1]-p[0]*p[0]);
	  x[i+1]=1.0-p[0];
	  x[i+2]=MODROSLAM;
  }
}

void jacmodros(double *p, double *jac, int m, int n, void *data)
{
register int i, j;

  for(i=j=0; i<n; i+=3){
    jac[j++]=-20.0*p[0];
	  jac[j++]=10.0;

	  jac[j++]=-1.0;
	  jac[j++]=0.0;

	  jac[j++]=0.0;
	  jac[j++]=0.0;
  }
}


/* Powell's function, minimum at (0, 0) */
void powell(double *p, double *x, int m, int n, void *data)
{
register int i;

  for(i=0; i<n; i+=2){
    x[i]=p[0];
    x[i+1]=10.0*p[0]/(p[0]+0.1) + 2*p[1]*p[1];
  }
}

void jacpowell(double *p, double *jac, int m, int n, void *data)
{
register int i, j;

  for(i=j=0; i<n; i+=2){
    jac[j++]=1.0;
    jac[j++]=0.0;

    jac[j++]=1.0/((p[0]+0.1)*(p[0]+0.1));
    jac[j++]=4.0*p[1];
  }
}

/* Wood's function, minimum at (1, 1, 1, 1) */
void wood(double *p, double *x, int m, int n, void *data)
{
register int i;

  for(i=0; i<n; i+=6){
    x[i]=10.0*(p[1] - p[0]*p[0]);
    x[i+1]=1.0 - p[0];
    x[i+2]=sqrt(90.0)*(p[3] - p[2]*p[2]);
    x[i+3]=1.0 - p[2];
    x[i+4]=sqrt(10.0)*(p[1]+p[3] - 2.0);
    x[i+5]=(p[1] - p[3])/sqrt(10.0);
  }
}

/* Meyer's (reformulated) problem, minimum at (2.48, 6.18, 3.45) */
void meyer(double *p, double *x, int m, int n, void *data)
{
register int i;
double ui;

	for(i=0; i<n; ++i){
		ui=0.45+0.05*i;
		x[i]=p[0]*exp(10.0*p[1]/(ui+p[2]) - 13.0);
	}
}

void jacmeyer(double *p, double *jac, int m, int n, void *data)
{
register int i, j;
double ui, tmp;

  for(i=j=0; i<n; ++i){
	  ui=0.45+0.05*i;
	  tmp=exp(10.0*p[1]/(ui+p[2]) - 13.0);

	  jac[j++]=tmp;
	  jac[j++]=10.0*p[0]*tmp/(ui+p[2]);
	  jac[j++]=-10.0*p[0]*p[1]*tmp/((ui+p[2])*(ui+p[2]));
  }
}

/* helical valley function, minimum at (1.0, 0.0, 0.0) */
#ifndef M_PI
#define M_PI   3.14159265358979323846  /* pi */
#endif

void helval(double *p, double *x, int m, int n, void *data)
{
double theta;

  if(p[0]<0.0)
     theta=atan(p[1]/p[0])/(2.0*M_PI) + 0.5;
  else if(0.0<p[0])
     theta=atan(p[1]/p[0])/(2.0*M_PI);
  else 
    theta=(p[1]>=0)? 0.25 : -0.25;

  x[0]=10.0*(p[2] - 10.0*theta);
  x[1]=10.0*(sqrt(p[0]*p[0] + p[1]*p[1]) - 1.0);
  x[2]=p[2];
}

void jachelval(double *p, double *jac, int m, int n, void *data)
{
register int i=0;
double tmp;

  tmp=p[0]*p[0] + p[1]*p[1];

  jac[i++]=50.0*p[1]/(M_PI*tmp);
  jac[i++]=-50.0*p[0]/(M_PI*tmp);
  jac[i++]=10.0;

  jac[i++]=10.0*p[0]/sqrt(tmp);
  jac[i++]=10.0*p[1]/sqrt(tmp);
  jac[i++]=0.0;

  jac[i++]=0.0;
  jac[i++]=0.0;
  jac[i++]=1.0;
}

/* Boggs - Tolle problem 3 (linearly constrained), minimum at (-0.76744, 0.25581, 0.62791, -0.11628, 0.25581)
 * constr1: p[0] + 3*p[1] = 0;
 * constr2: p[2] + p[3] - 2*p[4] = 0;
 * constr3: p[1] - p[4] = 0;
 */
void bt3(double *p, double *x, int m, int n, void *data)
{
register int i;
double t1, t2, t3, t4;

  t1=p[0]-p[1];
  t2=p[1]+p[2]-2.0;
  t3=p[3]-1.0;
  t4=p[4]-1.0;

  for(i=0; i<n; ++i)
    x[i]=t1*t1 + t2*t2 + t3*t3 + t4*t4;
}

void jacbt3(double *p, double *jac, int m, int n, void *data)
{
register int i, j;
double t1, t2, t3, t4;

  t1=p[0]-p[1];
  t2=p[1]+p[2]-2.0;
  t3=p[3]-1.0;
  t4=p[4]-1.0;

  for(i=j=0; i<n; ++i){
    jac[j++]=2.0*t1;
    jac[j++]=2.0*(t2-t1);
    jac[j++]=2.0*t2;
    jac[j++]=2.0*t3;
    jac[j++]=2.0*t4;
  }
}

/* Hock - Schittkowski problem 28 (linearly constrained), minimum at (0.5, -0.5, 0.5)
 * constr1: p[0] + 2*p[1] + 3*p[2] = 1;
 */
void hs28(double *p, double *x, int m, int n, void *data)
{
register int i;
double t1, t2;

  t1=p[0]+p[1];
  t2=p[1]+p[2];

  for(i=0; i<n; ++i)
    x[i]=t1*t1 + t2*t2;
}

void jachs28(double *p, double *jac, int m, int n, void *data)
{
register int i, j;
double t1, t2;

  t1=p[0]+p[1];
  t2=p[1]+p[2];

  for(i=j=0; i<n; ++i){
    jac[j++]=2.0*t1;
    jac[j++]=2.0*(t1+t2);
    jac[j++]=2.0*t2;
  }
}

/* Hock - Schittkowski problem 48 (linearly constrained), minimum at (1.0, 1.0, 1.0, 1.0, 1.0)
 * constr1: sum {i in 0..4} p[i] = 5;
 * constr2: p[2] - 2*(p[3]+p[4]) = -3;
 */
void hs48(double *p, double *x, int m, int n, void *data)
{
register int i;
double t1, t2, t3;

  t1=p[0]-1.0;
  t2=p[1]-p[2];
  t3=p[3]-p[4];

  for(i=0; i<n; ++i)
    x[i]=t1*t1 + t2*t2 + t3*t3;
}

void jachs48(double *p, double *jac, int m, int n, void *data)
{
register int i, j;
double t1, t2, t3;

  t1=p[0]-1.0;
  t2=p[1]-p[2];
  t3=p[3]-p[4];

  for(i=j=0; i<n; ++i){
    jac[j++]=2.0*t1;
    jac[j++]=2.0*t2;
    jac[j++]=-2.0*t2;
    jac[j++]=2.0*t3;
    jac[j++]=-2.0*t3;
  }
}

/* Hock - Schittkowski problem 51 (linearly constrained), minimum at (1.0, 1.0, 1.0, 1.0, 1.0)
 * constr1: p[0] + 3*p[1] = 4;
 * constr2: p[2] + p[3] - 2*p[4] = 0;
 * constr3: p[1] - p[4] = 0;
 */
void hs51(double *p, double *x, int m, int n, void *data)
{
register int i;
double t1, t2, t3, t4;

  t1=p[0]-p[1];
  t2=p[1]+p[2]-2.0;
  t3=p[3]-1.0;
  t4=p[4]-1.0;

  for(i=0; i<n; ++i)
    x[i]=t1*t1 + t2*t2 + t3*t3 + t4*t4;
}

void jachs51(double *p, double *jac, int m, int n, void *data)
{
register int i, j;
double t1, t2, t3, t4;

  t1=p[0]-p[1];
  t2=p[1]+p[2]-2.0;
  t3=p[3]-1.0;
  t4=p[4]-1.0;

  for(i=j=0; i<n; ++i){
    jac[j++]=2.0*t1;
    jac[j++]=2.0*(t2-t1);
    jac[j++]=2.0*t2;
    jac[j++]=2.0*t3;
    jac[j++]=2.0*t4;
  }
}

/* Hock - Schittkowski problem 01 (box constrained), minimum at (1.0, 1.0)
 * constr1: p[1]>=-1.5;
 */
void hs01(double *p, double *x, int m, int n, void *data)
{
double t;

  t=p[0]*p[0];
  x[0]=10.0*(p[1]-t);
  x[1]=1.0-p[0];
}

void jachs01(double *p, double *jac, int m, int n, void *data)
{
register int j=0;

  jac[j++]=-20.0*p[0];
  jac[j++]=10.0;

  jac[j++]=-1.0;
  jac[j++]=0.0;
}

/* Hock - Schittkowski MODIFIED problem 21 (box constrained), minimum at (2.0, 0.0)
 * constr1: 2 <= p[0] <=50;
 * constr2: -50 <= p[1] <=50;
 *
 * Original HS21 has the additional contraint 10*p[0] - p[1] >= 10; which is inactive
 * at the solution, so it is dropped here.
 */
void hs21(double *p, double *x, int m, int n, void *data)
{
  x[0]=p[0]/10.0;
  x[1]=p[1];
}

void jachs21(double *p, double *jac, int m, int n, void *data)
{
register int j=0;

  jac[j++]=0.1;
  jac[j++]=0.0;

  jac[j++]=0.0;
  jac[j++]=1.0;
}

/* Problem hatfldb (box constrained), minimum at (0.947214, 0.8, 0.64, 0.4096)
 * constri: p[i]>=0.0; (i=1..4)
 * constr5: p[1]<=0.8;
 */
void hatfldb(double *p, double *x, int m, int n, void *data)
{
register int i;

  x[0]=p[0]-1.0;

  for(i=1; i<m; ++i)
     x[i]=p[i-1]-sqrt(p[i]);
}

void jachatfldb(double *p, double *jac, int m, int n, void *data)
{
register int j=0;

  jac[j++]=1.0;
  jac[j++]=0.0;
  jac[j++]=0.0;
  jac[j++]=0.0;

  jac[j++]=1.0;
  jac[j++]=-0.5/sqrt(p[1]);
  jac[j++]=0.0;
  jac[j++]=0.0;

  jac[j++]=0.0;
  jac[j++]=1.0;
  jac[j++]=-0.5/sqrt(p[2]);
  jac[j++]=0.0;

  jac[j++]=0.0;
  jac[j++]=0.0;
  jac[j++]=1.0;
  jac[j++]=-0.5/sqrt(p[3]);
}

/* Problem hatfldc (box constrained), minimum at (1.0, 1.0, 1.0, 1.0)
 * constri: p[i]>=0.0; (i=1..4)
 * constri+4: p[i]<=10.0; (i=1..4)
 */
void hatfldc(double *p, double *x, int m, int n, void *data)
{
register int i;

  x[0]=p[0]-1.0;

  for(i=1; i<m-1; ++i)
     x[i]=p[i-1]-sqrt(p[i]);

  x[m-1]=p[m-1]-1.0;
}

void jachatfldc(double *p, double *jac, int m, int n, void *data)
{
register int j=0;

  jac[j++]=1.0;
  jac[j++]=0.0;
  jac[j++]=0.0;
  jac[j++]=0.0;

  jac[j++]=1.0;
  jac[j++]=-0.5/sqrt(p[1]);
  jac[j++]=0.0;
  jac[j++]=0.0;

  jac[j++]=0.0;
  jac[j++]=1.0;
  jac[j++]=-0.5/sqrt(p[2]);
  jac[j++]=0.0;

  jac[j++]=0.0;
  jac[j++]=0.0;
  jac[j++]=0.0;
  jac[j++]=1.0;
}

/* Equilibrium combustion problem, constrained nonlinear equation from the book by Floudas et al.
 * Minimum at (0.0034, 31.3265, 0.0684, 0.8595, 0.0370)
 * constri: p[i]>=0.0001; (i=1..5)
 * constri+5: p[i]<=100.0; (i=1..5)
 */
void combust(double *p, double *x, int m, int n, void *data)
{
  double R, R5, R6, R7, R8, R9, R10;

  R=10;
  R5=0.193;
  R6=4.10622*1e-4;
  R7=5.45177*1e-4;
  R8=4.4975*1e-7;
  R9=3.40735*1e-5;
  R10=9.615*1e-7;

  x[0]=p[0]*p[1]+p[0]-3*p[4];
  x[1]=2*p[0]*p[1]+p[0]+3*R10*p[1]*p[1]+p[1]*p[2]*p[2]+R7*p[1]*p[2]+R9*p[1]*p[3]+R8*p[1]-R*p[4];
  x[2]=2*p[1]*p[2]*p[2]+R7*p[1]*p[2]+2*R5*p[2]*p[2]+R6*p[2]-8*p[4];
  x[3]=R9*p[1]*p[3]+2*p[3]*p[3]-4*R*p[4];
  x[4]=p[0]*p[1]+p[0]+R10*p[1]*p[1]+p[1]*p[2]*p[2]+R7*p[1]*p[2]+R9*p[1]*p[3]+R8*p[1]+R5*p[2]*p[2]+R6*p[2]+p[3]*p[3]-1.0;
}

void jaccombust(double *p, double *jac, int m, int n, void *data)
{
register int j=0;
  double R, R5, R6, R7, R8, R9, R10;

  R=10;
  R5=0.193;
  R6=4.10622*1e-4;
  R7=5.45177*1e-4;
  R8=4.4975*1e-7;
  R9=3.40735*1e-5;
  R10=9.615*1e-7;

  for(j=0; j<m*n; ++j) jac[j]=0.0;

  j=0;
  jac[j]=p[1]+1;
  jac[j+1]=p[0];
  jac[j+4]=-3;

  j+=m;
  jac[j]=2*p[1]+1;
  jac[j+1]=2*p[0]+6*R10*p[1]+p[2]*p[2]+R7*p[2]+R9*p[3]+R8;
  jac[j+2]=2*p[1]*p[2]+R7*p[1];
  jac[j+3]=R9*p[1];
  jac[j+4]=-R;

  j+=m;
  jac[j+1]=2*p[2]*p[2]+R7*p[2];
  jac[j+2]=4*p[1]*p[2]+R7*p[1]+4*R5*p[2]+R6;
  jac[j+4]=-8;

  j+=m;
  jac[j+1]=R9*p[3];
  jac[j+3]=R9*p[1]+4*p[3];
  jac[j+4]=-4*R;

  j+=m;
  jac[j]=p[1]+1;
  jac[j+1]=p[0]+2*R10*p[1]+p[2]*p[2]+R7*p[2]+R9*p[3]+R8;
  jac[j+2]=2*p[1]*p[2]+R7*p[1]+2*R5*p[2]+R6;
  jac[j+3]=R9*p[1]+2*p[3];
}



int main()
{
register int i, j;
int problem, ret;
double p[9], // 6 is max(2, 3, 5)
	   x[122]; // 16 is max(2, 3, 5, 6, 16)
int m, n;
struct mydata d;
double opts[LM_OPTS_SZ], info[LM_INFO_SZ];
char *probname[]={
    "Rosenbrock function",
    "modified Rosenbrock problem",
    "Powell's function",
    "Wood's function",
    "Meyer's (reformulated) problem",
    "helical valley function",
    "Boggs & Tolle's problem #3",
    "Hock - Schittkowski problem #28",
    "Hock - Schittkowski problem #48",
    "Hock - Schittkowski problem #51",
    "Hock - Schittkowski problem #01",
    "Hock - Schittkowski (modified) problem #21",
    "hatfldb problem",
    "hatfldc problem",
    "equilibrium combustion problem"
};

  opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
  opts[4]=LM_DIFF_DELTA; // relevant only if the finite difference jacobian version is used 


  /* uncomment the appropriate line below to select a minimization problem */
  problem=
		  0; // Rosenbrock function
		  //1; // modified Rosenbrock problem
		  //2; // Powell's function
      //3; // Wood's function
		  //4; // Meyer's (reformulated) problem
     // 5; // helical valley function
#ifdef HAVE_LAPACK
      //6; // Boggs & Tolle's problem 3
      //7; // Hock - Schittkowski problem 28
      //8; // Hock - Schittkowski problem 48
      //9; // Hock - Schittkowski problem 51
#else // no LAPACK
#ifdef _MSC_VER
#pragma message("LAPACK not available, some test problems cannot be used")
#else
#warning LAPACK not available, some test problems cannot be used
#endif // _MSC_VER

#endif /* HAVE_LAPACK */
      //10; // Hock - Schittkowski problem 01
      //11; // Hock - Schittkowski (modified) problem 21
      //12; // hatfldb problem
      //13; // hatfldc problem
      //14; // equilibrium combustion problem
				
  switch(problem){
  default: fprintf(stderr, "unknown problem specified (#%d)! Note that some minimization problems require LAPACK.\n", problem);
           exit(1);
    break;
  case 0:
	/*Gaussian function*/
	  m=9; n=122;

	  d.y[0] = 3468.91;
	  d.y[1] = 3470.01;
	  d.y[2] = 3471.11;
d.y[3] =3472.21;
d.y[4] =3473.31;
d.y[5] =3474.41;
d.y[6] =3475.51;
d.y[7] =3476.61;
d.y[8] =3477.71;
d.y[9] =3478.81;
d.y[10] =3479.91;
d.y[11] =3481.01;
d.y[12] =3482.11;
d.y[13] =3483.22;
d.y[14] =3484.32;
d.y[15] =3485.42;
d.y[16] =3486.52;
d.y[17] =3487.62;
d.y[18] =3488.73;
d.y[19] =3489.83;
d.y[20] =3490.93;
d.y[21] =3492.03;
d.y[22] =3493.14;
d.y[23] =3494.24;
d.y[24] =3495.34;
d.y[25] =3496.45;
d.y[26] =3497.55;
d.y[27] =3498.65;
d.y[28] =3499.76;
d.y[29] =3500.86;
d.y[30] =3501.97;
d.y[31] =3503.07;
d.y[32] =3504.18;
d.y[33] =3505.28;
d.y[34] =3506.39;
d.y[35] =3507.49;
d.y[36] =3508.6;
d.y[37] =3509.7;
d.y[38] =3510.81;
d.y[39] =3511.92;
d.y[40] =3513.02;
d.y[41] =3514.13;
d.y[42] =3515.23;
d.y[43] =3516.34;
d.y[44] =3517.45;
d.y[45] =3518.56;
d.y[46] =3519.66;
d.y[47] =3520.77;
d.y[48] =3521.88;
d.y[49] =3522.99;
d.y[50] =3524.09;
d.y[51] =3525.2;
d.y[52] =3526.31;
d.y[53] =3527.42;
d.y[54] =3528.53;
d.y[55] =3529.64;
d.y[56] =3530.74;
d.y[57] =3531.85;
d.y[58] =3532.96;
d.y[59] =3534.07;
d.y[60] =3535.18;
d.y[61] =3536.29;
d.y[62] =3537.4;
d.y[63] =3538.51;
d.y[64] =3539.62;
d.y[65] =3540.73;
d.y[66] =3541.84;
d.y[67] =3542.95;
d.y[68] =3544.07;
d.y[69] =3545.18;
d.y[70] =3546.29;
d.y[71] =3547.4;
d.y[72] =3548.51;
d.y[73] =3549.62;
d.y[74] =3550.74;
d.y[75] =3551.85;
d.y[76] =3552.96;
d.y[77] =3554.07;
d.y[78] =3555.19;
d.y[79] =3556.3;
d.y[80] =3557.41;
d.y[81] =3558.53;
d.y[82] =3559.64;
d.y[83] =3560.75;
d.y[84] =3561.87;
d.y[85] =3562.98;
d.y[86] =3564.09;
d.y[87] =3565.21;
d.y[88] =3566.32;
d.y[89] =3567.44;
d.y[90] =3568.55;
d.y[91] =3569.67;
d.y[92] =3570.78;
d.y[93] =3571.9;
d.y[94] =3573.01;
d.y[95] =3574.13;
d.y[96] =3575.25;
d.y[97] =3576.36;
d.y[98] =3577.48;
d.y[99] =3578.6;
d.y[100] =3579.71;
d.y[101] =3580.83;
d.y[102] =3581.95;
d.y[103] =3583.06;
d.y[104] =3584.18;
d.y[105] =3585.3;
d.y[106] =3586.42;
d.y[107] =3587.53;
d.y[108] =3588.65;
d.y[109] =3589.77;
d.y[110] =3590.89;
d.y[111] =3592.01;
d.y[112] =3593.13;
d.y[113] =3594.24;
d.y[114] =3595.36;
d.y[115] =3596.48;
d.y[116] =3597.6;
d.y[117] =3598.72;
d.y[118] =3599.84;
d.y[119] =3600.96;
d.y[120] =3602.08;
d.y[121] =3603.2;

d.x[0] = 8.32177;
d.x[1] =8.5088;
d.x[2] =8.58723;
d.x[3] =8.42232;
d.x[4] =8.1629;
d.x[5] =8;
d.x[6] =7.83509;
d.x[7] =7.80895;
d.x[8] =7.80694;
d.x[9] =7.67019;
d.x[10] =7.72851;
d.x[11] =7.77878;
d.x[12] =7.76471;
d.x[13] =7.78281;
d.x[14] =7.71443;
d.x[15] =7.71041;
d.x[16] =7.73655;
d.x[17] =7.6903;
d.x[18] =7.79085;
d.x[19] =7.88939;
d.x[20] =7.88939;
d.x[21] =8.00402;
d.x[22] =8.0905;
d.x[23] =8.24334;
d.x[24] =8.46657;
d.x[25] =8.59125;
d.x[26] =8.74208;
d.x[27] =8.91101;
d.x[28] =9.31322;
d.x[29] =9.96682;
d.x[30] =10.6828;
d.x[31] =10.8698;
d.x[32] =10.5943;
d.x[33] =10.19;
d.x[34] =10.1036;
d.x[35] =10.1699;
d.x[36] =10.361;
d.x[37] =10.6506;
d.x[38] =11.0628;
d.x[39] =11.3826;
d.x[40] =11.6461;
d.x[41] =12.2896;
d.x[42] =13.0739;
d.x[43] =14.0774;
d.x[44] =14.5822;
d.x[45] =14.8758;
d.x[46] =15.1755;
d.x[47] =15.7205;
d.x[48] =17.4741;
d.x[49] =22.1176;
d.x[50] =28.9935;
d.x[51] =33.7697;
d.x[52] =33.1001;
d.x[53] =29.4761;
d.x[54] =25.9588;
d.x[55] =23.7949;
d.x[56] =23.3746;
d.x[57] =24.5329;
d.x[58] =25.7657;
d.x[59] =25.7697;
d.x[60] =24.3338;
d.x[61] =22.7612;
d.x[62] =21.5023;
d.x[63] =21.0055;
d.x[64] =21.7798;
d.x[65] =23.3826;
d.x[66] =24.3298;
d.x[67] =23.3082;
d.x[68] =21.4862;
d.x[69] =19.4731;
d.x[70] =17.8944;
d.x[71] =16.7662;
d.x[72] =15.8834;
d.x[73] =15.4248;
d.x[74] =14.725;
d.x[75] =14.2504;
d.x[76] =13.9507;
d.x[77] =13.6853;
d.x[78] =13.4379;
d.x[79] =12.8788;
d.x[80] =12.4525;
d.x[81] =12.0804;
d.x[82] =11.7567;
d.x[83] =11.3363;
d.x[84] =11.1875;
d.x[85] =10.8617;
d.x[86] =10.4113;
d.x[87] =10.4274;
d.x[88] =10.1478;
d.x[89] =9.90447;
d.x[90] =9.76571;
d.x[91] =9.53846;
d.x[92] =9.6008;
d.x[93] =9.42986;
d.x[94] =9.47009;
d.x[95] =9.68125;
d.x[96] =10.0171;
d.x[97] =10.0412;
d.x[98] =10.0754;
d.x[99] =9.98894;
d.x[100] =9.80593;
d.x[101] =9.6732;
d.x[102] =9.47813;
d.x[103] =9.24686;
d.x[104] =9.12217;
d.x[105] =9.09201;
d.x[106] =9.27903;
d.x[107] =9.33937;
d.x[108] =9.48215;
d.x[109] =9.67119;
d.x[110] =9.64103;
d.x[111] =9.80191;
d.x[112] =9.64907;
d.x[113] =9.40573;
d.x[114] =9.36752;
d.x[115] =9.53444;
d.x[116] =9.61689;
d.x[117] =9.7094;
d.x[118] =9.71543;
d.x[119] =9.6732;
d.x[120] =9.51433;
d.x[121] =9.46405;

	  p[0]=30; p[1]=3527.42; p[2]=10; p[3]=25; p[4]=3534 ;p[5]=10; p[6]=24.3; p[7]=3541.84; p[8]=10;
	  for(i=0; i<n; i++){
		  x[i]=0.0;
		  d.x[i]-=10;
	  }
	  ret=dlevmar_dif(gauss3,p,x,m,n,1000,opts,info,NULL,NULL,(void *)&d);

  /* Rosenbrock function */
//    m=2; n=2;
//    p[0]=-1.2; p[1]=1.0;
//    for(i=0; i<n; i++) x[i]=0.0;
//    ret=dlevmar_der(ros, jacros, p, x, m, n, 1000, opts, info, NULL, NULL, NULL); // with analytic jacobian
    //ret=dlevmar_dif(ros, p, x, m, n, 1000, opts, info, NULL, NULL, NULL);  // no jacobian
  break;

  case 1:
  /* modified Rosenbrock problem */
    m=2; n=3;
    p[0]=-1.2; p[1]=1.0;
    for(i=0; i<n; i++) x[i]=0.0;
    ret=dlevmar_der(modros, jacmodros, p, x, m, n, 1000, opts, info, NULL, NULL, NULL); // with analytic jacobian
    //ret=dlevmar_dif(modros, p, x, m, n, 1000, opts, info, NULL, NULL, NULL);  // no jacobian
  break;

  case 2:
  /* Powell's function */
    m=2; n=2;
    p[0]=3.0; p[1]=1.0;
    for(i=0; i<n; i++) x[i]=0.0;
    ret=dlevmar_der(powell, jacpowell, p, x, m, n, 1000, opts, info, NULL, NULL, NULL); // with analytic jacobian
    //ret=dlevmar_dif(powell, p, x, m, n, 1000, opts, info, NULL, NULL, NULL);		// no jacobian
  break;

  case 3:
  /* Woods's function */
    m=4; n=6;
    p[0]=-3.0; p[1]=-1.0; p[2]=-3.0; p[3]=-1.0;
    for(i=0; i<n; i++) x[i]=0.0;
    ret=dlevmar_dif(wood, p, x, m, n, 1000, opts, info, NULL, NULL, NULL);  // no jacobian
  break;

  case 4:
  /* Meyer's data fitting problem */
    m=3; n=16;
    p[0]=8.85; p[1]=4.0; p[2]=2.5;
    x[0]=34.780;	x[1]=28.610; x[2]=23.650; x[3]=19.630;
    x[4]=16.370;	x[5]=13.720; x[6]=11.540; x[7]=9.744;
    x[8]=8.261;	x[9]=7.030; x[10]=6.005; x[11]=5.147;
    x[12]=4.427;	x[13]=3.820; x[14]=3.307; x[15]=2.872;
   // ret=dlevmar_der(meyer, jacmeyer, p, x, m, n, 1000, opts, info, NULL, NULL, NULL); // with analytic jacobian

   { double *work, *covar;
    work=malloc((LM_DIF_WORKSZ(m, n)+m*m)*sizeof(double));
    if(!work){
    	fprintf(stderr, "memory allocation request failed in main()\n");
      exit(1);
    }
    covar=work+LM_DIF_WORKSZ(m, n);

    ret=dlevmar_dif(meyer, p, x, m, n, 1000, opts, info, work, covar, NULL); // no jacobian, caller allocates work memory, covariance estimated

    printf("Covariance of the fit:\n");
    for(i=0; i<m; ++i){
      for(j=0; j<m; ++j)
        printf("%g ", covar[i*m+j]);
      printf("\n");
    }
    printf("\n");

    free(work);
   }

/* uncomment the following block to verify jacobian */
/*
   {
    double err[16];
    dlevmar_chkjac(meyer, jacmeyer, p, m, n, NULL, err); 
    for(i=0; i<n; ++i) printf("gradient %d, err %g\n", i, err[i]);
   }
*/

  break;

  case 5:
  /* helical valley function */
    m=3; n=3;
    p[0]=-1.0; p[1]=0.0; p[2]=0.0;
    for(i=0; i<n; i++) x[i]=0.0;
    ret=dlevmar_der(helval, jachelval, p, x, m, n, 1000, opts, info, NULL, NULL, NULL); // with analytic jacobian
    //ret=dlevmar_dif(helval, p, x, m, n, 1000, opts, info, NULL, NULL, NULL);  // no jacobian
  break;

#ifdef HAVE_LAPACK
  case 6:
  /* Boggs-Tolle problem 3 */
    m=5; n=5;
    p[0]=2.0; p[1]=2.0; p[2]=2.0;
    p[3]=2.0; p[4]=2.0;
    for(i=0; i<n; i++) x[i]=0.0;

    {
      double A[3*5]={1.0, 3.0, 0.0, 0.0, 0.0,  0.0, 0.0, 1.0, 1.0, -2.0,  0.0, 1.0, 0.0, 0.0, -1.0},
             b[3]={0.0, 0.0, 0.0};

    ret=dlevmar_lec_der(bt3, jacbt3, p, x, m, n, A, b, 3, 1000, opts, info, NULL, NULL, NULL); // lin. constraints, analytic jacobian
    //ret=dlevmar_lec_dif(bt3, p, x, m, n, A, b, 3, 1000, opts, info, NULL, NULL, NULL); // lin. constraints, no jacobian
    }
  break;
  case 7:
  /* Hock - Schittkowski problem 28 */
    m=3; n=3;
    p[0]=-4.0; p[1]=1.0; p[2]=1.0;
    for(i=0; i<n; i++) x[i]=0.0;

    {
      double A[1*3]={1.0, 2.0, 3.0},
             b[1]={1.0};

    ret=dlevmar_lec_der(hs28, jachs28, p, x, m, n, A, b, 1, 1000, opts, info, NULL, NULL, NULL); // lin. constraints, analytic jacobian
    //ret=dlevmar_lec_dif(hs28, p, x, m, n, A, b, 1, 1000, opts, info, NULL, NULL, NULL); // lin. constraints, no jacobian
    }
  break;
  case 8:
  /* Hock - Schittkowski problem 48 */
    m=5; n=5;
    p[0]=3.0; p[1]=5.0; p[2]=-3.0;
    p[3]=2.0; p[4]=-2.0;
    for(i=0; i<n; i++) x[i]=0.0;

    {
      double A[2*5]={1.0, 1.0, 1.0, 1.0, 1.0,  0.0, 0.0, 1.0, -2.0, -2.0},
             b[2]={5.0, -3.0};

    ret=dlevmar_lec_der(hs48, jachs48, p, x, m, n, A, b, 2, 1000, opts, info, NULL, NULL, NULL); // lin. constraints, analytic jacobian
    //ret=dlevmar_lec_dif(hs48, p, x, m, n, A, b, 2, 1000, opts, info, NULL, NULL, NULL); // lin. constraints, no jacobian
    }
  break;
  case 9:
  /* Hock - Schittkowski problem 51 */
    m=5; n=5;
    p[0]=2.5; p[1]=0.5; p[2]=2.0;
    p[3]=-1.0; p[4]=0.5;
    for(i=0; i<n; i++) x[i]=0.0;

    {
      double A[3*5]={1.0, 3.0, 0.0, 0.0, 0.0,  0.0, 0.0, 1.0, 1.0, -2.0,  0.0, 1.0, 0.0, 0.0, -1.0},
             b[3]={4.0, 0.0, 0.0};

    ret=dlevmar_lec_der(hs51, jachs51, p, x, m, n, A, b, 3, 1000, opts, info, NULL, NULL, NULL); // lin. constraints, analytic jacobian
    //ret=dlevmar_lec_dif(hs51, p, x, m, n, A, b, 3, 1000, opts, info, NULL, NULL, NULL); // lin. constraints, no jacobian
    }
  break;
#endif /* HAVE_LAPACK */

  case 10:
  /* Hock - Schittkowski problem 01 */
    m=2; n=2;
    p[0]=-2.0; p[1]=1.0;
    for(i=0; i<n; i++) x[i]=0.0;
    //ret=dlevmar_der(hs01, jachs01, p, x, m, n, 1000, opts, info, NULL, NULL, NULL); // with analytic jacobian
    {
      double lb[2], ub[2];

      lb[0]=-DBL_MAX; lb[1]=-1.5;
      ub[0]=ub[1]=DBL_MAX;

      ret=dlevmar_bc_der(hs01, jachs01, p, x, m, n, lb, ub, 1000, opts, info, NULL, NULL, NULL); // with analytic jacobian
    }
    break;
  case 11:
  /* Hock - Schittkowski (modified) problem 21 */
    m=2; n=2;
    p[0]=-1.0; p[1]=-1.0;
    for(i=0; i<n; i++) x[i]=0.0;
    //ret=dlevmar_der(hs21, jachs21, p, x, m, n, 1000, opts, info, NULL, NULL, NULL); // with analytic jacobian
    {
      double lb[2], ub[2];

      lb[0]=2.0; lb[1]=-50.0;
      ub[0]=50.0; ub[1]=50.0;

      ret=dlevmar_bc_der(hs21, jachs21, p, x, m, n, lb, ub, 1000, opts, info, NULL, NULL, NULL); // with analytic jacobian
    }
    break;
  case 12:
  /* hatfldb problem */
    m=4; n=4;
    p[0]=p[1]=p[2]=p[3]=0.1;
    for(i=0; i<n; i++) x[i]=0.0;
    //ret=dlevmar_der(hatfldb, jachatfldb, p, x, m, n, 1000, opts, info, NULL, NULL, NULL); // with analytic jacobian
    {
      double lb[4], ub[4];

      lb[0]=lb[1]=lb[2]=lb[3]=0.0;

      ub[0]=ub[2]=ub[3]=DBL_MAX;
      ub[1]=0.8;

      ret=dlevmar_bc_der(hatfldb, jachatfldb, p, x, m, n, lb, ub, 1000, opts, info, NULL, NULL, NULL); // with analytic jacobian
    }
    break;
  case 13:
  /* hatfldc problem */
    m=4; n=4;
    p[0]=p[1]=p[2]=p[3]=0.9;
    for(i=0; i<n; i++) x[i]=0.0;
    //ret=dlevmar_der(hatfldc, jachatfldc, p, x, m, n, 1000, opts, info, NULL, NULL, NULL); // with analytic jacobian
    {
      double lb[4], ub[4];

      lb[0]=lb[1]=lb[2]=lb[3]=0.0;

      ub[0]=ub[1]=ub[2]=ub[3]=10.0;

      ret=dlevmar_bc_der(hatfldc, jachatfldc, p, x, m, n, lb, ub, 1000, opts, info, NULL, NULL, NULL); // with analytic jacobian
    }
    break;
  case 14:
  /* equilibrium combustion problem */
    m=5; n=5;
    p[0]=p[1]=p[2]=p[3]=p[4]=0.0001;
    for(i=0; i<n; i++) x[i]=0.0;
    //ret=dlevmar_der(combust, jaccombust, p, x, m, n, 1000, opts, info, NULL, NULL, NULL); // with analytic jacobian
    {
      double lb[5], ub[5];

      lb[0]=lb[1]=lb[2]=lb[3]=lb[4]=0.0001;

      ub[0]=ub[1]=ub[2]=ub[3]=ub[4]=100.0;

      ret=dlevmar_bc_der(combust, jaccombust, p, x, m, n, lb, ub, 5000, opts, info, NULL, NULL, NULL); // with analytic jacobian
    }
    break;
  } /* switch */
  
  printf("Results for %s:\n", probname[problem]);
  printf("Levenberg-Marquardt returned %d in %g iter, reason %g\nSolution: ", ret, info[5], info[6]);
  for(i=0; i<m; ++i)
    printf("%.7g ", p[i]);
  printf("\n\nMinimization info:\n");
  for(i=0; i<LM_INFO_SZ; ++i)
    printf("%g ", info[i]);
  printf("\n");

  return 0;
}
