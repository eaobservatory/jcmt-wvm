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
	Revision 1.6  2003/07/02 22:20:47  berndw
	Just more tests

	Revision 1.5  2003/07/02 22:14:33  berndw
	This is a test of cvs commit
	
	Revision 1.4  2003/05/13 23:36:16  mrippa
	Fixed array index out of bounds exception
	
	Revision 1.3  2003/05/12 03:19:08  mrippa
	Added another printf.
	
	Revision 1.2  2003/05/09 21:56:56  mrippa
	We now compute the empirical tau model on the C side.
	
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
#include "wvmCal.h"

/* 
   Function:
	pwv2tau converts millimeters of water vapor into a value
	representing the optical depth seen at 225GHz relative to
	zenith. This is otherwise known as 225Tau.

	This function currently works! This is another test.

   Support:
	Bernd Weferling: b.weferling@jach.hawaii.edu

   Author:
	Matt Rippa: m.rippa@jach.hawaii.edu
 */
double pwv2tau(double airMass, double mmH2O_a) {
  int j;
  double mult = 0.0;
  double const_m2, const_m1, const_c, correction, wvm_temp, c1, c2;

  if (TAU_DEBUG > 4)
    printf("You gave me an airMass of %f and a pwv of %f\n", airMass, mmH2O_a);
  
  /* If mmH2O_a is really bad just return crude conversion */
  if (mmH2O_a > MMH2O_MAX) {
    return mmH2O_a/21.0;
  }

  /* APPLY THE AIRMASS CORRECTION (POLYNOMIAL MODE) */
  const_m2 = coefs_m2[0] * pow(mmH2O_a, 2.0) + coefs_m2[1] * mmH2O_a + coefs_m2[2];
  if (TAU_DEBUG > 5)
    printf("m2 is: %f\n", const_m2);

  const_m1 = coefs_m1[0] * pow(mmH2O_a, 2.0) + coefs_m1[1] * mmH2O_a + coefs_m1[2];
  if (TAU_DEBUG > 5)
    printf("m1 is: %f\n", const_m1);

  const_c = coefs_c[0] * pow(mmH2O_a, 2.0) + coefs_c[1] * mmH2O_a + coefs_m1[2];
  if (TAU_DEBUG > 5)
    printf("c is: %f\n", const_c);

  c1 = (const_m2 + const_m1 + const_c);
  c2 = (const_m2 * pow(airMass, 2.0) + const_m1 * airMass + const_c);

  if (TAU_DEBUG > 5) {
    printf("c1 is: %f\n", c1);
    printf("c2 is: %f\n", c2);
  }

  correction =  c1 - c2;
  
  if (TAU_DEBUG > 5)
    printf("correction is: %f\n", correction);

  wvm_temp = mmH2O_a + correction;

  if (TAU_DEBUG > 5)
    printf("wvm_temp is: %f\n", wvm_temp);

  /* APPLY THE CSO TAU CONVERSION */
  for (j=0; j < WVM_COEFF_SIZE; j++) {
    mult += wvm_cso_coefs[j] * pow(wvm_temp, j);
  }

  if (TAU_DEBUG > 3) {
    printf("converted wvm_temp is: %f\n", wvm_temp);
    printf("mult is: %f\n", mult);
    printf("Final Tau is: %f\n", wvm_temp/mult);
  }
  return wvm_temp/mult;
}

