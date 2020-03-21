//
//  probdist.h
//  hypo_test_attempt
//
//  Created by David Kapatsa on 3/3/19.
//  Copyright © 2019 David Kapatsa. All rights reserved.
//

#ifndef probdist_h
#define probdist_h


void NORMAL( int type, double &x, double &p);
double pNormal(double x);
double xNormal(double prob);
void  CHI( int type, double n, double &x, double &p);
double pChi(double x, int n);
double xChi(double prob, int n);


#endif /* probdist_h */
