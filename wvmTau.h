/* 		
 *  Module name:
      wvmTau.h

 *  Function:
      Include file for wvmTau

 *  Description:
      This file provides prototypes for the functions used in the
      wvmTau.c

 *  Language:
      C

 *  Support: Matt Rippa

 
 *  History:
	$Log$
	Revision 1.1  2003/05/09 21:58:29  mrippa
	Header file for wcmTau.c. The function pwv2tau() is defined
	in wvmCal.h.

 */
#define WVM_COEFF_SIZE 11	/* Size of Coeffs array */
#define TAU_DEBUG 0		/* Debug level: 6 is verbose, 0 is off*/
#define MMH2O_MAX 7.747		/* Saturation level of calculation */


/* 
   Descibe the TAU model coefficients here.
 */
double wvm_cso_coefs[WVM_COEFF_SIZE] = {
  -0.37325545, 38.769126, -13.05137, -25.278241, 
  31.155206, -16.233469, 4.8036578, -0.86140855, 
  0.092759443, -0.0055237545, 0.00013989644
};

/* Describe the AIRMASS correction coefficients here. */
double coefs_m1[3] = {0.038863637, -0.18578918, 0.034048884};
double coefs_m2[3] = {-0.0073803229, 0.027694585, -0.010215906};
double coefs_c[3] = {-0.026203715, 1.1069635, 0.073422855};
