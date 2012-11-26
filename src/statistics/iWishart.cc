/* -*- Mode: C++; -*- */
// copyright (c) 2012 by Nikolaos Tziortziotis <ntziorzi@gmail.com>
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "iWishart.h"
#include "ranlib.h"
#include "SpecialFunctions.h"
#include "NormalDistribution.h"

iWishart::iWishart()
: k(1),
n(1),
V(Matrix::Unity(1,1))
{
    
}

iWishart::iWishart(real n_, const Matrix& V_)
: k(V_.Rows()),
n(n_),
V(V_)
{
    assert(V.Rows() == V.Columns());
}

iWishart::~iWishart()
{
    
}

void iWishart::generate(Matrix& X) const
{
    Serror("Not implemented\n");
}

///Smith & Hocking, "Wishart Variate Generator"
Matrix iWishart::generate() const
{
	NormalDistribution norm;
	std::vector<Matrix> QR;
	Matrix T = V.Cholesky();
	Matrix B(k,k);
	
	for(int i = 0; i < k; ++i){
	    real r = (real)genchi((real)(n - i));
		B(i,i) = sqrt(r);
	}
	
	for(int i = 0; i < k; ++i){
		for(int j = (i + 1); j < k; ++j){
			B(i,j) = norm.generate();
		}
	}
	QR = B.QRDecomposition();

	Matrix X = Transpose(T)*QR[1].Inverse_LU();
	return X*Transpose(X) / n;
}

/** the log pdf **/
real iWishart::log_pdf(const Matrix& X) const
{
	assert(X.isSymmetric());
	static real log_2	= log(2.0);
	static real log_pi	= log(M_PI);
	
	real rk = (real) k;
	
	real log_c = - ((0.5 * rk * n * log_2)  + (0.25 * rk * (rk - 1.0) * log_pi));
	
	for( int j = 0; j<k; ++j){
		log_c -= logGamma(0.5 * (n-j));
	}
	
	real trace_VX = 0.0;
	Matrix invX = X.Inverse_LU();
	for ( int i=0; i<k; ++i){
		for(int j = 0; j<k; ++j){
			trace_VX += V(i,j) * invX(j,i);
		}
	}
	
	real det_V = V.det();
	real det_X = X.det();
	
	real log_p = log_c
		+ 0.5 * n * log(det_V)
		- 0.5 * (n + rk + 1.0) * log(det_X)
		- 0.5 * trace_VX;
	
	return log_p;
}