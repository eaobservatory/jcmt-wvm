/* 
  File name: 
   wvmTau.c

  Description:
	Provides utility to convert millimeters of precipitable water
	vapor content into a value representing the optical depth seen
	at 225GHz relative to zenith. Otherwise known as 225Tau.

  Functions:
	double pwv2tau(double airMass, double mmH2O);
 History: 
 $Log$
 Revision 1.1  2003/05/06 03:13:41  mrippa
 Added wvmTau to convert pwv into tau value


 */


/* C Includes */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

/* WVM includes */
#include "wvmTau.h"

int main() {
  double a = 1.0;
  double b = 0.64;
  double result;
  printf("Converting airmass %f and mmH2O %f ...\n", a, b);
  result = pwv2tau(a,b);
  printf("Tau is: %f\n", result);
  return 0;
}

/* 
   Function:
	pwv2tau converts millimeters of water vapor into a value
	representing the optical depth seen at 225GHz relative to
	zenith. This is otherwise known as 225Tau.

   Support:
	Bernd Weferling: b.weferling@jach.hawaii.edu

   Author:
	Matt Rippa: m.rippa@jach.hawaii.edu
 */
double pwv2tau(double airMass, double mmH2O) {
  int j;
  double mult = 0.0;
  double const_m2, const_m1, const_c, correction, wvm_temp, c1, c2;

  /* If mmH2O is really bad just return crude conversion */
  if (mmH2O > 7.747) {
    return mmH2O/21.0;
  }

  /* APPLY THE AIRMASS CORRECTION (POLYNOMIAL MODE) */
  const_m2 = coefs_m2[0] * pow(mmH2O, 2.0) + coefs_m2[1] * mmH2O + coefs_m2[2];
  printf("m2 is: %f\n", const_m2);

  const_m1 = coefs_m1[0] * pow(mmH2O, 2.0) + coefs_m1[1] * mmH2O + coefs_m1[2];
  printf("m1 is: %f\n", const_m1);

  const_c = coefs_c[0] * pow(mmH2O, 2.0) + coefs_c[1] * mmH2O + coefs_m1[2];
  printf("c is: %f\n", const_c);

  c1 = (const_m2 + const_m1 + const_c);
  c2 = (const_m2 * pow(airMass, 2.0) + const_m1 * airMass + const_c);

  printf("c1 is: %f\n", c1);
  printf("c2 is: %f\n", c2);

  correction =  c1 - c2;
  printf("correction is: %f\n", correction);

  wvm_temp = mmH2O + correction;
  printf("wvm_temp is: %f\n", wvm_temp);

  /* APPLY THE CSO TAU CONVERSION */
  for (j=0; j <= WVM_CSO_SIZE; j++) {
    mult += wvm_cso_coefs[j] * pow(wvm_temp, j);
  }

  printf("converted wvm_temp is: %f\n", wvm_temp);
  printf("mult is: %f\n", mult);

  return wvm_temp/mult;
}

