// ********************************************************************
//
// FILENAME:  
// 
// 		Statistics.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Common floating point statistics functions for
//			Square()
//			Average()
//			LinearRegression()
//			StandardError()
//			StandardDeviation()
//			PercentCv()
//			
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include <math.h>				// required standard C library 


#define BAD		1
#define GOOD	0


// ********************************************************************
// FUNCTION:
//
//		Square() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Returns floating point input variable squared
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

float Square(float x)
{
	return 	x * x;
}


// ********************************************************************
// FUNCTION:
//
//		Average() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Returns average of points
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

float Average(float *x, unsigned numPoints)
{
	unsigned	i;
	float		average;

	average = 0.0;

	for (i = 0; i < numPoints; i++)
	{
		average += x[i];
	}

	average /= numPoints;

	return average;
}


// ********************************************************************
// FUNCTION:
//
//		LinearRegression() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculates linear regression for return slope (rate) & intercept of best fit line
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

unsigned LinearRegression(float *x, float *y, unsigned char numPoints, float *slope, float *intercept)
{
	unsigned	i;
	float		xSum;
	float		ySum;
	float		xySum;
	float		xxSum;
	float		denominator;

	// Initialize sum accummulators

	xSum = 0.0;
	ySum = 0.0;
	xySum = 0.0;
	xxSum = 0.0;

	// Calculate sums 

	for (i = 0; i < numPoints; i++)
	{
		xSum += x[i];
		ySum += y[i];
		xySum += x[i] * y[i];
		xxSum += Square(x[i]);
	}

	// Calculate denominator only once

	denominator = (numPoints * xxSum) - Square(xSum);

	// do not allow divide by 0
	 
	if (denominator == 0.0)
	{
		return BAD;
	}

	// Finish regression calculations

	*slope     = ((numPoints * xySum) - (xSum * ySum))  / denominator;
	*intercept = ((ySum * xxSum)      - (xSum * xySum)) / denominator;

	return	GOOD;
}


// ********************************************************************
// FUNCTION:
//
//		StandardError() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculates standard error (noise) from linear regression slope & intercept 
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

unsigned StandardError(float *x, float *y, unsigned char numPoints, float slope, float intercept, float *stdError)
{
	unsigned	i;
	float		xMean;
	float		xSumDelta;
	float		ySumDelta;
	float		yPrime;

	// need at least 3 points to calculate standard error

	if (numPoints < 3)
	{
		return BAD;					// for not enough points
	}

	// Calculate mean of x points

	xMean = Average(x, numPoints);

	// Sum the error terms

	xSumDelta = 0.0;
	ySumDelta = 0.0;

	for (i = 0; i < numPoints; i++)
	{
		yPrime = slope * x[i] + intercept;
		ySumDelta += Square(y[i] - yPrime);
		xSumDelta += Square(x[i] - xMean);
	}

	// Do not allow divide by 0

	if (xSumDelta == 0)
	{
		return BAD;					// for divide by 0.0
	}

	// Finish standard error calculation

	*stdError = sqrt(ySumDelta / (xSumDelta * (float)(numPoints - 2)));

	return	GOOD;
}


// ********************************************************************
// FUNCTION:
//
//		StandardDeviation() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate standard deviation SD of array of points 
//		for number of points
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

float StandardDeviation(float *x, unsigned numPoints)
{
	unsigned	i;

	float		sumOfPoints;
	float		squaredSumOfPoints;
	float		sumOfPointsSquared;
	float		root;

	// initialize the SD accumulators

	sumOfPoints = 0.0;
	sumOfPointsSquared = 0.0;

	// check if enough points for SD calc

	if (numPoints < 2)
	{
		return 0.0;					// for not enough points					
	}

	// accumulate terms and squares of the terms

	for	(i = 0;	i < numPoints; i++)
	{
		sumOfPoints += x[i];
		sumOfPointsSquared += Square(x[i]);
	}

	// calculate the square of the sum

	squaredSumOfPoints = Square(sumOfPoints);

	// prepare for negative square root test

	root = ((((float) numPoints) * sumOfPointsSquared) - squaredSumOfPoints) / Square((float)numPoints);

	if (root < 0.0)
	{								
		return 0.0;					// for negative root
	}

	// compute final SD

	return sqrt(root);
}


// ********************************************************************
// FUNCTION:
//
//		PercentCv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate percent coefficient of variance (CV) for an array of points 
//		for a number of points
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

float PercentCv(float *x, unsigned numPoints)
{
	unsigned	i;
	float		mean;
	float		sumDeltaSquared;

	// check if enough points for CV calc

	if (numPoints < 2)
	{
		return 0.0;					// for not enough points
	}

	// calc mean of points and check for divide by 0

	mean = Average(x, numPoints);

	if (mean == 0.0)
	{
		return 0.0;					// for divide by 0.0 
	}

	// initialize the CV accumulator

	sumDeltaSquared = 0.0;

	// compute sum of delta (each point - mean) squared

	for (i = 0; i < numPoints; i++)
	{
		sumDeltaSquared += Square(x[i] - mean);
	}

	// compute final percent CV
	 
	return	(sqrt(sumDeltaSquared / (float)(numPoints - 1))/ mean) * 100.0;
}
