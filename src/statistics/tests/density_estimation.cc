/* -*- Mode: C++; -*- */
// copyright (c) 2009 by Christos Dimitrakakis <christos.dimitrakakis@gmail.com>
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef MAKE_MAIN
#include "ContextTreeRealLine.h"
#include "Random.h"
#include <vector>
#include "EasyClock.h"
#include "NormalDistribution.h"
#include "BetaDistribution.h"


int main (int argc, char** argv)
{
    if (argc != 5) {
        Serror ("Usage: density_estimation T D Alpha Beta\n");
        exit(-1);
    }
    int T = atoi(argv[1]);
    if (T < 0) {
        Serror("T should be >= 0\n");
        exit(-1);
    }

    int max_depth = atoi(argv[2]);
    if (max_depth <= 0) {
        Serror("max_depth should be >= 0\n");
        exit(-1);
    }

    real Alpha = atof(argv[3]);
    if (Alpha < 0) {
        Serror("Alpha should be >= 0\n");
        exit(-1);
    }

    real Beta = atof(argv[4]);
    if (Beta < 0) {
        Serror("Beta should be >= 0\n");
        exit(-1);
    }

    BetaDistribution distribution(Alpha,Beta);
    ContextTreeRealLine pdf(2, max_depth);
    int randomise = urandom()*10000;
    for (int i=0; i<randomise; i++) {
        distribution.generate();
    }
    for (int t=0; t<T; ++t) {
        real x = distribution.generate();
        real p = pdf.Observe(x);
        printf ("%f %f #X\n", x, p);

        //        std::cout << p << std::endl;
    }
#if 1
    for (real x=0; x<1; x+=0.001) {
        real p = pdf.pdf(x);
        printf ("%f %f #Y\n", x, p);
    }
    //pdf.Show();
#endif
    return 0;
}

#endif